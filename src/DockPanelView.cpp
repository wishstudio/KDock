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

#include <KWindowSystem>

#include "DockPanel.h"
#include "DockPanelView.h"

DockPanelView::DockPanelView(DockGraphicsScene *scene)
	: DockProxyView(0, Qt::WA_TranslucentBackground)
{
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
	KWindowSystem::setStrut(winId(), 0, 0, 0, m_panel->size().height());
}
