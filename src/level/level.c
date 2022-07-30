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
#include "level.h"
#include "../game/levelSelect.h"
#include "../system/atlas.h"
#include "../game/options.h"
#include "../game/ending.h"
#include "../system/sprites.h"
#include "../system/widgets.h"
#include "../level/effects.h"
#include "../game/game.h"
#include "../system/text.h"
#include "../level/entities.h"
#include "../system/textures.h"
#include "../system/wipe.h"
#include "../level/levelLoader.h"
#include "../level/player.h"
#include "../system/draw.h"
#include "../system/sound.h"
#include "../util/maths.h"

enum
{
	SHOW_LEVEL,
	SHOW_TIPS,
	SHOW_PAUSE
};

extern App app;
extern Entity *self;
extern Game game;
extern Level level;

static void logic(void);
static void draw(void);
static void drawMap(void);
static void drawTopBar(void);
static void drawBottomBar(void);
static void initTiles(void);
static void drawRoute(void);
static void moveGuy(void);
static void nextLevel(void);
static void restartLevel(void);
static void doLevel(void);
static void doTips(void);
static void doPause(void);
static void drawTips(void);
static void drawPause(void);
static void resume(void);
static void tips(void);
static void options(void);
static void restart(void);
static void quit(void);
static void postOptions(void);
static void doBackground(void);
static void drawClippedBackground(void);

static AtlasImage *tiles[MAX_TILES];
static AtlasImage *routeBlob;
static AtlasImage *routeLink;
static AtlasImage *highlight;
static AtlasImage *selectRect;
static AtlasImage *tools;
static AtlasImage *tnt;
static int moveTimer;
static int restarting;
static Background background;
static int show;
static int currentTip;
static int tickTimer;
static int playingMusic = 0;

void initLevel(int id)
{
	game.stats[STAT_LEVELS_STARTED]++;

	memset(&level, 0, sizeof(Level));

	level.routeTail = &level.routeHead;

	initTiles();

	initPlayer();

	initEntities();

	initEffects();

	/* to handle demo */
	if (!loadLevel(id))
	{
		destroyLevel();

		initLevelSelect();

		return;
	}

	doEntities();

	background.texture = loadTexture("gfx/backgrounds/background2.jpg")->texture;
	background.x = LEVEL_RENDER_X;
	background.y = LEVEL_RENDER_Y;

	routeBlob = getImageFromAtlas("gfx/sprites/routeBlob.png", 1);
	routeLink = getImageFromAtlas("gfx/sprites/routeLink.png", 1);
	highlight = getImageFromAtlas("gfx/sprites/highlight.png", 1);
	selectRect = getImageFromAtlas("gfx/sprites/select.png", 1);
	tools = getImageFromAtlas("gfx/sprites/tools.png", 1);
	tnt = getImageFromAtlas("gfx/sprites/tnt.png", 1);

	background.dx = (-10.0 + rand() % 20) / 10.0;
	background.dy = (-10.0 + rand() % 20) / 10.0;

	background.r = 64 + rand() % 128;
	background.g = 64 + rand() % 128;
	background.b = 64 + rand() % 128;

	moveTimer = 0;

	tickTimer = 0;

	level.state = LS_INCOMPLETE;

	currentTip = 0;

	if (!restarting)
	{
		initWipe(WIPE_IN);
		playSound(SND_WIPE, -1);
		show = level.tips != NULL ? SHOW_TIPS : SHOW_LEVEL;
	}
	else
	{
		initWipe(WIPE_FADE);
		playSound(SND_FADE, -1);
		show = SHOW_LEVEL;
	}

	restarting = 0;

	getWidget("resume", "level")->action = resume;
	getWidget("tips", "level")->action = tips;
	getWidget("options", "level")->action = options;
	getWidget("restart", "level")->action = restart;
	getWidget("quit", "level")->action = quit;

	if (level.tips == NULL)
	{
		getWidget("tips", "level")->disabled = 1;
	}

	if (!playingMusic)
	{
		loadMusic("music/periwinkle.mp3");
		playMusic(1);
		playingMusic = 1;
	}

	app.delegate.logic = logic;
	app.delegate.draw = draw;
	app.delegate.postOptions = postOptions;

	/*level.moves = 100;*/

	saveGame();
}

static void initTiles(void)
{
	int i;
	char filename[MAX_FILENAME_LENGTH];

	for (i = 1 ; i < MAX_TILES ; i++)
	{
		sprintf(filename, "gfx/tiles/%d.png", i);

		tiles[i] = getImageFromAtlas(filename, 0);
	}
}

static void logic(void)
{
	int wiping;

	wiping = doWipe();

	doBackground();

	tickTimer++;

	animateSprites();

	if (!wiping)
	{
		switch (show)
		{
			case SHOW_TIPS:
				doTips();
				break;

			case SHOW_PAUSE:
				doPause();
				break;

			default:
				doLevel();
				break;
		}
	}
}

static void doBackground(void)
{
	int w, h;

	SDL_QueryTexture(background.texture, NULL, NULL, &w, &h);

	background.x += background.dx;
	background.y += background.dy;

	if (background.x < -w)
	{
		background.x = (SCREEN_WIDTH - (SCREEN_WIDTH - w)) - 1;
	}

	if (background.x >= SCREEN_WIDTH - (SCREEN_WIDTH - w))
	{
		background.x = 0;
	}

	if (background.y < -h)
	{
		background.y = (SCREEN_HEIGHT - (SCREEN_HEIGHT - h)) - 1;
	}

	if (background.y >= SCREEN_HEIGHT - (SCREEN_HEIGHT - h))
	{
		background.y = 0;
	}
}

static void doTips(void)
{
	if (app.mouse.button[SDL_BUTTON_LEFT])
	{
		currentTip++;

		if (currentTip >= level.numTips)
		{
			show = SHOW_LEVEL;
		}

		app.mouse.button[SDL_BUTTON_LEFT] = 0;

		playSound(SND_HINT, 0);
	}
}

static void doPause(void)
{
	doWidgets();

	if (app.keyboard[SDL_SCANCODE_ESCAPE])
	{
		show = SHOW_LEVEL;
		showWidgetGroup("");
		app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
		playSound(SND_SELECT, 0);
	}
}

static void doLevel(void)
{
	doPlayer();

	doEntities();

	doEffects();

	if (level.state == LS_INCOMPLETE)
	{
		if (level.walkRoute && --moveTimer <= 0)
		{
			moveGuy();

			moveEntities();

			level.guy->x += level.dx;
			level.guy->y += level.dy;

			guyTouchOthers();

			if (level.moves == 0 && level.state != LS_COMPLETE)
			{
				failLevel();
			}
		}
	}
	else
	{
		level.finishTimer--;

		if (level.finishTimer == 0)
		{
			switch (level.state)
			{
				case LS_COMPLETE:
					initWipe(WIPE_OUT);
					playSound(SND_WIPE, -1);
					break;

				default:
					break;
			}
		}
		else if (level.finishTimer < 0)
		{
			switch (level.state)
			{
				case LS_COMPLETE:
					nextLevel();
					break;

				case LS_FAILED:
					restartLevel();
					break;

				default:
					break;
			}
		}
	}

	if (app.keyboard[SDL_SCANCODE_ESCAPE])
	{
		show = SHOW_PAUSE;
		showWidgetGroup("level");
		app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
		playSound(SND_SELECT, 0);
	}
}

static void moveGuy(void)
{
	Entity *candidates[MAX_CANDIDATES];
	RouteNode *node;
	int n, blocked;

	level.dx = level.routeHead.next->x - level.guy->x;
	level.dy = level.routeHead.next->y - level.guy->y;

	moveTimer = 10 - (app.config.speed * 2);

	node = level.routeHead.next;
	level.routeHead.next = node->next;
	free(node);

	game.stats[STAT_SQUARES]++;

	playSound(SND_WALK, 0);

	getEntitiesAt(level.guy->x + level.dx, level.guy->y + level.dy, &n, level.guy, candidates);

	blocked = 0;

	if (n == 1)
	{
		self = candidates[0];

		blocked = self->isBlocking();

		if (blocked)
		{
			level.dx = level.dy = 0;

			clearRoute();
		}
	}

	if (blocked || level.routeHead.next == NULL)
	{
		level.walkRoute = 0;

		if (level.moves > -1)
		{
			level.moves--;
		}

		game.stats[STAT_MOVES]++;
	}
}

void completeLevel(void)
{
	if (level.state == LS_INCOMPLETE)
	{
		level.state = LS_COMPLETE;
		level.finishTimer = FPS * 1.5;
		clearRoute();

		game.stats[STAT_LEVELS_FINISHED]++;

		updateStar();
	}
}

static void nextLevel(void)
{
	int nextLevel;

	game.levelsCompleted = MAX(level.id, game.levelsCompleted);

	initWipe(WIPE_OUT);

	nextLevel = level.id + 1;

	destroyLevel();

	destroyEntities();

	if (nextLevel <= MAX_LEVELS)
	{
		initLevel(nextLevel);
	}
	else
	{
		saveGame();

		initEnding();
	}
}

void failLevel(void)
{
	if (level.state != LS_FAILED)
	{
		level.state = LS_FAILED;
		level.finishTimer = FPS * 2;
		clearRoute();

		playSound(SND_FAIL, -1);
	}
}

static void restartLevel(void)
{
	int id;

	id = level.id;

	restarting = 1;

	destroyLevel();

	destroyEntities();

	initLevel(id);
}

static void draw(void)
{
	drawClippedBackground();

	SDL_SetTextureColorMod(selectRect->texture, 255, 255, 255);

	drawEntities(1);

	drawMap();

	drawRoute();

	blitAtlasImage(selectRect, LEVEL_RENDER_X + level.guy->x * TILE_SIZE, LEVEL_RENDER_Y + level.guy->y * TILE_SIZE, 0);

	drawEntities(0);

	drawEffects();

	drawTopBar();

	drawBottomBar();

	if (show == SHOW_TIPS)
	{
		drawTips();
	}
	else if (show == SHOW_PAUSE)
	{
		drawPause();
	}

	drawWipe();
}

static void drawClippedBackground(void)
{
	int w, h;

	drawBackground(&background);

	w = LEVEL_RENDER_X + (MAP_WIDTH * TILE_SIZE);
	h = LEVEL_RENDER_Y + (MAP_HEIGHT * TILE_SIZE);

	drawFilledRect(0, 0, SCREEN_WIDTH, LEVEL_RENDER_Y, 0, 0, 0, 255);
	drawFilledRect(0, h, SCREEN_WIDTH, SCREEN_HEIGHT - h, 0, 0, 0, 255);

	drawFilledRect(0, 0, LEVEL_RENDER_X, SCREEN_HEIGHT, 0, 0, 0, 255);
	drawFilledRect(w, 0, SCREEN_WIDTH - w, SCREEN_HEIGHT, 0, 0, 0, 255);
}

static void drawMap(void)
{
	int x, y, i;

	for (y = 0 ; y < MAP_HEIGHT ; y++)
	{
		for (x = 0 ; x < MAP_WIDTH ; x++)
		{
			i = level.data[x][y];

			if (i > 0)
			{
				blitAtlasImage(tiles[i], LEVEL_RENDER_X + x * TILE_SIZE, LEVEL_RENDER_Y + y * TILE_SIZE, 0);
			}
		}
	}
}

static void drawRoute(void)
{
	RouteNode *n, *prev;
	int x, y;

	prev = &level.routeHead;

	if (level.routeHead.next != NULL)
	{
		for (n = level.routeHead.next ; n != NULL ; n = n->next)
		{
			x = LEVEL_RENDER_X + n->x * TILE_SIZE;
			y = LEVEL_RENDER_Y + n->y * TILE_SIZE;

			blitAtlasImage(highlight, x, y, 0);

			x += TILE_SIZE / 2;
			y += TILE_SIZE / 2;

			if (n != level.routeHead.next)
			{
				blitAtlasImageRotated(routeLink, x, y, 1, getAngle(prev->x, prev->y, n->x, n->y));
			}

			if (n->next != NULL)
			{
				blitAtlasImageRotated(routeLink, x, y, 1, getAngle(n->next->x, n->next->y, n->x, n->y));
			}

			prev = n;
		}

		x = LEVEL_RENDER_X + level.routeTail->x * TILE_SIZE;
		y = LEVEL_RENDER_Y + level.routeTail->y * TILE_SIZE;

		x += TILE_SIZE / 2;
		y += TILE_SIZE / 2;

		blitAtlasImage(routeBlob, x, y, 1);
	}
}

static void drawTopBar(void)
{
	setTextColor(255, 255, 255, 255);

	drawText(LEVEL_RENDER_X, 20, TEXT_ALIGN_LEFT, 32, app.strings[ST_LEVEL_NUM], level.id);

	if (level.moves != -1)
	{
		if (level.moves > 0 || level.state == LS_COMPLETE)
		{
			drawText(SCREEN_WIDTH - LEVEL_RENDER_X, 20, TEXT_ALIGN_RIGHT, 32, app.strings[ST_NUM_MOVES], level.moves);
		}
		else
		{
			if (tickTimer % 60 < 30)
			{
				setTextColor(255, 0, 0, 255);
			}

			drawText(SCREEN_WIDTH - LEVEL_RENDER_X, 20, TEXT_ALIGN_RIGHT, 32, app.strings[ST_OUT_OF_MOVES]);
		}
	}
}

static void drawBottomBar(void)
{
	blitAtlasImage(level.guy->sprite->frames[0], LEVEL_RENDER_X + 15, 1180, 0);

	if (level.message == NULL)
	{
		if (level.tools > 0)
		{
			blitAtlasImage(tools, LEVEL_RENDER_X + 175, 1187, 0);
		}

		if (level.tnt > 0)
		{
			blitAtlasImage(tnt, LEVEL_RENDER_X + 375, 1187, 0);
		}

		if (level.guy->carrying)
		{
			blitAtlasImage(level.guy->carrying->sprite->frames[0], LEVEL_RENDER_X + 575, 1200, 0);
		}

		if (level.tools > 0)
		{
			drawText(LEVEL_RENDER_X + 265, 1185, TEXT_ALIGN_LEFT, 40, "%d", level.tools);
		}

		if (level.tnt > 0)
		{
			drawText(LEVEL_RENDER_X + 465, 1185, TEXT_ALIGN_LEFT, 40, "%d", level.tnt);
		}
	}
	else
	{
		setTextWidth(700);
		drawText(LEVEL_RENDER_X + 100, 1175, TEXT_ALIGN_LEFT, 32, level.message);
		setTextWidth(0);
	}

	drawRect(LEVEL_RENDER_X, 1160, SCREEN_WIDTH - (LEVEL_RENDER_X * 2), 110, 192, 192, 192, 255);
}

static void drawTips(void)
{
	int x, y, w, h;

	w = 500;
	h = 400;
	y = 300;

	x = (SCREEN_WIDTH - w) / 2;

	drawFilledRect(x, y, w, h, 0, 0, 0, 192);

	drawRect(x, y, w, h, 255, 255, 255, 255);

	setTextWidth(600);

	setTextColor(255, 255, 255, 255);

	drawShadowText(x + 10, y + 10, TEXT_ALIGN_LEFT, 32, level.tips[currentTip]);

	setTextWidth(0);

	drawShadowText(x + 10, y + h - 34, TEXT_ALIGN_LEFT, 24, app.strings[ST_CLICK_TO_CONTINUE]);

	drawShadowText(x + w - 10, y + h - 34, TEXT_ALIGN_RIGHT, 24, "%d / %d", currentTip + 1, level.numTips);
}

static void drawPause(void)
{
	int x, y, w, h;

	w = 500;
	h = 700;

	x = (SCREEN_WIDTH - w) / 2;
	y = (SCREEN_HEIGHT - h) / 2;

	drawFilledRect(x, y, w, h, 0, 0, 0, 192);

	drawRect(x, y, w, h, 255, 255, 255, 255);

	setTextColor(255, 255, 0, 255);

	drawText(x + (w / 2), y + 35, TEXT_ALIGN_CENTER, 48, app.strings[ST_PAUSE]);

	drawWidgets();
}

void addFloor(int x, int y)
{
	level.data[x][y] = getRandomFloorTile();
}

int getRandomFloorTile(void)
{
	return TILE_FLOOR + rand() % 3;
}

static void resume(void)
{
	show = SHOW_LEVEL;

	playSound(SND_BUTTON, 0);
}

static void tips(void)
{
	show = SHOW_TIPS;

	currentTip = 0;

	playSound(SND_BUTTON, 0);
}

static void options(void)
{
	initOptions();

	playSound(SND_BUTTON, 0);
}

static void restart(void)
{
	playSound(SND_BUTTON, 1);

	restartLevel();
}

static void quit(void)
{
	initLevelSelect();

	playingMusic = 0;

	playSound(SND_BUTTON, 0);
}

static void postOptions(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	initWipe(WIPE_FADE);

	showWidgetGroup("level");
}

void destroyLevel(void)
{
	Entity *e;
	RouteNode *n;
	int i;

	while (level.entityHead.next)
	{
		e = level.entityHead.next;
		level.entityHead.next = e->next;
		free(e);
	}

	while (level.routeHead.next)
	{
		n = level.routeHead.next;
		level.routeHead.next = n->next;
		free(n);
	}

	for (i = 0 ; i < level.numTips ; i++)
	{
		free(level.tips[i]);
	}

	free(level.tips);
}

