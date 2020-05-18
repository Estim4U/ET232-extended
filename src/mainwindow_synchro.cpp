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

#include "ui_mainwindow.h"
#include "mainwindow.h"

void MainWindow::processStatus(int status, QString msg){
    qDebug() << "Status:" << status << msg;
    switch(status){

    case 0: // Failed to open port
        msg = "Cannot open serial port";
        break;

    case 1: // open ok
        emit requestFlush();
        return;

    case 2:{ // flush done
        const auto reply = QMessageBox::information(this, "Synchro",
                                                    "1. Select a mode,\n2. When you see blue leds flashing click OK",
                                                    QMessageBox::Ok | QMessageBox::Cancel);
        if( reply == QMessageBox::Cancel ){
            ui->statusBar->showMessage("Synchronization with ET232 cancelled");
            ui->statusBar->setStyleSheet("color:black");
            return;
        }
        QThread::msleep(2000);
        emit requestSynch();
        return;
    }

    case 3: // wrong magic string
        break;

    case 4: // ok we got the expected magic string
        emit requestPing();
        return;

    case 5: // ping failed
        msg = "Cannot talk to the ET232";
        break;

    case 6: // Synchro OK
        QMessageBox::information(this, "Synchro OK",
                                       "Synchronization with ET232: OK",
                                       QMessageBox::Ok);
        ui->statusBar->showMessage("Synchronization with ET232 SUCCEEDED");
        ui->statusBar->setStyleSheet("color:green");
        synchStatus = true;
        return;

    default:
        qDebug() << "got an UNEXPECTETD status:" << status;
        return;
    }

    QMessageBox::critical(this, "Synchro failed",
                                QString("Synchronization with ET232 FAILED\n%1.").arg(msg),
                                QMessageBox::Cancel);
    ui->statusBar->showMessage("Synchronization with ET232 FAILED");
    ui->statusBar->setStyleSheet("color:red");
    return;
}
