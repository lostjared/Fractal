
#include<opencv2/opencv.hpp>
#include<iostream>
#include<complex>
#include<string>
#include<thread>
#include<vector>

namespace frac {
    int MAX_ITER=80;
    void DrawFractal(cv::Mat &frame, double start, double end, double im_start, double im_end,int thread_count);
    int mandelbrot(const std::complex<double> &c);
    std::vector<cv::Vec3b> color_palette;
    void load_pal();
}


void frac::load_pal() {
    std::srand(time(0));
    color_palette.resize(MAX_ITER);
    for(int i = 0; i < MAX_ITER; ++i) {
        unsigned char r = std::rand() % 256;
        unsigned char g = std::rand() % 256;
        unsigned char b = std::rand() % 256;
        color_palette[i] = cv::Vec3b(b, g, r);
    }
}

bool splitDoublePair(const std::string &value, const std::string &sep, double &val1, double &val2) {
    auto pos = value.find(sep);
    if(pos == std::string::npos)
        return false;
    std::string left = value.substr(0, pos);
    std::string right = value.substr(pos+1, value.length()-pos);
    val1 = atof(left.c_str());
    val2 = atof(right.c_str());
    return true;
}

bool splitIntegerPair(const std::string &value, const std::string &sep, int &val1, int &val2) {
    auto pos = value.find(sep);
    if(pos == std::string::npos)
        return false;
    std::string left = value.substr(0, pos);
    std::string right = value.substr(pos+1, value.length()-pos);
    val1 = atoi(left.c_str());
    val2 = atoi(right.c_str());
    return true;
}

int frac::mandelbrot(const std::complex<double> &c) {
    int n = 0;
    std::complex<double> z(0, 0);
    while(std::abs(z) <= 2 && n < MAX_ITER) {
        z = z*z+c;
        n ++;
    }
    return n;
}

template<typename F>
void UseMultipleThreads(cv::Mat &frame, int cores, F func) {
    int size = frame.rows/cores;
    std::vector<std::thread> values;
    for(int i = 0; i < cores; ++i) {
        values.push_back(std::thread(func, &frame, i*size, frame.cols, size));
    }
    for(unsigned int i = 0; i < values.size(); ++i) {
        values[i].join();
    }
}


void frac::DrawFractal(cv::Mat &frame, double start, double end, double im_start, double im_end, int thread_count) {
    int width=frame.cols, height=frame.rows;
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
                double w = (double(i)/double(width));
                double h = (double(z)/double(height));
                std::complex<double> c(start + w * (end - start), im_start + h * (im_end - im_start));
                int n = mandelbrot(c);
                if (n == MAX_ITER) {
                    pixel[0] = 0;
                    pixel[1] = 0;
                    pixel[2] = 0;
                } else {
                    pixel = color_palette[n];
                }
            }
        }
    };
    UseMultipleThreads(frame, thread_count, callback);
}

int main(int argc, char **argv) {
    
    if(argc != 9) {
        std::cerr << "Error invalid arguments..\n";
        std::cerr << "arguments:\n mandelbrot output.png WidthxHeight upperleft_start,upperleft_stop, lowerright_start,lowerright_stop center_real,center_imag zoom iterations thread_count\n";
        exit(EXIT_FAILURE);
    }
        
    std::string filename = argv[1];
    
    int width = 0, height = 0;
    if(!splitIntegerPair(argv[2], "x", width, height)) {
        std::cerr << "invalid resolution pair..\nUse: WidthxHeight\n";
        exit(EXIT_FAILURE);
    }
    
    int iterations = atoi(argv[7]);
    std::string left = argv[3];
    std::string right = argv[4];
    std::string center = argv[5];
    std::string zoom_l = argv[6];
    int tc = atoi(argv[8]);
    
    double start = 0, end = 0, im_start = 0, im_end = 0;
    if(!splitDoublePair(left, ",", start, end)) {
        std::cerr << "Error invalid string pair. use left,right\n";
        exit(EXIT_FAILURE);
    }
    if(!splitDoublePair(right, ",", im_start, im_end)) {
        std::cerr << "Error invalid second string pair. use left,right\n";
        exit(EXIT_FAILURE);
    }

    double center_real = 0, center_imag = 0;
    if(!splitDoublePair(center, ",", center_real, center_imag)) {
        std::cerr << "Error invalid center ..\n";
        exit(EXIT_FAILURE);
    }
    
    double zoom_ = atof(zoom_l.c_str());
    
    
    if(width <= 32 || height <= 32 || iterations <= 0 || tc < 1) {
        std::cerr << "invalid arguments...\n";
        exit(EXIT_FAILURE);
    }
    
    if(iterations > 0)
        frac::MAX_ITER = iterations;
    
    frac::load_pal();
     
    double aspect_ratio = static_cast<double>(width) / height;
    double range_real = 4.0 / zoom_; // Adjusted for zoom level. Original range is 4
    double range_imag = range_real / aspect_ratio;

    start = center_real - range_real / 2;
    end = center_real + range_real / 2;
    im_start = center_imag - range_imag / 2;
    im_end = center_imag + range_imag / 2;
    
    std::cout << "creating: " << filename << " " << width << "x" << height << " threads: " << tc << " max_iter: " << frac::MAX_ITER << "\n";
    
    cv::Mat m;
    m.create(cv::Size(width, height), CV_8UC3);
    frac::DrawFractal(m, start, end, im_start, im_end, tc);
    if(!cv::imwrite(filename, m)) {
        std::cerr << "could not write file..\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "wrote file: " << filename << "\n";
    return 0;
}
