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
static void draw(void);

static AtlasImage *inactiveTeleporter;

void initTeleporter(Entity *e)
{
	e->type = ET_TELEPORTER;
	e->sprite = getSprite("Teleporter");
	e->touch = touch;
	e->isBlocking = blocking;
	e->describe = describe;
	e->activate = activate;
	e->draw = draw;
	
	e->active = 1;
	
	inactiveTeleporter = getImageFromAtlas("gfx/sprites/inactiveTeleporter.png", 1);
}

static void touch(Entity *other)
{
	Entity *candidates[MAX_CANDIDATES], *oldSelf;
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
		
		getEntitiesAt(other->x, other->y, &n, other, candidates);

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
		blitAtlasImage(inactiveTeleporter, x, y, 1);
	}
}

static void activate(void)
{
	Entity *candidates[MAX_CANDIDATES];
	int n, i;
	
	self->active = !self->active;

	if (self->active)
	{
		getEntitiesAt(self->x, self->y, &n, self, candidates);
		
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
