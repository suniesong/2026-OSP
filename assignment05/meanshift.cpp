#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {

    Mat input = imread("lena.jpg", IMREAD_COLOR);

    if (!input.data) {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    // 1) Color meanshift
    Mat color_output;
    pyrMeanShiftFiltering(input, color_output, 31, 20, 3);

    // 2) Grayscale meanshift
    Mat gray_input, gray_3ch, gray_output;
    cvtColor(input, gray_input, COLOR_BGR2GRAY);
    cvtColor(gray_input, gray_3ch, COLOR_GRAY2BGR); //  pyrMeanShift
    pyrMeanShiftFiltering(gray_3ch, gray_output, 31, 20, 3);
    Mat gray_result;
    cvtColor(gray_output, gray_result, COLOR_BGR2GRAY); // grayscale

    // output
    imshow("Original", input);
    imshow("Meanshift Color", color_output);
    imshow("Original Gray", gray_input);
    imshow("Meanshift Gray", gray_result);

    waitKey();
    return 0;
}