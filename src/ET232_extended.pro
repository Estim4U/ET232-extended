# SPDX-License-Identifier: GPL-3.0-only
#
# This file is part of the ET232-extended application.
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3 as published
# by the Free Software Foundation https://www.gnu.org/licenses/gpl-3.0.html
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T20:49:28
#
#-------------------------------------------------

QT += core gui serialport widgets

TARGET = ET232_extended
TEMPLATE = app

# emit warnings if any feature of Qt which has been marked as deprecated is used
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    gui_info.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_file.cpp \
    mainwindow_modes.cpp \
    mainwindow_switch.cpp \
    mainwindow_controls.cpp \
    serial_worker.cpp \
    mainwindow_synchro.cpp \
    select_serial_port.cpp \
    serial_ports_info.cpp \
    mode_info.cpp \
    configuration.cpp \
    options.cpp \
    mainwindow_graph.cpp

HEADERS += \
    gui_info.h \
    macros.h \
    mainwindow.h \
    version.h \
    version.rc \
    serial_worker.h \
    select_serial_port.h \
    serial_ports_info.h \
    mode_info.h \
    configuration.h \
    options.h

FORMS += \
    mainwindow.ui \
    select_serial_port.ui \
    serial_ports_info.ui \
    configuration.ui

RC_FILE = version.rc

RESOURCES += resources.qrc
