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

#include "laserTrap.h"

static int blocking(void);
static void tick(void);
static void die(void);
static void activate(void);
static void describe(void);
static void draw(void);
static void initLaserTrap(Entity *e);

static Sprite *laserBeam;
static AtlasImage *inactive;

void initHorizontalLaserTrap(Entity *e)
{
	initLaserTrap(e);
}

void initVerticalLaserTrap(Entity *e)
{
	initLaserTrap(e);
	
	e->angle = 90;
}

static void initLaserTrap(Entity *e)
{
	e->type = ET_LASER_TRAP;
	e->isBlocking = blocking;
	e->die = die;
	e->activate = activate;
	e->describe = describe;
	e->draw = draw;
	e->tick = tick;
	
	e->sprite = getSprite("LaserTrap");
	
	laserBeam = getSprite("LaserBeam");
	
	inactive = getImageFromAtlas("gfx/sprites/laserTrap.png", 1);
	
	e->solid = 1;
}

static void tick(void)
{
	int i, n, dx, dy, hit;
	Entity *candidates[MAX_CANDIDATES];
	
	self->tx = self->x;
	self->ty = self->y;
	
	if (self->active)
	{
		hit = dx = dy = 0;

		if (self->angle == 90)
		{
			dy = 1;
		}
		else
		{
			dx = 1;
		}
		
		while (!hit)
		{
			if (self->tx < 0 || self->ty < 0 || self->tx >= MAP_WIDTH || self->ty >= MAP_HEIGHT || level.data[self->tx][self->ty] >= TILE_WALL)
			{
				hit = 1;
			}
			else
			{
				getEntitiesAt(self->tx, self->ty, &n, self, candidates);
				
				for (i = 0 ; i < n ; i++)
				{
					if (isGuy(candidates[i]))
					{
						candidates[i]->alive = 0;
					}
					else if (candidates[i]->solid)
					{
						hit = 1;
					}
				}
				
				if (!hit)
				{
					self->tx += dx;
					self->ty += dy;
				}
			}
		}
	}
}

static void draw(void)
{
	int hit, x, y, dx, dy, ex, ey;
	
	if (self->active)
	{
		hit = dx = dy = 0;

		if (self->angle == 90)
		{
			dy = 1;
		}
		else
		{
			dx = 1;
		}
		
		ex = self->x;
		ey = self->y;

		while (!hit)
		{
			x = LEVEL_RENDER_X + ex * TILE_SIZE;
			y = LEVEL_RENDER_Y + ey * TILE_SIZE;
			
			x += TILE_SIZE / 2;
			y += TILE_SIZE / 2;
			
			blitAtlasImageRotated(getCurrentFrame(laserBeam), x, y, self->angle);
			
			ex += dx;
			ey += dy;
			
			hit = (ex == self->tx && ey == self->ty);
		}
		
		x = LEVEL_RENDER_X + (self->x * TILE_SIZE);
		y = LEVEL_RENDER_Y + (self->y * TILE_SIZE);
		
		x += TILE_SIZE / 2;
		y += TILE_SIZE / 2;

		blitAtlasImageRotated(getCurrentFrame(self->sprite), x, y, self->angle);
	}
	else
	{
		x = LEVEL_RENDER_X + self->x * TILE_SIZE;
		y = LEVEL_RENDER_Y + self->y * TILE_SIZE;
		
		x += TILE_SIZE / 2;
			y += TILE_SIZE / 2;

		blitAtlasImageRotated(inactive, x, y, self->angle);
	}
}

static void activate(void)
{
	self->active = !self->active;
}

static void die(void)
{
	playSound(SND_DIE, -1);
	
	addExplosionEffect(self->x, self->y, 255, 255, 255);
}

static void describe(void)
{
	if (self->active)
	{
		level.message = app.strings[ST_LASER_TRAP_DESC];
	}
	else
	{
		level.message = app.strings[ST_LASER_TRAP_OFF_DESC];
	}
}

static int blocking(void)
{
	return 1;
}
