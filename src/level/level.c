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

#include "level.h"

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
int getRandomFloorTile(void);
void destroyLevel(void);
static void doLevel(void);
static void doTips(void);
static void drawTips(void);
void failLevel(void);

static Atlas *tiles[MAX_TILES];
static Atlas *routeBlob;
static Atlas *routeLink;
static Atlas *highlight;
static Atlas *selectRect;
static Atlas *tools;
static Atlas *tnt;
static int moveTimer;
static int restarting;
static Background background;
static int show;
static int currentTip;
static int tickTimer;

void initLevel(int id)
{
	memset(&level, 0, sizeof(Level));
	
	initTiles();
	
	initPlayer();
	
	initEntities();
	
	initEffects();
	
	loadLevel(id);
	
	initGLRectangle(&background.rect, MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE);
	background.rect.texture = loadTexture("gfx/backgrounds/background2.jpg")->texture;
	
	routeBlob = getImageFromAtlas("gfx/sprites/routeBlob.png", 1);
	routeLink = getImageFromAtlas("gfx/sprites/routeLink.png", 1);
	highlight = getImageFromAtlas("gfx/sprites/highlight.png", 1);
	selectRect = getImageFromAtlas("gfx/sprites/select.png", 1);
	tools = getImageFromAtlas("gfx/sprites/tools.png", 1);
	tnt = getImageFromAtlas("gfx/sprites/tnt.png", 1);
	
	background.dx = (float) ((randF() - randF()) / 1000);
	background.dy = (float) ((randF() - randF()) / 1000);
	background.r = (float) (0.25 + (randF() * 0.5));
	background.g = (float) (0.25 + (randF() * 0.5));
	background.b = (float) (0.25 + (randF() * 0.5));
	
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
	
	app.delegate.logic = logic;
	app.delegate.draw = draw;
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
	
	background.x += background.dx;
	background.y += background.dy;
	
	tickTimer++;
	
	animateSprites();
	
	if (!wiping)
	{
		switch (show)
		{
			case SHOW_TIPS:
				doTips();
				break;
				
			default:
				doLevel();
				break;
		}
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
	}
}

static void doLevel(void)
{
	doPlayer();
		
	doEntities();
	
	doEffects();
	
	if (level.state == LS_INCOMPLETE)
	{
		if (level.walkRoute)
		{
			moveGuy();
			
			moveEntities();
			
			if (level.moves == 0)
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
}

static void moveGuy(void)
{
	if (--moveTimer < 1)
	{
		if (level.routeIndex == MAP_WIDTH * MAP_HEIGHT || level.route[level.routeIndex].x == -1)
		{
			level.walkRoute = 0;
			
			level.routeIndex = 0;
			
			if (level.moves > -1)
			{
				level.moves--;
			}
		}
		else
		{
			level.dx = level.route[level.routeIndex].x - level.guy->x;
			level.dy = level.route[level.routeIndex].y - level.guy->y;
			
			level.guy->x = level.route[level.routeIndex].x;
			level.guy->y = level.route[level.routeIndex].y;
			
			moveTimer = 4;
			
			level.route[level.routeIndex].x = -1;
			level.route[level.routeIndex].y = -1;
			
			level.routeIndex++;
			
			playSound(SND_WALK, 0);
		}
	}
}

void completeLevel(void)
{
	if (level.state == LS_INCOMPLETE)
	{
		level.state = LS_COMPLETE;
		level.finishTimer = FPS * 1.5;
		clearRoute();
	}
}

static void nextLevel(void)
{
	int nextLevel;
	
	initWipe(WIPE_OUT);
	
	nextLevel = level.id + 1;
	
	destroyLevel();
	
	initLevel(nextLevel);
}

void failLevel(void)
{
	if (level.state == LS_INCOMPLETE)
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
	
	initLevel(id);
}

static void draw(void)
{
	drawBackground(&background.rect, background.x, background.y, background.r, background.g, background.b);
	
	drawMap();
	
	drawRoute();
	
	drawEntities();
	
	drawEffects();
	
	drawTopBar();
	
	drawBottomBar();
	
	if (show == SHOW_TIPS)
	{
		drawTips();
	}
	
	drawWipe();
}

static void drawMap(void)
{
	int x, y, i;
	
	setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
	
	for (y = 0 ; y < MAP_HEIGHT ; y++)
	{
		for (x = 0 ; x < MAP_WIDTH ; x++)
		{
			i = level.data[x][y];
			
			if (i > 0)
			{
				drawGLRectangleBatch(&tiles[i]->rect, LEVEL_RENDER_X + x * TILE_SIZE, LEVEL_RENDER_Y + y * TILE_SIZE, 0);
			}
		}
	}
}

static void drawRoute(void)
{
	int i, x, y, start;
	
	setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
	
	start = (level.walkRoute) ? level.routeIndex : 0;
	
	for (i = start ; i < MAP_WIDTH * MAP_HEIGHT ; i++)
	{
		x = LEVEL_RENDER_X + level.route[i].x * TILE_SIZE;
		y = LEVEL_RENDER_Y + level.route[i].y * TILE_SIZE;
		
		if (level.route[i].x == -1)
		{
			if (i > start)
			{
				x = LEVEL_RENDER_X + level.route[i - 1].x * TILE_SIZE;
				y = LEVEL_RENDER_Y + level.route[i - 1].y * TILE_SIZE;
				
				x += TILE_SIZE / 2;
				y += TILE_SIZE / 2;
				
				drawGLRectangleBatch(&routeBlob->rect, x, y, 1);
			}
			
			return;
		}
		
		drawGLRectangleBatch(&highlight->rect, x, y, 0);
		
		glRectangleBatch.rotate = 1;
		
		if (i > start)
		{
			glRectangleBatch.angle = getAngle(level.route[i - 1].x, level.route[i - 1].y, level.route[i].x, level.route[i].y);
			drawGLRectangleBatch(&routeLink->rect, x, y, 0);
		}
		
		if (i < MAP_WIDTH * MAP_HEIGHT && level.route[i + 1].x != -1)
		{
			glRectangleBatch.angle = getAngle(level.route[i + 1].x, level.route[i + 1].y, level.route[i].x, level.route[i].y);
			drawGLRectangleBatch(&routeLink->rect, x, y, 0);
		}

		glRectangleBatch.rotate = 0;
	}
}

static void drawTopBar(void)
{
	useFont("cardigan32");
	
	setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
	
	drawText(LEVEL_RENDER_X, 20, TA_LEFT, app.strings[ST_LEVEL_NUM], level.id);
	
	if (level.moves > -1)
	{
		if (level.moves > 0 || level.state == LS_COMPLETE)
		{
			drawText(SCREEN_WIDTH - LEVEL_RENDER_X, 20, TA_RIGHT, app.strings[ST_NUM_MOVES], level.moves);
		}
		else
		{
			if (tickTimer % 60 < 30)
			{
				setGLRectangleBatchColor(1.0, 0.0, 0.0, 1.0);
			}
			
			drawText(SCREEN_WIDTH - LEVEL_RENDER_X, 20, TA_RIGHT, app.strings[ST_OUT_OF_MOVES]);
		}
	}
}

static void drawBottomBar(void)
{
	useFont("cardigan32");
	
	setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
	
	drawGLRectangleBatch(&level.guy->sprite->frames[0]->rect, LEVEL_RENDER_X + 15, 1180, 0);
	
	if (level.message == NULL)
	{
		drawGLRectangleBatch(&tools->rect, LEVEL_RENDER_X + 200, 1180, 0);
		
		drawGLRectangleBatch(&tnt->rect, LEVEL_RENDER_X + 400, 1180, 0);
		
		drawText(LEVEL_RENDER_X + 300, 1175, TA_LEFT, "%d", level.tools);
		
		drawText(LEVEL_RENDER_X + 500, 1175, TA_LEFT, "%d", level.tnt);
	}
	else
	{
		setTextWidth(700);
		drawText(LEVEL_RENDER_X + 100, 1175, TA_LEFT, level.message);
		setTextWidth(0);
	}
	
	drawRect(LEVEL_RENDER_X, 1160, SCREEN_WIDTH - (LEVEL_RENDER_X * 2), 110, 0.75f, 0.75f, 0.75f, 1.0f);
}

static void drawTips(void)
{
	int x, y, w, h;
	
	w = 500;
	h = 400;
	y = 300;
	
	x = (SCREEN_WIDTH - w) / 2;
	
	drawFilledRect(x, y, w, h, 0, 0, 0, 0.75f);
	
	drawRect(x, y, w, h, 1.0f, 1.0f, 1.0f, 1.0f);
	
	setTextWidth(600);
	
	useFont("cardigan32");
	
	setGLRectangleBatchColor(1.0, 1.0, 1.0, 1.0);
	
	drawText(x + 10, y + 10, TA_LEFT, level.tips[currentTip]);
	
	useFont("cardigan24");
	
	setTextWidth(0);
	
	drawText(x + 10, y + h - 34, TA_LEFT, app.strings[ST_CLICK_TO_CONTINUE]);
	
	drawText(x + w - 10, y + h - 34, TA_RIGHT, "%d / %d", currentTip + 1, level.numTips);
}

void addFloor(int x, int y)
{
	level.data[x][y] = getRandomFloorTile();
}

int getRandomFloorTile(void)
{
	return TILE_FLOOR + rand() % 3;
}
 
void destroyLevel(void)
{
	Entity *e;
	
	while (level.entityHead.next)
	{
		e = level.entityHead.next;
		level.entityHead.next = e->next;
		free(e);
	}
}
