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
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsWidget>
#include <QList>

#include "DockContainer.h"
#include "DockDragDropEvent.h"
#include "DockGraphicsScene.h"
#include "DockProxyView.h"

DockGraphicsScene::DockGraphicsScene(QObject *parent)
	: QGraphicsScene(parent), m_dragWidget(NULL), m_dragParentWidget(NULL), m_dragView(new DockProxyView())
{
}

DockGraphicsScene::~DockGraphicsScene()
{
	delete m_dragView;
}

void DockGraphicsScene::startDrag(QGraphicsWidget *widget, QGraphicsSceneMouseEvent *event)
{
	static bool multiGuard = false;
	// FIXME: QGraphicsItem will generate extra mouseMoveEvent when doing setParentItem
	// and it comes with incorrect lastScenePos() value
	// So we have to keep track with it by self
	if (!m_dragWidget && !multiGuard)
	{
		widget->ungrabMouse();
		multiGuard = true;
		m_lastDragPos = event->scenePos();
		QPointF widgetPos = widget->scenePos();
		QGraphicsWidget *parent = widget->parentWidget();
		DockDragDropEvent event = DockDragDropEvent(DockDragDropEvent::DragStart, widget);
		QApplication::sendEvent(parent, &event);
		widget->setParentItem(0);
		widget->setPos(widgetPos);
		m_dragView->setWidget(widget);
		m_dragView->show();
		m_dragParentWidget = parent;
		m_dragWidget = widget;
		multiGuard = false;
	}
}

void DockGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	static bool multiGuard = false;
	if (m_dragWidget && !multiGuard)
	{
		multiGuard = true;
		m_dragWidget->setPos(m_dragWidget->pos() + event->scenePos() - m_lastDragPos);
		m_lastDragPos = event->scenePos();
		if (m_dragParentWidget && m_dragParentWidget->collidesWithItem(m_dragWidget))
		{
			DockDragDropEvent event = DockDragDropEvent(DockDragDropEvent::DragMove, m_dragWidget);
			QApplication::sendEvent(m_dragParentWidget, &event);
		}
		else
		{
			if (m_dragParentWidget)
			{
				DockDragDropEvent event = DockDragDropEvent(DockDragDropEvent::DragLeave, m_dragWidget);
				QApplication::sendEvent(m_dragParentWidget, &event);
				m_dragParentWidget = NULL;
			}
			QList<QGraphicsItem *> items = collidingItems(m_dragWidget);

			for (QList<QGraphicsItem *>::ConstIterator it = items.begin(); it != items.end(); it++)
			{
				// TODO: HACK
				DockContainer *container = qobject_cast<DockContainer *>(static_cast<QObject *>(static_cast<QGraphicsWidget *>(*it)));
				if (container)
				{
					m_dragParentWidget = container;
					DockDragDropEvent event = DockDragDropEvent(DockDragDropEvent::DragEnter, m_dragWidget);
					QApplication::sendEvent(m_dragParentWidget, &event);
				}
			}
		}
		multiGuard = false;
	}
	else
		QGraphicsScene::mouseMoveEvent(event);
}

void DockGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_dragWidget)
	{
		if (m_dragParentWidget)
		{
			DockDragDropEvent event = DockDragDropEvent(DockDragDropEvent::Drop, m_dragWidget);
			QApplication::sendEvent(m_dragParentWidget, &event);
			m_dragWidget = m_dragParentWidget = NULL;
			m_dragView->hide();
			m_dragView->setWidget(NULL);
		}
	}
	else
		QGraphicsScene::mouseReleaseEvent(event);
}
