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

#include "init.h"

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
	int windowFlags;
	
	/* done in src/plat/ */
	createSaveFolder();
	
	windowFlags = 0;
	
	if (app.config.fullscreen)
	{
		windowFlags |= SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("Couldn't initialize SDL Mixer\n");
		exit(1);
    }

    Mix_AllocateChannels(MAX_SND_CHANNELS);
	
	app.config.soundVolume = 128;
	app.config.musicVolume = 128;

	Mix_Volume(-1, app.config.soundVolume);
	Mix_VolumeMusic(app.config.musicVolume);
	
	app.config.winWidth = 540;
	app.config.winHeight = 960;
	
	app.window = SDL_CreateWindow("3Guys", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, app.config.winWidth, app.config.winHeight, SDL_WINDOW_OPENGL);
	
	app.glContext = SDL_GL_CreateContext(app.window);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
		exit(1);
	}
}

void initOpenGL(void)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, app.config.winWidth, app.config.winHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	initGLRectangleBatch();
}

void initGameSystem(void)
{
	int i, numInitFuns;
	void (*initFuncs[]) (void) = {
		initLookups,
		initStrings,
		initAtlas,
		initFonts,
		initGraphics,
		initWidgets,
		initEntityDefs,
		initSprites,
		initSounds,
		initGame
	};

	numInitFuns = sizeof(initFuncs) / sizeof(void*);

	for (i = 0 ; i < numInitFuns ; i++)
	{
		showLoadingStep(i + 1, numInitFuns);

		initFuncs[i]();
	}
}

/*
 * Just in case the initial loading takes a while on the target machine. The rest of the loading a pretty quick by comparison.
 */
static void showLoadingStep(float step, float maxSteps)
{
	/*
	SDL_Rect r;

	prepareScene();

	r.w = SCREEN_WIDTH - 400;
	r.h = 14;
	r.x = (SCREEN_WIDTH / 2) - r.w / 2;
	r.y = (SCREEN_HEIGHT / 2) - r.h / 2;

	SDL_SetRenderDrawColor(app.renderer, 128, 128, 128, 255);
	SDL_RenderDrawRect(app.renderer, &r);

	r.w *= (step / maxSteps);
	r.x += 2;
	r.y += 2;
	r.w -= 4;
	r.h -= 4;

	SDL_SetRenderDrawColor(app.renderer, 128, 196, 255, 255);
	SDL_RenderFillRect(app.renderer, &r);

	presentScene();

	SDL_Delay(1);
	*/
}

void cleanup(void)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Cleaning up ...");
	
	SDL_GL_DeleteContext(app.glContext);
	
	SDL_DestroyWindow(app.window);
	
	TTF_Quit();
	
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Done.");
	
	SDL_Quit();
}
