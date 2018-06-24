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

#include "wipe.h"

static int wipeType;
static float wipeValue;

int doWipe(void)
{
	switch (wipeType)
	{
		case WIPE_IN:
		case WIPE_OUT:
			if (wipeValue < SCREEN_WIDTH)
			{
				wipeValue += WIPE_SPEED;
			}
			return wipeValue < SCREEN_WIDTH;
			
		case WIPE_FADE:
			if (wipeValue > 0)
			{
				wipeValue -= 0.025;
			}
			break;
			
		default:
			break;
	}
	
	return 0;
}

void drawWipe(void)
{
	switch (wipeType)
	{
		case WIPE_IN:
			if (wipeValue < SCREEN_WIDTH)
			{
				drawFilledRect(wipeValue, 0, SCREEN_WIDTH - wipeValue, SCREEN_HEIGHT, 0, 0, 0, 1);
			}
			break;
			
		case WIPE_OUT:
			if (wipeValue < SCREEN_WIDTH)
			{
				drawFilledRect(0, 0, wipeValue, SCREEN_HEIGHT, 0, 0, 0, 1);
			}
			break;
			
		case WIPE_FADE:
			if (wipeValue > 0)
			{
				drawFilledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, wipeValue);
			}
			break;
			
		default:
			break;
	}
}

void initWipe(int type)
{
	wipeType = type;
	wipeValue = 0;
	
	if (type == WIPE_FADE)
	{
		wipeValue = 1;
	}
}
