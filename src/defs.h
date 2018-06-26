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

#ifndef REVISION
#define REVISION 0
#endif

#ifndef DATA_DIR
#define DATA_DIR ""
#endif

#define _(string) getTranslatedString(string)

#define PI 3.14159265358979323846
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CAROLINE(a,b) MIN(a,b)
#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'
#define TO_RAIDANS(angleDegrees) (angleDegrees * PI / 180.0)
#define TO_DEGREES(angleRadians) (angleRadians * 180.0 / PI)

#define MAX_VERTS 		12
#define MAX_TEX_CORDS 	8
#define MAX_RECTS 		1024

#define SAVE_FILENAME		"game.save"
#define CONFIG_FILENAME		"config.json"

#define SCREEN_WIDTH	720
#define SCREEN_HEIGHT	1280

#define MAX_KEYBOARD_KEYS	350
#define MAX_MOUSE_BUTTONS	6

#define MAP_WIDTH			7
#define MAP_HEIGHT			11

#define LEVEL_RENDER_X		24
#define LEVEL_RENDER_Y		85

#define TILE_SIZE 		96
#define MAX_TILES		101

#define FPS				60
#define LOGIC_RATE		(1000.0 / FPS)

#define MAX_NAME_LENGTH				32
#define MAX_DESCRIPTION_LENGTH		512
#define MAX_LINE_LENGTH				1024
#define MAX_FILENAME_LENGTH			1024

#define NUM_TEXTURE_BUCKETS			32
#define NUM_ATLAS_BUCKETS			64

#define MAX_SND_CHANNELS			64

#define MAX_CANDIDATES				64

#define TILE_HOLE					0
#define TILE_RED					1
#define TILE_YELLOW					2
#define TILE_GREEN					3
#define TILE_FLOOR					10
#define TILE_WALL					100

enum
{
	JOYPAD_AXIS_X,
	JOYPAD_AXIS_Y,
	JOYPAD_AXIS_MAX
};

enum
{
	TA_LEFT,
	TA_RIGHT,
	TA_CENTER
};

enum
{
	WIPE_IN,
	WIPE_OUT,
	WIPE_FADE
};

enum
{
	WT_BUTTON,
	WT_SLIDER,
	WT_SPINNER,
	WT_INPUT
};

enum
{
	ET_MOVING_PLATFORM,
	ET_PRESSURE_PLATE,
	ET_CRUMBLING_FLOOR,
	ET_TELEPORTER,
	ET_BRIDGE,
	ET_NORMAL_DOOR,
	ET_RED_DOOR,
	ET_GREEN_DOOR,
	ET_YELLOW_DOOR,
	ET_FIRE_PIT,
	ET_SPIKE_TRAP,
	ET_DIAMOND,
	ET_NORMAL_KEY,
	ET_RED_KEY,
	ET_GREEN_KEY,
	ET_YELLOW_KEY,
	ET_STAR,
	ET_TNT,
	ET_TOOLS,
	ET_RED_GUY,
	ET_GREEN_GUY,
	ET_YELLOW_GUY,
	ET_WALL,
	ET_LASER_TRAP,
	ET_PUSH_BLOCK,
	ET_LASER
};

enum
{
	LS_INCOMPLETE,
	LS_COMPLETE,
	LS_FAILED
};

enum
{
	SND_BOMB,
	SND_BRIDGE,
	SND_BUTTON,
	SND_CRUMBLE,
	SND_DENIED,
	SND_DIAMOND,
	SND_DIE,
	SND_FADE,
	SND_FAIL,
	SND_HINT,
	SND_KEY,
	SND_OPEN,
	SND_OPTION,
	SND_PICKUP,
	SND_PRESSUREPLATE,
	SND_SELECT,
	SND_STAR,
	SND_TELEPORT,
	SND_WALK,
	SND_WIPE,
	SND_MAX
};

enum
{
	ST_CLICK_TO_CONTINUE,
	ST_OUT_OF_MOVES,
	ST_DIAMOND_DESC,
	ST_STAR_DESC,
	ST_LEVEL_NUM,
	ST_NUM_MOVES,
	ST_KEY_DESC,
	ST_ALREADY_HAVE_KEY,
	ST_DOOR_DESC,
	ST_PUSH_BLOCK_DESC,
	ST_TOOLS_DESC,
	ST_PRESSURE_PLATE_DESC,
	ST_PLATFORM_DESC,
	ST_CRUMBLING_FLOOR_DESC,
	ST_MAX
};
