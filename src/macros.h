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

#ifndef MACROS_H
#define MACROS_H

#define _str(a) #a              // quote a
#define STRINGIFY(a) _str(a)    // expand a and the quote

// Macros used to initialize the list of all GuiInfo
// Generates 1 or 2 lines like that:
//   {A::Freq::Val,  ui->verticalSlider_A_Freq,  ui->label_A_Freq, ui->value_A_Freq }
//   {B::Freq::Val,  ui->verticalSlider_B_Freq,  ui->label_B_Freq, ui->value_B_Freq }
#define GUI_INFO1(uiname, addr) { addr , ui->verticalSlider_ ## uiname, ui->label_ ## uiname, ui->value_ ## uiname }
#define GUI_INFO2(uiname, addr) GUI_INFO1(A_ ## uiname, A::addr), GUI_INFO1(B_ ## uiname, B::addr)
#define GUI_INFO(Value, Param)  GUI_INFO2(Value ## _ ## Param, Value::Param)

// Macros used to initialize a list of ParamInfo
// Generates 1 or 2 lines like that:
//   {A::Freq::Val,  "A::Freq::Val", 0 }
//   {B::Freq::Val,  "B::Freq::Val", 0 }
#define PARAM_INFO1(uiname, addr) { addr , _str(addr), 0 }
#define PARAM_INFO2(uiname, addr) PARAM_INFO1(A_ ## uiname, A::addr), PARAM_INFO1(B_ ## uiname, B::addr)
#define PARAM_INFO(Value, Param)  PARAM_INFO2(Value ## _ ## Param, Value::Param)

// Macros used to define slot function managing the vertical slider event 'on valueChanged'
// Generates 1 or 2 lines like that:
//   void MainWindow::on_verticalSlider_A_Width_Min_valueChanged(int value){ updateCurrentValue(A::Width::Min, value); }
//   void MainWindow::on_verticalSlider_B_Width_Min_valueChanged(int value){ updateCurrentValue(B::Width::Min, value); }
#define CODE_ON_VS1(uiname, addr) void MainWindow::on_verticalSlider_ ## uiname ## _valueChanged(int value){ updateCurrentValue(addr , value); }
#define CODE_ON_VS2(uiname, addr) CODE_ON_VS1(A_ ## uiname, A::addr) CODE_ON_VS1(B_ ## uiname, B::addr)
#define CODE_ON_VS(Value, Param)  CODE_ON_VS2(Value ## _ ## Param, Value::Param)

// Macros used to declare a slot to manage vertical slider for A and B channels
// Generate 1 or 2 lines like that:
//   void on_verticalSlider_Random_A_Ampli_Max_valueChanged(int value);
//   void on_verticalSlider_Random_B_Ampli_Max_valueChanged(int value);
#define DCL_ON_VS1(uiname) void on_verticalSlider_ ## uiname ## _valueChanged(int value);
#define DCL_ON_VS2(uiname) DCL_ON_VS1(A_ ## uiname) DCL_ON_VS1(B_ ## uiname)

#endif // MACROS_H
