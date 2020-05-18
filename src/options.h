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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QSettings>

class options
{
public:
    options();
    void save();

    void setGeometry(const QByteArray& g);
    const QByteArray geometry(){ return _geometry; }

    void setRestoreGeometry(bool flag);
    bool restoreGeometry(){ return _restoreGeometry; }

    void setGraphVisibility(bool flag);
    bool graphVisibility(){return _graphVisibility; }

    void setOnSwitchCtrl(int val);
    int  onSwitchCtrl(){ return _onSwitchCtrl; }

    void setOnSwitchParams(int val);
    int  onSwitchParams(){ return _onSwitchParams; }

    void setAllModesPath(QString val);
    QString  allModesPath(){ return _allModesPath; }

    void setCurrentModePath(QString val);
    QString  currentModePath(){ return _currentModePath; }

private:
    QSettings* cfg;
    QByteArray _geometry;
    bool _restoreGeometry;
    bool _graphVisibility;
    int  _onSwitchCtrl;
    int  _onSwitchParams;
    QString _allModesPath;
    QString _currentModePath;
};

extern options opt;

#endif // OPTIONS_H
