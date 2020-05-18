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

#include "serial_ports_info.h"
#include "select_serial_port.h"
#include "serial_worker.h"
#include "configuration.h"
#include "options.h"

#include "version.h"

#include <QTimer>

memInfo etMem[256];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle(APP_NAME " - " APP_VERSION);

    // setup the graphic area
    scene = new QGraphicsScene(0, 0, 4096, 256);
    view = ui->graphicsView;
    view->setScene(scene);

    ui->tabWidget->setCurrentIndex(0);

    bool showGraph = false;
    if( opt.restoreGeometry() ){
        restoreGeometry(opt.geometry());
        showGraph = opt.graphVisibility();
    }
    ui->groupBox_Graph->setVisible(showGraph);
    ui->actionShow_graph->setChecked(showGraph);

    allModesFileName    = opt.allModesPath();
    currentModeFileName = opt.currentModePath();

    // fire the splash screen
    QTimer::singleShot(10, this, SLOT(agreement()));

    thread = new QThread;
    worker = new SerialWorker();
    worker->moveToThread(thread);

    // signaux de gestion du thread
    connect(thread, SIGNAL(started()),     worker, SLOT(doStart()));
    connect(thread, SIGNAL(finished()),    worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()),    thread, SLOT(deleteLater()));
    // connect(this,   SIGNAL(aboutToQuit()), thread, SLOT(quit));

    // signaux emis par mainwindows
    connect(this, SIGNAL(requestInit(QString)), worker, SLOT(doInit(QString)));
    connect(this, SIGNAL(requestFlush()),       worker, SLOT(doFlush()));
    connect(this, SIGNAL(requestSynch()),       worker, SLOT(doSynch()));
    connect(this, SIGNAL(requestPing()),        worker, SLOT(doPing()));
    connect(this, SIGNAL(requestPeek(int, int)),worker, SLOT(doPeek(int,int)));
    connect(this, SIGNAL(requestPoke(int,int)), worker, SLOT(doPoke(int,int)));

    // traitement des signaux emis par SerialWorker
    connect(worker, SIGNAL(sendTimeOut(QString)),     this, SLOT(processTimeOut(QString)));
    connect(worker, SIGNAL(sendStatus(int, QString)), this, SLOT(processStatus(int, QString)));
    connect(worker, SIGNAL(sendResult(int, QString)), this, SLOT(processResult(int, QString)));
    connect(worker, SIGNAL(sendPeekResult(int, int)), this, SLOT(processPeekResult(int, int)));

    thread->start();

    // traitement des signaux du timer pour rafraichier les sliders
    refreshSlidersTimer = new QTimer(this);
    connect(refreshSlidersTimer, SIGNAL(timeout()), this, SLOT(processRefreshSlidersTimer()));

    // traitement des signaux du timer pour rafraichier le graphique
    grTimer = new QTimer(this);
    connect(grTimer, SIGNAL(timeout()), this, SLOT(processGrTimer()));

    grTimer->start(10);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    opt.setGeometry(saveGeometry());
    opt.save();

    // TODO warn the user to set its box in quite mode
    raz();
    poke(ET::MODE,   0x0F);
    poke(ET::ADCTRL, 0x00);
    QThread::msleep(333);   // time to execute
    event->accept();
}


MainWindow::~MainWindow()
{
    thread->quit();
    thread->wait();
    delete thread;
    delete ui;
}

void MainWindow::agreement(){
    if( displayAbout(QMessageBox::Ok | QMessageBox::Abort) == QMessageBox::Abort )
        close();
}


// --------------------------------------------- //
// Communication with ET232                      //
// --------------------------------------------- //
#define A2T(a) case a: return #a;

const char* MainWindow::addr2text(int addr){
    switch(addr){
      A2T(A::Width::Val); A2T(A::Freq::Val); A2T(A::Ampli::Val);  A2T(A::Pol::Val);
      A2T(A::Width::Min); A2T(A::Width::Max); A2T(A::Width::Tmin); A2T(A::Width::Tmax); A2T(A::Width::Tinc); A2T(A::Width::Tdec);
      A2T(A::Freq::Min); A2T(A::Freq::Max); A2T(A::Freq::Tmin); A2T(A::Freq::Tmax); A2T(A::Freq::Tinc); A2T(A::Freq::Tdec);
      A2T(A::Ampli::Min); A2T(A::Ampli::Max); A2T(A::Ampli::Tmin); A2T(A::Ampli::Tmax); A2T(A::Ampli::Tinc); A2T(A::Ampli::Tdec);

      A2T(B::Width::Val); A2T(B::Freq::Val); A2T(B::Ampli::Val);  A2T(B::Pol::Val);
      A2T(B::Width::Min); A2T(B::Width::Max); A2T(B::Width::Tmin); A2T(B::Width::Tmax); A2T(B::Width::Tinc); A2T(B::Width::Tdec);
      A2T(B::Freq::Min); A2T(B::Freq::Max); A2T(B::Freq::Tmin); A2T(B::Freq::Tmax); A2T(B::Freq::Tinc); A2T(B::Freq::Tdec);
      A2T(B::Ampli::Min); A2T(B::Ampli::Max); A2T(B::Ampli::Tmin); A2T(B::Ampli::Tmax); A2T(B::Ampli::Tinc); A2T(B::Ampli::Tdec);

      A2T(KNOB::A);  A2T(KNOB::MA); A2T(KNOB::B);
      A2T(ET::MODE); A2T(ET::ADCTRL);

      // Is it safe to return this pointer to a basic type ??? Local data ???
      default: return QString("0x%1").arg(addr,2,16,QLatin1Char('0')).toLocal8Bit().data();
    }
}

void MainWindow::processTimeOut(QString msg){
    qDebug() << "Timeout:" << msg;
    ui->statusBar->showMessage("Connection with ET232 has been lost: time out.");
    ui->statusBar->setStyleSheet("color:red");
}

void MainWindow::processResult(int code, QString msg){
    qDebug() << "result:" << code << msg;
    ui->statusBar->showMessage(msg);
    ui->statusBar->setStyleSheet("color:red");
}

void MainWindow::processPeekResult(int addr, int value){
    qDebug() << "process peek(" << addr2text(addr) << ") ->" << value;
    memInfo* const mi = &etMem[addr & 0xFF];
    mi->value   = value & 0xFF;
    mi->status &= ~VALUE_REQUESTED_1;
    mi->status |=  VALUE_SET;
    peekCount ++;
}

void MainWindow::poke(int addr, int value){
    emit requestPoke(addr, value);
    qDebug() << "Request poke(" << addr2text(addr) << "," << value << ")";
}

void MainWindow::peek(int addr, int defaultValue){
    emit requestPeek(addr, defaultValue);
    qDebug() << "Request peek(" << addr2text(addr) << ")";
}

void MainWindow::raz(){
    poke(KNOB::A,  0);
    poke(KNOB::B,  0);
    poke(KNOB::MA, 0);
}


// --------------------------------------------- //
// Application Menu                              //
// --------------------------------------------- //

void MainWindow::on_actionSynchronize_triggered(){
    SelectSerialPort dialog;
    dialog.setModal(true);
    const int dialogCode = dialog.exec();
    if( dialogCode == QDialog::Accepted ){
        ui->statusBar->showMessage(QString("Start Synchronization with ET232 on port %1").arg(dialog.portShortName));
        ui->statusBar->setStyleSheet("color:black");
        emit requestInit(dialog.portShortName); // see mainwindow_synchro.cpp
    }else{
        ui->statusBar->showMessage("Synchronization FAILED");
        ui->statusBar->setStyleSheet("color:red");
    }
}

void MainWindow::on_actionAbout_triggered(){
    displayAbout(QMessageBox::Close);
}

int MainWindow::displayAbout(QMessageBox::StandardButtons buttons){
    QString msg = QString(
        "<img src=\"qrc:///resources/logo.png\" align=\"left\" width=\"200\" height=\"100\"/>"
        "<center>"
        "<h1>" APP_NAME "</h1>"
        "Version: %1.%2 / %3.%4<br/>by " ORG_NAME "</p>"
        "</center>"
        "<p>This software is <b>not</b> developed, <b>not</b> maintained, "
        "<b>not</b> supported and <b>not</b> endorsed by ErosTek.</p>"
        "<p>THIS SOFTWARE IS PROVIDED <i>AS IS</i>, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, "
        "INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR "
        "PURPOSE AND NONINFRINGEMENT.<p>"
         "<p>IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR "
         "OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF "
         "OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.</p>"
    ).arg(APP_VERSION_MAJOR).arg(APP_VERSION_MINOR).arg(APP_REVISION_MAJOR).arg(APP_REVISION_MINOR);

    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle("About " APP_NAME);
    msgBox->setText(msg);
    msgBox->setStandardButtons(buttons);
    return msgBox->exec();
}

void MainWindow::on_actionSerial_ports_info_triggered()
{
    SerialPortsInfo dialog;
    dialog.setModal(true);
    dialog.exec();
}

void MainWindow::on_actionConfiguration_triggered()
{
    Configuration dialog(this);
    dialog.setModal(true);
    dialog.exec();
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::synchRequired(){
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(APP_NAME ": Synchronization Required");
    msgBox->setText("ET232 not synchronized. Resultat will be emulated");
    msgBox->setStandardButtons(QMessageBox::Close);
    msgBox->exec();
}

void MainWindow::on_actionShow_graph_triggered(bool checked)
{
    ui->groupBox_Graph->setVisible(checked);
    opt.setGraphVisibility(checked);
}

void MainWindow::on_actionHelp_triggered()
{
    qDebug() << "Help..."; // TBD ;-)
}

// ------------------------------------------------- //
// The rest of mainwindow implementation is spread   //
// in several files: mainwindow.h + mainwindow_*.cpp //
// ------------------------------------------------- //
