/*
Copyright (C) 2018 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "pressurePlate.h"

static void describe(void);
static int blocking(void);
static void tick(void);

void initPressurePlate(Entity *e)
{
	e->type = ET_PRESSURE_PLATE;
	e->sprite = getSprite("PressurePlate");
	e->tick = tick;
	e->describe = describe;
	e->isBlocking = blocking;
}

static void tick(void)
{
	int n;
	
	getEntitiesAt(self->x, self->y, &n, self);

	if (n != self->weight)
	{
		activateEntities(self->target);
	}
	
	self->weight = n;
}

static void describe(void)
{
	level.message = app.strings[ST_PRESSURE_PLATE_DESC];
}

static int blocking(void)
{
	return 0;
}
