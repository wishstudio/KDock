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

#ifndef DOCKAPP_H
#define DOCKAPP_H

#include <QList>
#include <QString>
#include <QObject>

#include <KSharedPtr>
#include <KWindowInfo>

namespace TaskManager
{
class Task;
}

class DockApp: public QObject, public KShared
{
	Q_OBJECT
	
public:
	typedef KSharedPtr<DockApp> Ptr;
	
	DockApp(QString desktop_file, bool isLauncher);
	DockApp(QString executable, QString icon, QString name, QString startup_wm_class);
	void launchApp();
	void toggleLauncher();
	bool runnable();
	
	static bool validQList(const QList<QString> &values);
	static DockApp::Ptr fromQList(const QList<QString> &values);
	QList<QString> toQList();
	
	bool isLauncher();
	QString desktop_file();
	QString executable();
	QString icon();
	QString name();
	QString command();
	QString wm_class();
	
	QList< ::TaskManager::Task *> tasks();
	void clearTasks();
	int countTasks();
	void addTask(::TaskManager::Task *task);
	void removeTask(::TaskManager::Task *task);
	void closeTasks();
	
public slots:
	void taskActivated();
	void taskDeactivated();
	
private:
	bool m_isLauncher;
	QString m_executable, m_icon, m_name, m_command, m_wm_class;
	
	QList< ::TaskManager::Task *> m_tasks;
	
signals:
	void appChanged();
	void appActivated();
	void appDeactivated();
};

#endif
