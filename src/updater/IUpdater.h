/*
 * IUpdater.h
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

#ifndef IUPDATER_H
#define IUPDATER_H

#include "global/modularity/ioc.h"
#include "global/ISettings.h"

class IUpdater : MODULE_EXPORT_INTERFACE
{
	public:
		virtual ~IUpdater() { }

		virtual bool updateAvailable() = 0;
		virtual void installUpdate() = 0;

	protected:
		// Use this to check if updates are enabled
		virtual bool updatesEnabled() final
		{
			return modularity::ioc()->resolve<ISettings>()->getValue("updater", "updateChecks").toBool();
		}
};

#endif // IUPDATER_H