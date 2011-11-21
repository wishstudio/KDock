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

#include <Qt>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QList>
#include <QSvgRenderer>
#include <QWidget>
#include <QDebug>

#include <KApplication>
#include <KIcon>
#include <KStandardDirs>

#include "AppMatcher.h"
#include "DockConfig.h"
#include "DockConfigDialog.h"
#include "DockContainer.h"
#include "DockGraphicsScene.h"
#include "DockIcon.h"
#include "DockPanel.h"
#include "DockPanelView.h"
#include "DockTaskManager.h"

DockPanel::DockPanel(DockGraphicsScene *scene, DockPanelView *view)
	: DockContainer(), m_view(view), m_leftOffset(0), m_rightOffset(0), m_hoverId(-1), m_dragReserveId(-1)
{
	m_svg = new QSvgRenderer(QString("skins/default.svgz"));
	scene->addItem(this);
	reposition();

	DockTaskManager::setContainer(this);
	setAcceptHoverEvents(true);

	connect(this, SIGNAL(parentChanged()), this, SLOT(parentChanged()));
	connect(DockConfig::self(), SIGNAL(configChanged()), this, SLOT(configChanged()));
}

DockPanel::~DockPanel()
{
	delete m_svg;
}

qreal DockPanel::getHoverIconSize() const
{
	return DockConfig::iconSize() * 1.5;
}

qreal DockPanel::getHoverStep() const
{
	return (getHoverIconSize() - getIconLength()) / (2 * getIconLength());
}

// This need to be adjusted when changing getHoverStep()
int DockPanel::getReservedLength() const
{
	return (getHoverIconSize() - getIconLength()) * 2;
}

int DockPanel::getReservedHeight() const
{
	return getHoverIconSize() + getDockHeightEdgeSpacing() - getDockHeight();
}

int DockPanel::getIconLength() const
{
	return DockConfig::iconSize() * 1.1;
}

int DockPanel::getDockLength() const
{
	return getDockLeftSpacing() + getDockCenterSpacing() + getDockRightSpacing();
}

int DockPanel::getDockHeight() const
{
	return DockConfig::iconSize() + getDockHeightEdgeSpacing();
}

int DockPanel::getDockCenterSpacing() const
{
	return (m_widgets.size() + (m_dragReserveId >= 0)) * getIconLength();
}

int DockPanel::getDockLeftSpacing() const
{
	QRectF rect = m_svg->boundsOnElement("south-left");
	return rect.width() / rect.height() * getDockHeight();
}

int DockPanel::getDockRightSpacing() const
{
	QRectF rect = m_svg->boundsOnElement("south-right");
	return rect.width() / rect.height() * getDockHeight();
}

int DockPanel::getDockHeightEdgeSpacing() const
{
	return DockConfig::iconSize() / 4;
}

QPoint DockPanel::getLauncherPosition(int app_id) const
{
	return QPoint(getDockLeftSpacing() + app_id * getIconLength(), 0);
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

int DockPanel::getClosestWidget(const QPointF &centerPos) const
{
	qreal min_distance;
	int min = -1;
	for (int i = 0; i < m_widgets.size() + 1; i++)
	{
		// Current position may not be the final position, mapToScene would get incorrect values
		// So here we add up the difference
		QPointF c = mapToScene(getLauncherPosition(i)) + QPointF(DockConfig::iconSize(), DockConfig::iconSize()) / 2 + getPanelPosition() - pos();
		qreal d = (c - centerPos).manhattanLength();
		if (min == -1 || d < min_distance)
		{
			min_distance = d;
			min = i;
		}
	}
	return min;
}

void DockPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	// Left
	m_svg->render(painter, "south-left", QRectF(-m_leftOffset, 0, getDockLeftSpacing(), getDockHeight()));

	// Center
	m_svg->render(painter, "south-center", QRectF(-m_leftOffset + getDockLeftSpacing(), 0, getDockCenterSpacing() + m_leftOffset + m_rightOffset, getDockHeight()));

	// Right
	m_svg->render(painter, "south-right", QRectF(getDockLeftSpacing() + getDockCenterSpacing() + m_rightOffset, 0, getDockRightSpacing(), getDockHeight()));
}

void DockPanel::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	Q_UNUSED(event);
	// TODO
	DockConfigDialog::showConfigDialog(QApplication::desktop());
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
	// A fake dragReserveId to correct position calculation
	m_dragReserveId = 0;
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

void DockPanel::configChanged()
{
	reposition();
}

QPoint DockPanel::getPanelPosition() const
{
	// TODO
	qreal width = getDockLength(), height = getDockHeight();
	QRectF sceneRect = QApplication::desktop()->geometry();
	return QPoint((sceneRect.width() - width) / 2, sceneRect.height() - height);
}

void DockPanel::reposition()
{
	for (int i = 0; i < m_widgets.size(); i++)
	{
		int posId = i;
		if (m_dragReserveId >= 0 && i >= m_dragReserveId)
			posId++;
		m_widgets[i]->setGeometry(QRectF(getLauncherPosition(posId), QSize(getIconLength(), DockConfig::iconSize())));
	}
	QPoint position = getPanelPosition();
	qreal width = getDockLength(), height = getDockHeight();
	resize(width, height);
	setPos(position);
	hoverTransform();
	m_view->setViewGeometry(QRect(position - QPoint(getReservedLength(), getReservedHeight()), QSize(width + getReservedLength() * 2, height + getReservedHeight())));
}

void DockPanel::calcCenterMask()
{
}

void DockPanel::calcAllMask()
{
	calcCenterMask();
}

void DockPanel::hoverTransform()
{
	for (int i = 0; i < m_widgets.size(); i++)
		m_widgets[i]->resetTransform();
	if (m_hoverId != -1)
	{
		// Position calculation algorithm:
		//
		// Original:
		//              hId
		//     +--+--+--+--+--+--+--+
		//     |  |  |  |M |  |  |  |
		//  ---+--+--+--+--+--+--+--+-
		//
		// Transformed:
		//
		//               hId
		//              +----+
		//          +---+    +---+
		//    +--+--+   | M  |   +--+--+
		//    |  |  |   |    |   |  |  |
		//  --+--+--+---+----+---+--+--+--
		//
		// We have to keep the mouse position consistent with the scaled image
		// Thus we have
		//
		//   (M.x - hId.x) / old_size = (M.x' - hId.x') / hover_size
		//
		// Then for left and right icons, decrease their size by their distance to mouse point:
		//
		//   id.size = max(hover_size - dist * step, old_size)
		//
		// where step is a constant, dist is M.x - id.right for left icons, id.left - M.x for right icons

		const QPointF localPos = mapFromScene(m_hoverPos);

		const qreal dLeft = localPos.x() - m_widgets[m_hoverId]->x();
		qreal d = dLeft / getIconLength() * getHoverIconSize() - dLeft;
		m_widgets[m_hoverId]->translate(-d, -m_widgets[m_hoverId]->size().height() * ((getHoverIconSize() - getIconLength()) / getIconLength()));
		m_widgets[m_hoverId]->scale(getHoverIconSize() / getIconLength(), getHoverIconSize() / getIconLength());
		for (int i = m_hoverId - 1; i >= 0; i--)
		{
			qreal hSize = qMax((qreal) getIconLength(), getHoverIconSize() - (localPos.x() - m_widgets[i]->geometry().right()) * getHoverStep());
			d += hSize - getIconLength();
			m_widgets[i]->translate(-d, -m_widgets[i]->size().height() * ((hSize - getIconLength()) / getIconLength()));
			m_widgets[i]->scale(hSize / getIconLength(), hSize / getIconLength());
		}
		m_leftOffset = d;
		qreal dRight = getIconLength() - dLeft;
		d = dRight / getIconLength() * getHoverIconSize() - dRight;
		for (int i = m_hoverId + 1; i < m_widgets.size(); i++)
		{
			qreal hSize = qMax((qreal) getIconLength(), getHoverIconSize() - (m_widgets[i]->geometry().left() - localPos.x()) * getHoverStep());
			m_widgets[i]->translate(d, -m_widgets[i]->size().height() * ((hSize - getIconLength()) / getIconLength()));
			m_widgets[i]->scale(hSize / getIconLength(), hSize / getIconLength());
			d += hSize - getIconLength();
		}
		m_rightOffset = d;
	}
	else
		m_leftOffset = m_rightOffset = 0;
	setWindowFrameMargins(m_leftOffset, 0, m_rightOffset, 0);
}

void DockPanel::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	m_hoverId = -1;
	for (int i = 0; i < m_widgets.size(); i++)
	{
		// toPoint() for FuzzyCompare at 0 and w
		QPoint localPos = m_widgets[i]->mapFromScene(event->scenePos()).toPoint();
		if (m_widgets[i]->boundingRect().contains(localPos))
		{
			m_hoverId = i;
			break;
		}
	}
	m_hoverPos = event->scenePos();
	hoverTransform();
}

void DockPanel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	m_hoverId = -1;
	hoverTransform();
}
