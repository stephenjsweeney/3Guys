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

#include "player.h"

static void handleEntityClick(Entity *e);
static void handleTNT(int x, int y);
static int addRouteNode(void);
void clearRoute(void);
static void cancelLastNode(void);
static void doControls(void);
static int isWalkableByGuy(void);
static void doDebugControls(void);

static SDL_Point lastRouteNode;

void initPlayer(void)
{
	clearRoute();
}

void doPlayer(void)
{
	if (!level.walkRoute)
	{
		doControls();
	}
	
	if (dev.debug)
	{
		doDebugControls();
	}
}

static void doControls(void)
{
	Entity *candidates[MAX_CANDIDATES];
	float x, y;
	int n, i;
	RouteNode *node;
	
	if (app.mouse.button[SDL_BUTTON_LEFT])
	{
		level.message = NULL;
		
		x = app.mouse.x;
		x -= LEVEL_RENDER_X;
		x /= TILE_SIZE;
		
		y = app.mouse.y;
		y -= LEVEL_RENDER_Y;
		y /= TILE_SIZE;
		
		if (level.routeHead.next == NULL)
		{
			getEntitiesAt(x, y, &n, NULL, candidates);
			
			if (n == 1)
			{
				handleEntityClick(candidates[0]);
			}
			else
			{
				for (i = 0 ; i < n ; i++)
				{
					handleEntityClick(candidates[i]);
				}
			}
			
			handleTNT(x, y);
		}
		else
		{
			if ((int)x != lastRouteNode.x || (int)y != lastRouteNode.y)
			{
				lastRouteNode.x = x;
				lastRouteNode.y = y;
				
				if (addRouteNode())
				{
					node = malloc(sizeof(RouteNode));
					memset(node, 0, sizeof(RouteNode));
					level.routeTail->next = node;
					level.routeTail = node;

					level.routeTail->x = lastRouteNode.x;
					level.routeTail->y = lastRouteNode.y;
				}
				
				cancelLastNode();
			}
		}
	}
	else
	{
		if (level.routeHead.next != NULL && level.routeHead.next->next != NULL)
		{
			level.walkRoute = 1;
			
			/* ignore 0, as it's the guy himself */
			node = level.routeHead.next;
			level.routeHead.next = node->next;
			free(node);
		}
		else
		{
			clearRoute();
		}
	}
}

static void doDebugControls(void)
{
	if (app.keyboard[SDL_SCANCODE_1])
	{
		completeLevel();
		
		app.keyboard[SDL_SCANCODE_1] = 0;
	}
}

static void handleTNT(int x, int y)
{
	if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT && level.data[x][y] == TILE_WALL && level.tnt != 0)
	{
		addFloor(x, y);

		addExplosionEffect(x, y, 1, 1, 1);

		playSound(SND_BOMB, -1);

		if (level.tnt != -1)
		{
			level.tnt--;
			
			game.stats[STAT_TNT_USED]++;
		}
	}
}

static void handleEntityClick(Entity *e)
{
	RouteNode *n;

	switch (e->type)
	{
		case ET_RED_GUY:
		case ET_YELLOW_GUY:
		case ET_GREEN_GUY:
			if (e == level.guy)
			{
				lastRouteNode.x = e->x;
				lastRouteNode.y = e->y;

				n = malloc(sizeof(RouteNode));
				memset(n, 0, sizeof(RouteNode));
				level.routeTail->next = n;
				level.routeTail = n;
				
				n->x = lastRouteNode.x;
				n->y = lastRouteNode.y;
			}
			else
			{
				level.guy = e;
			}
			break;
			
		default:
			if (level.routeHead.next == NULL && e->describe != NULL)
			{
				self = e;
				
				self->describe();
			}
			break;
	}
}

static int addRouteNode(void)
{
	int i, n;
	Entity *candidates[MAX_CANDIDATES];
	RouteNode *node;
	
	if (lastRouteNode.x < 0 || lastRouteNode.x >= MAP_WIDTH || lastRouteNode.y < 0 || lastRouteNode.y >= MAP_HEIGHT || level.data[lastRouteNode.x][lastRouteNode.y] == TILE_WALL || !isWalkableByGuy())
	{
		return 0;
	}
	
	/* too far away from last node */
	if (abs(level.routeTail->x - lastRouteNode.x) > 1 || abs(level.routeTail->y - lastRouteNode.y) > 1)
	{
		return 0;
	}

	/* don't allow diagonals */
	if (abs(level.routeTail->x - lastRouteNode.x) == 1 && abs(level.routeTail->y - lastRouteNode.y) == 1)
	{
		return 0;
	}
	
	getEntitiesAt(lastRouteNode.x, lastRouteNode.y, &n, NULL, candidates);
	
	for (i = 0 ; i < n ; i++)
	{
		self = candidates[i];
		
		if (level.guy != candidates[i] && candidates[i]->isBlocking())
		{
			return 0;
		}
	}
	
	for (node = level.routeHead.next ; node != NULL ; node = node->next)
	{
		if (node->x == lastRouteNode.x && node->y == lastRouteNode.y)
		{
			return 0;
		}
	}
	
	return 1;
}

static int isWalkableByGuy(void)
{
	switch (level.data[lastRouteNode.x][lastRouteNode.y])
	{
		case TILE_RED:
			return level.guy->type == ET_RED_GUY;
		
		case TILE_YELLOW:
			return level.guy->type == ET_YELLOW_GUY;
		
		case TILE_GREEN:
			return level.guy->type == ET_GREEN_GUY;
			
		default:
			return 1;
	}
}

static void cancelLastNode(void)
{
	RouteNode *n, *prev;

	prev = &level.routeHead;
	
	for (n = level.routeHead.next ; n != NULL ; n = n->next)
	{
		if (n == level.routeTail && prev->x == lastRouteNode.x && prev->y == lastRouteNode.y)
		{
			free(level.routeTail);
			prev->next = NULL;
			level.routeTail = prev;
			return;
		}
		
		prev = n;
	}
}

void stepBack(void)
{
	level.guy->x -= level.dx;
	level.guy->y -= level.dy;
}

void clearRoute(void)
{
	RouteNode *n;
	
	while (level.routeHead.next)
	{
		n = level.routeHead.next;
		level.routeHead.next = n->next;
		free(n);
	}

	memset(&level.routeHead, 0, sizeof(RouteNode));
	level.routeTail = &level.routeHead;
	
	level.dx = level.dy = 0;
	
	level.walkRoute = 0;
}
