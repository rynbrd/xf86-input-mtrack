/***************************************************************************
 *
 * Multitouch X driver
 * Copyright (C) 2011 Ryan Bourgeois <bluedragonx@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 **************************************************************************/

#ifndef MCONFIG_H
#define MCONFIG_H

#include "capabilities.h"
#include "mtrack.h"

/* Load the MConfig struct with its defaults.
 */
void mconfig_defaults(struct MConfig* cfg);

/* Initialize the MConfig struct.
 */
void mconfig_init(struct MConfig* cfg,
			const struct Capabilities* caps);

void mconfig_configure(struct MConfig* cfg,
			pointer opts);

#endif

