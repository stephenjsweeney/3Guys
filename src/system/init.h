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
#include "../json/cJSON.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_opengl.h"

#include "locale.h"

extern void createSaveFolder(void);
extern void initAtlas(void);
extern void initEntityDefs(void);
extern void initFonts(void);
extern void initGLRectangleBatch(void);
extern void initGraphics(void);
extern void initLookups(void);
extern void initSounds(void);
extern void initSprites(void);
extern void initStrings(void);
extern void initWidgets(void);
extern void prepareScene(void);
extern void presentScene(void);
extern void setLanguage(char *applicationName, char *languageCode);

extern App app;
