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

enum
{
	SHOW_LEVEL,
	SHOW_TIPS,
	SHOW_PAUSE
};

extern void animateSprites(void);
extern void clearRoute(void);
extern void destroyEntities(void);
extern void doEffects(void);
extern void doEntities(void);
extern void doPlayer(void);
extern void doWidgets(void);
extern int doWipe(void);
extern void drawBackground(GLRectangle *rect, float x, float y, float r, float g, float b);
extern void drawEffects(void);
extern void drawEntities(int backgroundPlane);
extern void drawFilledRect(int x, int y, int w, int h, float r, float g, float b, float a);
extern void drawGLRectangleBatch(GLRectangle *rect, int x, int y, int center);
extern void drawRect(int x, int y, int w, int h, float r, float g, float b, float a);
extern void drawText(int x, int y, int align, const char *format, ...);
extern void drawWidgets(void);
extern void drawWipe(void);
extern float getAngle(int x1, int y1, int x2, int y2);
extern Atlas *getImageFromAtlas(char *filename, int required);
extern Widget *getWidget(const char *name, const char *group);
extern void guyFallDownHoles(void);
extern void guyTouchOthers(void);
extern void initEffects(void);
extern void initEntities(void);
extern void initGLRectangle(GLRectangle *rect, int width, int height);
extern void initPlayer(void);
extern void initWipe(int type);
extern void loadLevel(int n);
extern Texture *loadTexture(const char *filename);
extern void moveEntities(void);
extern void playSound(int snd, int ch);
extern double randF(void);
extern void saveGame(void);
extern void setGLRectangleBatchColor(float r, float g, float b, float a);
extern void setTextWidth(int width);
extern void showWidgetGroup(const char *name);
extern void useFont(char *name);

extern App app;
extern GLRectangleBatch glRectangleBatch;
extern Game game;
extern Level level;
