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

#include <QApplication>
#include <QDesktopWidget>
#include <QX11Info>

#include <KWindowSystem>

#include "DockApplication.h"
#include "DockConfig.h"
#include "DockPanel.h"
#include "DockPanelView.h"

DockPanelView::DockPanelView(DockGraphicsScene *scene)
	: DockProxyView(0, Qt::WA_TranslucentBackground), m_unhideTrigger(None)
{
	connect(DockConfig::self(), SIGNAL(configChanged()), this, SLOT(configChanged()));
	setStyleSheet("border: 0px; background: transparent;");
	m_panel = new DockPanel(scene);
	setWidget(m_panel);
	connect(m_panel, SIGNAL(geometryChanged()), this, SLOT(panelGeometryChanged()));
	panelGeometryChanged();
}

DockPanelView::~DockPanelView()
{
	delete m_panel;
}

void DockPanelView::panelGeometryChanged()
{
	updateStrut();
}

void DockPanelView::configChanged()
{
	updateStrut();
	show();
}

void DockPanelView::checkUnhide(const QPoint &pos)
{
	if (DockConfig::autoHide())
	{
		if (m_triggerZone.contains(pos))
		{
			XDestroyWindow(QX11Info::display(), m_unhideTrigger);
			m_unhideTrigger = None;
			show();
		}
	}
}

void DockPanelView::leaveEvent(QEvent *)
{
	if (DockConfig::autoHide())
	{
		XSetWindowAttributes attributes;
		attributes.override_redirect = True;
		attributes.event_mask = EnterWindowMask | LeaveWindowMask | PointerMotionMask | KeyPressMask |
			ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | KeymapStateMask |
			VisibilityChangeMask | StructureNotifyMask | ResizeRedirectMask | SubstructureNotifyMask |
			SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask | ColormapChangeMask |
			OwnerGrabButtonMask;

		QDesktopWidget *desktop = QApplication::desktop();
		unsigned long valuemask = CWOverrideRedirect | CWEventMask;
		m_unhideTrigger = XCreateWindow(QX11Info::display(), QX11Info::appRootWindow(), m_panel->geometry().left(), desktop->height() - 1, m_panel->geometry().width(), 1, 0, CopyFromParent, InputOnly, CopyFromParent, valuemask, &attributes);
		m_triggerZone = QRect(m_panel->geometry().left(), desktop->height() - 1, m_panel->geometry().width(), 1);
		XMapWindow(QX11Info::display(), m_unhideTrigger);
		DockApplication::setPanelHidden(true);
		hide();
	}
}

void DockPanelView::updateStrut()
{
	if (DockConfig::alwaysShow())
		KWindowSystem::setStrut(winId(), 0, 0, 0, m_panel->size().height());
	else
		KWindowSystem::setStrut(winId(), 0, 0, 0, 0);
}
