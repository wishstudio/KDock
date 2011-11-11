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

#ifndef DOCKPANELVIEW_H
#define DOCKPANELVIEW_H

#include "DockProxyView.h"

class DockGraphicsScene;
class DockPanel;
class DockPanelView: public DockProxyView
{
	Q_OBJECT
	
public:
	DockPanelView(DockGraphicsScene *scene);
	~DockPanelView();
	
public slots:
	void panelGeometryChanged();
	
private:
	DockPanel *m_panel;
};

#endif
