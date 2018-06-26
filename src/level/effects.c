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

#include "effects.h"

static Atlas *explosionSphere;

void initEffects(void)
{
	level.effectTail = &level.effectHead;
	
	explosionSphere = getImageFromAtlas("gfx/sprites/explosionSphere.png", 1);
}

void doEffects(void)
{
	Effect *e, *prev;
	
	prev = &level.effectHead;
	
	for (e = level.effectHead.next ; e != NULL ; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;
		
		if (--e->life <= 0)
		{
			if (e == level.effectTail)
			{
				level.effectTail = prev;
			}
			
			prev->next = e->next;
			free(e);
			e = prev;
		}
		
		prev = e;
	}
}

void drawEffects(void)
{
	Effect *e;
	
	for (e = level.effectHead.next ; e != NULL ; e = e->next)
	{
		setGLRectangleBatchColor(e->r, e->g, e->b, 1);
		
		drawGLRectangleBatch(&explosionSphere->rect, e->x, e->y, 1);
	}
}

void addExplosionEffect(int mx, int my, float r, float g, float b)
{
	Effect *e;
	int i, x, y;
	
	x = LEVEL_RENDER_X;
	x += mx * TILE_SIZE;
	x += TILE_SIZE / 2;
	
	y = LEVEL_RENDER_Y;
	y += my * TILE_SIZE;
	y += TILE_SIZE / 2;
	
	for (i = 0 ; i < 4 ; i++)
	{
		e = malloc(sizeof(Effect));
		memset(e, 0, sizeof(Effect));
		level.effectTail->next = e;
		level.effectTail = e;
		
		e->x = x;
		e->y = y;
		e->r = r;
		e->g = g;
		e->b = b;
		e->life = FPS / 3;
		
		switch (i)
		{
			case 0:
				e->dx = -EFFECT_SPEED;
				e->dy = -EFFECT_SPEED;
				break;
				
			case 1:
				e->dx = EFFECT_SPEED;
				e->dy = -EFFECT_SPEED;
				break;
				
			case 2:
				e->dx = EFFECT_SPEED;
				e->dy = EFFECT_SPEED;
				break;
				
			case 3:
				e->dx = -EFFECT_SPEED;
				e->dy = EFFECT_SPEED;
				break;
				
			default:
				break;
		}
	}
}
