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

#include "select_serial_port.h"
#include "ui_select_serial_port.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSerialPortInfo>

SelectSerialPort::SelectSerialPort(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SelectSerialPort)
{
    ui->setupUi(this);

    int serialCount = 0;
    const auto infos = QSerialPortInfo::availablePorts();
    for( const QSerialPortInfo &info : infos ){
        QString s = info.portName()
                  + QObject::tr(": (") + info.description()
                  // + QObject::tr(") Man(")   + info.manufacturer()
                + ")" ;
        ui->serialPortComboBox->addItem(s);
        serialCount++;
    }
    if( serialCount==0 ){
        ui->serialPortComboBox->addItem("No serial port detected!");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

void SelectSerialPort::on_buttonBox_accepted()
{
    QString portLongName = ui->serialPortComboBox->currentText();

    if( ! portLongName.contains(QRegExp("^COM\\d:")) ){
        QMessageBox::critical(this, "Synchro: invalid port",
                                    "Synchronization with ET232: FAILED\ninvalid port.",
                                     QMessageBox::Cancel);
        return;
    }

    portShortName = portLongName.left(4);
}

SelectSerialPort::~SelectSerialPort()
{
    delete ui;
}
