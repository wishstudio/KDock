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
#include <QBitmap>
#include <QList>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include <KIcon>
#include <KLocale>
#include <KMenu>
#include <KWindowSystem>

#include "DockApp.h"
#include "DockConfig.h"
#include "DockGraphicsScene.h"
#include "DockIcon.h"
#include "DockProxyView.h"

DockIcon::DockIcon(QGraphicsItem *parent):
	QGraphicsWidget(parent), m_dragable(false)
{
	setCursor(Qt::PointingHandCursor);

	connect(this, SIGNAL(geometryChanged()), SLOT(geometryChanged()));
}

DockIcon::~DockIcon()
{
}

void DockIcon::setIcon(const QIcon &icon)
{
	m_icon = icon;
	geometryChanged();
}

void DockIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->drawPixmap(QPoint(0, 0), m_pixmap);
}

void DockIcon::setDragable(bool dragable)
{
	m_dragable = dragable;
}

void DockIcon::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_dragable)
		m_startPos = event->scenePos();
}

void DockIcon::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_dragable && (event->buttons() & Qt::LeftButton))
	{
		if ((event->scenePos() - m_startPos).toPoint().manhattanLength() > QApplication::startDragDistance())
		{
			DockGraphicsScene *dockScene = static_cast<DockGraphicsScene *>(scene());
			//dockScene->dragView()->setMask(m_mask);
			dockScene->startDrag(this, event);
		}
	}
}

void DockIcon::geometryChanged()
{
	m_pixmap = m_icon.pixmap(size().width(), size().height());
	m_mask = m_pixmap.createHeuristicMask();
}
