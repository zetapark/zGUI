#include<iostream>
#include"cvmatrix.h"
using namespace std;

int main()
{
	cv::Mat m = cv::imread("Lenna.png");
	cv::Mat m2 = m.row(200).clone();
	m2 = cv::Scalar{255,0,0};
	cv::imshow("test", m);
	cv::waitKey();
	m = 1.3 * m;
	cv::imshow("test2", m);
	cv::waitKey();

	if(m.isContinuous()) {
		uchar *p = m.data;
		for(int i=0; i<m.cols * m.rows; i++) *p = cv::saturate_cast<uchar>(*p + 30);
	}
}

