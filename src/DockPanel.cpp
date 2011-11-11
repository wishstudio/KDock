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

#include <qmath.h>
#include <Qt>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QList>
#include <QWidget>

#include <KApplication>
#include <KIcon>
#include <KStandardDirs>
#include <KWindowSystem>

#include "AppMatcher.h"
#include "DockConfig.h"
#include "DockContainer.h"
#include "DockGraphicsScene.h"
#include "DockIcon.h"
#include "DockPanel.h"
#include "DockTaskManager.h"

DockPanel::DockPanel(DockGraphicsScene *scene)
	: DockContainer(), m_dragReserveId(-1)
{
	scene->addItem(this);
	reposition();

	DockTaskManager::setContainer(this);
	
	connect(this, SIGNAL(parentChanged()), this, SLOT(parentChanged()));
}

qreal DockPanel::getDockLength()
{
	return (getDockLengthEdgeSpacing() * 2 - getLauncherSpacing()) + ((m_widgets.size() + (m_dragReserveId >= 0)) * (DockConfig::iconSize() + getLauncherSpacing()));
}

qreal DockPanel::getDockHeight()
{
	return DockConfig::iconSize() + getDockHeightEdgeSpacing() * 2;
}

qreal DockPanel::getDockLengthEdgeSpacing()
{
	return DockConfig::iconSize() / 1.7;
}

qreal DockPanel::getDockHeightEdgeSpacing()
{
	return DockConfig::iconSize() / 5;
}

qreal DockPanel::getLauncherSpacing()
{
	return DockConfig::iconSize() / 4;
}

QPoint DockPanel::getLauncherPosition(int app_id)
{
	return QPoint(getDockLengthEdgeSpacing() + app_id * (DockConfig::iconSize() + getLauncherSpacing()), getDockHeightEdgeSpacing() / 1.1);
}

void DockPanel::addWidget(QGraphicsWidget *widget)
{
	widget->setParentItem(this);
	m_widgets.append(widget);
	reposition();
}

void DockPanel::removeWidget(QGraphicsWidget *widget)
{
	m_widgets.removeOne(widget);
	reposition();
}

int DockPanel::getClosestWidget(const QPointF &centerPos)
{
	qreal min_distance;
	int min = -1;
	for (int i = 0; i < m_widgets.size() + 1; i++)
	{
		QPointF c = mapToScene(getLauncherPosition(i)) + QPointF(DockConfig::iconSize(), DockConfig::iconSize()) / 2 - centerPos;
		qreal d = qSqrt(c.x() * c.x() + c.y() * c.y()); // Euclidean distance
		if (min == -1 || d < min_distance)
		{
			min_distance = d;
			min = i;
		}
	}
	return min;
}

void DockPanel::dockDragStartEvent(DockDragDropEvent *event)
{
	int id = -1;
	for (int i = 0; i < m_widgets.size(); i++)
		if (m_widgets[i] == event->widget())
		{
			id = i;
			break;
		}
	m_widgets.removeAt(id);
	m_dragReserveId = id;
}

void DockPanel::dockDragEnterEvent(DockDragDropEvent *event)
{
	m_dragReserveId = getClosestWidget(event->widget()->geometry().center());
	reposition();
}

void DockPanel::dockDragMoveEvent(DockDragDropEvent *event)
{
	int newPosition = getClosestWidget(event->widget()->geometry().center());
	if (newPosition != m_dragReserveId)
	{
		m_dragReserveId = newPosition;
		reposition();
	}
}

void DockPanel::dockDragLeaveEvent(DockDragDropEvent *event)
{
	Q_UNUSED(event);
	m_dragReserveId = -1;
	reposition();
}

void DockPanel::dockDropEvent(DockDragDropEvent *event)
{
	QGraphicsWidget *widget = event->widget();
	widget->setParentItem(this);
	m_widgets.insert(m_dragReserveId, widget);
	m_dragReserveId = -1;
	reposition();
}

void DockPanel::parentChanged()
{
	reposition();
}

void DockPanel::reposition()
{
	qreal width = getDockLength(), height = getDockHeight();
	// TODO
	QRectF sceneRect = QApplication::desktop()->geometry();
	resize(width, height);
	setPos((sceneRect.width() - width) / 2, (sceneRect.height() - height));

	for (int i = 0; i < m_widgets.size(); i++)
	{
		int posId = i;
		if (m_dragReserveId >= 0 && i >= m_dragReserveId)
			posId++;
		m_widgets[i]->setGeometry(QRectF(getLauncherPosition(posId), QSize(DockConfig::iconSize(), DockConfig::iconSize())));
	}
}
