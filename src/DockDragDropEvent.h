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

#ifndef DOCKDRAGEVENT_H
#define DOCKDRAGEVENT_H

#include <QEvent>

class QGraphicsWidget;
class DockDragDropEvent: public QEvent
{
public:
	DockDragDropEvent(QEvent::Type type, QGraphicsWidget *widget);

	QGraphicsWidget *widget() const { return m_widget; }
	
	static QEvent::Type DragStart, DragEnter, DragMove, DragLeave, Drop;
	
private:
	QGraphicsWidget *m_widget;
};

#endif
