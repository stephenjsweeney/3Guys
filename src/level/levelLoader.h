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

extern char *buildFormattedString(const char *format, ...);
extern void destroyLevel(void);
extern int getRandomFloorTile(void);
extern void initDiamond(Entity *e);
extern void initGreenDoor(Entity *e);
extern void initGreenGuy(Entity *e);
extern void initGreenKey(Entity *e);
extern void initNormalDoor(Entity *e);
extern void initNormalKey(Entity *e);
extern void initPushBlock(Entity *e);
extern void initRedDoor(Entity *e);
extern void initRedGuy(Entity *e);
extern void initRedKey(Entity *e);
extern void initStar(Entity *e);
extern void initYellowDoor(Entity *e);
extern void initYellowKey(Entity *e);
extern char *readFile(const char *filename);

extern Level level;
