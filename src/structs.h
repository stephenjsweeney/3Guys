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
typedef struct AtlasImage AtlasImage;
typedef struct Bucket Bucket;
typedef struct Tuple Tuple;
typedef struct Effect Effect;
typedef struct Credit Credit;
typedef struct Font Font;
typedef struct Entity Entity;
typedef struct EntityDef EntityDef;
typedef struct RouteNode RouteNode;

typedef struct {
	int debug;
	int showFPS;
	int fps;
} Dev;

struct Texture {
	char name[MAX_DESCRIPTION_LENGTH];
	long hash;
	SDL_Texture *texture;
	Texture *next;
};

struct AtlasImage {
	char filename[MAX_FILENAME_LENGTH];
	SDL_Rect rect;
	SDL_Texture *texture;
	AtlasImage *next;
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

struct Font {
	char name[MAX_NAME_LENGTH];
	SDL_Texture *texture;
	AtlasImage glyphs[128];
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
	int sex;
	int speed;
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
	SDL_Renderer *renderer;
	SDL_Window *window;
	Delegate delegate;
	SDL_Texture *backBuffer;
	char *strings[ST_MAX];
	Config config;
} App;

struct Effect {
	float x;
	float y;
	int r;
	int g;
	int b;
	float dx;
	float dy;
	int life;
	AtlasImage *atlasImage;
	Effect *next;
};

struct RouteNode {
	int x;
	int y;
	RouteNode *next;
};

typedef struct {
	int id;
	int moves;
	int tnt;
	int tools;
	Entity *guy;
	int data[MAP_WIDTH][MAP_HEIGHT];
	RouteNode routeHead, *routeTail;
	Entity entityHead, *entityTail;
	Effect effectHead, *effectTail;
	int dx;
	int dy;
	int numTips;
	char **tips;
	char *message;
	int walkRoute;
	int finishTimer;
	int state;
} Level;

struct Sprite {
	char name[MAX_NAME_LENGTH];
	int *times;
	char **filenames;
	AtlasImage **frames;
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
	float value;
	float maxValue;
	int visible;
	int disabled;
	int centered;
	void (*action)(void);
	char **options;
	int numOptions;
	AtlasImage *atlasImage;
	Widget *parent;
	Widget *next;
};

struct Credit {
	char *text;
	float y;
	int size;
	int h;
	Credit *next;
};

typedef struct {
	float x;
	float y;
	int r;
	int g;
	int b;
	float dx;
	float dy;
	SDL_Texture *texture;
} Background;

typedef struct {
	int x;
	int y;
	int levelNum;
	int available;
	int hasStar;
	int hasFoundStar;
} LevelRect;

typedef struct {
	float x;
	float y;
	float dy;
	AtlasImage *atlasImage;
} Bouncer;

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
