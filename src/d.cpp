#include<iostream>
#include<opencv2/opencv.hpp>
#include"asyncq.h"
using namespace std;

int main()
{
	cv::Mat3b mat(40, 30);
	vector<cv::Mat3b> v;
	v.push_back(cv::Mat3b(40, 30));
	cout << v[0].cols << endl;
}

