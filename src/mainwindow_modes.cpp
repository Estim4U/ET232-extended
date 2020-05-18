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

ModeInfo* MainWindow::getAllInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("All", 0x00, {

        PARAM_INFO(Ampli, Min),
        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmin),
        PARAM_INFO(Ampli, Tmax),
        PARAM_INFO(Ampli, Tinc),
        PARAM_INFO(Ampli, Tdec),
        PARAM_INFO(Ampli, Val),

        PARAM_INFO(Freq,  Min),
        PARAM_INFO(Freq,  Max),
        PARAM_INFO(Freq,  Tmin),
        PARAM_INFO(Freq,  Tmax),
        PARAM_INFO(Freq,  Tinc),
        PARAM_INFO(Freq,  Tdec),
        PARAM_INFO(Freq,  Val),

        PARAM_INFO(Width, Min),
        PARAM_INFO(Width, Max),
        PARAM_INFO(Width, Tmin),
        PARAM_INFO(Width, Tmax),
        PARAM_INFO(Width, Tinc),
        PARAM_INFO(Width, Tdec),
        PARAM_INFO(Width, Val),

        PARAM_INFO(Pol, Val)
    }
    );
}

ModeInfo* MainWindow::getClimbInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Climb", 0x81, {

        PARAM_INFO(Freq,  Min),
        PARAM_INFO(Freq,  Max),
        PARAM_INFO(Freq,  Tmin),

        PARAM_INFO(Width, Val),

        PARAM_INFO(Pol, Val)
    }
    );
}

ModeInfo* MainWindow::getComboInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Combo", 0x84, {

        PARAM_INFO(Ampli, Min),
        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmin),
        PARAM_INFO(Ampli, Tmax),
        PARAM_INFO(Ampli, Tinc),
        PARAM_INFO(Ampli, Tdec),

        PARAM_INFO(Freq,  Min),
        PARAM_INFO(Freq,  Max),
        PARAM_INFO(Freq,  Tmin),
        PARAM_INFO(Freq,  Tmax),
        PARAM_INFO(Freq,  Tinc),
        PARAM_INFO(Freq,  Tdec),

        PARAM_INFO(Width, Min),
        PARAM_INFO(Width, Max),
        PARAM_INFO(Width, Tmin),
        PARAM_INFO(Width, Tmax),
        PARAM_INFO(Width, Tinc),
        PARAM_INFO(Width, Tdec),

        PARAM_INFO(Pol, Val)
    },
    "MA controls the polarity. You control everything else: 36 parameters!"
    );
}

ModeInfo* MainWindow::getHifreqInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("HiFreq", 0x85, {

        PARAM_INFO(Ampli, Min),
        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmin),
        PARAM_INFO(Ampli, Tmax),
        PARAM_INFO(Ampli, Tinc),
        PARAM_INFO(Ampli, Tdec),

        PARAM_INFO(Width, Val),

        PARAM_INFO(Pol, Val)
    }
    );
}

ModeInfo* MainWindow::getIntenseInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Intense", 0x8A, {

        PARAM_INFO(Ampli, Val),
        PARAM_INFO(Width, Val),

        PARAM_INFO(Pol, Val)

    },
    "MA control the frequency and the polarity."
    );
}

ModeInfo* MainWindow::getOffInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("OFF", 0x8F, {
        // there is no ui to manage.
    }
    );
}

ModeInfo* MainWindow::getRampInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Ramp", 0x89, {

        PARAM_INFO(Ampli, Min),
        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmin),
        PARAM_INFO(Ampli, Tmax),
        PARAM_INFO(Ampli, Tinc),
        PARAM_INFO(Ampli, Tdec),

        PARAM_INFO(Freq,  Min),
        PARAM_INFO(Freq,  Max),
        PARAM_INFO(Freq,  Tmin),
        PARAM_INFO(Freq,  Tmax),
        PARAM_INFO(Freq,  Tinc),
        PARAM_INFO(Freq,  Tdec),

        PARAM_INFO(Width, Max),
        PARAM_INFO(Width, Tmin),
        PARAM_INFO(Width, Tmax),
        PARAM_INFO(Width, Tinc),
        PARAM_INFO(Width, Tdec),

        PARAM_INFO(Pol, Val)
    },
    "MA controls Width.Min in[100..200]"
    );
}

ModeInfo* MainWindow::getRandomInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Random", 0x8E, {

        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmin),
        PARAM_INFO(Ampli, Tdec),

        PARAM_INFO(Freq,  Tmin),
        PARAM_INFO(Freq,  Tdec),

        PARAM_INFO(Width, Min),
        PARAM_INFO(Width, Max),
        PARAM_INFO(Width, Tmin),

        PARAM_INFO(Pol, Val)
    }
    );
}

ModeInfo* MainWindow::getStrokeInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Stroke", 0x8D, {

        PARAM_INFO(Ampli, Min),
        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmin),
        PARAM_INFO(Ampli, Tmax),
        PARAM_INFO(Ampli, Tinc),
        PARAM_INFO(Ampli, Tdec),

        PARAM_INFO(Freq,  Val),

        PARAM_INFO(Width, Val),

        PARAM_INFO(Pol, Val)
    },
    "When you use MA it changes Amplitude.Tmin,Tmax,Tinc,Tdec. When MA doesn't move you can control these values.\nTo disable MA set A.Amplitude.Min to > 0."
    );
}

ModeInfo* MainWindow::getThrobInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Throb", 0x80, {

        PARAM_INFO(Ampli, Min),
        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmax),
        PARAM_INFO(Ampli, Tinc),
        PARAM_INFO(Ampli, Tdec),

        PARAM_INFO(Width, Min),
        PARAM_INFO(Width, Max),
        PARAM_INFO(Width, Tmax),
        PARAM_INFO(Width, Tinc),
        PARAM_INFO(Width, Tdec),

        PARAM_INFO(Pol, Val)
    },
    "MA controls the Frequency and Amplitude.Tmin"
    );
}

ModeInfo* MainWindow::getThrustInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Thrust", 0x8C, {

        PARAM_INFO(Ampli, Min),
        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmin),
        PARAM_INFO(Ampli, Tmax),
        PARAM_INFO(Ampli, Tinc),
        PARAM_INFO(Ampli, Tdec),

        PARAM_INFO(Freq,  Val),

        PARAM_INFO(Width, Min),
        PARAM_INFO(Width, Max),
        PARAM_INFO(Width, Tmin),
        PARAM_INFO(Width, Tmax),

        PARAM_INFO(Pol, Val)
    },
    "MA controls Width.Tinc and Width.Tdec, and applies an offset to the value you set for Width.Tmin and Width.Tmax"
    );
}

ModeInfo* MainWindow::getThumpInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Thump", 0x88, {

        PARAM_INFO(Ampli, Min),
        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmax),
        PARAM_INFO(Ampli, Tinc),
        PARAM_INFO(Ampli, Tdec),

        PARAM_INFO(Freq,  Val),

        PARAM_INFO(Width, Val),

        PARAM_INFO(Pol, Val)
    },
    "MA controls Amplitude.Tmin"
    );
}

ModeInfo* MainWindow::getUserInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("User", 0x87, {
        // not implemented
    }
    );
}

ModeInfo* MainWindow::getWavesInfo(){
    static ModeInfo* info;
    if( info ) return info;
    return info = new ModeInfo("Waves", 0x8B, {

        PARAM_INFO(Ampli, Min),
        PARAM_INFO(Ampli, Max),
        PARAM_INFO(Ampli, Tmin),
        PARAM_INFO(Ampli, Tmax),

        PARAM_INFO(Freq,  Min),
        PARAM_INFO(Freq,  Max),
        PARAM_INFO(Freq,  Tmin),
        PARAM_INFO(Freq,  Tmax),

        PARAM_INFO(Width, Min),
        PARAM_INFO(Width, Max),
        PARAM_INFO(Width, Tmin),
        PARAM_INFO(Width, Tmax),

        PARAM_INFO(Pol, Val)
    }
    );
}
