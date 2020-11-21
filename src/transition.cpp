#include<iostream>
#include"cvmatrix.h"
using namespace std;

int main(int ac, char **av) {
	CVMat m = cv::imread(av[1]);
	cv::Mat arr[50];
	arr[0] = m.clone();
	m.gray();
	m.quantize(43);
	for(int i=1; i<25; i++) {
		m.tear();
		arr[i] = m.clone();
	}

	m = cv::imread(av[2]);
	arr[49] = m.clone();
	m.gray();
	m.quantize(43);
	for(int i=48; i>24; i--) {
		m.tear();
		arr[i] = m.clone();
	}

	for(int i=0; i<50; i++) {
		cv::imshow("glitch", arr[i]);
		cv::waitKey(30);
	}
}

