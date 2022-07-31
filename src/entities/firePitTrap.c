/*
Copyright (C) 2018,2022 Parallel Realities

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

#include "../common.h"

#include "../entities/guy.h"
#include "../level/effects.h"
#include "../level/entities.h"
#include "../level/level.h"
#include "../system/sound.h"
#include "../system/sprites.h"
#include "firePitTrap.h"

extern App	   app;
extern Entity *self;
extern Level   level;

static void touch(Entity *other);
static int	blocking(void);
static void tick(void);
static void describe(void);
static void die(void);

void initFirePitTrap(Entity *e)
{
	e->type = ET_FIRE_PIT;
	e->sprite = getSprite("FirePitTrap");
	e->touch = touch;
	e->die = die;
	e->tick = tick;
	e->isBlocking = blocking;
	e->describe = describe;
}

static void tick(void)
{
	Entity *candidates[MAX_CANDIDATES];
	int		i, n, guyStanding;

	guyStanding = 0;

	getEntitiesAt(self->x, self->y, &n, self, candidates);

	for (i = 0; i < n; i++)
	{
		if (isGuy(candidates[i]))
		{
			guyStanding = 1;
		}
	}

	if (guyStanding)
	{
		self->weight = 1;
	}
	else
	{
		if (self->weight > 0)
		{
			self->visible = 1;
		}
	}
}

static void touch(Entity *other)
{
	if (self->visible)
	{
		if (isGuy(other))
		{
			other->alive = 0;
		}
		else if (other->type == ET_PUSH_BLOCK)
		{
			addFloor(self->x, self->y);

			other->alive = 0;

			self->alive = 0;
		}
	}
}

static int blocking(void)
{
	return 0;
}

static void describe(void)
{
	level.message = app.strings[ST_FIRE_PIT_DESC];
}

static void die(void)
{
	playSound(SND_DIE, -1);

	addExplosionEffect(self->x, self->y, 255, 255, 255);
}
