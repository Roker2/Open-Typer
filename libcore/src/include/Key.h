/*
 * Key.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

#ifndef KEY_H
#define KEY_H

#include <QObject>

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

class CORE_LIB_EXPORT Key
{
		Q_GADGET
		Q_PROPERTY(QString text READ text WRITE setText)
		Q_PROPERTY(QString shiftText READ shiftText WRITE setShiftText)
	public:
		explicit Key();
		Key(QString text, QString shiftText);
		QString text(void);
		void setText(QString text);
		QString shiftText(void);
		void setShiftText(QString text);

	private:
		QString m_text;
		QString m_shiftText;
};

typedef QList<Key> KeyboardRow;

#endif // KEY_H
