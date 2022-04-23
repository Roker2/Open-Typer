/*
 * servermanager.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
 *
 * Open-Typer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Open-Typer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Typer. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QWidget>
#include "server/usermanager.h"
#include "server/adminselector.h"
#include "server/serversetup.h"
#include "server/classedit.h"
#include "server/widgets/classcontrols.h"
#include "server/widgets/studentdetails.h"

namespace Ui {
	class serverManager;
}

/*!
 * \brief The serverManager class is a widget that can be used as a server control panel.
 *
 * \image html serverManager.png
 */
class serverManager : public QWidget
{
	Q_OBJECT
	public:
		explicit serverManager(QWidget *parent = nullptr);
		~serverManager();

	private:
		Ui::serverManager *ui;
		bool expanded = true;
		QList<int> classes;
		bool disableClassOpening = false;

	signals:
		/*! A signal, which is emitted when the widget is collapsed. */
		void widgetCollapsed();
		/*! A signal, which is emitted when the widget is expanded. */
		void widgetExpanded();

	public slots:
		void collapse(void);
		void expand(void);

	private slots:
		bool init(void);
		void openUserManager(void);
		void changeSchoolName(void);
		void addClass(void);
		void openClass(void);
		void openDetails(int studentID);
		void removeClass(void);
};

#endif // SERVERMANAGER_H