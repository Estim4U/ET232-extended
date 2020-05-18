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

#include "configuration.h"
#include "ui_configuration.h"

#include "options.h"

Configuration::Configuration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration)
{
    ui->setupUi(this);

    ui->checkBox_Geometry->setChecked(opt.restoreGeometry());
    ui->comboBox_AMAB->setCurrentIndex(opt.onSwitchCtrl());
    ui->comboBox_Params->setCurrentIndex(opt.onSwitchParams());
}

Configuration::~Configuration()
{
    delete ui;
}

void Configuration::on_pushButton_default_clicked()
{
    ui->checkBox_Geometry->setChecked(false);
    ui->comboBox_AMAB->setCurrentIndex(0);
    ui->comboBox_Params->setCurrentIndex(0);
}

void Configuration::on_pushButton_save_clicked()
{
    opt.setRestoreGeometry(ui->checkBox_Geometry->checkState());
    opt.setOnSwitchCtrl(ui->comboBox_AMAB->currentIndex());
    opt.setOnSwitchParams(ui->comboBox_Params->currentIndex());
    close();
}

void Configuration::on_pushButton_cancel_clicked()
{
    close();
}
