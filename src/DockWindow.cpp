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
#include <QDesktopWidget>
#include <QWidget>

#include <KApplication>
#include <KMainWindow>
#include <KWindowSystem>

#include "DockWindow.h"

DockWindow::DockWindow(QWidget *parent):
	KMainWindow(parent, Qt::CustomizeWindowHint | Qt::FramelessWindowHint)
{
	setAttribute(Qt::WA_TranslucentBackground, true);
	reposition();
}

void DockWindow::reposition()
{
	qreal width = 100, height = 100;
	QDesktopWidget *desktop = KApplication::desktop();
	KWindowSystem::setStrut(winId(), 0, 0, 0, height);
	KWindowSystem::setState(winId(), NET::SkipTaskbar);
	KWindowSystem::setType(winId(), NET::Dock);
	setFixedSize(width, height);
	move((desktop->width() - width) / 2, (desktop->height() - height));
}
