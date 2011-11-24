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

#ifndef DOCKTASKMANAGER_H
#define DOCKTASKMANAGER_H

#include <QObject>

class DockApp;
class DockTaskIcon;
namespace TaskManager
{
class Task;
}

class DockTaskManager: public QObject
{
	Q_OBJECT

public:
	DockTaskManager();
	~DockTaskManager();

	static DockTaskManager *self();
	static void setContainer(DockContainer *container);

public slots:
	void taskAdded(::TaskManager::Task *task);
	void taskRemoved(::TaskManager::Task *task);

private:
	int findAppByWindowClass(QString name);
	int findAppByTask(::TaskManager::Task *task);
	void createLauncher(int app_id);
	void removeLauncher(int app_id);
	void updateTaskInfo(::TaskManager::Task *task, bool allowCreate);

	DockContainer *m_container;
	QList<DockApp *> m_apps;
	QList<DockTaskIcon *> m_icons;
};

#endif
