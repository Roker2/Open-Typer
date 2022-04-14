/*
 * exportdialog.cpp
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

#include "exportdialog.h"
#include "ui_exportdialog.h"

/*! Constructs exportDialog. */
exportDialog::exportDialog(QString text, QVariantMap result, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::exportDialog),
	inputText(text),
	performanceResult(result)
{
	ui->setupUi(this);
	// Set input text
	QString exportHtml = inputText.toHtmlEscaped().replace(" ", "&nbsp;").replace("\n", "<br>");
	ui->exportText->setHtml(exportHtml);
	// Set font
	QFont textFont = themeEngine::font();
	textFont.setPointSize(12);
	ui->exportText->setFont(textFont);
	// Fill table
	ui->exportTable->setRowCount(10);
	ui->exportTable->setColumnCount(4);
	ui->exportTable->setSpan(0, 0, 1, 4);
	ui->exportTable->setSpan(2, 2, 2, 1);
	ui->exportTable->setSpan(2, 3, 2, 1);
	ui->exportTable->setSpan(4, 2, 4, 1);
	ui->exportTable->setSpan(4, 3, 4, 1);
	ui->exportTable->setSpan(8, 2, 2, 1);
	ui->exportTable->setSpan(8, 3, 2, 1);
	updateTable();
	// Connections
	connect(ui->printButton, &QToolButton::clicked, this, &exportDialog::printResult);
	connect(ui->nameEdit, &QLineEdit::textChanged, this, &exportDialog::updateTable);
	connect(ui->classEdit, &QLineEdit::textChanged, this, &exportDialog::updateTable);
	connect(ui->numberEdit, &QLineEdit::textChanged, this, &exportDialog::updateTable);
	connect(ui->markEdit, &QLineEdit::textChanged, this, &exportDialog::updateTable);
	connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::close);
#ifdef Q_OS_WASM
	ui->printButton->hide();
#endif
}

/*! Destroys the exportDialog object. */
exportDialog::~exportDialog()
{
	delete ui;
}

/*! Updates the table. */
void exportDialog::updateTable(void)
{
	ui->exportTable->clearContents();
	QFont boldFont;
	boldFont.setBold(true);
	boldFont.setPointSize(14);
	QFont boldFont2;
	boldFont2.setBold(true);
	QTableWidgetItem *item;
	// Caption
	item = new QTableWidgetItem(tr("Typewriting performance result"));
	item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item->setFont(boldFont);
	ui->exportTable->setItem(0, 0, item);
	// Name
	item = new QTableWidgetItem(tr("Name: %1").arg(ui->nameEdit->text()));
	item->setFont(boldFont2);
	ui->exportTable->setItem(1, 0, item);
	// Class
	item = new QTableWidgetItem(tr("Class: %1").arg(ui->classEdit->text()));
	item->setFont(boldFont2);
	ui->exportTable->setItem(1, 1, item);
	// Date
	item = new QTableWidgetItem(tr("Date: %1").arg(QDate::currentDate().toString()));
	item->setFont(boldFont2);
	ui->exportTable->setItem(1, 2, item);
	// Number
	item = new QTableWidgetItem(tr("Number: %1").arg(ui->numberEdit->text()));
	item->setFont(boldFont2);
	ui->exportTable->setItem(1, 3, item);
	// Number of hits in finished lines
	ui->exportTable->setItem(2, 0, new QTableWidgetItem(tr("Number of hits in finished lines")));
	ui->exportTable->setItem(2, 1, new QTableWidgetItem(QString::number(performanceResult["finishedLinesHits"].toInt())));
	// Number of hits in unfinished lines
	ui->exportTable->setItem(3, 0, new QTableWidgetItem(tr("Number of hits in unfinished lines")));
	ui->exportTable->setItem(3, 1, new QTableWidgetItem(QString::number(performanceResult["unfinishedLinesHits"].toInt())));
	// Number of gross hits
	item = new QTableWidgetItem(tr("Number of gross hits"));
	item->setFont(boldFont2);
	ui->exportTable->setItem(4, 0, item);
	ui->exportTable->setItem(4, 1, new QTableWidgetItem(QString::number(performanceResult["grossHits"].toInt())));
	// Number of mistakes
	ui->exportTable->setItem(5, 0, new QTableWidgetItem(tr("Number of mistakes")));
	ui->exportTable->setItem(5, 1, new QTableWidgetItem(QString::number(performanceResult["mistakes"].toInt())));
	// Mistake penalty
	ui->exportTable->setItem(6, 0, new QTableWidgetItem(tr("Mistake penalty")));
	ui->exportTable->setItem(6, 1, new QTableWidgetItem(QString::number(performanceResult["penalty"].toInt())));
	// Number of penalty hits
	ui->exportTable->setItem(7, 0, new QTableWidgetItem(tr("Number of penalty hits")));
	int penaltyHits = performanceResult["penalty"].toInt() * performanceResult["mistakes"].toInt();
	ui->exportTable->setItem(7, 1, new QTableWidgetItem(QString::number(penaltyHits)));
	// Number of net hits
	ui->exportTable->setItem(8, 0, new QTableWidgetItem(tr("Number of net hits")));
	ui->exportTable->setItem(8, 1, new QTableWidgetItem(QString::number(performanceResult["netHits"].toInt())));
	// Number of net hits per minute
	item = new QTableWidgetItem(tr("Number of net hits per minute"));
	item->setFont(boldFont2);
	ui->exportTable->setItem(9, 0, item);
	ui->exportTable->setItem(9, 1, new QTableWidgetItem(QString::number(performanceResult["netHitsPerMinute"].toDouble())));
	// Inaccuracy
	ui->exportTable->setItem(2, 2, new QTableWidgetItem(tr("Inaccuracy")));
	double inaccuracy = (performanceResult["mistakes"].toInt() * 100.0) / (double) performanceResult["grossHits"].toInt();
	ui->exportTable->setItem(2, 3, new QTableWidgetItem(QString::number(inaccuracy)));
	// Achieved performance
	ui->exportTable->setItem(4, 2, new QTableWidgetItem(tr("Achieved performance")));
	ui->exportTable->setItem(4, 3, new QTableWidgetItem(QString::number((int) performanceResult["netHitsPerMinute"].toDouble())));
	// Mark
	item = new QTableWidgetItem(tr("Mark"));
	item->setFont(boldFont);
	item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	ui->exportTable->setItem(8, 2, item);
	ui->exportTable->setItem(8, 3, new QTableWidgetItem(ui->markEdit->text()));
	// Adjust size
	ui->exportTable->resizeColumnsToContents();
	ui->exportTable->resizeRowsToContents();
}

/*! Prints the result. */
void exportDialog::printResult(void)
{
#ifndef Q_OS_WASM
	// Set up printer
	QPrinter *printer = new QPrinter(QPrinter::HighResolution);
	QPrintDialog *dialog = new QPrintDialog(printer, this);
	dialog->setWindowTitle(tr("Print result"));
	dialog->setWindowModality(Qt::WindowModal);
	dialog->open();
	connect(dialog, &QDialog::accepted, this, [printer, this]() {
		// Print
		printer->setPageMargins(QMarginsF(25, 25, 15, 25), QPageLayout::Millimeter);
		QPainter painter;
		painter.begin(printer);
		painter.setFont(ui->exportText->font());
		QTextDocument *document = ui->exportText->document()->clone();
		int fontHeight = QFontMetrics(painter.font(), printer).height();
		QStringList lines = inputText.split('\n');
		int relativeLine = 0, page = 0;
		for(int i=0; i < lines.count(); i++)
		{
			if(fontHeight*relativeLine > printer->pageRect(QPrinter::DevicePixel).height())
			{
				printer->newPage();
				relativeLine = 0;
				page++;
			}
			document->setPlainText(lines[i]);
			painter.drawText(QPointF(0, fontHeight*relativeLine), lines[i]);
			relativeLine++;
		}
		double scale = printer->pageRect(QPrinter::DevicePixel).width() / double(ui->exportTable->width());
		painter.scale(scale, scale);
		int tablePos = (printer->pageRect(QPrinter::DevicePixel).height() - (ui->exportTable->height()*scale)) / scale;
		if(((fontHeight*relativeLine) / scale > tablePos) || (page == 0))
			printer->newPage();
		ui->exportTable->render(&painter, QPoint(0, tablePos));
		painter.end();
		delete printer;
	});
#endif // Q_OS_WASM
}