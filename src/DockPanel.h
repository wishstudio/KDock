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

#ifndef DOCKWINDOW_H
#define DOCKWINDOW_H

#include <KMenu>

#include <taskmanager/taskmanager.h>

#include "DockApp.h"
#include "DockContainer.h"

using TaskManager::TaskPtr;
class QWidget;
class DockGraphicsScene;
class DockIcon;
class DockPanel: public DockContainer
{
	Q_OBJECT
	
public:
	DockPanel(DockGraphicsScene *scene);

	void addWidget(QGraphicsWidget *widget);
	void removeWidget(QGraphicsWidget *widget);
	
public slots:
	void parentChanged();
	void configChanged();

protected:
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	virtual void dockDragStartEvent(DockDragDropEvent *event);
	virtual void dockDragEnterEvent(DockDragDropEvent *event);
	virtual void dockDragMoveEvent(DockDragDropEvent *event);
	virtual void dockDragLeaveEvent(DockDragDropEvent *event);
	virtual void dockDropEvent(DockDragDropEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	
private:
	qreal getIconLength() const;
	qreal getDockLength() const;
	qreal getDockHeight() const;
	qreal getDockLengthEdgeSpacing() const;
	qreal getDockHeightEdgeSpacing() const;
	int getClosestWidget(const QPointF &centerPos) const;

	QPointF getLauncherPosition(int app_id) const;
	QPointF getPanelPosition() const;
	void reposition();
	void hoverTransform();

	int m_hoverId;
	QPointF m_hoverPos;

	int m_dragReserveId;
	QList<QGraphicsWidget *> m_widgets;
	KMenu m_contextMenu;
};

#endif
