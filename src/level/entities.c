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
#include "entities.h"
#include "../system/sprites.h"
#include "../system/draw.h"

extern Entity *self;
extern Game game;
extern Level level;


static Entity deadHead;
static Entity *deadTail;

void initEntities(void)
{
	memset(&deadHead, 0, sizeof(Entity));
	deadTail = &deadHead;
	
	memset(&level.entityHead, 0, sizeof(Entity));
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
			self->next = NULL;
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
	Entity *candidates[MAX_CANDIDATES];
	int i, n;
	
	getEntitiesAt(level.guy->x, level.guy->y, &n, level.guy, candidates);
	
	for (i = 0 ; i < n ; i++)
	{
		self = candidates[i];
		
		if (self != NULL && self->touch != NULL)
		{
			self->touch(level.guy);
		}
	}
}

void drawEntities(int backgroundPlane)
{
	int x, y;
	
	for (self = level.entityHead.next ; self != NULL ; self = self->next)
	{
		if (self->visible && self->backgroundPlane == backgroundPlane)
		{
			if (self->draw)
			{
				self->draw();
			}
			else
			{
				x = LEVEL_RENDER_X + self->x * TILE_SIZE;
				y = LEVEL_RENDER_Y + self->y * TILE_SIZE;

				x += TILE_SIZE / 2;
				y += TILE_SIZE / 2;

				blitAtlasImageRotated(getCurrentFrame(self->sprite), x, y, 1, self->angle);
			}
		}
	}
}

void getEntitiesAt(int x, int y, int *n, Entity *ignore, Entity **candidates)
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
				*n = 1;
				return;
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

void updateStar(void)
{
	Entity *e;
	
	for (e = deadHead.next ; e != NULL ; e = e->next)
	{
		if (e->type == ET_STAR && game.starsFound[level.id] == 0)
		{
			game.starsFound[level.id] = 1;
			
			game.stats[STAT_STARS]++;
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

