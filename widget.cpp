#include "widget.h"
#include "ui_widget.h"
#include "QMessageBox"
#include <QDebug>
#include <QTimeLine>
#include <notes.h>
#include <wav_sound.h>
#include <frequencies_for_notes.h>
#include <fft.h>
#include <complex.h>
#include <exception.h>
#include <freq_names.h>
#include <note_pauses.h>
#include <global.h>
#include <queue.h>
#include <QMessageBox>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QObject::connect(ui->browseButton, SIGNAL(clicked()),this, SLOT(openWav()));
    QObject::connect(ui->startButton, SIGNAL(clicked()),this, SLOT(transform()));
    QObject::connect(ui->selectButton, SIGNAL(clicked()),this, SLOT(statePath()));
    QObject::connect(ui->lypathEdit, SIGNAL(textChanged(QString)), this, SLOT(changeText(QString)));


    ui->midnoiseBox->setChecked(true);
   // bool a = true;
    QObject::connect(ui->midnoiseBox,SIGNAL(clicked()),this,SLOT(doSAS()));
    mesBox =  new QMessageBox();
    mesBox->setText("OK");
    //qDebug()<<file_in<<file
    if (file_ly.isEmpty()){
        ui->selectButton->setDisabled(true);
        ui->startButton->setDisabled(true);
    }

}

Widget::~Widget()
{
    delete ui;
}

/*
void Widget::doSereAS()
{
    qDebug()<<"qqq";
}
*/

void Widget::changeText(QString str)
{

    ui->selectButton->setEnabled(!str.isEmpty());
    ui->startButton->setEnabled(!str.isEmpty());
//    if(!str.isEmpty())
//    ui->selectButton->setEnabled(true);
}


void Widget::openWav()
{

    file_wav = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/Qt/test_ui",
            tr("wav-files(*.wav)"));
    ui->wavpathEdit->setText(file_wav);
    qDebug()<<file_wav;
}

void Widget::statePath()
{
    //file_ly = QFileDialog::get2
      //      getOpenFileName(this, tr("Open File"), "C:/Qt/test_ui",tr("All files (*.*)"));

    file_ly = ui->lypathEdit->text();

//    ui->lypathEdit->setText(file_ly);
    qDebug()<<file_ly;

}
/*
void Widget::save()
{
    file_out = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                              tr("All files (*.*)"));
    ui->lineEdit_2->setText(file_out);

}
*/


void Widget::getPdffile()
{
   system(file_ly.toLocal8Bit());
}

/*
void Widget::openPdffile()
{
   system(file_ly.toLocal8Bit());
}
*/

void Widget::getLyfile()
{
    Notes notes;
    try {
        notes.initialize(file_wav.toStdString());
    }
    catch (Exception & e) {
        cout << e.getErrorMessage() << endl;
        //msg BOX!!
    }

    vector<Note> noteVect;
    notes.generateMidView(noteVect);

    Queue noteVectL(noteVect, true);
    Queue noteVectN(noteVect, false);

    noteVectN.breaker();
    noteVectL.breaker();
    //ofstream file("C:/Qt/test_ui/second.ly");
    ofstream file(file_ly.toStdString());

    file << "normal = \\new Staff { \n";

    noteVectN.drawStaff(noteVectL, file);

    file << "bass = \\new Staff { \n";
    file << "\\clef \"bass\" \n";

    noteVectL.drawStaff(noteVectN, file);

    file << "{\n";
    file << "\\new PianoStaff << \n";
    file << "\\normal \n";
    file << "\\bass \n";
    file << ">> \n";
    file << "} \n";

    file.close();





}

void Widget::transform()
{

    getLyfile();
    getPdffile();
//    mesBox->show();
    mesBox->setText("DONE!\n Your PDF is in the selected folder. ");
    mesBox->setIcon(QMessageBox::Information);
    mesBox->setWindowTitle("SUCCESS!");
    int n = mesBox->exec();


}


