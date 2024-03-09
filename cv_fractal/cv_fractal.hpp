#ifndef _CV_FRACTAL_HPP
#define _CV_FRACTAL_HPP
#include<opencv2/opencv.hpp>
#include<complex>
#include<thread>

namespace cv_fract {
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
    class CV_Fractal {
    public:
        CV_Fractal() = default;    
        CV_Fractal(double center_real_, double center_imag_, double zoom, int iterations_, int thread_count_) {
            initParameters(center_real_, center_imag_, zoom, iterations_, thread_count_);
        }
        void initParameters(double center_real_, double center_imag_, double zoom, int iterations_, int thread_count_) {
            center_real = center_real_;
            center_imag = center_imag_;
            zoom_ = zoom;
            iterations = iterations_;
            tc = thread_count_;
            if(iterations > 0)
                MAX_ITER = iterations;
            else 
                std::cerr << "Error invalid iterations\n";
            loadPalette();
        }
        void draw(cv::Mat &frame) {
             double aspect_ratio = static_cast<double>(frame.cols) / frame.rows;
             double range_real = 4.0 / zoom_; 
             double range_imag = range_real / aspect_ratio;
             double start = center_real - range_real / 2;
             double end = center_real + range_real / 2;
             double im_start = center_imag - range_imag / 2;
             double im_end = center_imag + range_imag / 2;
             DrawFractal(frame, start, end, im_start, im_end, tc);
        }
    private:
        double center_real, center_imag;
        double zoom_;
        int iterations;
        int tc; 
        int MAX_ITER=80;
        std::vector<cv::Vec3b> color_palette;

        void loadPalette() {
            std::srand(time(0));
            color_palette.resize(MAX_ITER);
            for(int i = 0; i < MAX_ITER; ++i) {
                unsigned char r = std::rand() % 256;
                unsigned char g = std::rand() % 256;
                unsigned char b = std::rand() % 256;
                color_palette[i] = cv::Vec3b(b, g, r);
            }
        }

        int mandelbrot(const std::complex<double> &c) {
            int n = 0;
            std::complex<double> z(0, 0);
            while(std::abs(z) <= 2 && n < MAX_ITER) {
                z = z*z+c;
                n ++;
            }
            return n;
        }

        void DrawFractal(cv::Mat &frame, double start, double end, double im_start, double im_end, int thread_count) {
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
    };   
}



#endif