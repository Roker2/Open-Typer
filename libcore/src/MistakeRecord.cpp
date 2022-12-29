/*
 * MistakeRecord.cpp
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

#include <QQmlEngine>
#include "MistakeRecord.h"

/*! Constructs MistakeRecord. */
MistakeRecord::MistakeRecord(QObject *parent) :
	QObject(parent)
{
	QQmlEngine::setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

/*! Sets mistake position. */
void MistakeRecord::setPosition(int pos)
{
	m_position = pos;
	emit positionChanged(pos);
}

/*! Returns mistake position. */
int MistakeRecord::position(void)
{
	return m_position;
}

/*! Sets mistake type. */
void MistakeRecord::setType(MistakeRecord::Type type)
{
	m_type = type;
	emit typeChanged(type);
}

/*! Returns mistake type. */
MistakeRecord::Type MistakeRecord::type(void)
{
	return m_type;
}

/*! Sets previous text. */
void MistakeRecord::setPreviousText(QString text)
{
	m_previousText = text;
	setPreviousVariant(text);
	emit previousTextChanged(text);
}

/*! Returns previous text. */
QString MistakeRecord::previousText(void)
{
	return m_previousText;
}

/*! Sets previous value (useful if you need to support non-QString values). */
void MistakeRecord::setPreviousVariant(QVariant value)
{
	m_previousVariant = value;
	emit previousVariantChanged(value);
	if(strcmp(value.typeName(), "QString") == 0)
	{
		m_previousText = value.toString();
		emit previousTextChanged(m_previousText);
	}
}

/*! Returns previous value (useful if you need to support non-QString values). */
QVariant MistakeRecord::previousVariant(void)
{
	return m_previousVariant;
}

/*! Sets previous position. */
void MistakeRecord::setPreviousPosition(int pos)
{
	m_previousPosition = pos;
	emit previousPositionChanged(pos);
}

/*! Returns previous position. */
int MistakeRecord::previousPosition(void)
{
	return m_previousPosition;
}

/*! Enables or disables this mistake. */
void MistakeRecord::setEnabled(bool enabled)
{
	m_isEnabled = enabled;
	emit enabledChanged(enabled);
}

/*! Returns true if this mistake is enabled. */
bool MistakeRecord::isEnabled(void)
{
	return m_isEnabled;
}

/*! Toggles merged mistake. */
void MistakeRecord::setMerged(bool merged)
{
	m_isMerged = merged;
	emit mergedChanged(merged);
}

/*! Returns true if this mistake is merged. */
bool MistakeRecord::isMerged(void)
{
	return m_isMerged;
}