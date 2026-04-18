#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

#define IM_TYPE	CV_64FC3

using namespace cv;

#if (IM_TYPE == CV_8UC3)
typedef uchar G;
typedef cv::Vec3b C;
#elif (IM_TYPE == CV_16SC3)
typedef short G;
typedef Vec3s C;
#elif (IM_TYPE == CV_32SC3)
typedef int G;
typedef Vec3i C;
#elif (IM_TYPE == CV_32FC3)
typedef float G;
typedef Vec3f C;
#elif (IM_TYPE == CV_64FC3)
typedef double G;
typedef Vec3d C;
#endif

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma);
Mat Gaussianfilter_Gray(const Mat input, int n, double sigmaT, double sigmaS, const char* opt);
Mat Gaussianfilter_RGB(const Mat input, int n, double sigmaT, double sigmaS, const char* opt);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat input_gray;

	if (!input.data) {
		printf("Could not open\n");
		return -1;
	}

	cvtColor(input, input_gray, COLOR_RGB2GRAY);

	input.convertTo(input, CV_64FC3, 1.0 / 255);
	input_gray.convertTo(input_gray, CV_64F, 1.0 / 255);

	Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
	Mat noise_RGB = Add_Gaussian_noise(input, 0, 0.1);

	Mat Denoised_Gray = Gaussianfilter_Gray(noise_Gray, 5, 1, 1, "zero-padding");
	Mat Denoised_RGB = Gaussianfilter_RGB(noise_RGB, 5, 1, 1, "zero-padding");

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	namedWindow("Gaussian Noise (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (Grayscale)", noise_Gray);

	namedWindow("Gaussian Noise (RGB)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (RGB)", noise_RGB);

	namedWindow("Denoised (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Denoised (Grayscale)", Denoised_Gray);

	namedWindow("Denoised (RGB)", WINDOW_AUTOSIZE);
	imshow("Denoised (RGB)", Denoised_RGB);

	waitKey(0);
	return 0;
}

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma) {
	Mat NoiseArr = Mat::zeros(input.rows, input.cols, input.type());
	RNG rng;
	rng.fill(NoiseArr, RNG::NORMAL, mean, sigma);
	add(input, NoiseArr, NoiseArr);
	return NoiseArr;
}

Mat Gaussianfilter_Gray(const Mat input, int n, double sigmaT, double sigmaS, const char* opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa, tempb;

	// 커널 초기화 및 정규화
	Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_64F);
	double denom = 0.0;
	for (int a = -n; a <= n; a++) {
		for (int b = -n; b <= n; b++) {
			double val = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
			kernel.at<double>(a + n, b + n) = val;
			denom += val;
		}
	}
	for (int a = -n; a <= n; a++)
		for (int b = -n; b <= n; b++)
			kernel.at<double>(a + n, b + n) /= denom;

	Mat output = Mat::zeros(row, col, input.type());

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-padding")) {
				double sum1 = 0.0;
				double sum2 = 0.0; 
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						if ((i + a >= 0) && (i + a < row) && (j + b >= 0) && (j + b < col)) {
							double w = kernel.at<double>(a + n, b + n);
							sum1 += w * input.at<G>(i + a, j + b);
							sum2 += w;
						}
					}
				}
				output.at<G>(i, j) = (G)(sum2 != 0 ? sum1 / sum2 : 0);
			}

			else if (!strcmp(opt, "mirroring")) {
				double sum1 = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						tempa = i + a;
						tempb = j + b;
						if (tempa < 0)        tempa = -(i + a);
						else if (tempa > row - 1) tempa = i - a;
						if (tempb < 0)        tempb = -(j + b);
						else if (tempb > col - 1) tempb = j - b;
						sum1 += kernel.at<double>(a + n, b + n) * input.at<G>(tempa, tempb);
					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				double sum1 = 0.0;
				double sum2 = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						if ((i + a >= 0) && (i + a < row) && (j + b >= 0) && (j + b < col)) {
							double w = kernel.at<double>(a + n, b + n);
							sum1 += w * input.at<G>(i + a, j + b);
							sum2 += w;
						}
					}
				}
				output.at<G>(i, j) = (G)(sum2 != 0 ? sum1 / sum2 : 0);
			}
		}
	}
	return output;
}

Mat Gaussianfilter_RGB(const Mat input, int n, double sigmaT, double sigmaS, const char* opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa, tempb;

	// 커널 초기화 및 정규화
	Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_64F);
	double denom = 0.0;
	for (int a = -n; a <= n; a++) {
		for (int b = -n; b <= n; b++) {
			double val = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
			kernel.at<double>(a + n, b + n) = val;
			denom += val;
		}
	}
	for (int a = -n; a <= n; a++)
		for (int b = -n; b <= n; b++)
			kernel.at<double>(a + n, b + n) /= denom;

	Mat output = Mat::zeros(row, col, input.type());

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-padding")) {
				double sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;
				double sum2 = 0.0; 
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						if ((i + a >= 0) && (i + a < row) && (j + b >= 0) && (j + b < col)) {
							double w = kernel.at<double>(a + n, b + n);
							sum_r += w * input.at<C>(i + a, j + b)[0];
							sum_g += w * input.at<C>(i + a, j + b)[1];
							sum_b += w * input.at<C>(i + a, j + b)[2];
							sum2 += w; 
						}
					}
				}
				if (sum2 != 0) {
					output.at<C>(i, j)[0] = (G)(sum_r / sum2);
					output.at<C>(i, j)[1] = (G)(sum_g / sum2);
					output.at<C>(i, j)[2] = (G)(sum_b / sum2);
				}
			}

			else if (!strcmp(opt, "mirroring")) {
				double sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						tempa = i + a;
						tempb = j + b;
						if (tempa < 0)        tempa = -(i + a);
						else if (tempa > row - 1) tempa = i - a;
						if (tempb < 0)        tempb = -(j + b);
						else if (tempb > col - 1) tempb = j - b;
						double w = kernel.at<double>(a + n, b + n);
						sum_r += w * input.at<C>(tempa, tempb)[0];
						sum_g += w * input.at<C>(tempa, tempb)[1];
						sum_b += w * input.at<C>(tempa, tempb)[2];
					}
				}
				output.at<C>(i, j)[0] = (G)sum_r;
				output.at<C>(i, j)[1] = (G)sum_g;
				output.at<C>(i, j)[2] = (G)sum_b;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				double sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;
				double sum2 = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						if ((i + a >= 0) && (i + a < row) && (j + b >= 0) && (j + b < col)) {
							double w = kernel.at<double>(a + n, b + n);
							sum_r += w * input.at<C>(i + a, j + b)[0];
							sum_g += w * input.at<C>(i + a, j + b)[1];
							sum_b += w * input.at<C>(i + a, j + b)[2];
							sum2 += w;
						}
					}
				}
				if (sum2 != 0) {
					output.at<C>(i, j)[0] = (G)(sum_r / sum2);
					output.at<C>(i, j)[1] = (G)(sum_g / sum2);
					output.at<C>(i, j)[2] = (G)(sum_b / sum2);
				}
			}
		}
	}
	return output;
}