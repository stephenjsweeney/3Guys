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

#include "atlas.h"

static void loadAtlasData(void);

static Atlas atlases[NUM_ATLAS_BUCKETS];
static Texture *atlasTexture;

void initAtlas(void)
{
	memset(&atlases, 0, sizeof(Atlas) * NUM_ATLAS_BUCKETS);
	
	atlasTexture = getTexture("gfx/atlas/atlas.png");

	loadAtlasData();
}

Atlas *getImageFromAtlas(char *filename, int required)
{
	Atlas *a;
	unsigned long i;

	i = hashcode(filename) % NUM_ATLAS_BUCKETS;
	
	for (a = atlases[i].next ; a != NULL ; a = a->next)
	{
		if (strcmp(a->filename, filename) == 0)
		{
			return a;
		}
	}
	
	if (required)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "No such atlas image '%s'", filename);
		exit(1);
	}
	
	return NULL;
}

static void loadAtlasData(void)
{
	Atlas *atlas, *a;
	int w, h;
	float x1, y1, x2, y2;
	cJSON *root, *node;
	char *text, *filename;
	unsigned long i;
	
	text = readFile("data/atlas/atlas.json");

	root = cJSON_Parse(text);
	
	for (node = root->child ; node != NULL ; node = node->next)
	{
		filename = cJSON_GetObjectItem(node, "filename")->valuestring;
		w = cJSON_GetObjectItem(node, "w")->valueint;
		h = cJSON_GetObjectItem(node, "h")->valueint;
		x1 = cJSON_GetObjectItem(node, "x1")->valuedouble;
		y1 = cJSON_GetObjectItem(node, "y1")->valuedouble;
		x2 = cJSON_GetObjectItem(node, "x2")->valuedouble;
		y2 = cJSON_GetObjectItem(node, "y2")->valuedouble;

		i = hashcode(filename) % NUM_ATLAS_BUCKETS;

		a = &atlases[i];

		/* horrible bit to look for the tail */
		while (a->next)
		{
			a = a->next;
		}
		
		atlas = malloc(sizeof(Atlas));
		memset(atlas, 0, sizeof(Atlas));
		a->next = atlas;
		
		STRNCPY(atlas->filename, filename, MAX_FILENAME_LENGTH);
		setGLRectangleSize(&atlas->rect, w, h);
		setGLRectangleTextureCoords(&atlas->rect, x1, y1, x2, y2);
		
		atlas->rect.texture = atlasTexture->texture;
	}
	
	cJSON_Delete(root);
	
	free(text);
}
