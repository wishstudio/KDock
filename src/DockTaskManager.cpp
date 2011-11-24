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

#include <QGraphicsScene>

#include <KGlobal>
#include <KStandardDirs>

#include "AppMatcher.h"
#include "DockContainer.h"
#include "DockTaskManager.h"

K_GLOBAL_STATIC(DockTaskManager, globalDockTaskManager)

DockTaskManager::DockTaskManager()
{
	m_container = NULL;

	QHash<WId, ::TaskManager::Task *> taskDict = ::TaskManager::TaskManager::self()->tasks();

	for (QHash<WId, ::TaskManager::Task *>::ConstIterator it = taskDict.begin(); it != taskDict.end(); it++)
		updateTaskInfo(*it, true);

	connect(::TaskManager::TaskManager::self(), SIGNAL(taskAdded(::TaskManager::Task*)), this, SLOT(taskAdded(::TaskManager::Task*)));
	connect(::TaskManager::TaskManager::self(), SIGNAL(taskRemoved(::TaskManager::Task*)), this, SLOT(taskRemoved(::TaskManager::Task*)));
}

DockTaskManager::~DockTaskManager()
{
	for (int i = 0; i < m_icons.size(); i++)
		delete m_icons[i];
}

void DockTaskManager::setContainer(DockContainer *container)
{
	self()->m_container = container;
	for (int i = 0; i < self()->m_icons.size(); i++)
		container->addWidget(self()->m_icons[i]);
}

int DockTaskManager::findAppByWindowClass(QString name)
{
	if (!name.isEmpty())
		for (int i = 0; i < m_apps.size(); i++)
			if (m_apps[i]->name().toLower() == name.toLower() || m_apps[i]->wm_class().toLower() == name.toLower())
				return i;
	return -1;
}

int DockTaskManager::findAppByTask(::TaskManager::Task *task)
{
	for (int app_id = 0; app_id < m_apps.size(); app_id++)
		if (m_apps[app_id]->tasks().contains(task))
			return app_id;
	return -1;
}

void DockTaskManager::taskAdded(::TaskManager::Task *task)
{
	updateTaskInfo(task, true);
}

void DockTaskManager::taskRemoved(::TaskManager::Task *task)
{
	int app_id = findAppByTask(task);
	if (app_id != -1)
	{
		m_apps[app_id]->removeTask(task);
		if (!m_apps[app_id]->countTasks() && !m_apps[app_id]->isLauncher())
			removeLauncher(app_id);
	}
}

// TODO: Take care of this func
void DockTaskManager::createLauncher(int app_id)
{
	DockTaskIcon *icon = new DockTaskIcon(m_apps[app_id], NULL);
	if (m_container)
		m_container->addWidget(icon);
	m_icons.append(icon);
}

void DockTaskManager::removeLauncher(int app_id)
{
	m_apps.removeAt(app_id);
	if (m_container)
	{
		m_container->removeWidget(m_icons[app_id]);
		m_container->scene()->removeItem(m_icons[app_id]);
	}
	delete m_icons[app_id];
	m_icons.removeAt(app_id);
}

void DockTaskManager::updateTaskInfo(::TaskManager::Task *task, bool allowCreate)
{
	int app_id = findAppByTask(task);
	if (app_id == -1)
		app_id = findAppByWindowClass(task->className());
	if (app_id == -1)
	{
		if (!allowCreate)
			return;
		QString desktop_file = AppMatcher::matchTask(task);
		if (!desktop_file.isEmpty()) // Desktop file found
		{
			m_apps.append(DockApp::Ptr(new DockApp(desktop_file, false)));
			app_id = m_apps.size() - 1;
			createLauncher(app_id);
		}
		else // Desktop file not found
		{
			QString exe = KStandardDirs::findExe(task->className().toLower());
			if (exe.isEmpty())
				exe = KStandardDirs::findExe(task->className());
			m_apps.append(DockApp::Ptr(new DockApp(exe, task->visibleName(), task->className(), task->className())));
			app_id = m_apps.size() - 1;
			createLauncher(app_id);
		}
	}
	m_apps[app_id]->addTask(task);
}

DockTaskManager *DockTaskManager::self()
{
	return globalDockTaskManager;
}
