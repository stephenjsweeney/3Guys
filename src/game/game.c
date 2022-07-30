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
#include "game.h"
#include "../json/cJSON.h"
#include "../system/lookup.h"
#include "../util/util.h"
#include "../system/io.h"

extern App app;
extern Game game;


void initGame(void)
{
	memset(&game, 0, sizeof(Game));
	
	loadGame();
}

void loadGame(void)
{
	cJSON *root, *node;
	char *text, *filename;
	
	filename = buildFormattedString("%s/%s", app.saveDir, SAVE_FILENAME);

	if (fileExists(filename))
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

		text = readFile(filename);
		root = cJSON_Parse(text);
		
		game.levelsCompleted = cJSON_GetObjectItem(root, "levelsCompleted")->valueint;
		
		for (node = cJSON_GetObjectItem(root, "starsFound")->child ; node != NULL ; node = node->next)
		{
			game.starsFound[node->valueint] = 1;
		}
		
		for (node = cJSON_GetObjectItem(root, "starsAvailable")->child ; node != NULL ; node = node->next)
		{
			game.starsAvailable[node->valueint] = 1;
		}
		
		for (node = cJSON_GetObjectItem(root, "stats")->child ; node != NULL ; node = node->next)
		{
			game.stats[lookup(node->string)] = node->valueint;
		}
		
		cJSON_Delete(root);
		
		free(text);
	}
	
	free(filename);
}

void saveGame(void)
{
	char *out, *filename;
	cJSON *root, *starsFound, *starsAvailable, *stats;
	int i;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Saving Game ...");

	root = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(root, "levelsCompleted", game.levelsCompleted);
	
	starsFound = cJSON_CreateArray();
	
	cJSON_AddItemToObject(root, "starsFound", starsFound);
	
	for (i = 0 ; i < MAX_LEVELS ; i++)
	{
		if (game.starsFound[i] != 0)
		{
			cJSON_AddItemToArray(starsFound, cJSON_CreateNumber(i));
		}
	}
	
	starsAvailable = cJSON_CreateArray();
	
	cJSON_AddItemToObject(root, "starsAvailable", starsAvailable);
	
	for (i = 0 ; i < MAX_LEVELS ; i++)
	{
		if (game.starsAvailable[i] != 0)
		{
			cJSON_AddItemToArray(starsAvailable, cJSON_CreateNumber(i));
		}
	}
	
	stats = cJSON_CreateObject();
	
	cJSON_AddItemToObject(root, "stats", stats);
	
	for (i = 0 ; i < STAT_MAX ; i++)
	{
		cJSON_AddNumberToObject(stats, getLookupName("STAT_", i), game.stats[i]);
	}
	
	out = cJSON_Print(root);
	
	filename = buildFormattedString("%s/%s", app.saveDir, SAVE_FILENAME);

	writeFile(filename, out);
	
	free(filename);

	cJSON_Delete(root);
	
	free(out);
}

