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

#include <QGraphicsWidget>

#include <KWindowSystem>

#include <Plasma/WindowEffects>

#include "DockProxyView.h"

// TODO: Find a better way for attr
DockProxyView::DockProxyView(QGraphicsWidget *widget)
	: QGraphicsView()
{
	Plasma::WindowEffects::enableBlurBehind(winId(), false);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setStyleSheet("border: 0px; background: transparent;");
	setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
	setFrameShape(QFrame::NoFrame);
	
	KWindowSystem::setState(winId(), NET::SkipTaskbar);
	KWindowSystem::setType(winId(), NET::Dock);
	
	m_widget = NULL;
	if (widget)
		setWidget(widget);
}

void DockProxyView::setWidget(QGraphicsWidget *widget)
{
	if (m_widget)
		disconnect(m_widget, SIGNAL(geometryChanged()), this, SLOT(widgetGeometryChanged()));
	m_widget = widget;
	if (widget)
	{
		setScene(m_widget->scene());
		widgetGeometryChanged();
		connect(m_widget, SIGNAL(geometryChanged()), this, SLOT(widgetGeometryChanged()));
	}
}

void DockProxyView::widgetGeometryChanged()
{
	resize(m_widget->size().toSize());
	move(m_widget->scenePos().toPoint());
	setSceneRect(QRectF(m_widget->scenePos().toPoint(), m_widget->size().toSize()));
}
