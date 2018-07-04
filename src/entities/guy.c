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

#include "guy.h"

static void die(void);
static int blocking(void);
static void draw(void);
static void tick(void);
static Atlas *bow;
static Atlas *eyelashes;

static void initGuy(Entity *e)
{
	e->tick = tick;
	e->die = die;
	e->isBlocking = blocking;
	e->draw = draw;
	
	e->solid = 1;

	bow = getImageFromAtlas("gfx/sprites/bow.png", 1);
	eyelashes = getImageFromAtlas("gfx/sprites/eyelashes.png", 1);
	
	e->sprite->currentTime = rand() % FPS;
	
	e->female = app.config.sex == SEX_FEMALE || (app.config.sex == SEX_RANDOM && rand() % 2 == 0);
}

void initRedGuy(Entity *e)
{
	e->type = ET_RED_GUY;
	e->sprite = getSprite("RedGuy");
	initGuy(e);
	
	level.guy = e;
}

void initGreenGuy(Entity *e)
{
	e->type = ET_GREEN_GUY;
	e->sprite = getSprite("GreenGuy");
	initGuy(e);
}

void initYellowGuy(Entity *e)
{
	e->type = ET_YELLOW_GUY;
	e->sprite = getSprite("YellowGuy");
	initGuy(e);
}

static void tick(void)
{
	int tile, fall, i, n;
	Entity **candidates;
	
	tile = level.data[(int)self->x][(int)self->y];

	if (tile == TILE_HOLE)
	{
		fall = 1;

		candidates = getEntitiesAt(self->x, self->y, &n, self);

		for (i = 0 ; i < n ; i++)
		{
			if (candidates[i]->type == ET_MOVING_PLATFORM)
			{
				fall = 0;
			}
		}

		if (fall)
		{
			if (self == level.guy && self->type == ET_YELLOW_GUY && level.tools != 0)
			{
				playSound(SND_BRIDGE, -1);
				
				addFloor(self->x, self->y);

				level.tools--;

				game.stats[STAT_TOOLS_USED]++;
			}
			else
			{
				self->alive = 0;
			}
		}
	}
}

static void draw(void)
{
	int x, y;
	
	x = LEVEL_RENDER_X + self->x * TILE_SIZE;
	y = LEVEL_RENDER_Y + self->y * TILE_SIZE;

	x += TILE_SIZE / 2;
	y += TILE_SIZE / 2;
	
	drawGLRectangleBatch(getCurrentFrame(self->sprite), x, y, 1);

	if (self->female)
	{
		drawGLRectangleBatch(&eyelashes->rect, x, y, 1);
		drawGLRectangleBatch(&bow->rect, x - 25, y - 30, 1);
	}
}

static int blocking(void)
{
	return 1;
}

static void die(void)
{
	self->alive = 0;
	
	switch (self->type)
	{
		case ET_RED_GUY:
			addExplosionEffect(self->x, self->y, 1, 0, 0);
			break;
			
		case ET_GREEN_GUY:
			addExplosionEffect(self->x, self->y, 0, 1, 0);
			break;
			
		case ET_YELLOW_GUY:
			addExplosionEffect(self->x, self->y, 1, 1, 0);
			break;
	}
	
	playSound(SND_DIE, -1);
	playSound(SND_FAIL, -1);
	
	failLevel();
	
	game.stats[STAT_LOSSES]++;
}

int isGuy(Entity *e)
{
	return (e->type == ET_RED_GUY ||e->type == ET_GREEN_GUY || e->type == ET_YELLOW_GUY); 
}
