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
#include "../system/sound.h"
#include "../system/sprites.h"
#include "star.h"

extern App	   app;
extern Entity *self;
extern Game	   game;
extern Level   level;

static void touch(Entity *other);
static void describe(void);
static int	blocking(void);

void initStar(Entity *e)
{
	e->type = ET_STAR;
	e->sprite = getSprite("Star");
	e->touch = touch;
	e->describe = describe;
	e->isBlocking = blocking;

	game.starsAvailable[level.id] = 1;

	if (game.starsFound[level.id])
	{
		e->alive = 0;
	}
}

static void touch(Entity *other)
{
	if (isGuy(other))
	{
		self->alive = 0;

		playSound(SND_STAR, -1);
	}
}

static void describe(void)
{
	level.message = app.strings[ST_STAR_DESC];
}

static int blocking(void)
{
	return 0;
}
