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

#ifndef DOCKCONFIGDIALOG_H
#define DOCKCONFIGDIALOG_H

#include <QObject>

#include "ui_AppearanceConfig.h"
#include "ui_BehaviorConfig.h"

class DockConfigDialog : public QObject
{
	Q_OBJECT

public:
	DockConfigDialog();
	virtual ~DockConfigDialog();

	static DockConfigDialog *self();

	static void showConfigDialog(QWidget *parent = NULL) { return self()->showConfigDialogPrivate(parent); }

signals:
	void settingsChanged();

private:
	void showConfigDialogPrivate(QWidget *parent = NULL);

	QWidget *widget_appearanceConfig, *widget_behaviorConfig;
	Ui::AppearanceConfig appearanceConfig;
	Ui::BehaviorConfig behaviorConfig;
};

#endif
