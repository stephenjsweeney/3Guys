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

#include "credits.h"

static void logic(void);
static void draw(void);
static void back(void);

static Background background;
static Widget *backWidget;

void initCredits(void)
{
	background.texture = loadTexture("gfx/backgrounds/background.jpg")->texture;
	background.r = background.g = background.b = 255;
	
	showWidgetGroup("credits");
	
	initWipe(WIPE_FADE);
	
	backWidget = getWidget("back", "credits");
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
	drawBackground(&background);
	
	setTextColor(255, 255, 255, 255);
	
	drawShadowText(SCREEN_WIDTH / 2, 100, TEXT_ALIGN_CENTER, 60, app.strings[ST_CREDITS]);
	
	drawShadowText(SCREEN_WIDTH / 2, 250, TEXT_ALIGN_CENTER, 45, "Sound Effects");
	
	drawShadowText(SCREEN_WIDTH / 2, 350, TEXT_ALIGN_CENTER, 30, "Created using Bfxr");
	drawShadowText(SCREEN_WIDTH / 2, 400, TEXT_ALIGN_CENTER, 30, "www.bfxr.net");
	
	drawShadowText(SCREEN_WIDTH / 2, 550, TEXT_ALIGN_CENTER, 45, "Music");
	
	drawShadowText(SCREEN_WIDTH / 2, 650, TEXT_ALIGN_CENTER, 30, "Cheerful 1 - Choro Bavario");
	drawShadowText(SCREEN_WIDTH / 2, 700, TEXT_ALIGN_CENTER, 30, "Copyright 2009 MSTR \"Choro Bavario\"");
	drawShadowText(SCREEN_WIDTH / 2, 750, TEXT_ALIGN_CENTER, 30, "www.jamendo.com/en/artist/349242/mstr");
	
	drawShadowText(SCREEN_WIDTH / 2, 825, TEXT_ALIGN_CENTER, 30, "Happy-Go-Lucky Puzzle");
	drawShadowText(SCREEN_WIDTH / 2, 875, TEXT_ALIGN_CENTER, 30, "Copyright 2014 Axton Crolley");
	
	drawWidgets();
	
	drawWipe();
}

static void back(void)
{
	initTitle();
	
	playSound(SND_SELECT, 0);
}
