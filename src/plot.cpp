#include <opencv2/highgui.hpp>
#include<opencv2/plot.hpp>
using namespace std;
using plt = cv::plot::Plot2d;

int main()
{
	array<double, 1000> ar;
	for(int i=0; i<1000; i++) ar[i] = sin(i * 0.01);
	auto a = plt::create(ar);
	cv::Mat mat;
	a->render(mat);
	cv::imshow("test", mat);
	cv::waitKey();
}

