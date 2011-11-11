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

#include "DockContainer.h"

DockContainer::DockContainer(QGraphicsItem *parent)
	: QGraphicsWidget(parent)
{
}

bool DockContainer::event(QEvent *event)
{
	if (event->type() == DockDragDropEvent::DragStart)
		dockDragStartEvent(static_cast<DockDragDropEvent *>(event));
	else if (event->type() == DockDragDropEvent::DragEnter)
		dockDragEnterEvent(static_cast<DockDragDropEvent *>(event));
	else if (event->type() == DockDragDropEvent::DragMove)
		dockDragMoveEvent(static_cast<DockDragDropEvent *>(event));
	else if (event->type() == DockDragDropEvent::DragLeave)
		dockDragLeaveEvent(static_cast<DockDragDropEvent *>(event));
	else if (event->type() == DockDragDropEvent::Drop)
		dockDropEvent(static_cast<DockDragDropEvent *>(event));
	else
		return QGraphicsWidget::event(event);
	return event->isAccepted();
}

void DockContainer::addWidget(QGraphicsWidget *widget)
{
	Q_UNUSED(widget);
}

void DockContainer::removeWidget(QGraphicsWidget *widget)
{
	Q_UNUSED(widget);
}

void DockContainer::dockDragStartEvent(DockDragDropEvent *event)
{
	Q_UNUSED(event);
}

void DockContainer::dockDragEnterEvent(DockDragDropEvent *event)
{
	Q_UNUSED(event);
}

void DockContainer::dockDragMoveEvent(DockDragDropEvent *event)
{
	Q_UNUSED(event);
}

void DockContainer::dockDragLeaveEvent(DockDragDropEvent *event)
{
	Q_UNUSED(event);
}

void DockContainer::dockDropEvent(DockDragDropEvent *event)
{
	Q_UNUSED(event);
}
