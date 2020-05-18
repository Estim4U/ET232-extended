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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"

#include <QFileDialog>
#include <QMap>

// --------------------------------------------- //
// Manage the file operations                    //
// --------------------------------------------- //

static bool isFile(const QString name){
    return name.length()>0 && QFileInfo(name).isFile();
}

QString MainWindow::defaultPath(const QString name){
    if( name.length() > 0 ){
        if( QFileInfo(name).isFile() ) return QFileInfo(name).path();
        if( QFileInfo(name).isDir()  ) return name;
    }
    return QDir::homePath();
}

static QMap<QString, int> hash;

//-------- All modes --------

void MainWindow::saveAllModesPath()
{
    opt.setAllModesPath(QFileInfo(allModesFileName).path());
}

QString MainWindow::getSaveFileName_All()
{
    if( hash.isEmpty() ){
        QMessageBox::information(this, "Save all used", "Nothing to save");
        return "";
    }
    return QFileDialog::getSaveFileName(this,
             "Save Parameters of all used modes",
              defaultPath(allModesFileName),
              "EXT Files (*.all.ext)");
}

void MainWindow::on_actionSaveAll_triggered()
{
    getModeStatus(currentModeInfo);
    if( ! isFile(allModesFileName) ){
        const QString tmpFileName = getSaveFileName_All();
        if( tmpFileName.length() == 0 ) return;
        allModesFileName = tmpFileName;
    }

    qDebug() << "Save all modes used To" << allModesFileName;
    saveHashTo(allModesFileName);
    saveAllModesPath();
}

void MainWindow::on_actionSaveAsAll_triggered()
{
    getModeStatus(currentModeInfo);
    const QString tmpFileName = getSaveFileName_All();
    if( tmpFileName.length() == 0 ) return;

    qDebug() << "Save all modes used As" << tmpFileName;
    saveHashTo(tmpFileName);
}

void MainWindow::on_actionLoadAll_triggered()
{
    const QString tmpFileName = QFileDialog::getOpenFileName(this,
         "Load Parameters for ALL modes",
         defaultPath(allModesFileName),
         "ETX Files (*.all.ext)");
    // For unknow reason, sometime, getOpenFileName() takes 4 or 5 sec to return
    if( tmpFileName.length() == 0 ) return;

    // TODO: get current mode, switch to OFF

    allModesFileName = tmpFileName;
    if( ! loadHashFrom(allModesFileName) ){
        ui->statusBar->showMessage(QString("Failed to load file %1").arg(allModesFileName));
        ui->statusBar->setStyleSheet("color:red");
        return;
    }

    ui->statusBar->showMessage(QString("File %1 loaded").arg(allModesFileName));
    ui->statusBar->setStyleSheet("color:green");
    saveAllModesPath();
    // TODO: restore mode

}

//-------- Current mode --------

void MainWindow::saveCurrentModePath()
{
    opt.setCurrentModePath(QFileInfo(currentModeFileName).path());
}

QString MainWindow::getSaveFileName_Current()
{
    if( hash.isEmpty() ){
        QMessageBox::information(this, "Save current mode", "Nothing to save");
        return "";
    }
    const QString modeName = ui->comboBox_Mode->currentText();
    return QFileDialog::getSaveFileName(this,
             QString("Save Parameters of %1 mode").arg(modeName),
             defaultPath(currentModeFileName),
             QString("EXT Files (*.%1.ext)").arg(modeName) );
}

void MainWindow::on_actionSaveCurrent_triggered()
{
    getModeStatus(currentModeInfo);
    if( ! isFile(currentModeFileName) ){
       const QString tmpFileName = getSaveFileName_Current();
       if( tmpFileName.length() == 0 ) return;
       currentModeFileName = tmpFileName;
    }

    const QString modeName = ui->comboBox_Mode->currentText();
    qDebug() << "Save mode" << modeName << "to" << currentModeFileName;
    saveHashTo(currentModeFileName, modeName);
    saveCurrentModePath();
}

void MainWindow::on_actionSaveAsCurrent_triggered()
{
    getModeStatus(currentModeInfo);
    const QString tmpFileName = getSaveFileName_Current();
    if( tmpFileName.length() == 0 ) return;

    // save as: we don't change the currentModeFileName
    const QString modeName = ui->comboBox_Mode->currentText();
    qDebug() << "Save mode" << modeName << "As" << tmpFileName;
    saveHashTo(tmpFileName, modeName);
}

void MainWindow::on_actionLoadCurrent_triggered()
{
    const QString modeName = ui->comboBox_Mode->currentText();
    const QString tmpFileName = QFileDialog::getOpenFileName(this,
            QString("Load Parameters for %1 mode").arg(modeName),
            defaultPath(currentModeFileName),
            QString("ETX Files (*.%1.ext)").arg(modeName));
    // For unknow reason, sometime, getOpenFileName() takes 4 or 5 sec to return
    if( tmpFileName.length() == 0 ) return;

    // TODO: get current mode, switch to OFF

    currentModeFileName = tmpFileName;
    if( ! loadHashFrom(currentModeFileName) ){
        ui->statusBar->showMessage(QString("Failed to load file %1").arg(currentModeFileName));
        ui->statusBar->setStyleSheet("color:red");
        return;
    }

    ui->statusBar->showMessage(QString("File %1 loaded").arg(currentModeFileName));
    ui->statusBar->setStyleSheet("color:green");
    saveCurrentModePath();
    // TODO: restore
}

//-------------

// Get the info about a specific mode and save the values
// of the sliders used by this mode in the hash
void MainWindow::getModeStatus(const ModeInfo* mode)
{
    auto iter = (mode->list).begin();
    QString prefix = mode->modeName + "::";
    while( iter != (mode->list).end() ){
        hash[prefix + iter->key] = iter->lastEtValue;
        iter++;
    }
}

bool MainWindow::saveHashTo(const QString fileName, const QString modeName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)){
        qDebug() << "Error: unable to open" << fileName << "in RW mode";
        QMessageBox::critical(this, "Error opening file in RW mode", file.errorString());
        return false;
    }

    QTextStream out(&file);
    QMapIterator<QString, int> iter(hash);
    while( iter.hasNext() ){
        iter.next();
        if( modeName.length()==0 || iter.key().startsWith(modeName,Qt::CaseInsensitive) ){
            out << iter.key() << "=" << iter.value() << "\n";
            qDebug() << "Write:" << iter.key() << "=" << iter.value();
        }
    }
    file.close();
    return true;
}

bool MainWindow::loadHashFrom(const QString fileName, const QString modeName)
{
    qDebug() << "Loading" << fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "Error: unable to open" << fileName << "in RO mode";
        QMessageBox::critical(this, "Error opening file", file.errorString());
        return false;
    }

    QRegularExpression re_line("^(\\w+)::((?:A|B)::\\w+::\\w+)\\s*=\\s*(\\d{1,3})$");
    QTextStream inp(&file);
    while( !inp.atEnd() ){
        QCoreApplication::processEvents();
        QString line = inp.readLine();
        QRegularExpressionMatch match = re_line.match(line);
        if( ! match.hasMatch() ){
            QMessageBox::warning(this, "Invalid line", line);
            continue;
        }
        if( modeName.length()==0 || line.startsWith(modeName,Qt::CaseInsensitive) ){
            if( ! updateModeParam(match.captured(1), match.captured(2), match.captured(3).toInt()) ){
                QMessageBox::warning(this, "Invalid line", line);
                continue;
            }
        }
    }
    file.close();
    return true;
}

bool MainWindow::updateModeParam(const QString modeName, const QString key, const int value)
{
    qDebug() << "Update " << modeName << "with key=" << key << "value=" << value;

    ModeInfo* modeInfo = getModeInfo(modeName);
    if( modeInfo == nullptr) return false;

    ParamInfo* paramInfo = (*modeInfo)[key];
    if( paramInfo == nullptr ) return false;

    //To avoid surprise, do not update slider and et232 now.
    paramInfo->lastEtValue = value;
    modeInfo->paramSrc = ModeInfo::src::file;

    return true;
}
