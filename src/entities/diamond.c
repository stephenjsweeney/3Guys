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

#include "diamond.h"

static void touch(Entity *other);
static void describe(void);
static int blocking(void);

void initDiamond(Entity *e)
{
	e->type = ET_DIAMOND;
	e->sprite = getSprite("Diamond");
	e->touch = touch;
	e->describe = describe;
	e->isBlocking = blocking;
}

static void touch(Entity *other)
{
	if (other->type == ET_RED_GUY)
	{
		self->alive = 0;
		
		completeLevel();

		playSound(SND_DIAMOND, 1);

		/*
		game.stats[STAT_DIAMONDS]++;
		*/
	}
}

static void describe(void)
{
	level.message = app.strings[ST_DIAMOND_DESC];
}

static int blocking(void)
{
	return 0;
}
