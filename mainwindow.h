#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <ui_windowsresize.h>
#include <imgviewer.h>


#include <QtWidgets/QFileDialog>



using namespace cv;

namespace Ui {
    class MainWindow;
}
//--------------------------------------------------------------------------Interfaz para planos----
class WindowResizeInterface : public QDialog, public Ui::WindowResizeInterface
{
    Q_OBJECT

public:
    WindowResizeInterface(QDialog *parent=0) : QDialog(parent){
        setupUi(this);
    }
};
//--------------------------------------------------------------------------Interfaz para planos----
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;

    int x, y;
    bool TRACKINGactivated;
    bool DEMOGactivated;
    VideoCapture *cap;
    ImgViewer *visorS, *visorR;
    Mat colorImage, grayImage, imageOriginal,imageR;
    Mat destColorImage, destGrayImage;
    bool winSelected;
    Rect imageWindow;
    String procesosEjecutados;
    WindowResizeInterface wri;   //Creacion instancia para Resize
    int ALTOoriginal=0, ANCHOoriginal=0;
    QString sala;
    std::vector<Point> listaDEMO;

public slots:
    void compute();
    void start_stop_capture(bool start);
    void change_color_gray(bool color);
    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();

private slots:
    void on_CargarPlano_clicked();
    void on_comboBox_activated(const QString &arg1);
    void on_TRACKING_clicked();
    void on_DEMO_clicked();
    void cargarListaDemo();
};


#endif // MAINWINDOW_H
