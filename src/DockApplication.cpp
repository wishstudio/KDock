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

#include <QDesktopWidget>

#include <KGlobal>

#include "DockApplication.h"
#include "DockGraphicsScene.h"
#include "DockPanelView.h"

K_GLOBAL_STATIC(DockApplication, globalDockApplication)

DockApplication *DockApplication::self()
{
	return globalDockApplication;
}

void DockApplication::setPanelHidden(bool panelHidden)
{
	self()->m_panelHidden = panelHidden;
}

DockApplication::DockApplication()
	: KUniqueApplication(), m_panelHidden(false)
{
	m_scene = new DockGraphicsScene();
	QDesktopWidget *desktopWidget = QApplication::desktop();
	m_scene->setSceneRect(desktopWidget->geometry());

	m_panelView = new DockPanelView(m_scene);
	m_panelView->show();
}

bool DockApplication::x11EventFilter(XEvent *event)
{
	if (m_panelHidden && (event->type == ClientMessage || (event->xany.send_event != True && (event->type == EnterNotify || event->type == MotionNotify))))
	{
		if (m_panelView->unhideTrigger() == event->xcrossing.window)
		{
			// TODO: dnd support
			XMotionEvent *motion = (XMotionEvent *)event;
			m_panelView->checkUnhide(QPoint(motion->x_root, motion->y_root));
		}
	}
	return KUniqueApplication::x11EventFilter(event);
}
