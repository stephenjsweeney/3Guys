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
#include "item.h"
#include "../util/maths.h"

extern Entity *self;

static void tick(void);
static void resetSpinTimer(void);

void initItem(Entity *e)
{
	self = e;
	
	resetSpinTimer();
	
	e->tick = tick;
}

static void tick(void)
{
	if (--self->spinTimer < 0)
	{
		self->spin += 0.1f;

		self->angle = (float) sin(self->spin) * 25;

		if (self->spinTimer < -(FPS * 3) && (int) self->angle == 0)
		{
			resetSpinTimer();
		}
	}
}

static void resetSpinTimer(void)
{
	self->angle = 0;

	self->spinTimer = randF() * FPS * 25;
}
