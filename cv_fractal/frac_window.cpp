#include"frac_window.hpp"
#include<QLabel>
#include<QFileDialog>

//0.743643887032151,0.142625924205330 1900 100 8

 MainWindow::MainWindow() {
    setFixedSize(420, 240);
    setWindowTitle("Fractal Gen");
    QLabel *lbl_1 = new QLabel(tr("Real: "), this);
    lbl_1->setGeometry(15, 15, 50, 25);
    input_real = new QLineEdit(this);
    input_real->setGeometry(15+50+15, 15, 100, 25);
    input_real->setToolTip("Real");
    QLabel *lbl_2 = new QLabel(tr("Imag: "), this);
    lbl_2->setGeometry(15+50+15+100+15, 15, 75, 25);
    input_imag = new QLineEdit(this);
    input_imag->setGeometry(15+50+15+100+15+100, 15, 100, 25);
    input_imag->setToolTip("Imaginary");
    QLabel *lbl_3 = new QLabel(tr("Zoom: "), this);
    lbl_3->setGeometry(15, 45, 50, 25);
    input_zoom = new QLineEdit(this);
    input_zoom->setGeometry(15+50+10, 45, 100,25);
    QLabel *lbl_4 = new QLabel(tr("Iter: "), this);
    lbl_4->setGeometry(15, 45+25+10, 50, 25);
    input_iterations = new QLineEdit(this);
    input_iterations->setGeometry(15+50+10,45+25+10, 100, 25);
    QLabel *lbl_5 = new QLabel(tr("Threads: "), this);
    lbl_5->setGeometry(15,45+25+10+25+10, 50, 25);
    input_cores = new QLineEdit(this);
    input_cores->setGeometry(15+50+10, 45+25+10+25+10, 100, 25);
    input_width = new QLineEdit(this);
    input_height = new QLineEdit(this);
    input_width->setGeometry(15,25+10+45+25+10+25+10, 100, 25);
    input_height->setGeometry(15+100+10,25+10+45+25+10+25+10,100, 25);
    btn_gen = new QPushButton(tr("Generate"), this);
    btn_gen->setGeometry(15, 25+10+45+25+10+25+10+25+10, 100, 25);
    btn_save = new QPushButton(tr("Save"), this);
    btn_save->setGeometry(25+10+15+100+10, 25+10+45+25+10+25+10+25+10, 100, 25);
    
    connect(btn_gen, SIGNAL(clicked()), this, SLOT(Generate()));
    connect(btn_save, SIGNAL(clicked()), this, SLOT(Save()));

    input_real->setText("-0.743643887032151");
    input_imag->setText("0.142625924205330");
    input_zoom->setText("1900");
    input_iterations->setText("150");
    input_cores->setText("8");
    input_width->setText("1920");
    input_height->setText("1080");
    cv::namedWindow("window1");
 }

void MainWindow::Generate() {
    cv::Mat m;
    int w = input_width->text().toInt();
    int h = input_height->text().toInt();
    m.create(cv::Size(w, h), CV_8UC3);  
    double r = atof(input_real->text().toStdString().c_str());
    double i = atof(input_imag->text().toStdString().c_str());
    double z = atof(input_zoom->text().toStdString().c_str());
    int it = atoi(input_iterations->text().toStdString().c_str());
    int c = atoi(input_cores->text().toStdString().c_str());
    fractal.initParameters(r, i, z, it, c);
    fractal.resetPalette();
    fractal.draw(m);
    cv::imshow("window1", m);
    cp_ = m.clone();
    cv::waitKey(10);
}

void MainWindow::Save() {

    if(cp_.empty())
        return;

    QString filename = QFileDialog::getSaveFileName(this,tr("Save Image"), "", tr("image files (*.png *.jpg"));
    if(filename != "") {
        cv::imwrite(filename.toStdString(), cp_);
    }
}
