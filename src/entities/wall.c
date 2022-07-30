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
#include "wall.h"
#include "../system/atlas.h"
#include "../entities/guy.h"
#include "../system/sprites.h"
#include "../system/sound.h"
#include "../level/effects.h"
#include "../level/entities.h"
#include "../level/player.h"
#include "../system/draw.h"

extern Entity *self;

static void touch(Entity *other);
static int blocking(void);
static void die(void);
static void activate(void);
static void draw(void);

static AtlasImage *wallDown;

void initWall(Entity *e)
{
	e->type = ET_WALL;
	e->sprite = getSprite("Wall");
	e->touch = touch;
	e->isBlocking = blocking;
	e->die = die;
	e->activate = activate;
	e->draw = draw;
	
	e->solid = 1;
	e->active = 1;
	
	wallDown = getImageFromAtlas("gfx/sprites/wallDown.png", 1);
}

static void touch(Entity *other)
{
	if (self->active && isGuy(other))
	{
		stepBack();

		clearRoute();
	}
}

static void activate(void)
{
	Entity *candidates[MAX_CANDIDATES];
	int i, n;
	
	self->solid = self->active = !self->active;
	
	getEntitiesAt(self->x, self->y, &n, self, candidates);

	for (i = 0 ; i < n ; i++)
	{
		if (isGuy(candidates[i]))
		{
			candidates[i]->alive = 0;
		}
		else if (candidates[i]->type == ET_PUSH_BLOCK)
		{
			self->alive = 0;
		}
	}
	
	playSound(SND_WALL, 2);
}

static void draw(void)
{
	int x, y;
	
	x = LEVEL_RENDER_X + self->x * TILE_SIZE;
	y = LEVEL_RENDER_Y + self->y * TILE_SIZE;

	x += TILE_SIZE / 2;
	y += TILE_SIZE / 2;
	
	if (self->active)
	{
		blitAtlasImage(getCurrentFrame(self->sprite), x, y, 1);
	}
	else
	{
		blitAtlasImage(wallDown, x, y, 1);
	}
}

static void die(void)
{
	playSound(SND_DIE, -1);
	
	addExplosionEffect(self->x, self->y, 255, 255, 255);
}

static int blocking(void)
{
	return self->active;
}

