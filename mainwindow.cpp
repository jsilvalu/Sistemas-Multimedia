#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <stdio.h>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cap = new VideoCapture(0);
    winSelected = false;

    colorImage.create(240,320,CV_8UC3);
    grayImage.create(240,320,CV_8UC1);
    destColorImage.create(240,320,CV_8UC3);
    destGrayImage.create(240,320,CV_8UC1);

    visorS = new ImgViewer(&grayImage, ui->imageFrameS);

    ui->procesos->setText("Programa iniciado...\n");
    x=0;
    y=0;
    TRACKINGactivated=false;
    DEMOGactivated = false;

    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(visorS,SIGNAL(windowSelected(QPointF, int, int)),this,SLOT(selectWindow(QPointF, int, int)));
    connect(visorS,SIGNAL(pressEvent()),this,SLOT(deselectWindow()));

    connect(wri.CLOSE,SIGNAL(clicked()),&wri,SLOT(close()));
    timer.start(30);
}

MainWindow::~MainWindow(){
    delete ui;
    delete cap;
    delete visorS;

    colorImage.release();
    grayImage.release();
    destColorImage.release();
    destGrayImage.release();
}

void MainWindow::compute(){

    //Captura de imagen
    if(ui->captureButton->isChecked() && cap->isOpened())
    {
        *cap >> colorImage;
        cv::resize(colorImage, colorImage, Size(320,240));
        cvtColor(colorImage, grayImage, COLOR_BGR2GRAY);
        cvtColor(colorImage, colorImage, COLOR_BGR2RGB);

    }

    if(TRACKINGactivated)
        on_TRACKING_clicked();

cout<<"TRACKING: "<<TRACKINGactivated<<endl;
    //Actualización de los visores

    if(winSelected)
        visorS->drawSquare(QRect(imageWindow.x, imageWindow.y, imageWindow.width,imageWindow.height), Qt::green );

    visorS->update();

    if(wri.isVisible())
        visorR->update();
    else{
        TRACKINGactivated=false;
        DEMOGactivated = false;
    }

}

void MainWindow::start_stop_capture(bool start)
{
    if(start)   ui->captureButton->setText("Stop capture");
    else        ui->captureButton->setText("Start capture");
}

void MainWindow::change_color_gray(bool color){
    if(color){
        ui->colorButton->setText("Gray image");
        visorS->setImage(&colorImage);
    }else{
        ui->colorButton->setText("Color image");
        visorS->setImage(&grayImage);
    }
}

void MainWindow::selectWindow(QPointF p, int w, int h){
    QPointF pEnd;
    if(w>0 && h>0)
    {
        imageWindow.x = p.x()-w/2;
        if(imageWindow.x<0)
            imageWindow.x = 0;
        imageWindow.y = p.y()-h/2;
        if(imageWindow.y<0)
            imageWindow.y = 0;
        pEnd.setX(p.x()+w/2);
        if(pEnd.x()>=320)
            pEnd.setX(319);
        pEnd.setY(p.y()+h/2);
        if(pEnd.y()>=240)
            pEnd.setY(239);
        imageWindow.width = pEnd.x()-imageWindow.x+1;
        imageWindow.height = pEnd.y()-imageWindow.y+1;

        winSelected = true;
    }
}

void MainWindow::deselectWindow(){
    winSelected = false;
}


//==============================================================================================================
//==============================================================================================================
//==============================================================================================================

void MainWindow::on_CargarPlano_clicked(){

    //QString fileName = QFileDialog::getOpenFileName(this, tr("Seleccione una IMAGEN"), "", tr("All Files (*)"));

    Mat image;

    if(sala == "Sala Beta")
        image =imread("BETA.png");
    else if (sala == "Sala Reuniones")
        image =imread("REUNIONES.png");

    imageOriginal = image;  //Guardo copia de la imagen original (ampliacion)
    ANCHOoriginal = imageOriginal.cols;
    ALTOoriginal  = imageOriginal.rows;   //Guardo copia de resolucion original (alto)

    cout<<"Se ha cargado un imagen con dimensiones - ANCHO: "<<ANCHOoriginal<<" ALTO: "<<ALTOoriginal<<endl;
    if(sala == "Sala Beta")
        ui->procesos->setText("Se ha cargado el plano de sala BETA");
     else if (sala == "Sala Reuniones")
        ui->procesos->setText("Se ha cargado el plano de sala REUNIONES");

    wri.show();
    wri.setMinimumWidth(ANCHOoriginal+80+80);  //Selecciono el alto mínima de la interfaz - alto + 80 botón OK + 80
    wri.setMinimumHeight(ALTOoriginal+80+80);//Selecciono el ancho mínimo de la interfaz - ancho + 80 botón OK + 80

    wri.imageFrameR->setGeometry(80,80, ANCHOoriginal,ALTOoriginal);  //tamaño y posición de imagen

    imageOriginal.create(ALTOoriginal, ANCHOoriginal,CV_8UC3);
    //cvtColor(imageOriginal, imageOriginal, COLOR_BGR2GRAY);

    visorR = new ImgViewer(&imageOriginal, wri.imageFrameR);

}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    sala = arg1;
}

void MainWindow::on_TRACKING_clicked()
{
    TRACKINGactivated=true;
    ui->procesos->setText("Ejecutando TRACKING...");

    x= 233;
    y= 340;

    //En este punto deben actualizarse las coordenadas recibidas del exterior y asignarlas a las variables x  e y
    if(x < ANCHOoriginal && x > 0 && y > 0 && y <ALTOoriginal){
            visorR->drawEllipse(QPoint(x, y),30,30,Qt::red, true);

    }


}

void MainWindow::on_DEMO_clicked()
{
    DEMOGactivated=true;
    ui->procesos->setText("Ejecutando DEMO...");
    cargarListaDemo();
    for (Point p : listaDEMO){
        visorR->drawEllipse(QPoint(p.x, p.y),10,10,Qt::red, true);
    }

}

void MainWindow::cargarListaDemo(){

    Point p;
    int coorX=0;
    int coorY=120;

    for(coorX=0; coorX < ANCHOoriginal-100; coorX++){
        p.x = coorX;
        p.y = coorY;
        listaDEMO.push_back(p);

    }

}
