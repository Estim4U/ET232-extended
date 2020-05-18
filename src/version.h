// SPDX-License-Identifier: GPL-3.0-only

/*\
This file is part of the ET232-extended application.

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License version 3 as published
by the Free Software Foundation https://www.gnu.org/licenses/gpl-3.0.html

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
\*/

#ifndef VERSION_H
#define VERSION_H

#define ORG_NAME             "EStim4U"

#define APP_NAME             "ET232 Extended"

#define APP_VERSION_MAJOR    2
#define APP_VERSION_MINOR    6

#define APP_REVISION_MAJOR  20  //  YY [17,18,19,20 ...]
#define APP_REVISION_MINOR 140  // DDD [1,2 ... 365,366]

#define _str(a) #a
#define STRINGIFY(a) _str(a)

#define APP_VERSION STRINGIFY(APP_VERSION_MAJOR) "." STRINGIFY(APP_VERSION_MINOR)

#endif // VERSION_H
