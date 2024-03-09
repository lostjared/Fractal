#ifndef _FRAC_WINDOW_HPP_
#define _FRAC_WINDOW_HPP_

#include<QMainWindow>
#include<QLineEdit>
#include<QPushButton>
#include"cv_fractal.hpp"


class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();
    QLineEdit *input_real, *input_imag, *input_zoom, *input_iterations, *input_cores, *input_width, *input_height;
    QPushButton *btn_gen, *btn_save;
public slots:
    void Generate();
    void Save();
private:
    cv_fract::CV_Fractal fractal;
    cv::Mat cp_;
};


#endif