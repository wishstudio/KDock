/*
 *   KDock
 *   Copyright (C) 2011 Xiangyan Sun <wishstudio@gmail.com>
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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KMessageBox>
#include <KLocale>

int main(int argc, char *argv[])
{
	KAboutData aboutData(
		"KDock",
		0,
		ki18n("KDock"),
		"0.1",
		ki18n("A dock for KDE."),
		KAboutData::License_GPL,
		ki18n("(c) KDock Authors 2011"),
		ki18n(""),
		"",
		"");
	KCmdLineArgs::init(argc, argv, &aboutData);
	KApplication app;
	KGuiItem yesButton(i18n("Hello"), QString(), i18n("This is a tooltip"), i18n("This is a WhatsThis help text."));
	KMessageBox::questionYesNo(0, i18n("Hello World"), i18n("Hello"), yesButton);
	return 0;
}
