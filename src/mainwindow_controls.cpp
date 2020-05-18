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

#include "mainwindow.h"
#include "ui_mainwindow.h"

// --------------------------------------------- //
// Management of the Controls (A, MA, B)         //
// --------------------------------------------- //

void MainWindow::on_checkBox_Control_stateChanged(int status)
{
    poke(ET::ADCTRL, status ? 0xFF : 0);
}

void MainWindow::on_spinBox_Control_MAX_valueChanged(int value)
{
    if( value <= MaxLevel ) return;
    if( QMessageBox::question(this, "SECURITY !",
                              QString("Are you sure you want to set the maximum level over %1 ?").arg(MaxLevel)) == QMessageBox::Yes ) return;
    ui->spinBox_Control_MAX->setValue(MaxLevel);
}

void MainWindow::on_verticalSlider_Control_A_valueChanged(int value)
{
    const int max = ui->spinBox_Control_MAX->value();
    if( value > max){
        ui->verticalSlider_Control_A->setValue(max);
        return;
    }
    poke(KNOB::A, value);
    ui->value_Control_A ->setNum(value);
}

void MainWindow::on_verticalSlider_Control_MA_valueChanged(int value)
{
    poke(KNOB::MA, value);
    ui->value_Control_MA ->setNum(value);
}

void MainWindow::on_verticalSlider_Control_B_valueChanged(int value)
{
    const int max = ui->spinBox_Control_MAX->value();
    if( value > max){
        ui->verticalSlider_Control_B->setValue(max);
        return;
    }
    poke(KNOB::B, value);
    ui->value_Control_B ->setNum(value);
}

void MainWindow::on_pushButton_Control_Reset_clicked()
{
    ui->verticalSlider_Control_A->setValue(0);
    ui->verticalSlider_Control_B->setValue(0);
    ui->verticalSlider_Control_MA->setValue(0);
}
