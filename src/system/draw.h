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

void drawBackground(Background *background);
void drawRect(int x, int y, int w, int h, int r, int g, int b, int a);
void drawFilledRect(int x, int y, int w, int h, int r, int g, int b, int a);
void blitAtlasImageRotated(AtlasImage *atlasImage, int x, int y, int center, float angle);
void blitAtlasImage(AtlasImage *atlasImage, int x, int y, int center);
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y, int center);
void blit(SDL_Texture *texture, int x, int y, int center);
void presentScene(void);
void prepareScene(void);
void initGraphics(void);
