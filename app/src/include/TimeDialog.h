/*
 * TimeDialog.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021-2022 - adazem009
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

#ifndef TIMEDIALOG_H
#define TIMEDIALOG_H

#include <QDialog>
#include <QTime>

namespace Ui {
	class TimeDialog;
}

/*!
 * \brief The TimeDialog class is a time selector dialog used for timed exercises.
 *
 * \image html TimeDialog.png
 *
 * Usage example:
 * \code
 * TimeDialog dialog;
 * dialog.exec();
 * int hours, minutes, seconds;
 * hours = dialog.hours; // Get hours
 * minutes = dialog.minutes; // Get minutes
 * seconds = dialog.seconds; // Get seconds
 * \endcode
 */
class TimeDialog : public QDialog
{
		Q_OBJECT
	public:
		explicit TimeDialog(QWidget *parent = nullptr);
		~TimeDialog();
		int hours, minutes, seconds;

	private:
		Ui::TimeDialog *ui;

	private slots:
		void setTime(QTime time);
};

#endif // TIMEDIALOG_H
