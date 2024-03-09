#include"cv_fractal.hpp"

int main(int argc, char **argv) {
    cv::Mat m;
    m.create(cv::Size(1080, 1080), CV_8UC3);
    cv_fract::CV_Fractal cvfrac;
    cvfrac.initParameters(0-0.743643887032151,0.142625924205330,1900,100,8);
    cvfrac.draw(m);
    cv::imwrite("output.png", m);
    return 0;
}