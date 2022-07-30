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

#include "../common.h"
#include "tnt.h"
#include "../system/sprites.h"
#include "../system/sound.h"
#include "../entities/guy.h"

extern App app;
extern Entity *self;
extern Level level;

static void touch(Entity *other);
static void describe(void);
static int blocking(void);

void initTNT(Entity *e)
{
	e->type = ET_TNT;
	e->sprite = getSprite("TNT");
	e->touch = touch;
	e->describe = describe;
	e->isBlocking = blocking;
}

static void touch(Entity *other)
{
	if (isGuy(other))
	{
		self->alive = 0;

		playSound(SND_PICKUP, -1);
		
		level.tnt++;
	}
}

static void describe(void)
{
	level.message = app.strings[ST_TNT_DESC];
}

static int blocking(void)
{
	return 0;
}

