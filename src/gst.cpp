#include"cvmatrix.h"
using namespace std;

int main()
{
	cv::VideoCapture cap{"autovideosrc ! zebrastripe ! videoconvert ! queue ! appsink", cv::CAP_GSTREAMER};
	cv::VideoWriter out{"appsrc ! queue ! videoconvert ! autovideosink", cv::CAP_GSTREAMER,
			cv::VideoWriter::fourcc('D','I','V','X'), 30, cv::Size(1280,720), true};
	cv::Mat frame;
	while(1) {
		cap >> frame;
		//cv::imshow("show", frame);
		out << frame;
		//cv::waitKey(10);
	}
}

