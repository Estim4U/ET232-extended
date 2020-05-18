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

#ifndef SELECT_SERIAL_PORT_H
#define SELECT_SERIAL_PORT_H

#include <QDialog>
#include "serial_worker.h"

namespace Ui {
  class SelectSerialPort;
}

class SelectSerialPort : public QDialog
{
  Q_OBJECT
public:
  explicit SelectSerialPort(QWidget *parent = nullptr);
  ~SelectSerialPort();
  QString portShortName;

private slots:
  void on_buttonBox_accepted();

private:
  Ui::SelectSerialPort *ui;
};

#endif // SELECT_SERIAL_PORT_H
