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

#include "spikeTrap.h"

static Sprite *activeTrap;

static void touch(Entity *other);
static int blocking(void);
static void die(void);

void initSpikeTrap(Entity *e)
{
	e->type = ET_SPIKE_TRAP;
	e->sprite = getSprite("HiddenSpikeTrap");
	e->touch = touch;
	e->die = die;
	e->isBlocking = blocking;
	
	activeTrap = getSprite("SpikeTrap");
}

static void touch(Entity *other)
{
	if (isGuy(other))
	{
		other->alive = 0;
		
		self->sprite = activeTrap;
	}
	else if (other->type == ET_PUSH_BLOCK)
	{
		self->alive = 0;
	}
}

static int blocking(void)
{
	return 0;
}

static void die(void)
{
	playSound(SND_DIE, -1);
	
	addExplosionEffect(self->x, self->y, 255, 255, 255);
}
