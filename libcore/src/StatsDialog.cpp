/*
 * StatsDialog.cpp
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

#include "StatsDialog.h"
#include "ui_StatsDialog.h"

/*! Constructs StatsDialog. */
StatsDialog::StatsDialog(bool offline, QList<QStringList> data, QPair<int, int> studentComparison, QString configName, int lesson, int sublesson, int exercise, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::StatsDialog)
{
	ui->setupUi(this);
	ui->statsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->statsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	// Load data
	QStringList response;
	if(offline)
		response = QStringList({ QString("ok"), QString::number(HistoryParser::historySize(configName, lesson, sublesson, exercise)) });
	else
		response = QStringList({ QString("ok"), QString::number(data.count()) });
	if(response[0] != "ok")
	{
		QMetaObject::invokeMethod(this, "reject", Qt::QueuedConnection);
		return;
	}
	ui->statsTable->clear();
	ui->statsTable->setColumnCount(3);
	ui->statsTable->setHorizontalHeaderLabels({ tr("Speed"), tr("Mistakes"), tr("Time") });
	int i, count = response[1].toInt();
	ui->statsTable->setRowCount(count);
	speedSeries = new QLineSeries;
	mistakesSeries = new QLineSeries;
	timeSeries = new QLineSeries;
	QTableWidgetItem *item;
	for(i = 0; i < count; i++)
	{
		if(offline)
		{
			response = QStringList({ "ok" });
			QStringList entry = HistoryParser::historyEntry(configName, lesson, sublesson, exercise, i);
			for(int i2 = 0; i2 < entry.count(); i2++)
				response += entry[i2].toUtf8();
		}
		else
			response = data[i];
		if(response[0] != "ok")
		{
			QMetaObject::invokeMethod(this, "reject", Qt::QueuedConnection);
			return;
		}
		// Speed
		item = new QTableWidgetItem(QString(response[1]));
		ui->statsTable->setItem(i, 0, item);
		speedSeries->append(i, response[1].toInt());
		// Mistakes
		item = new QTableWidgetItem(QString(response[2]));
		ui->statsTable->setItem(i, 1, item);
		mistakesSeries->append(i, response[2].toInt());
		// Time
		item = new QTableWidgetItem(QString(response[3]));
		ui->statsTable->setItem(i, 2, item);
		timeSeries->append(i, response[3].toInt());
	}
	// Set up charts
	// Speed
	speedChart = new QChart;
	QChartView *speedChartView = new QChartView(speedChart, ui->chartTab);
	ui->chartTabLayout->addWidget(speedChartView);
	speedChart->addSeries(speedSeries);
	speedChart->legend()->hide();
	speedChart->createDefaultAxes();
	speedChart->axes(Qt::Vertical).value(0)->setMin(0);
	speedChart->setTitle(tr("Speed"));
	// Mistakes
	mistakesChart = new QChart;
	QChartView *mistakesChartView = new QChartView(mistakesChart, ui->chartTab);
	ui->chartTabLayout->addWidget(mistakesChartView);
	mistakesChart->addSeries(mistakesSeries);
	mistakesChart->legend()->hide();
	mistakesChart->createDefaultAxes();
	mistakesChart->axes(Qt::Vertical).value(0)->setMin(0);
	mistakesChart->setTitle(tr("Mistakes"));
	// Time
	timeChart = new QChart;
	QChartView *timeChartView = new QChartView(timeChart, ui->chartTab);
	ui->chartTabLayout->addWidget(timeChartView);
	timeChart->addSeries(timeSeries);
	timeChart->legend()->hide();
	timeChart->createDefaultAxes();
	timeChart->axes(Qt::Vertical).value(0)->setMin(0);
	timeChart->setTitle(tr("Time"));
	// Set charts theme
	QChart::ChartTheme theme;
	if(Settings::applicationStyle() == 1)
		theme = QChart::ChartThemeDark;
	else
		theme = QChart::ChartThemeLight;
	speedChart->setTheme(theme);
	mistakesChart->setTheme(theme);
	timeChart->setTheme(theme);
	// Load comparison data
	if(offline)
	{
		ui->betterStudentsLabel->hide();
		ui->worseStudentsLabel->hide();
	}
	else
	{
		ui->betterStudentsLabel->setText(tr("Better students: %1").arg(studentComparison.first));
		ui->worseStudentsLabel->setText(tr("Worse students: %1").arg(studentComparison.second));
	}
	// Connections
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

/*! Destroys the StatsDialog object. */
StatsDialog::~StatsDialog()
{
	delete ui;
}
