#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionShut_Down->setDisabled(true);
    ui->actionStop->setDisabled(true);

    connect(&thread, SIGNAL(ReadyToDisplay(QImage)), this, SLOT(updatePixmap(QImage)));

    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    ui->Console->setPalette(p);

    thread.renderConsole(ui->Console);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (pixmap.isNull()) {
        painter.fillRect(rect(), Qt::black);
        painter.setPen(Qt::green);
        painter.drawText(rect(), Qt::AlignCenter, tr("Rendering image, please wait..."));
        return;
    }

    QPoint pixmapOffset(20,60);
    painter.drawPixmap(pixmapOffset,pixmap);

    QRect rect = QRect(200,200,240,120);
    painter.setPen(Qt::green);
    painter.drawRect(rect);

}

void MainWindow::updatePixmap(const QImage &image)
{
    pixmap = QPixmap::fromImage(image);
    //update();
    repaint();
}

void MainWindow::on_actionStart_triggered()
{
    while (!CameraManager::X().AreCamerasInitialized());

    if(CamInit())
        ui->Console->append(">> Camera is Detected and Initialized!");

    ui->actionStart->setDisabled(true);
    ui->actionStop->setEnabled(true);
    ui->actionShut_Down->setEnabled(true);

    CamDefaultSettings();
    Cam->Start();
    ui->Console->append(">> Camera Started!");

    thread.renderCamera(Cam);
}

bool MainWindow::CamInit()
{
    bool retVal = false;

    Cam = CameraManager::X().GetCamera();
    if(Cam == NULL)
        ui->Console->append(">> Failed to create a camera object!");
    else
    {
        ui->Console->append(">> Camera object created successfully!");
        QString ptrStr = QString(">> cam addr: 0x%1").arg((quintptr)Cam, QT_POINTER_SIZE * 2, 16, QChar('0'));
        ui->Console->append(ptrStr);
        retVal = true;
    }

    return retVal;
}
void MainWindow::on_actionShut_Down_triggered()
{
    if(Cam != NULL){
        // Stoping camera
        Cam->Stop();
        ui->Console->append(">> Camera is Stopped!");

        // Releasing pointers
        Cam->Release();
    }

    // Properly shuting down the hardware
    CameraManager::X().Shutdown();
    ui->Console->append(">> Camera is Shut Down!");

    ui->actionStart->setEnabled(true);
    ui->actionShut_Down->setDisabled(true);
}

void MainWindow::on_actionClose_triggered()
{
    MainWindow::~MainWindow();
}

void MainWindow::CamDefaultSettings()
{
    Cam->SetVideoType(Core::GrayscaleMode);
    ui->CameraModesCBox->setCurrentIndex(4);

    int DefaultExposure = 50;
    Cam->SetExposure(DefaultExposure);
    ui->ExposureSlider->setValue(DefaultExposure);

    int DefaultThreshold = 0;
    Cam->SetThreshold(DefaultThreshold);
    ui->ThresholdSlider->setValue(DefaultThreshold);

    int DefaultIntensity = 2;
    Cam->SetIntensity(DefaultIntensity);
    ui->IntensitySlider->setValue(DefaultIntensity);
}

void MainWindow::on_ExposureSlider_valueChanged(int value)
{
    Cam->SetExposure(value);
}

void MainWindow::on_ThresholdSlider_valueChanged(int value)
{
    Cam->SetThreshold(value);
}

void MainWindow::on_IntensitySlider_valueChanged(int value)
{
    Cam->SetIntensity(value);
}

void MainWindow::on_CameraModesCBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
/*    switch (index)
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        Cam->SetVideoType(Core::GrayscaleMode);
        break;
    case 5:
        break;
    default:
        break;
*/
}

void MainWindow::on_actionStop_triggered()
{
    thread.retainCamera();
    ui->actionStart->setEnabled(true);
    ui->actionStop->setDisabled(true);
}

void MainWindow::on_actionSave_Frame_As_an_Image_triggered()
{
    bool b = thread.saveImage();
    if(b)
        ui->Console->append(">> Image is Saved!!");
    else
        ui->Console->append(">> Couldn't Save the Image!!");
}

void MainWindow::on_AEC_clicked(bool checked)
{
    if(Cam)
        Cam->SetAEC(checked);
}

void MainWindow::on_AGC_clicked(bool checked)
{
    if(Cam)
        Cam->SetAGC(checked);
}
