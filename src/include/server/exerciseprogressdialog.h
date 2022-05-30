/*
 * exerciseprogressdialog.h
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

#ifndef EXERCISEPROGRESSDIALOG_H
#define EXERCISEPROGRESSDIALOG_H

#include <QDialog>
#include <QUuid>
#include <QMap>
#include <QPushButton>
#include <QMessageBox>
#include "exportdialog.h"
#include "core/database.h"
#ifndef Q_OS_WASM
#include "core/server.h"
#endif // Q_OS_WASM

namespace exerciseProgressDialogConfig {
	extern int dialogCount;
}

namespace Ui {
	class exerciseProgressDialog;
}

/*!
 * \brief The exerciseProgressDialog class is a dialog for exercise monitoring.
 * \image html exerciseProgressDialog.png
 */
class exerciseProgressDialog : public QDialog
{
	Q_OBJECT
	public:
		explicit exerciseProgressDialog(int classID, QList<int> targets, QWidget *parent = nullptr);
		~exerciseProgressDialog();

	private:
		Ui::exerciseProgressDialog *ui;
		QList<int> exerciseTargets;
		QMap<int, QString> names;
		QMap<int, QVariantMap> results;
		QMap<int, QString> inputTexts;
		QMap<int, bool> abortList;
		QMap<int, QList<QVariantMap>> recordedMistakeLists;
		void setupTable(void);

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void loadResult(int targetID, QList<QVariantMap> recordedMistakes, QString inputText, int grossHits, int netHits, double netHitsPerMinute, int mistakes, double time);
		void abortExercise(int targetID);
		void printAll(void);
};

#endif // EXERCISEPROGRESSDIALOG_H
