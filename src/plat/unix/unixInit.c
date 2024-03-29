/*
Copyright (C) 2018,2022 Parallel Realities

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

#include <errno.h>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../common.h"

#include "../../util/util.h"
#include "unixInit.h"

extern App app;

static void mkpath(const char *path);

void createSaveFolder(void)
{
	char *userHome, *dir;

	userHome = getenv("HOME");

	if (!userHome)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Unable to determine user save folder.");
		exit(1);
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "User home = %s", userHome);

	dir = buildFormattedString("%s/.local/share/3Guys", userHome);

	mkpath(dir);

	free(dir);

	app.saveDir = buildFormattedString("%s/.local/share/3Guys", userHome);
}

static void mkpath(const char *path)
{
	char dir[MAX_FILENAME_LENGTH];
	int	 i, rootPath;

	strcpy(dir, "");

	rootPath = 1;

	for (i = 0; i < strlen(path); i++)
	{
		if (path[i] == '/')
		{
			if (!rootPath)
			{
				if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0 && errno != EEXIST)
				{
					SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Failed to create save dir '%s'.", dir);
					exit(1);
				}
			}

			rootPath = 0;
		}

		dir[i] = path[i];
		dir[i + 1] = '\0';
	}

	if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0 && errno != EEXIST)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Failed to create save dir '%s'.", dir);
		exit(1);
	}
}
