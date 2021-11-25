/*
 * configfile.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "utils.h"

#pragma message "TODO: Remove _REPEAT_LIMIT"
#define _REPEAT_LIMIT 128 // deprecated

class configParser : public QObject
{
	Q_OBJECT
	public:
		explicit configParser(QObject *parent = nullptr);
		static const int defaultRepeatLimit = 128;
		static const int defaultLineLength = 60;
		bool open(const QString fileName);
		int lessonCount(void);
		int sublessonCount(int lesson);
		int exerciseCount(int lesson, int sublesson);
		int exerciseLine(int lesson, int sublesson, int exercise);
		bool exerciseRepeatBool(int lesson, int sublesson, int exercise);
		QString exerciseRepeatType(int lesson, int sublesson, int exercise);
		int exerciseRepeatLimit(int lesson, int sublesson, int exercise);
		int exerciseLineLength(int lesson, int sublesson, int exercise);
		QString lessonDesc(int lesson);
		QString exerciseRawText(int lesson, int sublesson, int exercise);

	private:
		QFile *configFile;
		int exerciseID(const QString line, const int part);
		bool exerciseRepeatBool(const QString config);
		QString exerciseRepeatType(const QString config);
		QString exerciseRepeatConfig(const QString line);
		QString exerciseAttribute(const QString config, const int id);
		QString exerciseAttributes(const QString line);
		QString exerciseRawText(const QString line);
		QString generateText(QString rawText, bool repeat, QString repeatType, int repeatLimit);
};

int _lesson_count(FILE *cr) __attribute__ ((deprecated));
int _lesson_sublesson_count(FILE *cr, int tlesson) __attribute__ ((deprecated));
int _lesson_sublesson_level_count(FILE *cr, int tlesson, int tsublesson) __attribute__ ((deprecated));
int _lesson_sublesson_level_line(FILE *cr, int tlesson, int tsublesson, int tlevel) __attribute__ ((deprecated));
char* _lesson_sublesson_level_repeat_type(FILE *cr, int tlesson, int tsublesson, int tlevel) __attribute__ ((deprecated));
char *_lesson_desc(FILE *cr, int tlesson) __attribute__ ((deprecated));
int _lesson_sublesson_level_limit_extension(FILE *cr, int tlesson, int tsublesson, int tlevel) __attribute__ ((deprecated));
int _lesson_sublesson_level_length_extension(FILE *cr, int tlesson, int tsublesson, int tlevel) __attribute__ ((deprecated));
char* _lesson_sublesson_level_raw_text(FILE *cr, int tlesson, int tsublesson, int tlevel) __attribute__ ((deprecated));
char *_lesson_sublesson_level_text(FILE *cr, int tlesson, int tsublesson, int tlevel);
// Write functions
void _add_level(FILE *cr, int tlesson, int tsublesson, int tlevel, bool repeat, char *repeat_type, int limit_extension, int length_extension, char *desc, const char *level_text);

#endif // CONFIGFILE_H
