#include<QApplication>
#include"frac_window.hpp"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow window1;
    window1.show();
    app.exec();
    return 0;
}