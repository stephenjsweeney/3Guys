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

static void touch(Entity *other);
static void die(void);
static int blocking(void);

static void initGuy(Entity *e)
{
	e->touch = touch;
	e->die = die;
	e->isBlocking = blocking;
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

static void touch(Entity *other)
{

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
}

int isGuy(Entity *e)
{
	return (e->type == ET_RED_GUY ||e->type == ET_GREEN_GUY || e->type == ET_YELLOW_GUY); 
}
