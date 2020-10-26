#include<iostream>
#include<opencv2/highgui.hpp>
#include"button.h"
using namespace std;

int main()
{
	zeta::Window win{"test", {0, 0, 500, 500}};
	zeta::Button btn{{100, 100, 300, 300}};
	win.add(btn);
	win.show();
	cv::waitKey();
}

