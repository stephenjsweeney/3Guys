/*
Copyright (C) 2018 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General License for more details.

You should have received a copy of the GNU General License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

typedef struct Texture Texture;
typedef struct Lookup Lookup;
typedef struct Sprite Sprite;
typedef struct Widget Widget;
typedef struct Atlas Atlas;
typedef struct Bucket Bucket;
typedef struct Tuple Tuple;
typedef struct Effect Effect;
typedef struct Credit Credit;
typedef struct Font Font;
typedef struct Entity Entity;
typedef struct EntityDef EntityDef;

typedef struct {
	int debug;
	int showFPS;
	int fps;
} Dev;

struct Texture {
	char name[MAX_DESCRIPTION_LENGTH];
	long hash;
	uint32_t texture;
	Texture *next;
};

typedef struct {
	float x;
	float y;
} PointF;

typedef struct {
	SDL_Color red;
	SDL_Color orange;
	SDL_Color yellow;
	SDL_Color green;
	SDL_Color blue;
	SDL_Color cyan;
	SDL_Color purple;
	SDL_Color white;
	SDL_Color black;
	SDL_Color lightGrey;
	SDL_Color darkGrey;
} Colors;

struct Lookup {
	char name[MAX_NAME_LENGTH];
	long value;
	Lookup *next;
};

typedef struct {
	uint32_t texture;
	int w;
	int h;
	float vertices[MAX_VERTS];
	float textureCords[MAX_TEX_CORDS];
} GLRectangle;

typedef struct {
	float vertices[MAX_RECTS * MAX_VERTS];
	int indices[MAX_RECTS * 6];
	float colors[MAX_RECTS * 16];
	float textureCords[MAX_TEX_CORDS * MAX_RECTS];
	int numAddedRects;
	uint32_t currentTexture;
	int vIndex;
	int cIndex;
	int tIndex;
	int flipHorizontal;
	int flipVertical;
	int rotate;
	float scale;
	float angle;
	float color[4];
	int flushes;
} GLRectangleBatch;

struct Font {
	char name[32];
	GLRectangle glyphs[128];
	Font *next;
};

typedef struct {
	void (*logic)(void);
	void (*draw)(void);
	void (*postOptions)(void);
	void (*handleClick)(int x, int y, int btn);
	void (*handleDrag)(int x, int y, int dx, int dy, int cx, int cy);
	void (*handleMouseUp)(int x, int y, int btn);
} Delegate;

typedef struct {
	int levelsCompleted;
	int starsAvailable[MAX_LEVELS];
	int starsFound[MAX_LEVELS];
	int stats[STAT_MAX];
} Game;

struct EntityDef {
	char type[MAX_NAME_LENGTH];
	void (*initFunc)(Entity *e);
	EntityDef *next;
};

struct Entity {
	char name[MAX_NAME_LENGTH];
	char target[MAX_NAME_LENGTH];
	int type;
	int x, y;
	int dx, dy;
	int tx, ty;
	int visible;
	int solid;
	int alive;
	int spinTimer;
	float spin;
	float angle;
	int weight;
	int active;
	int backgroundPlane;
	int female;
	Sprite *sprite;
	Entity *carrying;
	Entity *owner;
	void (*move)(void);
	void (*touch)(Entity *other);
	void (*activate)(void);
	void (*tick)(void);
	void (*draw)(void);
	void (*describe)(void);
	int (*isBlocking)(void);
	void (*die)(void);
	Entity *next;
};

struct Tuple {
	char key[MAX_NAME_LENGTH];
	union {
		char s[MAX_NAME_LENGTH];
		int i;
		float f;
	} value;
	Tuple *next;
};

typedef struct {
	int x;
	int y;
	int w;
	int h;
	int dx;
	int dy;
	int button[MAX_MOUSE_BUTTONS];
	int dragging;
} Mouse;

typedef struct {
	int winWidth;
	int winHeight;
	int fullscreen;
	int soundVolume;
	int musicVolume;
} Config;

typedef struct {
	char *saveDir;
	float scaleX;
	float scaleY;
	Mouse mouse;
	SDL_Joystick *joypad;
	int keyboard[MAX_KEYBOARD_KEYS];
	int joypadButton[SDL_CONTROLLER_BUTTON_MAX];
	int joypadAxis[JOYPAD_AXIS_MAX];
	SDL_GLContext *glContext;
	SDL_Window *window;
	Delegate delegate;
	char *strings[ST_MAX];
	Config config;
} App;

struct Effect {
	float x;
	float y;
	float r;
	float g;
	float b;
	float dx;
	float dy;
	int life;
	Atlas *atlas;
	Effect *next;
};

typedef struct {
	int id;
	int moves;
	int tnt;
	int tools;
	Entity *guy;
	int data[MAP_WIDTH][MAP_HEIGHT];
	SDL_Point route[MAP_WIDTH * MAP_HEIGHT];
	Entity entityHead, *entityTail;
	Effect effectHead, *effectTail;
	int dx;
	int dy;
	int numTips;
	char **tips;
	char *message;
	int walkRoute;
	int routeIndex;
	int finishTimer;
	int state;
} Level;

struct Sprite {
	char name[MAX_NAME_LENGTH];
	int *times;
	char **filenames;
	Atlas **frames;
	int currentFrame;
	float currentTime;
	int w;
	int h;
	int numFrames;
	Sprite *next;
};

struct Widget {
	char name[MAX_NAME_LENGTH];
	char group[MAX_NAME_LENGTH];
	char label[MAX_NAME_LENGTH];
	int type;
	int x;
	int y;
	int w;
	int h;
	int value;
	int minValue;
	int maxValue;
	int visible;
	int disabled;
	int centered;
	void (*action)(void);
	Widget *next;
};

struct Atlas {
	char filename[MAX_FILENAME_LENGTH];
	GLRectangle rect;
	Atlas *next;
};

struct Credit {
	char *text;
	float y;
	int size;
	int h;
	Credit *next;
};

typedef struct {
	int x;
	int y;
	float r;
	float g;
	float b;
	float dx;
	float dy;
	float tx;
	float ty;
	GLRectangle rect;
} Background;

typedef struct
{
	int x;
	int y;
	int levelNum;
	int available;
	int hasStar;
	int hasFoundStar;
} LevelRect;

/* ===== i18n stuff ==== */

struct Bucket {
	char *key, *value;
	Bucket *next;
};

typedef struct {
	Bucket **bucket;
	int *bucketCount;
} HashTable;

typedef struct {
	int32_t magicNumber, version, stringCount;
	int32_t originalOffset, translationOffset;
} MOHeader;

typedef struct {
	int32_t length, offset;
} MOEntry;
