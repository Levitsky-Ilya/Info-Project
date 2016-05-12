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
    float slevel;
    QMessageBox *mesBox;
    void getLyfile();
    void getPdffile();
    void openPdffile();

public slots:
     void changeText(QString str);
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
