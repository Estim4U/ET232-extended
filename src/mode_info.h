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

#ifndef MODE_INFO_H
#define MODE_INFO_H

#include <QLabel>
#include <QSlider>

#include <initializer_list>
#include <vector>

#include "macros.h"

class ParamInfo {
public:
  int      addr;
  QString  key;
  int      lastEtValue;
};

class ModeInfo {
public:
  ModeInfo(const char* name, int et_code, std::initializer_list<ParamInfo> il, const char* info = nullptr);
  ParamInfo* operator[] (int addr);
  ParamInfo* operator[] (QString key);
  std::vector<ParamInfo> list;
  int comboIndex, etCode, A, B, MA, MAX;
  enum src {none=0, et, file, user};
  src paramSrc;
  QString modeName;
  const char* text;
private:
  int lastAddr = -1;
  ParamInfo* lastParamInfo = nullptr;
};

#endif // MODE_INFO_H
