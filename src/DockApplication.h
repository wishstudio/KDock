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

#ifndef DOCKAPPLICATION_H
#define DOCKAPPLICATION_H

#include <KUniqueApplication>

class DockGraphicsScene;
class DockPanelView;
class DockApplication: public KUniqueApplication
{
	Q_OBJECT

public:
	DockApplication();

	static DockApplication *self();
	static void setPanelHidden(bool panelHidden);

protected:
	virtual bool x11EventFilter(XEvent *event);

private:
	DockGraphicsScene *m_scene;
	DockPanelView *m_panelView;
	bool m_panelHidden;
};

#endif
