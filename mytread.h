#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QMutex>
#include <QFile>
#include <QThread>
#include <QString>


//int g_counter = 0;

class MyThread : public QThread
{
    QString m_file_in, m_file_out;
    int startP;
    int endP;
    QFile in_;
    QFile out_;
    int part_size;
public:
    MyThread(QString file_in, QString file_out, int thread_limit, int start, int finish);
    ~MyThread();
     void run();

};

#endif // MYTHREAD_H
