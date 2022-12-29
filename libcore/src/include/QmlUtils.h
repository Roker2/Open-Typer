/*
 * QmlUtils.h
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

#ifndef QMLUTILS_H
#define QMLUTILS_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QQuickItem>

/*! \brief The QmlUtils class acts like a bridge for QML code. */
class CORE_LIB_EXPORT QmlUtils : public QObject
{
		Q_OBJECT
		Q_PROPERTY(QQuickItem *blurSource READ blurSource WRITE setBlurSource NOTIFY blurSourceChanged)
	public:
		enum StandardIcon
		{
			NoIcon = 0,
			Information = 1,
			Warning = 2,
			Critical = 3,
			Question = 4
		};
		Q_ENUM(StandardIcon)

		void setBlurSource(QQuickItem *item);
		QQuickItem *blurSource(void);
		Q_INVOKABLE static bool nativeMenuBar(void);
		Q_INVOKABLE static int qtVersionMajor(void);
		Q_INVOKABLE static int qtVersionMinor(void);
		Q_INVOKABLE static int qtVersionPatch(void);
		Q_INVOKABLE static bool osWasm(void);
		Q_INVOKABLE void reloadMenuBar(void);
		Q_INVOKABLE static void printExercise(int lesson, int sublesson, int exercise, QString text);
		Q_INVOKABLE static void printExercise(QString text);
		static QPixmap standardIcon(StandardIcon icon);
		Q_INVOKABLE static QString standardIconStr(StandardIcon icon);

	private:
		static QString convertPixmap(QPixmap pixmap);
		QQuickItem *m_blurSource = nullptr;

	signals:
		void blurSourceChanged(QQuickItem *item);
		void menuBarReloadTriggered(void);
};

#endif // QMLUTILS_H