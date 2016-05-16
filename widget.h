#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QString>
#include <QMessageBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void check();

private:
    Ui::Widget *ui;
    QString file_wav;
    QString file_ly = "";
    QString file_pdf;
    QString file_name;
    QString file_dir;
    float slevel = 0.5;
    QMessageBox *mesBox;
    QMessageBox *errmesBox;
    void getLyfile();
    void getPdffile();
    void openPdffile();
    bool no_error = true;


public slots:
     void changeText(QString);
     void transform();
     void openWav();
     void statePath();
     void changeLevel(int part);
     void setdefLevel();



};

/*
class str : public QString
{
    Q_OBJECT

 private:
    QString str;
public slots:

};
*/

#endif // WIDGET_H
