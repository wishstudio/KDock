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

#include <KConfigDialog>
#include <KGlobal>

#include "DockConfig.h"
#include "DockConfigDialog.h"

DockConfigDialog::DockConfigDialog()
{
	widget_appearanceConfig = new QWidget();
	appearanceConfig.setupUi(widget_appearanceConfig);

	widget_behaviorConfig = new QWidget();
	behaviorConfig.setupUi(widget_behaviorConfig);
}

DockConfigDialog::~DockConfigDialog()
{
	delete widget_appearanceConfig;
}

K_GLOBAL_STATIC(DockConfigDialog, globalConfigDialog)

DockConfigDialog *DockConfigDialog::self()
{
	return globalConfigDialog;
}

void DockConfigDialog::showConfigDialogPrivate(QWidget *parent)
{
	if (KConfigDialog::showDialog("KDockConfigDialog"))
		return;

	KConfigDialog *dialog = new KConfigDialog(parent, "KDockConfigDialog", DockConfig::self());

	dialog->addPage(widget_appearanceConfig, i18n("Appearance"), "preferences-desktop-theme");
	dialog->addPage(widget_behaviorConfig, i18n("Behavior"), "configure-toolbars");
	dialog->resize(640, 480);

	connect(dialog, SIGNAL(settingsChanged(QString)), this, SIGNAL(settingsChanged()));

	dialog->show();
}
