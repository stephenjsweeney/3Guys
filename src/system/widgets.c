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
static Atlas *plus;
static Atlas *minus;
static Atlas *arrow;
static Atlas *whiteSquare;

static void loadWidgets(void);
static void loadWidgetSet(char *filename);
static void loadWidget(cJSON *root);
static void createOptions(Widget *w, cJSON *options);
static void createSpinnerControls(Widget *parent);
static void createSliderControls(Widget *parent);

void initWidgets(void)
{
	memset(&widgetHead, 0, sizeof(Widget));
	widgetTail = &widgetHead;
	
	plus = getImageFromAtlas("gfx/main/plus.png", 1);
	minus = getImageFromAtlas("gfx/main/minus.png", 1);
	arrow = getImageFromAtlas("gfx/main/arrow.png", 1);
	whiteSquare = getImageFromAtlas("gfx/main/whiteSquare.png", 1);
	
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
				switch (w->type)
				{
					case WT_BUTTON:
					case WT_IMAGE:
						w->action();
						break;
						
					case WT_SLIDER_MINUS:
						w->parent->value--;
						w->parent->action();
						break;
						
					case WT_SLIDER_PLUS:
						w->parent->value++;
						w->parent->action();
						break;
						
					case WT_SPINNER_LEFT:
						w->parent->value = wrap(w->parent->value + 1, 0, w->parent->numOptions - 1);
						w->parent->action();
						break;
						
					case WT_SPINNER_RIGHT:
						w->parent->value = wrap(w->parent->value - 1, 0, w->parent->numOptions - 1);
						w->parent->action();
						break;
				}
				
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
					
				case WT_IMAGE:
					drawGLRectangleBatch(&w->atlas->rect, w->x, w->y, 0);
					break;
					
				case WT_SLIDER:
					drawFilledRect(w->x + 260, w->y + 10, 300, 40, 0, 0, 0, 0.75f);
					drawRect(w->x + 260, w->y + 10, 300, 40, 1.0f, 1.0f, 1.0f, 1.0f);
					drawFilledRect(w->x + 262, w->y + 12, 296 * (w->value / w->maxValue), 36, 1, 1, 1, 1.0f);
					setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
					drawText(w->x, w->y, TA_LEFT, w->label);
					break;
					
				case WT_SLIDER_MINUS:
					drawGLRectangleBatch(&minus->rect, w->x, w->y, 0);
					break;
					
				case WT_SLIDER_PLUS:
					drawGLRectangleBatch(&plus->rect, w->x, w->y, 0);
					break;
					
				case WT_SPINNER:
					setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
					calcTextDimensions(w->label, &w->w, &w->h);
					drawText(w->x, w->y, TA_LEFT, w->label);
					drawText(w->x + 400, w->y, TA_CENTER, w->options[(int)w->value]);
					break;
					
				case WT_SPINNER_LEFT:
					glRectangleBatch.rotate = 1;
					glRectangleBatch.angle = 180;
					drawGLRectangleBatch(&arrow->rect, w->x, w->y, 0);
					glRectangleBatch.rotate = 0;
					break;
					
				case WT_SPINNER_RIGHT:
					drawGLRectangleBatch(&arrow->rect, w->x, w->y, 0);
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
	w->x = cJSON_GetObjectItem(root, "x")->valueint;
	w->y = cJSON_GetObjectItem(root, "y")->valueint;
	
	if (w->x == -1)
	{
		w->centered = 1;
	}
	
	switch (w->type)
	{
		case WT_BUTTON:
			STRNCPY(w->label, cJSON_GetObjectItem(root, "label")->valuestring, MAX_NAME_LENGTH);
			break;
			
		case WT_IMAGE:
			w->atlas = getImageFromAtlas(cJSON_GetObjectItem(root, "image")->valuestring, 1);
			w->w = w->atlas->rect.w;
			w->h = w->atlas->rect.h;
			break;
			
		case WT_SLIDER:
			STRNCPY(w->label, cJSON_GetObjectItem(root, "label")->valuestring, MAX_NAME_LENGTH);
			w->maxValue = cJSON_GetObjectItem(root, "maxValue")->valueint;
			createSliderControls(w);
			break;
			
		case WT_SPINNER:
			STRNCPY(w->label, cJSON_GetObjectItem(root, "label")->valuestring, MAX_NAME_LENGTH);
			createOptions(w, cJSON_GetObjectItem(root, "options"));
			createSpinnerControls(w);
			break;
	}
}

static void createOptions(Widget *w, cJSON *options)
{
	int n;
	cJSON *node;
	char *option;
	
	n = 0;
	
	for (node = options->child ; node != NULL ; node = node->next)
	{
		n++;
	}
	
	w->options = malloc(sizeof(char*) * n);
	w->numOptions = n;
	
	n = 0;
	
	for (node = options->child ; node != NULL ; node = node->next)
	{
		option = node->valuestring;
		w->options[n] = malloc(strlen(option));
		strcpy(w->options[n], option);
		
		n++;
	}
}

static void createSpinnerControls(Widget *parent)
{
	Widget *w;
	
	w = malloc(sizeof(Widget));
	memset(w, 0, sizeof(Widget));
	widgetTail->next = w;
	widgetTail = w;
	w->type = WT_SPINNER_LEFT;
	snprintf(w->name, MAX_NAME_LENGTH, "%sL", parent->name);
	strcpy(w->group, parent->group);
	w->parent = parent;
	w->x = parent->x + 200;
	w->y = parent->y + 14;
	w->w = arrow->rect.w;
	w->h = arrow->rect.h;
	
	w = malloc(sizeof(Widget));
	memset(w, 0, sizeof(Widget));
	widgetTail->next = w;
	widgetTail = w;
	w->type = WT_SPINNER_RIGHT;
	snprintf(w->name, MAX_NAME_LENGTH, "%sR", parent->name);
	strcpy(w->group, parent->group);
	w->parent = parent;
	w->x = SCREEN_WIDTH - 75;
	w->y = parent->y + 14;
	w->w = arrow->rect.w;
	w->h = arrow->rect.h;
}

static void createSliderControls(Widget *parent)
{
	Widget *w;
	
	w = malloc(sizeof(Widget));
	memset(w, 0, sizeof(Widget));
	widgetTail->next = w;
	widgetTail = w;
	w->type = WT_SLIDER_MINUS;
	snprintf(w->name, MAX_NAME_LENGTH, "%s-", parent->name);
	strcpy(w->group, parent->group);
	w->parent = parent;
	w->x = parent->x + 200;
	w->y = parent->y + 14;
	w->w = arrow->rect.w;
	w->h = arrow->rect.h;
	
	w = malloc(sizeof(Widget));
	memset(w, 0, sizeof(Widget));
	widgetTail->next = w;
	widgetTail = w;
	w->type = WT_SLIDER_PLUS;
	snprintf(w->name, MAX_NAME_LENGTH, "%s+", parent->name);
	strcpy(w->group, parent->group);
	w->parent = parent;
	w->x = SCREEN_WIDTH - 75;
	w->y = parent->y + 14;
	w->w = arrow->rect.w;
	w->h = arrow->rect.h;
}
