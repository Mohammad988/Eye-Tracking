#include "renderthread.h"
#include <QDebug>
#include <QVector>

RenderThread::RenderThread(QObject *parent)
    : QThread(parent)
{
    restart = false;
    abort = false;

    startTimer(40);
}

RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void RenderThread::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    StartThread();
}

void RenderThread::StartThread()
{
    QMutexLocker locker(&mutex);

    if (!isRunning()) {
        start(HighPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}

void RenderThread::renderConsole(QTextEdit *ptrConsole)
{
    Console = ptrConsole;
    Console->append(">> Console is Rendered!!");
}

void RenderThread::retainCamera()
{
    if(Cam != NULL){
        // Stoping camera
        Cam->Stop();
        Console->append(">> Camera is Stopped!");

        // Releasing pointers
        Cam->Release();
        CamRendered = false;
    }
}

bool RenderThread::saveImage()
{
    uint imWidth = 640;
    uint imHeight = 480;
    QSize frameSize(imWidth,imHeight);
    QImage image(frameSize,QImage::Format_Grayscale8);
    qsrand(QTime::currentTime().msec());

    uchar *scanLine = image.scanLine(0);
    if (CamRendered)
    {
        Frame *frame = Cam->GetLatestFrame();
        if (frame)
        {
            // Rasterize frame
            frame->Rasterize(imWidth,imHeight,640,8,scanLine);
            image = image.mirrored(true,false);

            // Release frame
            frame->Release();
        }
    }
    else
    {
        for (uint i = 0; i < imHeight * imWidth; ++i)
            *scanLine++ = (qrand() > RAND_MAX/2) * 255;
    }

    /****** Save Image ******/
    return image.save("../test.png","PNG",100);
}

void RenderThread::renderCamera(Camera *ptrCamera)
{
    Cam = ptrCamera;
    if(Cam != NULL)
    {
        Console->append(">> Camera is Rendered!!");
        CamRendered = true;
    }
    else
        Console->append(">> Camera is not Rendered Properly!!");
}

void RenderThread::run()
{

    if (restart || abort)
        return;

    uint imWidth = 640;
    uint imHeight = 480;
    QSize frameSize(imWidth,imHeight);
    QImage image(frameSize,QImage::Format_Grayscale8);
    qsrand(QTime::currentTime().msec());

    QSize frameSize2(240,120);
    QImage image2(frameSize2,QImage::Format_Grayscale8);

    uchar *scanLine = image.scanLine(0);
    if (CamRendered)
    {
        Frame *frame = Cam->GetLatestFrame();
        if (frame)
        {
            // Rasterize frame
            frame->Rasterize(imWidth,imHeight,640,8,scanLine);
            image = image.mirrored(true,false);

            /****** Process ******/
            image2 = image.copy(200,200,240,120);
            uchar *scanLine2 = image2.scanLine(0);
            QVector<uint> GrayLevel(256,0);
            uint *hist = GrayLevel.data();
            for (int i = 200; i < 440; ++i)
                for (int j = 200; j < 320; ++j)
                    hist[*scanLine2++]++;

            QVector<uint> CumulativeDist(256,0);
            uint *cumsum = CumulativeDist.data();
            cumsum[0] = hist[0];
            for (int i = 1; i < 256; ++i)
                    cumsum[i] = cumsum[i-1] + hist[i];
            /****** Process ******/

            // Release frame
            frame->Release();
        }
    }
    else
    {
        for (uint i = 0; i < imHeight * imWidth; ++i)
            *scanLine++ = (qrand() > RAND_MAX/2) * 255;
    }

    if (!restart)
        emit ReadyToDisplay(image);

    mutex.lock();
    if (!restart)
        condition.wait(&mutex);
    restart = false;
    mutex.unlock();
}

