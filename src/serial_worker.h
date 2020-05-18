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

#ifndef SERIAL_WORKER_H
#define SERIAL_WORKER_H

#include <QSerialPort>

class SerialWorker : public QObject {
  Q_OBJECT
public:
  SerialWorker();
  ~SerialWorker();

public slots:
  void doStart();
  void doInit(QString port);
  void doFlush();
  void doSynch();
  void doPing();
  void doPeek(int addr, int defaultValue);
  void doPoke(int addr, int value);

signals:
  void sendStatus(int code, QString msg);
  void sendTimeOut(QString msg);
  void sendResult(int code, QString msg);
  void sendPeekResult(int addr, int value);

private:
  QSerialPort* serial;
};

#endif // SERIAL_WORKER_H
