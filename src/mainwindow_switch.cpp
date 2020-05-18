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

#include "options.h"

#include <QFileInfo>
#include <QTimer>

AllGuiInfo* MainWindow::getAllGuiInfo(){
    static AllGuiInfo* allInfo;
    if( allInfo ) return allInfo;
    return allInfo = new AllGuiInfo({
        GUI_INFO(Ampli, Min),
        GUI_INFO(Ampli, Max),
        GUI_INFO(Ampli, Tmin),
        GUI_INFO(Ampli, Tmax),
        GUI_INFO(Ampli, Tinc),
        GUI_INFO(Ampli, Tdec),
        GUI_INFO(Ampli, Val),

        GUI_INFO(Freq,  Min),
        GUI_INFO(Freq,  Max),
        GUI_INFO(Freq,  Tmin),
        GUI_INFO(Freq,  Tmax),
        GUI_INFO(Freq,  Tinc),
        GUI_INFO(Freq,  Tdec),
        GUI_INFO(Freq,  Val),

        GUI_INFO(Width, Min),
        GUI_INFO(Width, Max),
        GUI_INFO(Width, Tmin),
        GUI_INFO(Width, Tmax),
        GUI_INFO(Width, Tinc),
        GUI_INFO(Width, Tdec),
        GUI_INFO(Width, Val),

        GUI_INFO(Pol, Val)
    });
}

// --------------------------------------------- //
// Manage the on_verticalSlider_xxxx events      //
// --------------------------------------------- //

// call by each on_verticalSlider_xxxx()
void MainWindow::updateCurrentValue(int addr, int value){

    // update the value in the ET232
    const int etValue = (addr==A::Pol::Val || addr==B::Pol::Val) ? value*2 + 1 : value;
    poke(addr, etValue);

    if(!currentModeInfo){
        qDebug() << "No mode selected";
        return;
    }

    // update the value on screen
    ParamInfo* item = (*currentModeInfo)[addr];
    if( item ){
        item->lastEtValue = etValue;
        const auto allGuiInfo = getAllGuiInfo();
        const auto uiInfo = (*allGuiInfo)[addr];
        uiInfo->labelValue->setNum(value);
        if( currentModeInfo->paramSrc == ModeInfo::src::et )
            ui->comboBox_Mode->setItemIcon(currentModeInfo->comboIndex, QIcon(":/resources/red-dot.png"));
        currentModeInfo->paramSrc = ModeInfo::src::user;
        qDebug() << addr << "set to" << etValue;
    }else{
        qDebug() << "ERROR: Cannot find ui-info for addr=" << addr2text(addr);
    }
}

CODE_ON_VS(Ampli, Min)
CODE_ON_VS(Ampli, Max)
CODE_ON_VS(Ampli, Tmin)
CODE_ON_VS(Ampli, Tmax)
CODE_ON_VS(Ampli, Tinc)
CODE_ON_VS(Ampli, Tdec)
CODE_ON_VS(Ampli, Val)

CODE_ON_VS(Freq,  Min)
CODE_ON_VS(Freq,  Max)
CODE_ON_VS(Freq,  Tmin)
CODE_ON_VS(Freq,  Tmax)
CODE_ON_VS(Freq,  Tinc)
CODE_ON_VS(Freq,  Tdec)
CODE_ON_VS(Freq,  Val)

CODE_ON_VS(Width, Min)
CODE_ON_VS(Width, Max)
CODE_ON_VS(Width, Tmin)
CODE_ON_VS(Width, Tmax)
CODE_ON_VS(Width, Tinc)
CODE_ON_VS(Width, Tdec)
CODE_ON_VS(Width, Val)

CODE_ON_VS(Pol, Val)

// --------------------------------------------- //
// Manage the mode switching                     //
// --------------------------------------------- //

ModeInfo* MainWindow::getModeInfo(QString modeName){
    if( modeName == "OFF"    ) return getOffInfo();
    if( modeName == "Waves"  ) return getWavesInfo();
    if( modeName == "Intense") return getIntenseInfo();
    if( modeName == "Random" ) return getRandomInfo();
    if( modeName == "User"   ) return getUserInfo();
    if( modeName == "HiFreq" ) return getHifreqInfo();
    if( modeName == "Climb"  ) return getClimbInfo();
    if( modeName == "Throb"  ) return getThrobInfo();
    if( modeName == "Combo"  ) return getComboInfo();
    if( modeName == "Thrust" ) return getThrustInfo();
    if( modeName == "Thump"  ) return getThumpInfo();
    if( modeName == "Ramp"   ) return getRampInfo();
    if( modeName == "Stroke" ) return getStrokeInfo();
    if( modeName == "All"    ) return getAllInfo();
    qDebug() << "Unknow mode" << modeName;
    return nullptr;
}


void MainWindow::on_comboBox_Mode_activated(int index)
{
    if( ! synchStatus ) synchRequired();
    const QString modeName = ui->comboBox_Mode->itemText(index);
    if( modeName == "OFF") on_pushButton_Control_Reset_clicked();
    ModeInfo* newModeInfo = getModeInfo(modeName);
    if( newModeInfo == nullptr ){
        qDebug() << "ERROR unknown mode: " << modeName;
        return;
    }

    qDebug() << "Switwing to mode" << modeName;
    getModeStatus(currentModeInfo);

    newModeInfo->comboIndex = index;
    ui->label_info_mode->setText(newModeInfo->text ? newModeInfo->text : "");

    // show/hide slider depending on the new mode
    const auto allGuiInfo = getAllGuiInfo();
    const auto listAll = allGuiInfo->list;
    auto iter = listAll.begin();
    while( iter != listAll.end() ){
        const int addr = iter->addr;
        const auto gui = (*allGuiInfo)[addr];
        if( (*newModeInfo)[addr] != nullptr ){
            gui->slider->show();
            gui->labelName->show();
            gui->labelValue->show();
        }else{
            gui->slider->hide();
            gui->labelName->hide();
            gui->labelValue->hide();
        }
        iter++;
    }

    // Swicth to the new mode
    refreshSlidersTimer->stop();

    // save current positions of A, MA, B.
    currentModeInfo->A  = ui->verticalSlider_Control_A->value();
    currentModeInfo->B  = ui->verticalSlider_Control_B->value();
    currentModeInfo->MA = ui->verticalSlider_Control_MA->value();
    currentModeInfo->MAX= ui->spinBox_Control_MAX->value();

    // always restore the MAX value for this mode
    ui->spinBox_Control_MAX->setValue(newModeInfo->MAX);

    switch( opt.onSwitchCtrl() ){
        case 0: // reset to 0
            ui->verticalSlider_Control_A->setValue(0);
            ui->verticalSlider_Control_B->setValue(0);
            break;
        case 2: // divide by 2
            ui->verticalSlider_Control_A->setValue(ui->verticalSlider_Control_A->value()/2);
            ui->verticalSlider_Control_B->setValue(ui->verticalSlider_Control_B->value()/2);
            break;
        case 1: // keep current values: nothing to do.
        case 3: // restore last values: nothing to do now.
       default: break;
    }

    if( currentModeInfo != newModeInfo ){
        currentModeFileName = defaultPath(currentModeFileName);
    }
    // must be done before to update the sliders
    currentModeInfo = newModeInfo;

    qDebug() << currentModeInfo->list.size() << "parameters to set";
    if( currentModeInfo->paramSrc == ModeInfo::src::none || (opt.onSwitchParams() == 0) ){
        switchTo(newModeInfo->etCode);
        // get the current values from the ET232 and set the sliders accordingly
        setSlidersFromET232(currentModeInfo);
        currentModeInfo->paramSrc = ModeInfo::src::et;
    } else if ( currentModeInfo->paramSrc == ModeInfo::src::et ) {
        switchTo(newModeInfo->etCode);
        // nothing to do. This mode has not been modified since its initialization with et232 values
    }else{ // value from file or modified by user
        if( currentModeInfo->paramSrc == ModeInfo::src::file){
            // Le values we have are from file
            QMessageBox::warning(this, "Switching",
                                       "ET232 will be configured with values from loaded file");
        }
        switchTo(newModeInfo->etCode);
        setSlidersFromMem(currentModeInfo);
    }

    // set the colored dot
    if( modeName == "OFF" ){
        // always green dot for 'OFF' mode
        ui->comboBox_Mode->setItemIcon(index, QIcon(":/resources/green-dot.png"));
    }else if( currentModeInfo->paramSrc == ModeInfo::src::et ){
        // blue dot if values used are those set by et232
        ui->comboBox_Mode->setItemIcon(index, QIcon(":/resources/blue-dot.png"));
    }else{
        // red dot if value are defined by the user (from file, or preview live values)
        ui->comboBox_Mode->setItemIcon(index, QIcon(":/resources/red-dot.png"));
    }

    if( opt.onSwitchCtrl() == 3 ){
        // Restore last Control values used by this mode.
        // These values are 0 is this is the first use of this mode.
        ui->verticalSlider_Control_A->setValue(currentModeInfo->A);
        ui->verticalSlider_Control_B->setValue(currentModeInfo->B);
        ui->verticalSlider_Control_MA->setValue(currentModeInfo->MA);
    }

    qDebug() << "current:" << ui->tabWidget->currentIndex();

    // if "OFF" is visible select the "Channel A" tab
    if( ui->tabWidget->currentIndex()==0 ){
        ui->tabWidget->setCurrentIndex(1);
    }
    qDebug() << "Switch to" << modeName << "done" << currentModeInfo->paramSrc;
}

void MainWindow::switchTo(int etCode){
    poke(ET::MODE, etCode); // switch to new mode
    QThread::msleep(333);   // don't bother the ET232 when it is switching modes
}

void MainWindow::setSlidersFromMem(const ModeInfo* mode){
    const auto allGuiInfo = getAllGuiInfo();
    for( auto item : mode->list ){
        const int addr = item.addr;
        qDebug() << item.key << "addr=" << addr << ", last value=" << item.lastEtValue;
        const auto uiInfo = (*allGuiInfo)[addr];
        const int vsValue = (addr==A::Pol::Val || addr==B::Pol::Val)? (item.lastEtValue & 0x0E)/2 : item.lastEtValue;
        uiInfo->slider->setValue(vsValue);
    }
}

void MainWindow::setSlidersFromET232(const ModeInfo* mode){
    // get the current values from the ET232 and set the sliders accordingly
    qDebug() << "Refreshing...";
    const auto allGuiInfo = getAllGuiInfo();
    for( auto item : mode->list ){
        const int addr = item.addr;
        qDebug() << "Addr for" << item.key << "is" << addr;
        peekCount = 0;
        peek(addr, addr*4);
        while (peekCount==0)
            QCoreApplication::processEvents();
        const auto uiInfo = (*allGuiInfo)[addr];
        const int etValue = etMem[addr].value;
        const int vsValue = (addr==A::Pol::Val || addr==B::Pol::Val)? (etValue & 0x0E)/2 : etValue;
        uiInfo->slider->setValue(vsValue);
    }
}

void MainWindow::processRefreshSlidersTimer(){
    if( currentModeInfo ) setSlidersFromET232(currentModeInfo);
}
