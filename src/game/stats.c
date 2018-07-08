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

#include "stats.h"

static void logic(void);
static void draw(void);
static char *statString[STAT_MAX];
static void back(void);

static Background background;
static Widget *backWidget;

void initStats(void)
{
	initGLRectangle(&background.rect, SCREEN_WIDTH, SCREEN_HEIGHT);
	background.rect.texture = loadTexture("gfx/backgrounds/background.jpg")->texture;
	background.r = background.g = background.b = 1.0;
	
	showWidgetGroup("stats");
	
	initWipe(WIPE_FADE);
	
	statString[STAT_MOVES] = _("Moves made");
	statString[STAT_SQUARES] = _("Squares walked");
	statString[STAT_DIAMONDS] = _("Diamonds eaten");
	statString[STAT_STARS] = _("Stars eaten");
	statString[STAT_LEVELS_STARTED] = _("Levels started");
	statString[STAT_LEVELS_FINISHED] = _("Levels completed");
	statString[STAT_LOSSES] = _("Guys lost");
	statString[STAT_KEYS_USED] = _("Keys used");
	statString[STAT_TOOLS_USED] = _("Tools used");
	statString[STAT_TNT_USED] = _("TNT used");
	statString[STAT_TIME_PLAYED] = _("Time played");
	
	backWidget = getWidget("back", "stats");
	backWidget->action = back;
	
	app.delegate.logic = logic;
	app.delegate.draw = draw;
}

static void logic(void)
{
	doWidgets();
	
	doWipe();
}

static void draw(void)
{
	int i, y;
	
	drawBackground(&background);
	
	drawShadowText(SCREEN_WIDTH / 2, 100, TEXT_ALIGN_CENTER, 60, app.strings[ST_STATS]);
	
	y = 250;
	
	for (i = 0 ; i < STAT_MAX ; i++)
	{
		drawShadowText(25, y, TEXT_ALIGN_LEFT, 35, statString[i]);
		
		if (i != STAT_TIME_PLAYED)
		{
			drawShadowText(SCREEN_WIDTH - 25, y, TEXT_ALIGN_RIGHT, 35, "%d", game.stats[i]);
		}
		else
		{
			drawShadowText(SCREEN_WIDTH - 25, y, TEXT_ALIGN_RIGHT, 35, "%s", timeToString(game.stats[i] / FPS, 1));
		}
		
		y += 75;
	}
	
	drawWidgets();
	
	drawWipe();
}

static void back(void)
{
	initTitle();
	
	playSound(SND_SELECT, 0);
}
