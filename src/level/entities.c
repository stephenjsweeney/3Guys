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

#include "entities.h"

static void guyTouchOthers(void);
static void guyFallDownHoles(void);
Entity **getEntitiesAt(int x, int y, int *n);

static Entity *candidates[MAX_CANDIDATES];
static Entity deadHead;
static Entity *deadTail;

void initEntities(void)
{
	memset(&deadHead, 0, sizeof(Entity));
	deadTail = &deadHead;
	
	level.entityTail = &level.entityHead;
}

void doEntities(void)
{
	Entity *prev;
	
	prev = &level.entityHead;
	
	for (self = level.entityHead.next ; self != NULL ; self = self->next)
	{
		if (self->tick != NULL)
		{
			self->tick();
		}
		
		if (!self->alive)
		{
			if (self == level.entityTail)
			{
				level.entityTail = prev;
			}
			
			prev->next = self->next;
			
			/* move to dead list */
			deadTail->next = self;
			deadTail = self;
			
			self = prev;
		}
		
		prev = self;
	}
}

void moveEntities(void)
{
	for (self = level.entityHead.next ; self != NULL ; self = self->next)
	{
		if (self->move != NULL)
		{
			self->move();
		}
	}
	
	guyTouchOthers();
	
	guyFallDownHoles();
}

static void guyTouchOthers(void)
{
	Entity **candidates;
	int i, n;
	
	candidates = getEntitiesAt(level.guy->x, level.guy->y, &n);
	
	for (i = 0 ; i < n ; i++)
	{
		self = candidates[i];
		
		if (self != NULL && self != level.guy && self->touch != NULL)
		{
			self->touch(level.guy);
		}
	}
}

static void guyFallDownHoles(void)
{
	int fall, i, n;
	Entity **candidates;
	
	fall = 0;
	
	if (level.data[(int)level.guy->x][(int)level.guy->y] == TILE_HOLE)
	{
		fall = 1;
		
		candidates = getEntitiesAt(level.guy->x, level.guy->y, &n);
		
		for (i = 0 ; i < n ; i++)
		{
			if (candidates[i]->type == ET_MOVING_PLATFORM)
			{
				fall = 0;
			}
		}
		
		if (fall)
		{
			if (level.guy->type == ET_YELLOW_GUY && level.tools != 0)
			{
				playSound(SND_BRIDGE, -1);
				
				addFloor(level.guy->x, level.guy->y);

				level.tools--;

				/*game.saveData.stats[Stat.TOOLS_USED.ordinal()]++;*/
			}
			else
			{
				level.guy->die();
			}
		}
	}
}

void drawEntities(void)
{
	Entity *e;
	int x, y;
	
	setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
	
	for (e = level.entityHead.next ; e != NULL ; e = e->next)
	{
		if (e->visible)
		{
			x = LEVEL_RENDER_X + e->x * TILE_SIZE;
			y = LEVEL_RENDER_Y + e->y * TILE_SIZE;
			
			x += TILE_SIZE / 2;
			y += TILE_SIZE / 2;
		
			glRectangleBatch.rotate = 1;
			glRectangleBatch.angle = e->angle;
			drawGLRectangleBatch(getCurrentFrame(e->sprite), x, y, 1);
			glRectangleBatch.rotate = 0;
		}
	}
}

Entity *getEntityAt(int x, int y)
{
	Entity *e;
	
	for (e = level.entityHead.next ; e != NULL ; e = e->next)
	{
		if (e->x == x && e->y == y)
		{
			return e;
		}
	}
	
	return NULL;
}

Entity **getEntitiesAt(int x, int y, int *n)
{
	Entity *e;
	
	*n = 0;
	
	memset(candidates, 0, sizeof(Entity*) * MAX_CANDIDATES);
	
	for (e = level.entityHead.next ; e != NULL ; e = e->next)
	{
		if (e->x == x && e->y == y)
		{
			candidates[*n] = e;
			
			*n = *n + 1;
			
			if (*n >= MAX_CANDIDATES)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Out of entity candiate space");
				exit(1);
			}
		}
	}
	
	return candidates;
}

void destroyEntities(void)
{
	Entity *e;
	
	while (deadHead.next)
	{
		e = deadHead.next;
		deadHead.next = e->next;
		free(e);
	}
}
