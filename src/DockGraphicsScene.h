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

#ifndef DOCKGRAPHICSSCENE_H
#define DOCKGRAPHICSSCENE_H

#include <QGraphicsScene>

class QGraphicsWidget;
class DockProxyView;
class DockGraphicsScene: public QGraphicsScene
{
	Q_OBJECT
	
public:
	DockGraphicsScene(QObject *parent = NULL);
	virtual ~DockGraphicsScene();

	void startDrag(QGraphicsWidget *widget, QGraphicsSceneMouseEvent *event);
	DockProxyView *dragView() { return m_dragView; }

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	QPointF m_lastDragPos;
	QGraphicsWidget *m_dragWidget, *m_dragParentWidget;
	DockProxyView *m_dragView;
};

#endif
