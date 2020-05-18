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

#include "gui_info.h"

//-----------------------------------------------//
// link each slider to its label, its value and  //
// to the memory adress in the et232             //
//-----------------------------------------------//

AllGuiInfo::AllGuiInfo(std::initializer_list<GuiInfo> il) {
    for (auto item : il) list.push_back(item);
}

GuiInfo* AllGuiInfo::operator[] (int addr)
{
    std::vector<GuiInfo>::iterator iter = list.begin();
    while( iter != list.end() ){
        if( addr == iter->addr) return lastGuiInfo = &(*iter);
        iter++;
    }
    return lastGuiInfo = nullptr;
}
