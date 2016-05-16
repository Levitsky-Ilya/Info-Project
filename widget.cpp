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
#include <cstring>
#include <string>

#define silenceDefault 50
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->levelSlider->setValue(50);
    ui->levelspinBox->setValue(50);
    ui->levelSlider->setMaximum(100);
    ui->levelspinBox->setMaximum(100);
    QObject::connect(ui->browseButton, SIGNAL(clicked()),this, SLOT(openWav()));
    QObject::connect(ui->startButton, SIGNAL(clicked()),this, SLOT(transform()));
    QObject::connect(ui->browselyButton, SIGNAL(clicked()),this, SLOT(statePath()));
    QObject::connect(ui->lypathEdit, SIGNAL(textChanged(QString)), this, SLOT(changeText(QString)));
    QObject::connect(ui->wavpathEdit, SIGNAL(textChanged(QString)), this, SLOT(changeText(QString)));
    QObject::connect(ui->levelSlider, SIGNAL(valueChanged(int)), this, SLOT (changeLevel(int)));
    QObject::connect(ui->levelspinBox, SIGNAL(valueChanged(int)), this, SLOT (changeLevel(int)));
    QObject::connect(ui->levelspinBox, SIGNAL(valueChanged(int)), ui->levelSlider, SLOT(setValue(int)));
    QObject::connect(ui->levelSlider, SIGNAL(valueChanged(int)), ui->levelspinBox, SLOT(setValue(int)));
    QObject::connect(ui->sdefButton, SIGNAL(clicked()), this, SLOT(setdefLevel()));

    ui->srcLabel->setToolTip("Choose wav-file you want to transform");


    //ui->midnoiseBox->setChecked(true);
   // bool a = true;
   // QObject::connect(ui->midnoiseBox,SIGNAL(clicked()),this,SLOT(doSAS()));
    mesBox =  new QMessageBox();
    errmesBox = new QMessageBox();
   // mesBox->setText("OK");
    //qDebug()<<file_in<<file
    if (file_ly.isEmpty() || file_wav.isEmpty()){
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


void Widget::changeLevel(int part)
{
    slevel = part*0.01;
}

void Widget::setdefLevel()
{
    ui->levelSlider->setValue(silenceDefault);
}

void Widget::changeText(QString str)
{
   // ui->startButton->setEnabled(!str.isEmpty());
    if (!file_dir.isEmpty() && !file_wav.isEmpty()){
        ui->startButton->setDisabled(false);
    }

}


void Widget::openWav()
{

    file_wav = QFileDialog::getOpenFileName(this, tr("Open File"), "/home",
            tr("wav-files(*.wav)"));
    ui->wavpathEdit->setText(file_wav);
    qDebug()<<file_wav;
}

void Widget::statePath()
{
    //file_ly = QFileDialog::get2
      //      getOpenFileName(this, tr("Open File"), "C:/Qt/test_ui",tr("All files (*.*)"));

   // file_ly = ui->lypathEdit->text();
    file_dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home" ,
                                                QFileDialog::ShowDirsOnly);
    ui->lypathEdit->setText(file_dir);
    qDebug() << file_dir;

    QString ly_name;
    QString last;
    int last_index = 0;
    ly_name = file_wav;
   // qDebug()<< ly_name;
    last = "/";
    last_index = ly_name.lastIndexOf(last); //find position of last slash -filename start

    ly_name.remove(0, last_index); //remove all before file name

    ly_name.chop(3); //remove wav
    file_name = ly_name;
    file_name.chop(1);
    file_name.remove(0, 1); //get only name
    ly_name.append("ly"); // add ly
    file_ly = file_dir;
    qDebug() << file_ly;
    file_ly.append(ly_name); //get path+filename .ly
    qDebug()<<file_ly;

}


void Widget::getPdffile()
{
   system(file_ly.toLocal8Bit());


}


void Widget::openPdffile()
{
   file_pdf = file_ly;
   file_pdf.chop(2);  //other letters appear ((((((
   file_pdf.append("pdf");
   system(file_pdf.toLocal8Bit());
}


void Widget::getLyfile()
{
    Notes notes;
        try {
            notes.initialize(file_wav.toStdString());
        }
        catch (runtime_error & e) {
            cout << e.what() << endl;
            errmesBox->setText(e.what());
           // errmesBox->setIcon(QMessageBox::Abort);
            errmesBox->setIcon(QMessageBox::Critical);
            errmesBox->setWindowTitle("ERROR");
            qDebug() << "error ypopt";
            int error = errmesBox->exec();

             //if (errmesBox->buttonClicked(true))
            exit(EXIT_FAILURE);

        }

        vector<Note> noteVect;
        notes.setSilenceLevel(0.5);
        notes.generateMidView(noteVect);

        Queue noteVectL(noteVect, true);
        Queue noteVectN(noteVect, false);

        noteVectN.breaker();
        noteVectL.breaker();

        ofstream file(file_ly.toStdString());

        file << "\\header {title = " << '"' << file_name.toStdString() << '"' << "}\n";
        file << "\\score { \n";
        file << "\\new PianoStaff << \n";

        if (noteVectN.size() != 0) {
            file << "\\new Staff { \n";
            noteVectN.drawStaff(noteVectL, file);
        }

        if (noteVectL.size() != 0) {
            file << "\\new Staff { \n";
            file << "\\clef \"bass\" \n";
            noteVectL.drawStaff(noteVectN, file);
        }

        file << ">> \n";
        file << "} \n";

        file.close();

        cout << "Assemble complete" << endl;

}

void Widget::transform()
{

    getLyfile();
    getPdffile();
//    mesBox->show();
    mesBox->setText("Done!\n Your PDF is in the selected folder. ");
    mesBox->setIcon(QMessageBox::Information);
    mesBox->setWindowTitle("SUCCESS!");
    int show = mesBox->exec();
   // openPdffile();


}


