/*
 *   Copyright (C) 2011 by Xiangyan Sun <wishstudio@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "AppMatcher.h"

#include <QDir>
#include <QFile>

#include <KApplication>
#include <KConfigGroup>
#include <KDesktopFile>
#include <KService>
#include <KServiceTypeTrader>
#include <KWindowInfo>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

pid_t AppMatcher::getWindowPID(WId win)
{
	Atom atom = XInternAtom(QX11Info::display(), "_NET_WM_PID", false);
	Atom type;
	int format;
	unsigned long length, after;
	unsigned long* pid;
	if (XGetWindowProperty(QX11Info::display(), win, atom, 0, 32, false, XA_CARDINAL, &type, &format, &length, &after, (unsigned char **)&pid) == Success)
	{
		if (type == XA_CARDINAL)
		{
			int ret = *pid;
			delete pid;
			return ret;
		}
		delete pid;
	}
	return 0;
}

QString AppMatcher::readAllContent(QString file_name)
{
	QFile file(file_name);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return QString();
	QString ret = file.readAll();
	file.close();
	return ret;
}

QString AppMatcher::analysisBinaryName(QString cmdline)
{
	QStringList options = cmdline.split(" ");
	foreach (QString opt, options)
		if (!opt.startsWith("-"))
		{
			QStringList paths = opt.split(QDir::separator());
			if (!paths.isEmpty() && !paths.last().isEmpty())
				return paths.last();
		}
	return QString();
}

QString AppMatcher::getProcessName(pid_t pid)
{
	QString status = readAllContent(QString("/proc/%1/status").arg(pid));
	if (!status.isEmpty())
	{
		QStringList lines = status.split("\n");
		if (lines.size())
		{
			QStringList sections = lines[0].split("\t");
			if (sections.size() > 1)
				return sections[1];
		}
	}
	return QString();
}

QString AppMatcher::getProcessBinaryName(pid_t pid)
{
	return analysisBinaryName(readAllContent(QString("/proc/%1/cmdline").arg(pid)));
}

// TODO: Directly maintain a list of desktop files to optimize this function,
//       and also support some required features that KTrader cannot provide
/* The current matching priority:
 * 1. The desktop_file hint of window is set (TODO)
 * 2. Desktop file name matches window class, window class specified in desktop file also matches
 * 3. Window class specified in desktop file matches
 * 4. Try Name in /proc/pid/status or /proc/pid/cmdline to match Exec line in desktop file
 */
QString AppMatcher::matchTask(TaskManager::Task *task)
{
	QString window_class = task->className().toLower();
	if (!window_class.isEmpty())
	{
		{
			// File name AND window class matches
			QString query = QString("'%1' =~ DesktopEntryName and ((exist Name and '%1' =~ Name) or (exist StartupWMClass and '%1' =~ StartupWMClass))").arg(window_class);
			KService::List services = KServiceTypeTrader::self()->query("Application", query);
			if (!services.empty())
				return services.first()->entryPath();
		}
		{
			// File name OR window class matches
			QString query = QString("'%1' =~ DesktopEntryName or ((exist Name and '%1' =~ Name) or (exist StartupWMClass and '%1' =~ StartupWMClass))").arg(window_class);
			KService::List services = KServiceTypeTrader::self()->query("Application", query);
			if (!services.empty())
				return services.first()->entryPath();
		}
	}
	pid_t pid = getWindowPID(task->window());
	if (pid)
	{
		// TODO: This matching process wastes a lot of time
		QList<QString> desktop_file_list;
		QString process_name = getProcessName(pid);
		if (!process_name.isEmpty())
		{
			QString query = QString("exist Exec and '%1' ~~ Exec").arg(process_name);
			KService::List services = KServiceTypeTrader::self()->query("Application", query);
			foreach (KService::Ptr service, services)
				desktop_file_list << service->entryPath();
		}
		QString binary_name = getProcessBinaryName(pid);
		if (!binary_name.isEmpty())
		{
			QString query = QString("exist Exec and '%1' ~~ Exec").arg(binary_name);
			KService::List services = KServiceTypeTrader::self()->query("Application", query);
			foreach (KService::Ptr service, services)
				desktop_file_list << service->entryPath();
		}
		foreach (QString desktop_file, desktop_file_list)
		{
			if (KDesktopFile::isDesktopFile(desktop_file))
			{
				KDesktopFile desk(desktop_file);
				KConfigGroup desktop = desk.desktopGroup();
				QString exec = analysisBinaryName(desktop.readEntry("Exec", QString()));
				if (exec == process_name || exec == binary_name)
					return desktop_file;
			}
		}
	}
	return QString();
}
