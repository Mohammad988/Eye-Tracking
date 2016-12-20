#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QWidget>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QTime>
#include <QTextEdit>

#include <cameralibrary.h>
using namespace CameraLibrary;

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

class RenderThread : public QThread
{
    Q_OBJECT

public:
    RenderThread(QObject *parent = 0);
    ~RenderThread();

    void StartThread();
    void renderCamera(Camera *ptrCamera);
    void renderConsole(QTextEdit *ptrConsole);
    void retainCamera();
    bool saveImage();

signals:
    void ReadyToProcess(const QImage &image);
    void ReadyToDisplay(const QImage &image);

private slots:
    void timerEvent(QTimerEvent *event);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    QMutex mutex;
    QWaitCondition condition;
    bool restart;
    bool abort;

    QTextEdit *Console;
    Camera *Cam;
    bool CamRendered = false;
};

#endif // RENDERTHREAD_H
