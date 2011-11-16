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

#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>

#include "DockApplication.h"

int main(int argc, char *argv[])
{
	KAboutData aboutData(
		"KDock",
		0,
		ki18n("KDock"),
		"0.1",
		ki18n("A dock for KDE."),
		KAboutData::License_GPL,
		ki18n("(c) 2011 KDock Developers"),
		ki18n(""),
		"",
		"");
	aboutData.addAuthor(ki18n("Xiangyan Sun"), ki18n("Main developer"), "wishstudio@gmail.com", "", "");
	KCmdLineArgs::init(argc, argv, &aboutData);
	
	DockApplication *app = DockApplication::self();
	int ret = app->exec();
	delete app;
	return ret;
}
