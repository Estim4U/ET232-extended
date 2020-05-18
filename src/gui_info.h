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

#ifndef GUI_INFO_H
#define GUI_INFO_H

#include <QLabel>
#include <QSlider>

#include <initializer_list>
#include <vector>

class GuiInfo {
public:
  int      addr;
  QSlider* slider;
  QLabel*  labelName;
  QLabel*  labelValue;
};

class AllGuiInfo {
public:
  AllGuiInfo(std::initializer_list<GuiInfo> il);
  GuiInfo* operator[] (int addr);
  std::vector<GuiInfo> list;
private:
  GuiInfo* lastGuiInfo = nullptr;
};

#endif // GUI_INFO_H
