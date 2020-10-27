#include<iostream>
#include<opencv2/highgui.hpp>
using namespace std;

int main()
{
	cv::Mat mat = cv::imread("/home/zeta/Pictures/11.jpg");
	cv::imshow("test", mat);
	cv::imshow("test2", mat);
	cv::waitKey();
	cv::destroyWindow("test");
	cv::imshow("test3", mat);
	while(1) cv::waitKey(1);
}
