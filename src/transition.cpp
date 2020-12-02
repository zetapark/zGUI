#include<iostream>
#include"cvmatrix.h"
using namespace std;

int main(int ac, char **av) {
	CVMat m = cv::imread(av[1]);
	cv::Mat arr[36], mat, m1 = m.clone(), m2;
	for(int i : {0, 1, 2, 3, 4, 5, 8, 9}) arr[i] = m1;
	m.gray();
	m.salt();
	m.quantize(43);
	for(int i : {6, 7, 10, 11, 12, 13, 14, 15, 17, 19, 21}) {
		m.zoom({3, 3, m.cols - 6, m.rows - 6});
		m.tear();
		cv::cvtColor(m, mat, cv::COLOR_GRAY2BGR);
		arr[i] = mat.clone();
	}

	m = cv::imread(av[2]);
	m2 = m.clone();
	for(int i : {35, 34, 33, 30, 29}) arr[i] = m2;
	m.gray();
	m.salt();
	m.quantize(43);
	for(int i : {32, 31, 28, 27, 26, 25, 24, 23, 22, 20, 18, 16}) {
		m.zoom({3, 3, m.cols - 6, m.rows - 6});
		m.tear();
		cv::cvtColor(m, mat, cv::COLOR_GRAY2BGR);
		arr[i] = mat.clone();
	}

	cv::VideoWriter w{"glitch.avi", cv::VideoWriter::fourcc('D', 'I', 'V', 'X'), 30, {512, 512}, true};
	for(const auto &a : arr) {
		cv::imshow("glitch", a);
		w << a;
		cv::waitKey(30);
	}
}

