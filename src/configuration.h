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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>

namespace Ui {
class Configuration;
}

class Configuration : public QDialog
{
    Q_OBJECT
public:
    explicit Configuration(QWidget *parent = nullptr);
    ~Configuration();

private slots:
    void on_pushButton_default_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::Configuration *ui;
};

#endif // CONFIGURATION_H
