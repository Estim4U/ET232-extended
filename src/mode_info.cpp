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

#include "mode_info.h"

#include <QDebug>

ModeInfo::ModeInfo(const char* name, int et_code, std::initializer_list<ParamInfo> il, const char* info) {
    modeName = QString(name);
    etCode = et_code;
    for (auto item : il) list.push_back(item);
    text = info;
    comboIndex = A = B = MA = 0;
    MAX = 135;
    paramSrc = none;
}

ParamInfo* ModeInfo::operator[] (int addr)
{
    if( addr == lastAddr ) return lastParamInfo;
    std::vector<ParamInfo>::iterator iter = list.begin();
    while( iter != list.end() ){
        if( addr == iter->addr) {
            lastAddr = addr;
            return lastParamInfo = &(*iter);
        }
        iter++;
    }
    return lastParamInfo = nullptr;
}

ParamInfo* ModeInfo::operator[] (QString key)
{
    std::vector<ParamInfo>::iterator iter = list.begin();
    while( iter != list.end() ){
        if( key == iter->key) return &(*iter);
        iter++;
    }
    return nullptr;
}
