#include"cv_fractal.hpp"
#include<string>


class splitException {};

std::pair<double, double> splitPair(std::string_view src, std::string_view sep) {
    const auto pos {src.find(sep)};
    if(pos == std::string::npos)
        throw splitException{};
    std::string left, right;
    left = src.substr(0, pos);
    right = src.substr(pos+1, src.length()-pos);
    if(left.length() == 0 || right.length() == 0)
        throw splitException{};
    
    const double val1{atof(left.c_str())}, val2{atof(right.c_str())};
    return std::make_pair(val1, val2);
}



int main(int argc, char **argv) {
    if(argc != 6) {
        std::cerr << "arguments:\nWidthXHeight Real,Imag Zoom Iter Cores\n";
        return 0;
    }

    try {
        std::string_view img_size = argv[1];
        std::string_view real_size = argv[2];
        std::string zoom_info = argv[3];
        std::string iter_info = argv[4];
        std::string core_info = argv[5];
        const std::pair<double, double> isize = splitPair(img_size, "x");
        const std::pair<double, double> m_values = splitPair(real_size, ",");
        const double zoom_value = atof(zoom_info.c_str());
        const int iter_value = atoi(iter_info.c_str());
        const int core_value = atoi(core_info.c_str());
        std::cout << "Fractal: "  << static_cast<int>(isize.first) << "x" << static_cast<int>(isize.second) << " "  << m_values.first << "," << m_values.second << " " << zoom_value << " " << iter_value << " " << core_value << "\n";
        //-0.743643887032151
        //0.142625924205330
        cv::Mat m;
        m.create(cv::Size(static_cast<int>(isize.first), static_cast<int>(isize.second)), CV_8UC3);
        cv_fract::CV_Fractal cvfrac;
        cvfrac.initParameters(m_values.first,m_values.second, zoom_value, iter_value, core_value);
        cvfrac.resetPalette();
        cvfrac.draw(m);
        cv::imwrite("output.png", m);
    } catch(splitException &e) {
        std::cerr << "Exception has occoured...\n";
    }
    return 0;
}
