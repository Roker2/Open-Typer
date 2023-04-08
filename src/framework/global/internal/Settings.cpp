/*
 * Settings.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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

#include <QApplication>
#include "Settings.h"

std::shared_ptr<Settings> Settings::m_instance = std::make_shared<Settings>();
QSettings *Settings::settingsInstance = nullptr;
QSettings *Settings::mainSettingsInstance = nullptr;
bool Settings::frozen = false;
QMap<QPair<QString, QString>, Settings::Key> Settings::m_keys;
#ifdef Q_OS_WASM
bool Settings::tempSettingsCopied = false;
#endif // Q_OS_WASM

/*! Returns the static instance of Settings. */
std::shared_ptr<Settings> Settings::instance()
{
	return m_instance;
}

/*! Initializes settings. Run Settings#init() after the application starts. */
void Settings::init(void)
{
#ifdef Q_OS_WASM
	settingsInstance = new QSettings(qApp);
#else
	settingsInstance = new QSettings(FileUtils::mainSettingsLocation(), QSettings::IniFormat, qApp);
#endif
}

/*! Registers a key. */
void Settings::addKey(QString moduleName, QString keyName, QString key, QVariant defaultValue)
{
	Key k(moduleName, key, defaultValue);
	m_keys.insert({ moduleName, keyName }, k);
}

/*! Sets the key value. */
void Settings::setValue(QString moduleName, QString keyName, QVariant value)
{
	QPair<QString, QString> key = { moduleName, keyName };
	Q_ASSERT(m_keys.contains(key));
	if(m_keys.contains(key))
		set(m_keys[key].key, value);
}

/*! Sets the key value (module name and key name are stored in the Key object). */
void Settings::setValue(Key key, QVariant value)
{
	setValue(key.moduleName, key.key, value);
}

/*! Returns the value of the given key. */
QVariant Settings::getValue(QString moduleName, QString keyName)
{
	QPair<QString, QString> key = { moduleName, keyName };
	Q_ASSERT(m_keys.contains(key));
	if(m_keys.contains(key))
	{
		Key k = m_keys[key];
		QVariant value = get(k.key, k.defaultValue);
		if((value == "true") || (value == "false"))
			value = value.toBool();
		else if(value.toReal() == value)
			value = value.toReal();
		return value;
	}
	else
		return QVariant();
}

/*! Returns the value of the given key (module name and key name are stored in the Key object). */
QVariant Settings::getValue(Key key)
{
	return getValue(key.moduleName, key.key);
}

/*! Returns true if the given key is defined. */
bool Settings::containsKey(QString moduleName, QString keyName)
{
	QPair<QString, QString> key = { moduleName, keyName };
	Q_ASSERT(m_keys.contains(key));
	if(m_keys.contains(key))
	{
		return contains(m_keys[key].key);
	}
	else
		return false;
}

/*! Returns true if the given key is defined (module name and key name are stored in the Key object). */
bool Settings::containsKey(Key key)
{
	return containsKey(key.moduleName, key.key);
}

/*!
 * Switches to temporary settings. You can decide to saveChanges() or discardChanges() later.\n
 * This is useful for settings dialogs with a discard button.
 */
void Settings::freeze(void)
{
	Q_ASSERT(!frozen);
	mainSettingsInstance = settingsInstance;
	settingsInstance = new QSettings(FileUtils::mainSettingsLocation() + ".tmp", QSettings::IniFormat, qApp);
	copySettings(mainSettingsInstance, settingsInstance);
	frozen = true;
	emit stateChanged();
}

/*! Saves changes to real settings and switches back to them. \see freeze() */
void Settings::saveChanges(void)
{
	Q_ASSERT(frozen);
	copySettings(settingsInstance, mainSettingsInstance);
	mainSettingsInstance->sync();
	settingsInstance->clear();
	settingsInstance->deleteLater();
	settingsInstance = mainSettingsInstance;
	frozen = false;
	emit stateChanged();
	emit saved();
}

/*! Discards changes and switches back to real settings. \see freeze() */
void Settings::discardChanges(void)
{
	Q_ASSERT(frozen);
	settingsInstance->clear();
	settingsInstance->deleteLater();
	settingsInstance = mainSettingsInstance;
	frozen = false;
	emit stateChanged();
	emit discarded();
}

/*! Returns true if settings are frozen. \see freeze(). */
bool Settings::isFrozen(void)
{
	return frozen;
}

/*! Returns the value of the given key. */
QVariant Settings::get(QString key, QVariant defaultValue)
{
	Q_ASSERT(settingsInstance != nullptr);
#ifdef Q_OS_WASM
	if(settingsInstance->isWritable())
	{
		if(!tempSettingsCopied)
			copyTempSettings();
		return settingsInstance->value(key, defaultValue);
	}
	else
	{
		// Use temporary settings until sandbox is initialized
		QSettings settings(FileUtils::mainSettingsLocation(), QSettings::IniFormat);
		return settings.value(key, defaultValue);
	}
#else
	return settingsInstance->value(key, defaultValue);
#endif // Q_OS_WASM
}

/*! Returns true if the given key exists. */
bool Settings::contains(QString key)
{
	Q_ASSERT(settingsInstance != nullptr);
#ifdef Q_OS_WASM
	if(settingsInstance->isWritable())
	{
		if(!tempSettingsCopied)
			copyTempSettings();
		return settingsInstance->contains(key);
	}
	else
	{
		// Use temporary settings until sandbox is initialized
		QSettings settings(FileUtils::mainSettingsLocation(), QSettings::IniFormat);
		return settings.contains(key);
	}
#else
	return settingsInstance->contains(key);
#endif // Q_OS_WASM
}

/*! Sets the key value. */
void Settings::set(QString key, QVariant value)
{
	Q_ASSERT(settingsInstance != nullptr);
#ifdef Q_OS_WASM
	if(settingsInstance->isWritable())
	{
		if(!tempSettingsCopied)
			copyTempSettings();
		settingsInstance->setValue(key, value);
		settingsInstance->sync();
	}
	else
	{
		// Use temporary settings until sandbox is initialized
		QSettings settings(FileUtils::mainSettingsLocation(), QSettings::IniFormat);
		settings.setValue(key, value);
	}
#else
	settingsInstance->setValue(key, value);
#endif // Q_OS_WASM
}

/*! Copies values from source settings to target settings. */
void Settings::copySettings(QSettings *source, QSettings *target)
{
#ifndef Q_OS_WASM
	target->clear();
#endif
	QStringList keys = source->allKeys();
	for(int i = 0; i < keys.count(); i++)
		target->setValue(keys[i], source->value(keys[i]));
}

#ifdef Q_OS_WASM
/*! Copies temporary settings to real settings after the WebAssembly sandbox gets initialized. */
void Settings::copyTempSettings(void)
{
	QSettings settings(FileUtils::mainSettingsLocation(), QSettings::IniFormat);
	copySettings(&settings, settingsInstance);
	settingsInstance->sync();
	tempSettingsCopied = true;
}
#endif // Q_OS_WASM

/*! Getter for main/windowX. */
int Settings::windowX(void) { return get("main/windowX", 0).toInt(); }

/*! Returns true if there's a main/windowX key. */
bool Settings::containsWindowX(void) { return contains("main/windowX"); }

/*! Setter for main/windowX. */
void Settings::setWindowX(int value) { set("main/windowX", value); }

/*! Getter for main/windowY. */
int Settings::windowY(void) { return get("main/windowY", 0).toInt(); }

/*! Returns true if there's a main/windowY key. */
bool Settings::containsWindowY(void) { return contains("main/windowY"); }

/*! Setter for main/windowY. */
void Settings::setWindowY(int value) { set("main/windowY", value); }

/*! Getter for main/windowWidth. */
int Settings::windowWidth(void) { return get("main/windowWidth", 1200).toInt(); }

/*! Returns true if there's a main/windowWidth key. */
bool Settings::containsWindowWidth(void) { return contains("main/windowWidth"); }

/*! Setter for main/windowWidth. */
void Settings::setWindowWidth(int value) { set("main/windowWidth", value); }

/*! Getter for main/windowHeight. */
int Settings::windowHeight(void) { return get("main/windowHeight", 800).toInt(); }

/*! Returns true if there's a main/windowHeight key. */
bool Settings::containsWindowHeight(void) { return contains("main/windowHeight"); }

/*! Setter for main/windowHeight. */
void Settings::setWindowHeight(int value) { set("main/windowHeight", value); }

/*! Getter for main/windowMaximized. */
bool Settings::windowMaximized(void) { return get("main/windowMaximized", true).toBool(); }

/*! Returns true if there's a main/windowMaximized key. */
bool Settings::containsWindowMaximized(void) { return contains("main/windowMaximized"); }

/*! Setter for main/windowMaximized. */
void Settings::setWindowMaximized(bool value) { set("main/windowMaximized", value); }

// updateChecks

/*! Getter for main/updatechecks. */
bool Settings::updateChecks(void) { return get("main/updatechecks", true).toBool(); }

/*! Returns true if there's a main/updatechecks key. */
bool Settings::containsUpdateChecks(void) { return contains("main/updatechecks"); }

/*! Setter for main/updatechecks. */
void Settings::setUpdateChecks(bool value) { set("main/updatechecks", value); }

// lessonPack

/*! Getter for main/configfile. */
QString Settings::lessonPack(void) { return get("main/configfile", "").toString(); }

/*! Returns true if there's a main/configfile key. */
bool Settings::containsLessonPack(void) { return contains("main/configfile"); }

/*! Setter for main/configfile. */
void Settings::setLessonPack(QString value) { set("main/configfile", value); }

// customLessonPack

/*! Getter for main/customconfig. */
bool Settings::customLessonPack(void) { return get("main/customconfig", false).toBool(); }

/*! Returns true if there's a main/customconfig key. */
bool Settings::containsCustomLessonPack(void) { return contains("main/customconfig"); }

/*! Setter for main/customconfig. */
void Settings::setCustomLessonPack(bool value) { set("main/customconfig", value); }

// errorPenalty

/*! Getter for main/errorpenalty. */
int Settings::errorPenalty(void) { return get("main/errorpenalty", 10).toInt(); }

/*! Returns true if there's a main/errorpenalty key. */
bool Settings::containsErrorPenalty(void) { return contains("main/errorpenalty"); }

/*! Setter for main/errorpenalty. */
void Settings::setErrorPenalty(int value) { set("main/errorpenalty", value); }

// mistakeLimit

/*! Getter for main/mistakelimit. */
bool Settings::mistakeLimit(void) { return get("main/mistakelimit", true).toBool(); }

/*! Returns true if there's a main/mistakelimit key. */
bool Settings::containsMistakeLimit(void) { return contains("main/mistakelimit"); }

/*! Setter for main/mistakelimit. */
void Settings::setMistakeLimit(bool value) { set("main/mistakelimit", value); }

// mistakeChars

/*! Getter for main/mistakechars. */
int Settings::mistakeChars(void) { return get("main/mistakechars", 6).toInt(); }

/*! Returns true if there's a main/mistakechars key. */
bool Settings::containsMistakeChars(void) { return contains("main/mistakechars"); }

/*! Setter for main/mistakechars. */
void Settings::setMistakeChars(int value) { set("main/mistakechars", value); }

// editorGeometry

/*! Getter for main/editorGeometry. \deprecated The pack editor window doesn't use "geometry" anymore. */
QByteArray Settings::editorGeometry(void) { return get("main/editorGeometry", "").toByteArray(); }

/*! Returns true if there's a main/editorGeometry key. \deprecated The pack editor window doesn't use "geometry" anymore. */
bool Settings::containsEditorGeometry(void) { return contains("main/editorGeometry"); }

/*! Setter for main/editorGeometry. \deprecated The pack editor window doesn't use "geometry" anymore. */
void Settings::setEditorGeometry(QByteArray value) { set("main/editorGeometry", value); }

// initFinished

/*! Getter for main/initfinished. */
bool Settings::initFinished(void) { return get("main/initfinished", false).toBool(); }

/*! Returns true if there's a main/initfinished key. */
bool Settings::containsInitFinished(void) { return contains("main/initfinished"); }

/*! Setter for main/initfinished. */
void Settings::setInitFinished(bool value) { set("main/initfinished", value); }
