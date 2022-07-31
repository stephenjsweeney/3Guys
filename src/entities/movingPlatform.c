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
#include "../level/entities.h"
#include "../system/sprites.h"
#include "movingPlatform.h"

extern App	   app;
extern Entity *self;
extern Level   level;

static void touch(Entity *other);
static void describe(void);
static int	blocking(void);
static void move(void);
static void initPlatformDestination(void);

void initMovingPlatform(Entity *e)
{
	e->type = ET_MOVING_PLATFORM;
	e->sprite = getSprite("MovingPlatform");
	e->touch = touch;
	e->describe = describe;
	e->move = move;
	e->isBlocking = blocking;

	e->backgroundPlane = 1;
}

static void touch(Entity *other)
{
	if (!self->active && isGuy(other))
	{
		initPlatformDestination();
	}
}

static void initPlatformDestination(void)
{
	Entity *candidates[MAX_CANDIDATES];
	int		n, i, hit;

	self->active = self->dx = self->dy = 0;

	getEntitiesAt(self->x, self->y, &n, self, candidates);

	for (i = 0; i < n; i++)
	{
		if (candidates[i] == level.guy)
		{
			self->dx = level.dx;
			self->dy = level.dy;
			self->active = 1;
		}
	}

	if (self->active)
	{
		hit = 0;

		self->tx = self->x;
		self->ty = self->y;

		do
		{
			self->tx += self->dx;
			self->ty += self->dy;

			if (self->tx < 0 || self->ty < 0 || self->tx >= MAP_WIDTH || self->ty >= MAP_HEIGHT || level.data[self->tx][self->ty] != TILE_HOLE)
			{
				hit = 1;
			}
			else
			{
				getEntitiesAt(self->tx, self->ty, &n, self, candidates);

				for (i = 0; i < n; i++)
				{
					if ((candidates[i]->solid && candidates[i]->visible) || candidates[i]->type == ET_MOVING_PLATFORM)
					{
						hit = 1;
					}
				}
			}
		} while (!hit);

		self->tx -= self->dx;
		self->ty -= self->dy;

		if (self->tx == self->x && self->ty == self->y)
		{
			self->active = self->dx = self->dy = 0;
		}
	}
}

static void move(void)
{
	if (!self->active)
	{
		initPlatformDestination();
	}

	self->x += self->dx;
	self->y += self->dy;

	if (self->x == self->tx && self->y == self->ty)
	{
		self->dx = self->dy = 0;
		self->active = 0;
	}
}

static void describe(void)
{
	level.message = app.strings[ST_PLATFORM_DESC];
}

static int blocking(void)
{
	return 0;
}
