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

#include "widgets.h"

static Widget widgetHead;
static Widget *widgetTail;
static Widget *selectedWidget;

static void loadWidgets(void);
static void loadWidgetSet(char *filename);
static void loadWidget(cJSON *root);

void initWidgets(void)
{
	memset(&widgetHead, 0, sizeof(Widget));
	widgetTail = &widgetHead;
	
	loadWidgets();
}

void doWidgets(void)
{
	Widget *w;
	float x, y;
	
	selectedWidget = NULL;
	
	x = app.mouse.x;
	x /= app.scaleX;
	
	y = app.mouse.y;
	y /= app.scaleY;
	
	for (w = widgetHead.next ; w != NULL ; w = w->next)
	{
		if (w->visible && !w->disabled && collision(x, y, 1, 1, w->x, w->y, w->w, w->h))
		{
			selectedWidget = w;
			
			if (app.mouse.button[SDL_BUTTON_LEFT])
			{
				w->action();
				
				app.mouse.button[SDL_BUTTON_LEFT] = 0;
			}
		}
	}
}

void drawWidgets(void)
{
	Widget *w;
	
	for (w = widgetHead.next ; w != NULL ; w = w->next)
	{
		if (w->visible)
		{
			setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
			
			if (w->disabled)
			{
				setGLRectangleBatchColor(1.0, 1.0, 1.0, 0.5);
			}
			
			if (w == selectedWidget)
			{
				setGLRectangleBatchColor(0.0, 1.0, 0.0, 1.0);
			}
			
			switch (w->type)
			{
				case WT_BUTTON:
					calcTextDimensions(w->label, &w->w, &w->h);
					if (w->centered)
					{
						w->x = (SCREEN_WIDTH - w->w) / 2;
					}
					drawText(w->x, w->y, TA_LEFT, w->label);
					break;
					
				case WT_SLIDER:
					break;
					
				case WT_SPINNER:
					break;
			}
		}
	}
}

void showWidgetGroup(const char *group)
{
	Widget *w;
	
	for (w = widgetHead.next ; w != NULL ; w = w->next)
	{
		w->visible = (strcmp(w->group, group) == 0);
	}
	
	selectedWidget = NULL;
}

Widget *getWidget(const char *name, const char *group)
{
	Widget *w;
	
	for (w = widgetHead.next ; w != NULL ; w = w->next)
	{
		if (strcmp(w->name, name) == 0 && strcmp(w->group, group) == 0)
		{
			return w;
		}
	}
	
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "No such widget [name = '%s', group = '%s']", name, group);
	exit(1);

	return NULL;
}

static void loadWidgets(void)
{
	char **filenames;
	char path[MAX_FILENAME_LENGTH];
	int count, i;

	filenames = getFileList("data/widgets", &count);

	for (i = 0 ; i < count ; i++)
	{
		sprintf(path, "data/widgets/%s", filenames[i]);

		loadWidgetSet(path);

		free(filenames[i]);
	}

	free(filenames);
}

static void loadWidgetSet(char *filename)
{
	cJSON *root, *node;
	char *text;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	text = readFile(filename);
	root = cJSON_Parse(text);
	
	for (node = root->child ; node != NULL ; node = node->next)
	{
		loadWidget(node);
	}
	
	cJSON_Delete(root);
	
	free(text);
}

static void loadWidget(cJSON *root)
{
	Widget *w;
	
	w = malloc(sizeof(Widget));
	memset(w, 0, sizeof(Widget));
	widgetTail->next = w;
	widgetTail = w;
	
	w->type = lookup(cJSON_GetObjectItem(root, "type")->valuestring);
	STRNCPY(w->name, cJSON_GetObjectItem(root, "name")->valuestring, MAX_NAME_LENGTH);
	STRNCPY(w->group, cJSON_GetObjectItem(root, "group")->valuestring, MAX_NAME_LENGTH);
	STRNCPY(w->label, cJSON_GetObjectItem(root, "label")->valuestring, MAX_NAME_LENGTH);
	
	w->x = cJSON_GetObjectItem(root, "x")->valueint;
	w->y = cJSON_GetObjectItem(root, "y")->valueint;
	
	if (w->x == -1)
	{
		w->centered = 1;
	}
}
