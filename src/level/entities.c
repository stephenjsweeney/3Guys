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

Entity **getEntitiesAt(int x, int y, int *n, Entity *ignore);

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
			if (self->die != NULL)
			{
				self->die();
			}
			
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
}

void guyTouchOthers(void)
{
	Entity **candidates;
	int i, n;
	
	candidates = getEntitiesAt(level.guy->x, level.guy->y, &n, level.guy);
	
	for (i = 0 ; i < n ; i++)
	{
		self = candidates[i];
		
		if (self != NULL && self != level.guy && self->touch != NULL)
		{
			self->touch(level.guy);
		}
	}
}

void guyFallDownHoles(void)
{
	int fall, i, n;
	Entity **candidates;
	
	fall = 0;
	
	if (level.data[level.guy->x][level.guy->y] == TILE_HOLE)
	{
		fall = 1;
		
		candidates = getEntitiesAt(level.guy->x, level.guy->y, &n, level.guy);
		
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
				level.guy->alive = 0;
			}
		}
	}
}

void drawEntities(int backgroundPlane)
{
	Entity *e;
	int x, y;
	
	setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
	
	for (e = level.entityHead.next ; e != NULL ; e = e->next)
	{
		if (e->visible && e->backgroundPlane == backgroundPlane)
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

Entity **getEntitiesAt(int x, int y, int *n, Entity *ignore)
{
	Entity *e;
	
	*n = 0;
	
	memset(candidates, 0, sizeof(Entity*) * MAX_CANDIDATES);
	
	for (e = level.entityHead.next ; e != NULL ; e = e->next)
	{
		if (e->visible && e->x == x && e->y == y && e != ignore)
		{
			/* only one solid thing can occupy a space */
			if (e->solid)
			{
				memset(candidates, 0, sizeof(Entity*) * MAX_CANDIDATES);
				candidates[0] = e;
				*n = 0;
				return candidates;
			}
			
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

void activateEntities(char *targetName)
{
	Entity *e, *oldSelf;
	
	for (e = level.entityHead.next ; e != NULL ; e = e->next)
	{
		if (strcmp(e->name, targetName) == 0 && e->activate != NULL)
		{
			oldSelf = self;
			
			self = e;
			
			self->activate();
			
			self = oldSelf;
		}
	}
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
