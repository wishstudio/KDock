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

#ifndef DOCKWINDOW_H
#define DOCKWINDOW_H

#include <QBitmap>

#include <KMenu>

#include <taskmanager/taskmanager.h>

#include "DockApp.h"
#include "DockContainer.h"

class QParallelAnimationGroup;
class QPropertyAnimation;
class QSvgRenderer;
class QWidget;
class DockGraphicsScene;
class DockIcon;
class DockPanelView;
class DockPanel: public DockContainer
{
	Q_OBJECT
	Q_PROPERTY(qreal leftOffset READ leftOffset WRITE setLeftOffset)
	Q_PROPERTY(qreal rightOffset READ rightOffset WRITE setRightOffset)
	
public:
	DockPanel(DockGraphicsScene *scene, DockPanelView *view);
	virtual ~DockPanel();

	QSize normalSize() const;
	void addWidget(DockIcon *widget);
	void removeWidget(DockIcon *widget);
	qreal leftOffset() const { return m_leftOffset; }
	void setLeftOffset(qreal leftOffset);
	qreal rightOffset() const { return m_rightOffset; }
	void setRightOffset(qreal rightOffset);
	
public slots:
	void parentChanged();
	void configChanged();
	void resetMasks();

signals:
	void setViewGeometry(QRect rect);

protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	virtual void dockDragStartEvent(DockDragDropEvent *event);
	virtual void dockDragEnterEvent(DockDragDropEvent *event);
	virtual void dockDragMoveEvent(DockDragDropEvent *event);
	virtual void dockDragLeaveEvent(DockDragDropEvent *event);
	virtual void dockDropEvent(DockDragDropEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	
private:
	qreal getHoverIconSize() const;
	qreal getHoverStep() const;
	int getReservedLength() const;
	int getReservedHeight() const;
	int getIconLength() const;
	int getDockLength() const;
	int getDockHeight() const;
	int getDockLeftSpacing() const;
	int getDockCenterSpacing() const;
	int getDockRightSpacing() const;
	int getDockHeightEdgeSpacing() const;
	int getClosestWidget(const QPointF &centerPos) const;

	QPoint getLauncherPosition(int app_id) const;
	QPoint getPanelPosition() const;
	void reposition();
	void calcMasks();
	void hoverAnimation();

	DockPanelView *m_view;

	QSvgRenderer *m_svg;
	qreal m_leftOffset, m_rightOffset;
	QBitmap m_leftMask, m_centerMask, m_rightMask;
	QBitmap m_normalMask, m_hoverMask;

	QParallelAnimationGroup *m_hoverAnimation;
	QPropertyAnimation *m_leftAnimation, *m_rightAnimation;
	QList<QPropertyAnimation *> m_widgetAnimations;

	int m_hoverId;
	QPointF m_hoverPos;

	int m_dragReserveId;
	QList<DockIcon *> m_widgets;
	KMenu m_contextMenu;
};

#endif
