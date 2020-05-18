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
// Management of the Graphic panel               //
// --------------------------------------------- //

static const QPen redPen(Qt::red);
static const QPen greenPen(Qt::green);
static const QPen bluePen(Qt::blue);
static const QPen blackPen(Qt::black);

static const int listAddrVal[] = {
    A::Width::Val, A::Freq::Val, A::Ampli::Val, A::Pol::Val,
    B::Width::Val, B::Freq::Val, B::Ampli::Val, B::Pol::Val,
};

static int requests;
static int xtime;

void MainWindow::processGrTimer()
{
    if( requests == 0 ) return;

    // There is at least one value to graph.
    const int addr = listAddrVal[xtime%8];
    if( (etMem[addr].status & VALUE_REQUESTED_n) == 0 ){
        xtime = (xtime+1) % 4096;
        return;
     }

    // request the value for this addr
    peek(addr,(3*addr + xtime)%256);

    // do we have a previous value for this addr?
    if( (etMem[addr].status & (VALUE_SET|VALUE_REQUESTED_n)) != (VALUE_SET|VALUE_REQUESTED_n) ){
        xtime = (xtime+1) % 4096;
        return;
    }

    etMem[addr].status &= ~VALUE_SET;

    const auto vp = view->size();
    if( xtime<8 ){
        scene->clear();
        view->viewport()->update();
    }
    int y = 256 - etMem[addr].value;
    const QPen *pen;
    switch(addr){
        case A::Width::Val:
        case B::Width::Val: pen = &bluePen; break;
        case A::Freq::Val:
        case B::Freq::Val:  pen = &greenPen; break;
        case A::Ampli::Val:
        case B::Ampli::Val: pen = &redPen; break;
        default:            pen = &blackPen;
                             y *= 16; // Polarity
    }
    scene->addRect(xtime, y, 1, 1, *pen);
    view->centerOn((xtime - (vp.width()/2) + 20), 128);

    xtime = (xtime+1) % 4096;
}


void MainWindow::pollMem(int addr, bool state)
{
    if( state ){
        if (! synchStatus) synchRequired();
        etMem[addr].status |=  VALUE_REQUESTED_n;
        requests ++;
    }else{
        etMem[addr].status &= ~VALUE_REQUESTED_n;
        requests --;
    }
}

void MainWindow::on_checkBox_Graph_A_Width_stateChanged(int state)
{
    pollMem(A::Width::Val, state!=0);
}

void MainWindow::on_checkBox_Graph_A_Freq_stateChanged(int state)
{
    pollMem(A::Freq::Val, state!=0);
}

void MainWindow::on_checkBox_Graph_A_Ampli_stateChanged(int state)
{
    pollMem(A::Ampli::Val, state!=0);
}

void MainWindow::on_checkBox_Graph_A_Pol_stateChanged(int state)
{
    pollMem(A::Pol::Val, state!=0);
}

void MainWindow::on_checkBox_Graph_B_Width_stateChanged(int state)
{
    pollMem(B::Width::Val, state!=0);
}

void MainWindow::on_checkBox_Graph_B_Freq_stateChanged(int state)
{
    pollMem(B::Freq::Val, state!=0);
}

void MainWindow::on_checkBox_Graph_B_Ampli_stateChanged(int state)
{
    pollMem(B::Ampli::Val, state!=0);
}

void MainWindow::on_checkBox_Graph_B_Pol_stateChanged(int state)
{
    pollMem(B::Pol::Val, state!=0);
}
