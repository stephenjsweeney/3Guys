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
#include "levelLoader.h"
#include "../json/cJSON.h"
#include "../entities/laserTrap.h"
#include "../entities/firePitTrap.h"
#include "../entities/teleporter.h"
#include "../entities/guy.h"
#include "../entities/crumblingFloor.h"
#include "../entities/diamond.h"
#include "../entities/star.h"
#include "../level/level.h"
#include "../entities/tools.h"
#include "../entities/key.h"
#include "../entities/tnt.h"
#include "../util/util.h"
#include "../entities/wall.h"
#include "../system/io.h"
#include "../entities/door.h"
#include "../entities/pushBlock.h"
#include "../entities/movingPlatform.h"
#include "../entities/spikeTrap.h"
#include "../entities/pressurePlate.h"

extern Level level;

static void loadMeta(cJSON *metaJSON);
static void loadMapData(cJSON *mapJSON);
static void loadEntities(cJSON *entitiesJSON);
static void addEntityDef(char *type, void (*initFunc)(Entity *));
static int entComparator(const void *a, const void *b);
static Entity *createEntity(const char *type);
static char *getLevelFilename(int id);

static EntityDef entityDefHead;
static EntityDef *entityDefTail;

int loadLevel(int id)
{
	char *text, *filename;
	cJSON *root;
	
	destroyLevel();
	
	level.id = id;
	
	filename = getLevelFilename(id);
	
	if (fileExists(filename))
	{
		text = readFile(filename);

		root = cJSON_Parse(text);
		
		loadMeta(cJSON_GetObjectItem(root, "meta"));
		
		loadMapData(cJSON_GetObjectItem(root, "map"));
		
		loadEntities(cJSON_GetObjectItem(root, "entities"));
		
		cJSON_Delete(root);
		
		free(text);
		
		free(filename);
		
		return 1;
	}
	
	free(filename);
	
	return 0;
}

static char *getLevelFilename(int id)
{
	char *filename;
	
	filename = buildFormattedString("data/levels/%d.json", id);
	
	if (!fileExists(filename))
	{
		free(filename);
		
		filename = buildFormattedString(DATA_DIR"/data/levels/%d.json", id);
	}
	
	return filename;
}

static void loadMeta(cJSON *metaJSON)
{
	cJSON *tips, *tip;
	int n;
	
	level.moves = cJSON_GetObjectItem(metaJSON, "moves")->valueint;
	level.tools = cJSON_GetObjectItem(metaJSON, "tools")->valueint;
	level.tnt = cJSON_GetObjectItem(metaJSON, "tnt")->valueint;
	
	tips = cJSON_GetObjectItem(metaJSON, "tips");
	
	if (tips != NULL)
	{
		n = 0;
		
		for (tip = tips->child ; tip != NULL ; tip = tip->next)
		{
			n++;
		}
		
		if (n > 0)
		{
			level.tips = malloc(sizeof(char*) * n);
			
			n = 0;
			
			for (tip = tips->child ; tip != NULL ; tip = tip->next)
			{
				level.tips[n] = malloc(strlen(_(tip->valuestring)) + 1);
				strcpy(level.tips[n], _(tip->valuestring));
				
				n++;
			}
			
			level.numTips = n;
		}
	}
}

static void loadMapData(cJSON *mapJSON)
{
	int x, y, i;
	cJSON *mapLine;
	char *p;
	
	mapLine = mapJSON->child;
	
	for (y = 0 ; y < MAP_HEIGHT ; y++)
	{
		p = mapLine->valuestring;
		
		for (x = 0 ; x < MAP_WIDTH ; x++)
		{
			sscanf(p, "%d", &i);
			
			level.data[x][y] = i;
			
			do {p++;} while (*p != ' ');
		}
		
		mapLine = mapLine->next;
	}
	
	for (y = 0; y < MAP_HEIGHT; y++)
	{
		for (x = 0; x < MAP_WIDTH; x++)
		{
			if (level.data[x][y] >= TILE_FLOOR && level.data[x][y] < TILE_WALL)
			{
				level.data[x][y] = getRandomFloorTile();
			}
		}
	}
}

static Entity *createEntity(const char *type)
{
	Entity *e;
	EntityDef *def;
	
	for (def = entityDefHead.next ; def != NULL ; def = def->next)
	{
		if (strcmp(def->type, type) == 0)
		{
			e = malloc(sizeof(Entity));
			memset(e, 0, sizeof(Entity));
			
			e->alive = 1;
			
			def->initFunc(e);
			
			return e;
		}
	}
	
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "No such entity def '%s'", type);
	exit(1);
	
	return NULL;
}

static void loadEntities(cJSON *entitiesJSON)
{
	cJSON *entityJSON;
	Entity *e, **candidates;
	int i, n;
	
	n = 0;
	
	for (entityJSON = entitiesJSON->child ; entityJSON != NULL ; entityJSON = entityJSON->next)
	{
		n++;
	}
	
	candidates = malloc(sizeof(Entity*) * n);
	memset(candidates, 0, sizeof(Entity*) * n);
	
	i = 0;
	
	for (entityJSON = entitiesJSON->child ; entityJSON != NULL ; entityJSON = entityJSON->next)
	{
		e = createEntity(cJSON_GetObjectItem(entityJSON, "type")->valuestring);
		e->x = cJSON_GetObjectItem(entityJSON, "x")->valueint;
		e->y = cJSON_GetObjectItem(entityJSON, "y")->valueint;
		e->visible = cJSON_GetObjectItem(entityJSON, "visible")->valueint;
		
		if (cJSON_GetObjectItem(entityJSON, "name") != NULL)
		{
			STRNCPY(e->name, cJSON_GetObjectItem(entityJSON, "name")->valuestring, MAX_NAME_LENGTH);
		}
		
		if (cJSON_GetObjectItem(entityJSON, "target") != NULL)
		{
			STRNCPY(e->target, cJSON_GetObjectItem(entityJSON, "target")->valuestring, MAX_NAME_LENGTH);
		}
		
		if (cJSON_GetObjectItem(entityJSON, "tx") != NULL)
		{
			e->tx = cJSON_GetObjectItem(entityJSON, "tx")->valueint;
			e->ty = cJSON_GetObjectItem(entityJSON, "ty")->valueint;
		}
		
		if (cJSON_GetObjectItem(entityJSON, "active") != NULL)
		{
			e->active = cJSON_GetObjectItem(entityJSON, "active")->valueint;
		}
		
		candidates[i++] = e;
	}
	
	qsort(candidates, n, sizeof(Entity*), entComparator);
	
	for (i = 0 ; i < n ; i++)
	{
		level.entityTail->next = candidates[i];
		level.entityTail = candidates[i];
	}
	
	free(candidates);
}

void initEntityDefs(void)
{
	entityDefTail = &entityDefHead;
	
	addEntityDef("Diamond", initDiamond);
	addEntityDef("RedGuy", initRedGuy);
	addEntityDef("GreenGuy", initGreenGuy);
	addEntityDef("Star", initStar);
	addEntityDef("NormalKey", initNormalKey);
	addEntityDef("NormalDoor", initNormalDoor);
	addEntityDef("YellowKey", initYellowKey);
	addEntityDef("GreenKey", initGreenKey);
	addEntityDef("RedKey", initRedKey);
	addEntityDef("YellowDoor", initYellowDoor);
	addEntityDef("GreenDoor", initGreenDoor);
	addEntityDef("RedDoor", initRedDoor);
	addEntityDef("PushBlock", initPushBlock);
	addEntityDef("YellowGuy", initYellowGuy);
	addEntityDef("Tools", initTools);
	addEntityDef("SpikeTrap", initSpikeTrap);
	addEntityDef("PressurePlate", initPressurePlate);
	addEntityDef("Wall", initWall);
	addEntityDef("MovingPlatform", initMovingPlatform);
	addEntityDef("CrumblingFloor", initCrumblingFloor);
	addEntityDef("TNT", initTNT);
	addEntityDef("Teleporter", initTeleporter);
	addEntityDef("HorizontalLaserTrap", initHorizontalLaserTrap);
	addEntityDef("VerticalLaserTrap", initVerticalLaserTrap);
	addEntityDef("FirePitTrap", initFirePitTrap);
}

static void addEntityDef(char *type, void (*initFunc)(Entity *))
{
	EntityDef *def;
	
	def = malloc(sizeof(EntityDef));
	memset(def, 0, sizeof(EntityDef));
	
	STRNCPY(def->type, type, MAX_NAME_LENGTH);
	def->initFunc = initFunc;
	
	entityDefTail->next = def;
	entityDefTail = def;
}

static int entComparator(const void *a, const void *b)
{
	Entity *e1 = *((Entity**)a);
	Entity *e2 = *((Entity**)b);

	return e1->type - e2->type;
}

