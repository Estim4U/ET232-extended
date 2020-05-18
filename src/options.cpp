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

#include "options.h"
#include "version.h"

options opt;

options::options()
{
    cfg = new QSettings(QSettings::IniFormat, QSettings::UserScope, ORG_NAME, APP_NAME);
    _geometry        = cfg->value("geometry").toByteArray();
    _restoreGeometry = cfg->value("restoreGeometry", false).toBool();
    _onSwitchCtrl    = cfg->value("onSwitchCtrl", 0).toInt();
    _onSwitchParams  = cfg->value("onSwitchParams", 0).toInt();
    _graphVisibility = cfg->value("showGraph", false).toBool();
    _allModesPath    = cfg->value("allModesPath").toString();
    _currentModePath = cfg->value("currentModePath").toString();
}

void options::save(){
    cfg->sync();
}

//---- setters ----//

void options::setGeometry(const QByteArray& g){
    _geometry = g;
    cfg->setValue("geometry", g);
}

void options::setGraphVisibility(bool flag){
    _graphVisibility = flag;
    cfg->setValue("showGraph", flag);
}

void options::setRestoreGeometry(bool flag){
    _restoreGeometry=flag;
    cfg->setValue("restoreGeometry", flag);
}

void options::setOnSwitchCtrl(int val){
    _onSwitchCtrl=val;
    cfg->setValue("onSwitchCtrl", val);
}

void options::setOnSwitchParams(int val){
    _onSwitchParams=val;
    cfg->setValue("onSwitchParams", val);
}

void options::setAllModesPath(QString val){
    _allModesPath=val;
    cfg->setValue("allModesPath", val);
}

void options::setCurrentModePath(QString val){
    _currentModePath=val;
    cfg->setValue("currentModePath", val);
}
