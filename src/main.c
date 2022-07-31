/*
Copyright (C) 2018,2022 Parallel Realities

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

#include <time.h>

#include "common.h"

#include "game/ending.h"
#include "game/title.h"
#include "main.h"
#include "system/draw.h"
#include "system/init.h"
#include "system/input.h"
#include "system/lookup.h"
#include "test/test.h"

App		app;
Colors	colors;
Dev		dev;
Entity *self;
Game	game;
Level	level;

static void capFrameRate(long *then, float *remainder);
static void handleCommandLine(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	long  then, nextSecond, frames;
	float remainder;

	memset(&app, 0, sizeof(App));

	atexit(cleanup);

	srand(time(NULL));

	initLookups();

	init18N(argc, argv);

	initSDL();

	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

	initGameSystem();

	handleCommandLine(argc, argv);

	/*
	dev.debug = 1;
	dev.showFPS = 1;
	*/

	remainder = 0;

	frames = 0;

	nextSecond = SDL_GetTicks() + 1000;

	while (1)
	{
		then = SDL_GetTicks();

		handleInput();

		app.delegate.logic();

		prepareScene();

		app.delegate.draw();

		presentScene();

		capFrameRate(&then, &remainder);

		frames++;

		game.stats[STAT_TIME_PLAYED]++;

		if (SDL_GetTicks() >= nextSecond)
		{
			dev.fps = frames;

			frames = 0;

			nextSecond = SDL_GetTicks() + 1000;
		}
	}

	return 0;
}

static void capFrameRate(long *then, float *remainder)
{
	long wait;

	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	wait -= (SDL_GetTicks() - *then);

	if (wait < 1)
	{
		wait = 1;
	}

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}

static void handleCommandLine(int argc, char *argv[])
{
	int i, levelNum, ending;

	levelNum = 0;
	ending = 0;

	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-level") == 0)
		{
			levelNum = atoi(argv[++i]);
		}

		if (strcmp(argv[i], "-debug") == 0)
		{
			dev.debug = dev.showFPS = 1;
		}

		if (strcmp(argv[i], "-ending") == 0)
		{
			ending = 1;
		}
	}

	if (levelNum > 0)
	{
		initLevelTest(levelNum);
	}
	else if (ending)
	{
		initEnding();
	}
	else
	{
		initTitle();
	}
}
