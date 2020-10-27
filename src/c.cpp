#include<iostream>
#include<opencv2/viz/widgets.hpp>
#include<opencv2/viz/viz3d.hpp>
#include<opencv2/highgui.hpp>
using namespace std;
using namespace cv;

int main()
{
	viz::WCircle circle(20);
	viz::Viz3d viz;
	viz.showWidget("test", circle);
	auto m = viz.getScreenshot();
	imshow("test", m);
	waitKey();
}
