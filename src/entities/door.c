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

#include "door.h"

static int canOpen(Entity *other);
static void touch(Entity *other);
static void describe(void);
static int blocking(void);

void initNormalDoor(Entity *e)
{
	e->type = ET_NORMAL_DOOR;
	e->sprite = getSprite("NormalDoor");
	e->touch = touch;
	e->describe = describe;
	e->isBlocking = blocking;
	
	e->solid = 1;
}

void initYellowDoor(Entity *e)
{
	e->type = ET_YELLOW_DOOR;
	e->sprite = getSprite("YellowDoor");
	e->touch = touch;
	e->describe = describe;
	e->isBlocking = blocking;
}

void initGreenDoor(Entity *e)
{
	e->type = ET_GREEN_DOOR;
	e->sprite = getSprite("GreenDoor");
	e->touch = touch;
	e->describe = describe;
	e->isBlocking = blocking;
}

void initRedDoor(Entity *e)
{
	e->type = ET_RED_DOOR;
	e->sprite = getSprite("RedDoor");
	e->touch = touch;
	e->describe = describe;
	e->isBlocking = blocking;
}

static void touch(Entity *other)
{
	if (isGuy(other))
	{
		if (other->carrying != NULL && canOpen(other))
		{
			other->carrying = NULL;
			
			self->alive = 0;
			
			playSound(SND_OPEN, -1);
			
			game.stats[STAT_KEYS_USED]++;
		}
		else
		{
			stepBack();
			
			clearRoute();
		}
	}
}

static int canOpen(Entity *other)
{
	switch (self->type)
	{
		case ET_RED_DOOR:
			return other->carrying->type == ET_RED_KEY;
			
		case ET_GREEN_DOOR:
			return other->carrying->type == ET_GREEN_KEY;
			
		case ET_YELLOW_DOOR:
			return other->carrying->type == ET_YELLOW_KEY;
			
		default:
			return 1;
	}
}

static void describe(void)
{
	level.message = app.strings[ST_DOOR_DESC];
}

static int blocking(void)
{
	return 0;
}
