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

#define MAX_LEVEL_PER_PAGE		12

extern int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern void doWidgets(void);
extern void drawBackground(Background *background);
extern void drawGLRectangleBatch(GLRectangle *rect, int x, int y, int center);
extern void drawShadowText(int x, int y, int align, const char *format, ...);
extern void drawWidgets(void);
extern Atlas *getImageFromAtlas(char *filename, int required);
extern Widget *getWidget(const char *name, const char *group);
extern void initGLRectangle(GLRectangle *rect, int width, int height);
extern void initLevel(int id);
extern Texture *loadTexture(const char *filename);
extern void playSound(int snd, int ch);
extern void setGLRectangleBatchColor(float r, float g, float b, float a);
extern void showWidgetGroup(const char *name);
extern void useFont(char *name);

extern App app;
extern Game game;
