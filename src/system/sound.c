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

#include "sound.h"

static void loadSounds(void);

static Mix_Chunk *sounds[SND_MAX];
static Mix_Music *music;

void initSounds(void)
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);

	music = NULL;

	loadSounds();
}

void loadMusic(char *filename)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}

	music = Mix_LoadMUS(getFileLocation(filename));
}

void playMusic(int loop)
{
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}

void stopMusic(void)
{
	Mix_HaltMusic();
}

void fadeMusic(int ms)
{
	Mix_FadeOutMusic(ms);
}

void playSound(int snd, int ch)
{
	Mix_PlayChannel(ch, sounds[snd], 0);
}

void pauseSound(int pause)
{
	if (pause)
	{
		Mix_Pause(-1);
	}
	else
	{
		Mix_Resume(-1);
	}
}

int isPlayingMusic(void)
{
	return Mix_PlayingMusic();
}

static Mix_Chunk *loadSound(char *filename)
{
	return Mix_LoadWAV(getFileLocation(filename));
}

static void loadSounds(void)
{
	sounds[SND_BOMB] = loadSound("sound/bomb.ogg");
	sounds[SND_BRIDGE] = loadSound("sound/bridge.ogg");
	sounds[SND_BUTTON] = loadSound("sound/button.ogg");
	sounds[SND_CRUMBLE] = loadSound("sound/crumble.ogg");
	sounds[SND_DENIED] = loadSound("sound/denied.ogg");
	sounds[SND_DIAMOND] = loadSound("sound/diamond.ogg");
	sounds[SND_DIE] = loadSound("sound/die.ogg");
	sounds[SND_FADE] = loadSound("sound/fade.ogg");
	sounds[SND_FAIL] = loadSound("sound/fail.ogg");
	sounds[SND_HINT] = loadSound("sound/hint.ogg");
	sounds[SND_KEY] = loadSound("sound/key.ogg");
	sounds[SND_OPEN] = loadSound("sound/open.ogg");
	sounds[SND_OPTION] = loadSound("sound/option.ogg");
	sounds[SND_PICKUP] = loadSound("sound/pickup.ogg");
	sounds[SND_PRESSUREPLATE] = loadSound("sound/pressurePlate.ogg");
	sounds[SND_SELECT] = loadSound("sound/select.ogg");
	sounds[SND_STAR] = loadSound("sound/star.ogg");
	sounds[SND_TELEPORT] = loadSound("sound/teleport.ogg");
	sounds[SND_WALK] = loadSound("sound/walk.ogg");
	sounds[SND_WIPE] = loadSound("sound/wipe.ogg");
}

void destroySounds(void)
{
	int i;

	for (i = 0 ; i < SND_MAX ; i++)
	{
		if (sounds[i])
		{
			Mix_FreeChunk(sounds[i]);
		}
	}

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}
}
