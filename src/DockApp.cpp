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

#include "DockApp.h"

#include <QString>
#include <QX11Info>

#include <NETRootInfo>
#include <KConfigGroup>
#include <KDesktopFile>
#include <KRun>
#include <KWindowInfo>
#include <KWindowSystem>

#include <taskmanager/taskmanager.h>

DockApp::DockApp(QString desktop_file, bool isLauncher)
	: m_tasks(QList< ::TaskManager::Task *>())
{
	if (KDesktopFile::isDesktopFile(desktop_file))
	{
		KDesktopFile desk(desktop_file);
		KConfigGroup desktop = desk.desktopGroup();
		m_executable = desktop_file;
		m_icon = desktop.readEntry("Icon", QString());
		m_name = desktop.readEntry("Name", QString());
		m_command = desktop.readEntry("Exec", QString());
		m_wm_class = desktop.readEntry("StartupWMClass", QString());
		m_isLauncher = isLauncher;
	}
	else
	{
		m_executable = QString();
		m_icon = QString();
		m_name = QString();
		m_command = QString();
		m_wm_class = QString();
		m_isLauncher = false;
	}
}

DockApp::DockApp(QString executable, QString icon, QString name, QString wm_class)
	: m_isLauncher(false), m_executable(executable), m_icon(icon), m_name(name), m_command(executable), m_wm_class(wm_class),
		m_tasks(QList< ::TaskManager::Task *>())
{
}

void DockApp::launchApp()
{
	if (!runnable())
		return;
	QString command = m_command
		.replace(QString("%f"), QString(""))
		.replace(QString("%F"), QString(""))
		.replace(QString("%u"), QString(""))
		.replace(QString("%U"), QString(""))
		.replace(QString("%i"), QString(""))
		.replace(QString("%c"), QString(""))
		.replace(QString("%k"), QString(""));
	KRun::runCommand(command, m_name, m_icon, NULL, NULL);
}

void DockApp::toggleLauncher()
{
	m_isLauncher = !m_isLauncher;
	emit appChanged();
}

bool DockApp::runnable()
{
	return !m_executable.isEmpty();
}

bool DockApp::validQList(const QList<QString> &values)
{
	if (values.empty())
		return false;
	if (KDesktopFile::isDesktopFile(values[0]))
		return values.size() == 1;
	else
		return values.size() == 4;
}

DockApp::Ptr DockApp::fromQList(const QList<QString> &values)
{
	QString executable = values[0];
	if (KDesktopFile::isDesktopFile(executable))
		return DockApp::Ptr(new DockApp(executable, true));
	else
	{
		QString icon = values[1];
		QString name = values[2];
		QString wm_class = values[3];
		return DockApp::Ptr(new DockApp(executable, icon, name, wm_class));
	}
}

QList<QString> DockApp::toQList()
{
	QList<QString> ret;
	ret << m_executable;
	if (!KDesktopFile::isDesktopFile(m_executable))
	{
		ret << m_icon;
		ret << m_name;
		ret << m_wm_class;
	}
	return ret;
}

bool DockApp::isLauncher()
{
	return m_isLauncher;
}

QString DockApp::desktop_file()
{
	if (KDesktopFile::isDesktopFile(m_executable))
		return m_executable;
	return QString();
}

QString DockApp::executable()
{
	return m_executable;
}

QString DockApp::icon()
{
	return m_icon;
}

QString DockApp::name()
{
	return m_name;
}

QString DockApp::command()
{
	return m_command;
}

QString DockApp::wm_class()
{
	return m_wm_class;
}

QList< ::TaskManager::Task *> DockApp::tasks()
{
	return m_tasks;
}

void DockApp::clearTasks()
{
	m_tasks.clear();
}

int DockApp::countTasks()
{
	return m_tasks.size();
}

void DockApp::addTask(::TaskManager::Task *task)
{
	m_tasks.append(task);
	connect(task, SIGNAL(activated()), this, SLOT(taskActivated()));
}

void DockApp::removeTask(::TaskManager::Task *task)
{
	m_tasks.removeOne(task);
	disconnect(task, SIGNAL(deactivated()), this, SLOT(taskDeactivated()));
}

void DockApp::closeTasks()
{
	for (QList< ::TaskManager::Task *>::Iterator it = m_tasks.begin(); it != m_tasks.end(); it++)
	{
		::TaskManager::Task *task = *it;
		task->close();
	}
}

void DockApp::taskActivated()
{
	emit appActivated();
}

void DockApp::taskDeactivated()
{
	emit appDeactivated();
}
