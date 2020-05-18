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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serial_worker.h"
#include "mode_info.h"
#include "gui_info.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QThread>
#include <QGraphicsView>
#include <QGraphicsScene>

namespace A {

  namespace Width {
    enum { Val=0x08, Tmin=0x21, Tinc, Tmax, Tdec, Min, Max };
  }

  namespace Freq {
    enum { Val=0x09, Tmin=0x29, Tinc, Tmax, Tdec, Min, Max };
  }


  namespace Ampli {
    enum { Val=0x0A, Tmin=0x31, Tinc, Tmax, Tdec, Min, Max};
  }

  namespace Pol {
    enum { Val=0x0C };
  }

}

namespace B {


  namespace Width {
    enum { Val=0x0E, Tmin=0x39, Tinc, Tmax, Tdec, Min, Max };
  }

  namespace Freq {
    enum { Val=0x0F, Tmin=0x41, Tinc, Tmax, Tdec, Min, Max };
  }

  namespace Ampli {
    enum { Val=0x10, Tmin=0x49, Tinc, Tmax, Tdec, Min, Max};
  }

  namespace Pol {
    enum { Val=0x12 };
  }

}

namespace KNOB {
  enum { A=0x8C, MA=0x89, B=0x88 };
}

namespace ET {
  enum { MODE=0xA3, ADCTRL=0xA4 };
}

const int MaxLevel = 135;

// ---- memory ---- //

enum memStatus {
    VALUE_UNKNOWN,
    VALUE_SET         = 0x01, // Bit1
    VALUE_REQUESTED_1 = 0x02, // Bit2
    VALUE_REQUESTED_n = 0x04, // Bit3
};

struct memInfo {
    int value;
    int status;
};

extern memInfo etMem[256];


// ---- The mainwindow ---- //

namespace Ui {
  class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

//---- GLOBAL ----//
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
    ~MainWindow();

public slots:
    void agreement();
    void processGrTimer();
    void processRefreshSlidersTimer();

//---- Application Menu ----//
public slots:
    // File
    void on_actionLoadAll_triggered();
    void on_actionSaveAll_triggered();
    void on_actionSaveAsAll_triggered();
    void on_actionLoadCurrent_triggered();
    void on_actionSaveCurrent_triggered();
    void on_actionSaveAsCurrent_triggered();
    void on_actionQuit_triggered();
    // Connexion
    void on_actionSynchronize_triggered();
    void on_actionSerial_ports_info_triggered();
    // Setup
    void on_actionConfiguration_triggered();
    void on_actionShow_graph_triggered(bool checked);
    // Help
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();

//---- Exchange with ET232 ----//
public slots:
    void processTimeOut(QString msg);
    void processResult(int code, QString msg);
    void processPeekResult(int addr, int value);
    void processStatus(int code, QString msg);

signals:
    void requestInit(QString port);
    void requestFlush();
    void requestSynch();
    void requestPing();
    void requestPeek(int addr, int defaultValue);
    void requestPoke(int addr, int value);

private:
    const char* addr2text(int addr);
    void raz();
    void init(QString port);
    void synchRequired();
    void synch();
    void ping();
    void peek(int addr, int defaultValue=0);
    void poke(int addr, int value);

//---- Off ----//
private:
    ModeInfo* getOffInfo();

//---- Waves ----//
private:
    ModeInfo* getWavesInfo();

//---- Intense ----//
private:
    ModeInfo* getIntenseInfo();

//---- Random ----//
private:
    ModeInfo* getRandomInfo();

//---- User ----//
private:
    ModeInfo* getUserInfo();

//---- HiFreq ----//
private:
    ModeInfo* getHifreqInfo();

//---- Climb ----//
private:
    ModeInfo* getClimbInfo();

//---- Throb ----//
private:
    ModeInfo* getThrobInfo();

//---- Combo ----//
private:
    ModeInfo* getComboInfo();

//---- Thrust ----//
private:
    ModeInfo* getThrustInfo();

//---- Thump ----//
private:
    ModeInfo* getThumpInfo();

//---- Ramp ----//
private:
    ModeInfo* getRampInfo();

//---- Stroke ----//
private:
    ModeInfo* getStrokeInfo();

//---- All ----//
private:
    ModeInfo* getAllInfo();

//---- signals from sliders ----//
private slots:
    DCL_ON_VS2(Ampli_Min)
    DCL_ON_VS2(Ampli_Max)
    DCL_ON_VS2(Ampli_Tmin)
    DCL_ON_VS2(Ampli_Tmax)
    DCL_ON_VS2(Ampli_Tinc)
    DCL_ON_VS2(Ampli_Tdec)
    DCL_ON_VS2(Ampli_Val)

    DCL_ON_VS2(Freq_Min)
    DCL_ON_VS2(Freq_Max)
    DCL_ON_VS2(Freq_Tmin)
    DCL_ON_VS2(Freq_Tmax)
    DCL_ON_VS2(Freq_Tinc)
    DCL_ON_VS2(Freq_Tdec)
    DCL_ON_VS2(Freq_Val)

    DCL_ON_VS2(Width_Min)
    DCL_ON_VS2(Width_Max)
    DCL_ON_VS2(Width_Tmin)
    DCL_ON_VS2(Width_Tmax)
    DCL_ON_VS2(Width_Tinc)
    DCL_ON_VS2(Width_Tdec)
    DCL_ON_VS2(Width_Val)

    DCL_ON_VS2(Pol_Val)

//---- Controls ----//
private slots:
    void on_spinBox_Control_MAX_valueChanged(int value);
    void on_verticalSlider_Control_A_valueChanged(int value);
    void on_verticalSlider_Control_MA_valueChanged(int value);
    void on_verticalSlider_Control_B_valueChanged(int value);
    void on_pushButton_Control_Reset_clicked();
    void on_checkBox_Control_stateChanged(int state);
    void on_comboBox_Mode_activated(int index);

//---- Graphic ----//
private slots:
    void on_checkBox_Graph_A_Width_stateChanged(int state);
    void on_checkBox_Graph_A_Freq_stateChanged(int state);
    void on_checkBox_Graph_A_Ampli_stateChanged(int state);
    void on_checkBox_Graph_A_Pol_stateChanged(int state);
    void on_checkBox_Graph_B_Width_stateChanged(int state);
    void on_checkBox_Graph_B_Freq_stateChanged(int state);
    void on_checkBox_Graph_B_Ampli_stateChanged(int state);
    void on_checkBox_Graph_B_Pol_stateChanged(int state);

//---- Memory ----//

private:
    void initMem(std::initializer_list<int>);
    void pollMem(int addr, bool state);

private:
    int       displayAbout(QMessageBox::StandardButtons buttons);
    void      getModeStatus(const ModeInfo* mode);
    ModeInfo* getModeInfo(QString modeName);
    AllGuiInfo* getAllGuiInfo();
    bool      saveHashTo(const QString fileName, const QString modeName = "");
    bool      loadHashFrom(const QString fileName, const QString modeName = "");
    bool      updateModeParam(QString modeName, QString key, int value);
    QString   getSaveFileName_All();
    QString   getSaveFileName_Current();
    void      saveAllModesPath();
    void      saveCurrentModePath();
    QString   defaultPath(QString name);
    void      switchTo(int etCode);
    void      setSlidersFromET232(const ModeInfo* mode);
    void      setSlidersFromMem(const ModeInfo* mode);
    void      updateCurrentValue(int addr, int value);

    Ui::MainWindow *ui;
    QThread*        thread;
    SerialWorker*   worker;
    QTimer*         grTimer;
    QTimer*         refreshSlidersTimer;
    QGraphicsScene* scene;
    QGraphicsView * view;
    ModeInfo*       currentModeInfo = getOffInfo();
    QString         currentModeFileName;
    QString         allModesFileName;
    bool            synchStatus = false;
    unsigned int    peekCount = 0;
};

#endif // MAINWINDOW_H
