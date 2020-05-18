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

#include "serial_ports_info.h"
#include "ui_serial_ports_info.h"

#include <QSerialPortInfo>

SerialPortsInfo::SerialPortsInfo(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SerialPortsInfo)
{
    ui->setupUi(this);

    int count = 0;
    const auto infos = QSerialPortInfo::availablePorts();
    for( const QSerialPortInfo &info : infos ){

       QString s = QObject::tr("Port: ")               + info.portName()       + "\n"
                 + QObject::tr("Location: ")           + info.systemLocation() + "\n"
                 + QObject::tr("Description: ")        + info.description()    + "\n"
                 + QObject::tr("Manufacturer: ")       + info.manufacturer()   + "\n"
                 + QObject::tr("Serial Number: ")      + info.serialNumber()   + "\n"
                 + QObject::tr("Vendor Identifier: ")  + (info.hasVendorIdentifier()  ? QString::number(info.vendorIdentifier(), 16)  : QString()) + "\n"
                 + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                 + QObject::tr("Busy: ")               + (info.isBusy()               ? QObject::tr("Yes") : QObject::tr("No"))                    + "\n"
                 ;

        ui->textBrowser->append(s);
        count++;
    }

    if( count==0 ){
        ui->textBrowser->append(
            "There is no COM port detected on this computer.\n\n"
            "For USB/SERIAL adapter:\n"
            "- check status in Device Manager,\n"
            "- check driver compatibility,\n"
            "- check cable.\n\n"
            "For serial port on the mother board:\n"
            "- Check status in Device Manager,\n"
            "- Check BIOS setup."
        );
    }

}

void SerialPortsInfo::on_closeButton_clicked()
{
    close();
}

SerialPortsInfo::~SerialPortsInfo()
{
    delete ui;
}


