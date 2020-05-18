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

#include "serial_worker.h"

#include <QDebug>


SerialWorker::SerialWorker()
{
    qDebug() << "SerialWorker constructor";
}


void SerialWorker::doStart(){
    qDebug() << "SerialWorker doStart()";
}


void SerialWorker::doInit(QString port){
    qDebug() << "Serial init:" << port;
    if( serial ){
        serial->close();
        delete serial;
    }
    serial = new QSerialPort(port);
    serial->setBaudRate(QSerialPort::Baud19200);
    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);

    if( serial->error() != QSerialPort::NoError ){
        emit sendStatus(0, "Fail to open");
    }else{
        emit sendStatus(1, "OK");
    }
}


void SerialWorker::doFlush(){
    if(!serial){
        emit sendResult(-1, "No serial port opened");
        return;
    }
    while (!serial->atEnd()) serial->read(1);
    emit sendStatus(2, "Flush done");
}


void SerialWorker::doSynch(){
    if(!serial){
        emit sendResult(-1, "No serial port opened");
        return;
    }

    char data[8];
    qint64 count = serial->read(data, 8);
    if( count==3 && data[0] == 0 && data[1] == 'C' && data[2] == 'C' ){
        emit sendStatus(4, "Synch OK");
    }else if( count!=3 ){
        emit sendStatus(3, QString("Expecting 3 bytes, got %1").arg(count));
    }else{
        emit sendStatus(3, QString("Invalid data"));
    }
}


void SerialWorker::doPing(){
    if(!serial){
        emit sendResult(-1, "No serial port opened");
        return;
    }

    // send the J command
    serial->write("J4A\r");

    // read the reply: "FF\n"
    unsigned int i = 0;
    char buffer[8];
    do {
        if( !serial->waitForReadyRead(500) ){
            emit sendTimeOut("Ping timeout");
            emit sendStatus(5, "Ping timeout");
            return;
        }
        i += serial->read(buffer+i,sizeof(buffer)-i);
    } while( i<3 );

    if( i==3 && buffer[0]=='F' && buffer[1]=='F' && buffer[2]=='\n' ){
        emit sendStatus(6, "Ping ok");
    }else{
        emit sendStatus(5, "Ping failed");
    }
}


void SerialWorker::doPeek(const int addr, const int defaultValue){
    if(!serial){
        emit sendResult(-1, "No serial port opened");
        emit sendPeekResult(addr, defaultValue);
        return;
    }

    // build the command: "H<msb-addr><lsb-addr><msb-checksum><lsb-checksum>\r"
    char bufferout[16];
    sprintf(bufferout, "H%02X", addr);
    int sum = bufferout[0] + bufferout[1] + bufferout[2];
    sprintf(bufferout+3, "%02X\r", sum & 0xFF);

    // send the H command
    serial->write(bufferout);

    // read the reply: "<msbValue><lsbValue>\n"
    char bufferinp[16] = {0};
    unsigned int i = 0;
    do {
        if( !serial->waitForReadyRead(500) ){
            emit sendTimeOut("Peek timeout");
            return;
        }
        i += serial->read(bufferinp+i,sizeof(bufferinp)-i);
    } while( i<3 );

    int value = -1;
    if( i==3 && isxdigit(bufferinp[0]) && isxdigit(bufferinp[1]) && bufferinp[2]=='\n' ){
        bufferinp[2]=0; // replace the \n
        value = strtol(bufferinp, nullptr, 16);
    }else{
        emit sendResult(-1, QString("Peek(%1) invalid reply: %2 bytes = '%3'").arg(bufferout).arg(i).arg(bufferinp));
        return;
    }
    emit sendPeekResult(addr, value);
}


void SerialWorker::doPoke(const int addr, const int value){
    if(!serial){
        emit sendResult(-1, "No serial port opened");
        return;
    }

    // build the command: "I<msb-addr><lsb-addr><msb-value><lsb-value><msb-checksum><lsb-checksum>\r"
    char bufferout[10];
    sprintf(bufferout, "I%02X%02X", addr, value);
    int sum = bufferout[0] + bufferout[1] + bufferout[2] + bufferout[3] + bufferout[4];
    sprintf(bufferout+5, "%02X\r", sum & 0xFF);

    // send the I command
    serial->write(bufferout);

    // read the reply. Expected "$\n"
    char bufferinp[16] = {0};
    unsigned int i = 0;
    do {
        if( !serial->waitForReadyRead(500) ){
            emit sendTimeOut("Poke timeout");
            return;
        }
        i += serial->read(bufferinp+i,sizeof(bufferinp)-i);
    } while( i<2 );

    if( ! (i==2 && bufferinp[0]=='$' && bufferinp[1]=='\n') ){
        emit sendResult(-1, QString("Poke(%1) invalid reply: %2 bytes ='%3'").arg(bufferout).arg(i).arg(bufferinp));
    }
}


SerialWorker::~SerialWorker()
{
    qDebug() << "SerialWorker deleted";
    if(serial){
        serial->close();
        delete serial;
     }
}
