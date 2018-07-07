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

#include "strings.h"

void initStrings(void)
{
	app.strings[ST_CLICK_TO_CONTINUE] = _("Click to continue");
	app.strings[ST_OUT_OF_MOVES] = _("Out of moves!");
	app.strings[ST_DIAMOND_DESC] = _("A diamond. It's very shiny. I'll have it for lunch.");
	app.strings[ST_STAR_DESC] = _("A star. It looks tasty. Will make a nice side for the diamond.");
	app.strings[ST_LEVEL_NUM] = _("Level %03d / 100");
	app.strings[ST_NUM_MOVES] = _("Moves: %d");
	app.strings[ST_KEY_DESC] = _("A key. I can use it to open doors.");
	app.strings[ST_ALREADY_HAVE_KEY] = _("I've already got a key.");
	app.strings[ST_DOOR_DESC] = _("It's a door. It can be opened with a key.");
	app.strings[ST_PUSH_BLOCK_DESC] = _("A heavy object. Green Guy can push it.");
	app.strings[ST_TOOLS_DESC] = _("Tools. They can be used to build a bridge.");
	app.strings[ST_PRESSURE_PLATE_DESC] = _("A pressure plate. I can weigh it down to activate it.");
	app.strings[ST_PLATFORM_DESC] = _("A moving platform. I can walk on it to move it around.");
	app.strings[ST_CRUMBLING_FLOOR_DESC] = _("A crumbling floor. It will break after I walk on it.");
	app.strings[ST_TNT_DESC] = _("TNT. It can be used to destroy walls.");
	app.strings[ST_TELEPORTER_DESC] = _("A teleporter. I can use it to traverse the Nth dimension.");
	app.strings[ST_TELEPORTER_OFF_DESC] = _("A teleporter. It's not working right now.");
	app.strings[ST_LASER_TRAP_DESC] = _("A laser trap. The beams look like they'd hurt.");
	app.strings[ST_LASER_TRAP_OFF_DESC] = _("A laser trap. Luckily, it's switched off.");
	app.strings[ST_FIRE_PIT_DESC] = _("A fire pit trap. Falling in would hurt quite a lot.");
	app.strings[ST_LEVEL_SELECT] = _("Level Select");
	app.strings[ST_LEVELS] = _("Levels: %d / %d");
	app.strings[ST_STARS] = _("Stars: %d / %d");
	app.strings[ST_PAUSE] = _("Pause");
	app.strings[ST_OPTIONS] = _("Options");
	app.strings[ST_STATS] = _("Stats");
	app.strings[ST_CREDITS] = _("Credits");
	app.strings[ST_CONGRATULATIONS] = _("Congratulations");
	app.strings[ST_ENDING] = _("Thanks to his pals, Red's hunger is finally sated! Unfortunately, he's been away from the office for quite a few hours now, so should probably should get back if he wants to avoid having to work late. Again.");
}
