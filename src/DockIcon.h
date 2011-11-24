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

#ifndef DOCKICON_H
#define DOCKICON_H

#include <QBitmap>
#include <QGraphicsWidget>
#include <QIcon>
#include <QList>
#include <QObject>
#include <QPoint>

#include <KMenu>

class DockApp;
class DockProxyView;
class DockWindow;
class DockIcon: public QGraphicsWidget
{
	Q_OBJECT

public:
	DockIcon(QGraphicsItem *parent = NULL);
	~DockIcon();

	void setDragable(bool dragable);
	void setIcon(const QIcon &icon);

public slots:
	void configChanged();
	
signals:
	void beginIconDrag(DockIcon *obj);
	void dragReposition(QPointF pos);
	
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
	QPointF m_startPos;

	QIcon m_icon;
	QPixmap m_pixmap;
	QBitmap m_mask;

	bool m_dragable;
}; 

#endif
