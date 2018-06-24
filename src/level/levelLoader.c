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

#include "levelLoader.h"

static void loadMeta(cJSON *metaJSON);
static void loadMapData(cJSON *mapJSON);
static void loadEntities(cJSON *entitiesJSON);
static void addEntityDef(char *type, void (*initFunc)(Entity *));
static Entity *createEntity(const char *type);

static EntityDef entityDefHead;
static EntityDef *entityDefTail;

void loadLevel(int id)
{
	char *text, *filename;
	cJSON *root;
	
	destroyLevel();
	
	level.id = id;
	
	filename = buildFormattedString("data/levels/%d.json", id);
	
	text = readFile(filename);

	root = cJSON_Parse(text);
	
	loadMeta(cJSON_GetObjectItem(root, "meta"));
	
	loadMapData(cJSON_GetObjectItem(root, "map"));
	
	loadEntities(cJSON_GetObjectItem(root, "entities"));
	
	cJSON_Delete(root);
	
	free(text);
	
	free(filename);
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

static void loadEntities(cJSON *entitiesJSON)
{
	cJSON *entityJSON;
	Entity *e;
	
	for (entityJSON = entitiesJSON->child ; entityJSON != NULL ; entityJSON = entityJSON->next)
	{
		e = createEntity(cJSON_GetObjectItem(entityJSON, "type")->valuestring);
		e->x = cJSON_GetObjectItem(entityJSON, "x")->valueint;
		e->y = cJSON_GetObjectItem(entityJSON, "y")->valueint;
		e->visible = cJSON_GetObjectItem(entityJSON, "visible")->valueint;
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
			level.entityTail->next = e;
			level.entityTail = e;
			
			e->alive = 1;
			
			def->initFunc(e);
			
			return e;
		}
	}
	
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "No such entity def '%s'", type);
	exit(1);
	
	return NULL;
}

void initEntityDefs(void)
{
	entityDefTail = &entityDefHead;
	
	addEntityDef("Diamond", initDiamond);
	addEntityDef("RedGuy", initRedGuy);
	addEntityDef("GreenGuy", initGreenGuy);
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
