/*
 * MonitorServer.h
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

#ifndef MONITORSERVER_H
#define MONITORSERVER_H

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QNetworkInterface>
#include <QSslCertificate>
#include <QSslKey>
#include <QPointer>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include "DatabaseManager.h"
#include "Settings.h"

/*! \brief The MonitorServer class is a QWebSocketServer that is used to communicate with Open-Typer clients. */
class CORE_LIB_EXPORT MonitorServer : public QWebSocketServer
{
		Q_OBJECT
	public:
		explicit MonitorServer(bool silent = false, QObject *parent = nullptr);
		static quint16 port(void);
		static QHostAddress address(void);
		void sendSignal(QByteArray name, QStringList data, QList<QByteArray> usernames);
		void sendSignal(QByteArray name, QStringList data, QList<QHostAddress> addresses);
		bool isLoggedIn(QString username);
		bool isConnected(QHostAddress address);
		QList<int> runningExerciseStudents(void);
		QString deviceStudentName(int deviceID);
		void setDeviceStudentName(int deviceID, QString name);

	protected:
		void incomingConnection(qintptr socketDescriptor);

	signals:
		/*! A signal, which is emitted, when a student uploads a monitored exercise result. */
		void resultUploaded(int, QString, QVector<QPair<QString, int>>, qreal);
		/*! A signal, which is emitted, when a student aborts a monitored exercise. */
		void exerciseAborted(int);
		/*! A signal, which is emitted, when a student logs in/out. */
		void loggedInStudentsChanged();
		/*! A signal, which is emitted, when a device connects or disconnects (works only in easy mode). */
		void connectedDevicesChanged();
		/*! A signal, which is emitted, when a device changes its configuration (student name, etc..; works only in easy mode). \see deviceStudentName() */
		void deviceConfigurationChanged();

	private slots:
		void acceptConnection(void);
		void disconnectClient(void);
		void sendResponse(QString message);

	private:
		QList<QWebSocket *> clientSockets;
		QString convertData(bool *ok, QStringList input);
		QString convertData(QStringList input);
		QStringList readData(QString input);
		bool studentAuthAvailable(QString nickname);
		void generateRandomCertKey(void);
		QMap<QWebSocket *, QString> sessions; /*!< Stores student sessions (socket, username). */
		QList<QWebSocket *> exerciseSockets;
		QMap<QWebSocket *, QVector<QPair<QString, int>>> recordedCharacters;
		QMap<int, QString> deviceStudentNames;
};

extern QPointer<MonitorServer> serverPtr;

#endif // MONITORSERVER_H