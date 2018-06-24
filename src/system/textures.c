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

#include "textures.h"

static Texture *addTextureToCache(const char *name, GLint texture);
GLint toTexture(SDL_Surface *surface, int destroySurface);
Texture *getTexture(const char *name);

static Texture textures[NUM_TEXTURE_BUCKETS];

void initTextures(void)
{
	memset(&textures, 0, sizeof(Texture) * NUM_TEXTURE_BUCKETS);
}

static Texture *addTextureToCache(const char *name, GLint texture)
{
	Texture *t, *new;
	int i;

	i = hashcode(name) % NUM_TEXTURE_BUCKETS;

	t = &textures[i];

	/* horrible bit to look for the tail */
	while (t->next)
	{
		t = t->next;
	}
	
	new = malloc(sizeof(Texture));
	memset(new, 0, sizeof(Texture));

	STRNCPY(new->name, name, MAX_DESCRIPTION_LENGTH);
	new->texture = texture;

	t->next = new;
	
	return new;
}

Texture *loadTexture(const char *filename)
{
	SDL_Surface *surface;
	GLint texture;
	
	surface = IMG_Load(getFileLocation(filename));
	
	texture = toTexture(surface, 1);
	
	return addTextureToCache(filename, texture);
}

Texture *getTexture(const char *filename)
{
	Texture *t;
	int i;

	i = hashcode(filename) % NUM_TEXTURE_BUCKETS;

	/* check if the texture is already loaded */
	for (t = textures[i].next ; t != NULL ; t = t->next)
	{
		if (strcmp(t->name, filename) == 0)
		{
			return t;
		}
	}
	
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, "%s not in texture cache", filename);
	
	return loadTexture(filename);
}

GLint toTexture(SDL_Surface *surface, int destroySurface)
{
	GLint numCols;
	GLenum textureFormat;
	GLuint texture;
	
	numCols = surface->format->BytesPerPixel;
	
	if (numCols == 4)
	{
		if (surface->format->Rmask == 0x000000ff)
		{
			textureFormat = GL_RGBA;
		}
		else
		{
			textureFormat = GL_BGRA;
		}
	}
	else if (numCols == 3)
	{
		if (surface->format->Rmask == 0x000000ff)
		{
			textureFormat = GL_RGB;
		}
		else
		{
			textureFormat = GL_BGR;
		}
	}
	else
	{
		exit(1);
	}
	
	glGenTextures(1, &texture);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, numCols, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	
	if (destroySurface)
	{
		SDL_FreeSurface(surface);
	}
	
	return texture;
}


void destroyTextures(void)
{
	Texture *t, *next;
	int i;

	for (i = 0 ; i < NUM_TEXTURE_BUCKETS ; i++)
	{
		t = textures[i].next;

		while (t)
		{
			next = t->next;
			glDeleteTextures(1, &t->texture);
			free(t);
			t = next;
		}

		textures[i].next = NULL;
	}
}
