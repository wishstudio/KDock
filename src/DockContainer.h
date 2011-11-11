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

#ifndef DOCKCONTAINER_H
#define DOCKCONTAINER_H

#include <QGraphicsWidget>

#include "DockDragDropEvent.h"

class DockContainer: public QGraphicsWidget
{
	Q_OBJECT
	
public:
	DockContainer(QGraphicsItem *parent = NULL);

	virtual void addWidget(QGraphicsWidget *widget);
	virtual void removeWidget(QGraphicsWidget *widget);

protected:
	virtual bool event(QEvent *event);
	virtual void dockDragStartEvent(DockDragDropEvent *event);
	virtual void dockDragEnterEvent(DockDragDropEvent *event);
	virtual void dockDragMoveEvent(DockDragDropEvent *event);
	virtual void dockDragLeaveEvent(DockDragDropEvent *event);
	virtual void dockDropEvent(DockDragDropEvent *event);
};

#endif
