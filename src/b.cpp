#include<iostream>
#include<opencv2/highgui.hpp>
#include"button.h"
using namespace std;

int main()
{
	zeta::Window win{"test", {0, 0, 500, 500}};
	zeta::Button btn{{100, 100, 50, 100}};
	zeta::Button btn2{{200, 100, 100, 100}};
	win.add(btn);
	win.add(btn2);
	win.show();
	cv::waitKey();
}

