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

#include "pushBlock.h"

static void touch(Entity *other);
static void describe(void);
static int blocking(void);
static void die(void);

void initPushBlock(Entity *e)
{
	e->type = ET_PUSH_BLOCK;
	e->solid = 1;
	e->sprite = getSprite("PushBlock");
	e->touch = touch;
	e->die = die;
	e->describe = describe;
	e->isBlocking = blocking;
}

static void touch(Entity *other)
{
	int mx, my, blocked, tile, i, n;
	Entity *candidates[MAX_CANDIDATES], *oldSelf;
	
	mx = self->x + level.dx;
	my = self->y + level.dy;
	
	blocked = 1;

	if (mx >= 0 && my >= 0 && mx < MAP_WIDTH && my < MAP_HEIGHT)
	{
		tile = level.data[mx][my];

		if (tile == TILE_GREEN || (tile >= TILE_FLOOR && tile < TILE_WALL))
		{
			blocked = 0;
			
			getEntitiesAt(mx, my, &n, self, candidates);

			for (i = 0 ; i < n ; i++)
			{
				if (candidates[i]->solid && candidates[i]->visible)
				{
					blocked = 1;
				}
			}
		}
	}

	if (!blocked)
	{
		self->x = mx;
		self->y = my;
		
		getEntitiesAt(self->x, self->y, &n, self, candidates);

		/* block touch anything in the new square */
		for (i = 0 ; i < n ; i++)
		{
			if (candidates[i]->touch)
			{
				oldSelf = self;
				
				self = candidates[i];
				
				self->touch(oldSelf);
				
				self = oldSelf;
			}
		}
		
		getEntitiesAt(other->x, other->y, &n, other, candidates);

		/* pusher touch anything that was revealed by the block */
		for (i = 0 ; i < n ; i++)
		{
			if (candidates[i]->touch)
			{
				oldSelf = self;
				
				self = candidates[i];
				
				self->touch(other);
				
				self = oldSelf;
			}
		}
	}
	else
	{
		other->x -= level.dx;
		other->y -= level.dy;

		clearRoute();
	}
}

static void describe(void)
{
	level.message = app.strings[ST_PUSH_BLOCK_DESC];
}

static int blocking(void)
{
	return level.guy->type != ET_GREEN_GUY;
}

static void die(void)
{
	self->alive = 0;
	
	playSound(SND_DIE, -1);
	
	addExplosionEffect(self->x, self->y, 255, 255, 255);
}
