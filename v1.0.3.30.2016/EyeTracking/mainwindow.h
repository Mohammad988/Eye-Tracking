#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include "renderthread.h"

#include <cameralibrary.h>
using namespace CameraLibrary;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool CamInit();
    void CamDefaultSettings();
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updatePixmap(const QImage &image);

    void on_actionStart_triggered();
    void on_actionShut_Down_triggered();
    void on_actionClose_triggered();
    void on_actionStop_triggered();
    void on_actionSave_Frame_As_an_Image_triggered();

    void on_ExposureSlider_valueChanged(int value);
    void on_ThresholdSlider_valueChanged(int value);
    void on_IntensitySlider_valueChanged(int value);

    void on_CameraModesCBox_currentIndexChanged(int index);

    void on_AEC_clicked(bool checked);
    void on_AGC_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    RenderThread thread;
    QPixmap pixmap;
    Camera *Cam;
};

#endif // MAINWINDOW_H
