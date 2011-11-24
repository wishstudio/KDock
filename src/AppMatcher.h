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

#ifndef APPMATCHER_H
#define APPMATCHER_H

#include <sys/types.h>

#include <QString>

#include <KWindowSystem>
#include <taskmanager/taskmanager.h>

class AppMatcher
{
public:
	static QString matchTask(TaskManager::Task *task);
	
private:
	static pid_t getWindowPID(WId win);
	static QString readAllContent(QString file_name);
	static QString analysisBinaryName(QString cmdline);
	static QString getProcessName(pid_t pid);
	static QString getProcessBinaryName(pid_t pid);
};

#endif
