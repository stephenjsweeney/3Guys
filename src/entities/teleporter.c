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

#include "teleporter.h"

static void touch(Entity *other);
static int blocking(void);
static void activate(void);
static void describe(void);

static Sprite *inactiveTeleporter;

void initTeleporter(Entity *e)
{
	e->type = ET_TELEPORTER;
	e->sprite = getSprite("Teleporter");
	e->touch = touch;
	e->isBlocking = blocking;
	e->describe = describe;
	e->activate = activate;
	
	e->active = 1;
	
	inactiveTeleporter = getSprite("InactiveTeleporter");
}

static void touch(Entity *other)
{
	Entity **candidates, *oldSelf;
	int i, n;
	
	if (self->active)
	{
		playSound(SND_TELEPORT, -1);

		other->x = self->tx;
		other->y = self->ty;

		if (other == level.guy)
		{
			clearRoute();
		}

		addTeleportStars(self->tx, self->ty, 35);
		
		candidates = getEntitiesAt(other->x, other->y, &n, other);

		for (i = 0 ; i < n ; i++)
		{
			oldSelf = self;
			
			self = candidates[i];
			
			if (candidates[i]->solid)
			{
				if (candidates[i]->visible)
				{
					candidates[i]->alive = 0;
				}
			}
			else if (candidates[i]->touch)
			{
				candidates[i]->touch(other);
			}
			
			self = oldSelf;
		}
	}
}

static void activate(void)
{
	Entity **candidates;
	int n, i;
	
	self->active = !self->active;

	if (self->active)
	{
		candidates = getEntitiesAt(self->x, self->y, &n, self);
		
		for (i = 0 ; i < n ; i++)
		{
			self->touch(candidates[i]);
		}
	}
}

static void describe(void)
{
	if (self->active)
	{
		level.message = app.strings[ST_TELEPORTER_DESC];
	}
	else
	{
		level.message = app.strings[ST_TELEPORTER_OFF_DESC];
	}
}

static int blocking(void)
{
	return 0;
}
