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
	connect(DockConfig::self(), SIGNAL(configChanged()), this, SLOT(configChanged()));
}

DockIcon::~DockIcon()
{
}

void DockIcon::setIcon(const QIcon &icon)
{
	m_icon = icon;
	configChanged();
}

void DockIcon::setDragable(bool dragable)
{
	m_dragable = dragable;
}

void DockIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	qreal iconSize = size().height();
	painter->drawPixmap((size().width() - iconSize) / 2, 0, iconSize, iconSize, m_pixmap);
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
			setPos(pos() + event->scenePos() - m_startPos);
			dockScene->startDrag(this, event);
		}
	}
}

void DockIcon::configChanged()
{
	m_pixmap = m_icon.pixmap(DockConfig::iconSize() * 2, DockConfig::iconSize() * 2);
	emit update();
}
