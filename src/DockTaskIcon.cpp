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

#include <QGraphicsSceneMouseEvent>

#include <KIcon>
#include <KLocale>

#include <taskmanager/taskmanager.h>

#include "DockConfig.h"
#include "DockTaskIcon.h"

DockTaskIcon::DockTaskIcon(DockApp::Ptr app, QGraphicsItem *parent)
	: DockIcon(parent), m_app(app)
{
	setDragable(true);
	setIcon(KIcon(app->icon()));
}

void DockTaskIcon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::MidButton)
	{
		if (DockConfig::middleClickNewInstance())
			m_app->launchApp();
	}
	else if (event->button() == Qt::LeftButton)
	{
		tmp_taskList = m_app->tasks();

		// Single app instance
		if (tmp_taskList.size() == 1)
			tmp_taskList.first()->activateRaiseOrIconify();
		// Multiple app instances
		else if (tmp_taskList.size() > 1)
		{
			tmp_taskMenu.clear();

			for (QList<TaskManager::Task *>::ConstIterator it = tmp_taskList.begin(); it != tmp_taskList.end(); it++)
			{
				QAction *action = new QAction(KIcon(m_app->icon()), (*it)->visibleName(), this);
				connect(action, SIGNAL(triggered(bool)), this, SLOT(taskMenuClicked()));

				tmp_taskMenu.addAction(action);
			}
			tmp_taskMenu.exec(event->screenPos());
		}
		else // No instance
			m_app->launchApp();
	}
}

void DockTaskIcon::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	KMenu contextMenu;
	contextMenu.addTitle(KIcon(m_app->icon()), m_app->name());
	QAction *action = new QAction(KIcon("application-x-executable"), i18n("New instance"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(newInstance()));
	contextMenu.addAction(action);
	if (m_app->runnable())
	{
		QAction *action = new QAction(KIcon(), i18n("Keep a launcher icon"), this);
		action->setCheckable(true);
		action->setChecked(m_app->isLauncher());
		connect(action, SIGNAL(triggered()), this, SLOT(toggleKeepLauncher()));
		contextMenu.addAction(action);
	}
	if (m_app->countTasks())
	{
		contextMenu.addSeparator();
		QAction *action = new QAction(KIcon("edit-delete"), i18n("Quit"), this);
		connect(action, SIGNAL(triggered()), this, SLOT(quit()));
		contextMenu.addAction(action);
	}
	contextMenu.exec(event->screenPos());
}

void DockTaskIcon::taskMenuClicked()
{
	QList<QAction *> tmp_taskActions = tmp_taskMenu.actions();
	::TaskManager::Task *task = NULL;
	for (int i = 0; i < tmp_taskActions.size(); i++)
		if (tmp_taskActions[i] == sender())
		{
			task = tmp_taskList[i];
			break;
		}
	task->activateRaiseOrIconify();
}

void DockTaskIcon::newInstance()
{
	m_app->launchApp();
}

void DockTaskIcon::toggleKeepLauncher()
{
	m_app->toggleLauncher();
}

void DockTaskIcon::quit()
{
	m_app->closeTasks();
}
