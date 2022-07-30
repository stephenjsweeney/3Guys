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
#include "init.h"
#include "locale.h"
#include <SDL2/SDL_image.h>
#include "../json/cJSON.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "../system/strings.h"
#include "../system/atlas.h"
#include "../system/sprites.h"
#include "../game/game.h"
#include "../system/sound.h"
#include "../util/util.h"
#include "../system/i18n.h"
#include "../system/io.h"
#include "../system/text.h"
#include "../level/levelLoader.h"
#include "../system/draw.h"
#include "../system/widgets.h"
#include "../plat/win32/win32Init.h"

extern App app;

static void loadConfig(void);
static void loadConfigFile(char *filename);
static void showLoadingStep(float step, float maxSteps);

void init18N(int argc, char *argv[])
{
	int i;
	int languageId = -1;

	setlocale(LC_NUMERIC, "");

	for (i = 1 ; i < argc ; i++)
	{
		if (strcmp(argv[i], "-language") == 0)
		{
			languageId = i + 1;

			if (languageId >= argc)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "You must specify a language to use with -language. Using default.");
			}
		}
	}

	setLanguage("3Guys", languageId == -1 ? NULL : argv[languageId]);

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Numeric is %s", setlocale(LC_NUMERIC, "C"));
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "atof(2.75) is %f", atof("2.75"));
}

void initSDL(void)
{
	/* done in src/plat/ */
	createSaveFolder();

	loadConfig();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
		exit(1);
    }

	Mix_Volume(-1, app.config.soundVolume);
	Mix_VolumeMusic(app.config.musicVolume);

	app.window = SDL_CreateWindow("3Guys", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, app.config.winWidth, app.config.winHeight, SDL_WINDOW_OPENGL);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
		exit(1);
	}
}

void initGameSystem(void)
{
	int i, numInitFuncs;
	void (*initFuncs[]) (void) = {
		initStrings,
		initFonts,
		initWidgets,
		initEntityDefs,
		initSprites,
		initSounds,
		initGame
	};

	initAtlas();

	initGraphics();

	numInitFuncs = sizeof(initFuncs) / sizeof(void*);

	for (i = 0 ; i < numInitFuncs ; i++)
	{
		showLoadingStep(i + 1, numInitFuncs);

		initFuncs[i]();
	}
}

/*
 * Just in case the initial loading takes a while on the target machine. The rest of the loading a pretty quick by comparison.
 */
static void showLoadingStep(float step, float maxSteps)
{
	SDL_Rect r;

	prepareScene();

	r.w = SCREEN_WIDTH - 400;
	r.h = 14;
	r.x = (SCREEN_WIDTH / 2) - r.w / 2;
	r.y = (SCREEN_HEIGHT / 2) - r.h / 2;

	drawFilledRect(r.x, r.y, r.w, r.h, 128, 128, 128, 255);

	r.x++;
	r.y++;
	r.w -= 2;
	r.h -= 2;

	drawFilledRect(r.x, r.y, r.w, r.h, 0, 0, 0, 255);

	r.w *= (step / maxSteps);
	r.x++;
	r.y++;
	r.w -= 2;
	r.h -= 2;

	drawFilledRect(r.x, r.y, r.w, r.h, 128, 192, 255, 255);

	presentScene();

	SDL_Delay(1);
}

static void loadConfig(void)
{
	char *filename;

	/* load default config first */
	loadConfigFile("data/app/"CONFIG_FILENAME);

	filename = buildFormattedString("%s/%s", app.saveDir, CONFIG_FILENAME);

	if (fileExists(filename))
	{
		loadConfigFile(filename);
	}

	free(filename);

	/* so that the player doesn't get confused if this is a new game */
	saveConfig();
}

static void loadConfigFile(char *filename)
{
	cJSON *root;
	char *text;

	text = readFile(filename);

	root = cJSON_Parse(text);

	app.config.winWidth = cJSON_GetObjectItem(root, "winWidth")->valueint;
	app.config.winHeight = cJSON_GetObjectItem(root, "winHeight")->valueint;
	app.config.musicVolume = cJSON_GetObjectItem(root, "musicVolume")->valueint;
	app.config.soundVolume = cJSON_GetObjectItem(root, "soundVolume")->valueint;
	app.config.sex = cJSON_GetObjectItem(root, "sex")->valueint;
	app.config.speed = cJSON_GetObjectItem(root, "speed")->valueint;

	/* volumes are 0 - 10 */
	app.config.soundVolume *= 12.8;
	app.config.musicVolume *= 12.8;

	cJSON_Delete(root);
	free(text);
}

void saveConfig(void)
{
	char *out, *filename;
	cJSON *root;

	filename = buildFormattedString("%s/%s", app.saveDir, CONFIG_FILENAME);

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Saving config ...");

	root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "winWidth", app.config.winWidth);
	cJSON_AddNumberToObject(root, "winHeight", app.config.winHeight);
	cJSON_AddNumberToObject(root, "musicVolume", app.config.musicVolume / 12.8);
	cJSON_AddNumberToObject(root, "soundVolume", app.config.soundVolume / 12.8);
	cJSON_AddNumberToObject(root, "sex", app.config.sex);
	cJSON_AddNumberToObject(root, "speed", app.config.speed);

	out = cJSON_Print(root);

	if (!writeFile(filename, out))
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Failed to save config");
	}

	cJSON_Delete(root);

	free(out);

	free(filename);
}

void cleanup(void)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Cleaning up ...");

	SDL_DestroyWindow(app.window);

	TTF_Quit();

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Done.");

	SDL_Quit();
}
