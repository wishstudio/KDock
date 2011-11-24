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
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSvgRenderer>
#include <QWidget>

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
	m_hoverAnimation = new QParallelAnimationGroup();
	m_leftAnimation = new QPropertyAnimation(this, "leftOffset");
	m_leftAnimation->setDuration(50);
	m_rightAnimation = new QPropertyAnimation(this, "rightOffset");
	m_rightAnimation->setDuration(50);
	m_hoverAnimation->addAnimation(m_leftAnimation);
	m_hoverAnimation->addAnimation(m_rightAnimation);

	m_svg = new QSvgRenderer(QString("skins/default.svgz"));
	scene->addItem(this);
	configChanged();

	DockTaskManager::setContainer(this);
	setAcceptHoverEvents(true);

	connect(m_hoverAnimation, SIGNAL(finished()), this, SLOT(resetMasks()));
	connect(this, SIGNAL(parentChanged()), this, SLOT(parentChanged()));
	connect(DockConfig::self(), SIGNAL(configChanged()), this, SLOT(configChanged()));
}

DockPanel::~DockPanel()
{
	delete m_svg;
	delete m_hoverAnimation;
	delete m_leftAnimation;
	delete m_rightAnimation;
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
	return QPoint(getReservedLength() + getDockLeftSpacing() + app_id * getIconLength(), getReservedHeight());
}

QSize DockPanel::normalSize() const
{
	return QSize(getDockLength(), getDockHeight());
}

void DockPanel::addWidget(DockIcon *widget)
{
	widget->setParentItem(this);
	m_widgets.append(widget);
	QPropertyAnimation *animation = new QPropertyAnimation(widget, "geometry");
	animation->setDuration(50);
	m_widgetAnimations.append(animation);
	m_hoverAnimation->addAnimation(animation);
	reposition();
	resetMasks();
}

void DockPanel::removeWidget(DockIcon *widget)
{
	int id = m_widgets.indexOf(widget);
	m_widgets.removeAt(id);
	delete m_widgetAnimations.at(id);
	m_widgetAnimations.removeAt(id);
	reposition();
	resetMasks();
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

void DockPanel::setLeftOffset(qreal leftOffset)
{
	m_leftOffset = leftOffset;
	emit update();
}

void DockPanel::setRightOffset(qreal rightOffset)
{
	m_rightOffset = rightOffset;
	emit update();
}

void DockPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	// Left
	m_svg->render(painter, "south-left", QRectF(getReservedLength() - m_leftOffset, getReservedHeight(), getDockLeftSpacing(), getDockHeight()));

	// Center
	m_svg->render(painter, "south-center", QRectF(getReservedLength() - m_leftOffset + getDockLeftSpacing(), getReservedHeight(), getDockCenterSpacing() + m_leftOffset + m_rightOffset, getDockHeight()));

	// Right
	m_svg->render(painter, "south-right", QRectF(getReservedLength() + getDockLeftSpacing() + getDockCenterSpacing() + m_rightOffset, getReservedHeight(), getDockRightSpacing(), getDockHeight()));
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
	DockIcon *widget = static_cast<DockIcon *>(event->widget());
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
	calcMasks();
	resetMasks();
}

QPoint DockPanel::getPanelPosition() const
{
	// TODO
	QSize mysize = size().toSize();
	QRectF sceneRect = QApplication::desktop()->geometry();
	return QPoint((sceneRect.width() - mysize.width()) / 2, sceneRect.height() - mysize.height());
}

void DockPanel::reposition()
{
	QPoint position = getPanelPosition();
	qreal width = getDockLength(), height = getDockHeight();
	resize(width + getReservedLength() * 2, height + getReservedHeight());
	setPos(position);
	hoverAnimation();
	m_view->setViewGeometry(QRect(position, size().toSize()));
}

void DockPanel::resetMasks()
{
	QBitmap mask(getDockLength() + getReservedLength() * 2, getDockHeight() + getReservedHeight());
	QPainter painter(&mask);
	if (m_hoverId == -1)
	{
		mask.fill(Qt::color0);
		painter.drawPixmap(getReservedLength(), getReservedHeight(), getDockLeftSpacing(), getDockHeight(), m_leftMask);
		painter.fillRect(getReservedLength() + getDockLeftSpacing(), getReservedHeight(), getDockCenterSpacing(), getDockHeight(), Qt::color1);
		painter.drawPixmap(getReservedLength() + getDockLeftSpacing() + getDockCenterSpacing(), getReservedHeight(), getDockRightSpacing(), getDockHeight(), m_rightMask);
	}
	else
		mask.fill(Qt::color1);
	m_view->setMask(mask);
}

void DockPanel::calcMasks()
{
	{
		QPixmap pixmap(getDockLeftSpacing(), getDockHeight());
		QPainter painter(&pixmap);
		pixmap.fill(Qt::black);
		m_svg->render(&painter, "south-left");
		m_leftMask = pixmap.createMaskFromColor(Qt::black);
	}
	{
		QPixmap pixmap(getDockRightSpacing(), getDockHeight());
		QPainter painter(&pixmap);
		pixmap.fill(Qt::black);
		m_svg->render(&painter, "south-right");
		m_rightMask = pixmap.createMaskFromColor(Qt::black);
	}
}

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
void DockPanel::hoverAnimation()
{
	for (int i = 0; i < m_widgets.size(); i++)
		m_widgetAnimations[i]->setStartValue(m_widgets[i]->geometry());
	m_hoverAnimation->stop();
	m_leftAnimation->setStartValue(m_leftOffset);
	m_rightAnimation->setStartValue(m_rightOffset);
	if (m_hoverId != -1)
	{
		const QPointF localPos = mapFromScene(m_hoverPos);

		const qreal dLeft = localPos.x() - getLauncherPosition(m_hoverId).x();
		qreal d = dLeft / getIconLength() * getHoverIconSize() - dLeft;
		{
			const QPointF destPos = getLauncherPosition(m_hoverId) + QPointF(-d, -DockConfig::iconSize() * ((getHoverIconSize() - getIconLength()) / getIconLength()));
			const QSizeF destSize = QSizeF(getHoverIconSize(), DockConfig::iconSize() * getHoverIconSize() / getIconLength());
			m_widgetAnimations[m_hoverId]->setEndValue(QRectF(destPos, destSize));
		}
		for (int i = m_hoverId - 1; i >= 0; i--)
		{
			const qreal hSize = qMax((qreal) getIconLength(), getHoverIconSize() - (localPos.x() - (getLauncherPosition(i).x() + getIconLength())) * getHoverStep());
			d += hSize - getIconLength();
			const QPointF destPos = getLauncherPosition(i) + QPointF(-d, -DockConfig::iconSize() * ((hSize - getIconLength()) / getIconLength()));
			const QSizeF destSize = QSizeF(hSize, DockConfig::iconSize() * hSize / getIconLength());
			m_widgetAnimations[i]->setEndValue(QRectF(destPos, destSize));
		}
		m_leftAnimation->setEndValue(d);
		const qreal dRight = getIconLength() - dLeft;
		d = dRight / getIconLength() * getHoverIconSize() - dRight;
		for (int i = m_hoverId + 1; i < m_widgets.size(); i++)
		{
			const qreal hSize = qMax((qreal) getIconLength(), getHoverIconSize() - (getLauncherPosition(i).x() - localPos.x()) * getHoverStep());
			const QPointF destPos = getLauncherPosition(i) + QPointF(d, -DockConfig::iconSize() * ((hSize - getIconLength()) / getIconLength()));
			const QSizeF destSize = QSizeF(hSize, DockConfig::iconSize() * hSize / getIconLength());
			m_widgetAnimations[i]->setEndValue(QRectF(destPos, destSize));
			d += hSize - getIconLength();
		}
		m_rightAnimation->setEndValue(d);
	}
	else
	{
		m_leftAnimation->setEndValue(0);
		m_rightAnimation->setEndValue(0);
		for (int i = 0; i < m_widgets.size(); i++)
		{
			int posId = i;
			if (m_dragReserveId >= 0 && i >= m_dragReserveId)
				posId++;
			//m_widgets[i]->setGeometry(QRectF(getLauncherPosition(posId), QSize(getIconLength(), DockConfig::iconSize())));
			m_widgetAnimations[i]->setEndValue(QRectF(getLauncherPosition(posId), QSize(getIconLength(), DockConfig::iconSize())));
		}
	}
	m_hoverAnimation->start();
}

void DockPanel::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	bool hoverIn = m_hoverId == -1;
	m_hoverId = -1;
	for (int i = 0; i < m_widgets.size(); i++)
	{
		// toPoint() for fuzzy comparison at 0 and w
		QPoint localPos = m_widgets[i]->mapFromScene(event->scenePos()).toPoint();
		if (m_widgets[i]->boundingRect().contains(localPos))
		{
			m_hoverId = i;
			break;
		}
	}
	if (m_hoverId != -1)
	{
		m_hoverPos = event->scenePos();
		if (hoverIn)
			resetMasks();
		hoverAnimation();
	}
	else if (!hoverIn)
		hoverLeaveEvent(event);
}

void DockPanel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	m_hoverId = -1;
	hoverAnimation();
}
