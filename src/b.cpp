#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include"button.h"
using namespace std;

z::Popup popup{"yes no", {0, 0, 300, 300}, "What do you want?"};
void f(int, int) {
	if(popup.open()) cout << "yes" << endl;
	else cout << "no" << endl;
}
int main()
{
	cv::startWindowThread();
	z::Window win{"Title", {0, 0, 500, 500}};
	z::Button btn{"Popup", {100, 100, 90, 30}};
	z::Button btn2{"Quit", {200, 100, 80, 30}}, btn3{"click", {300, 100, 90, 30}};
	cv::Mat mat = cv::imread("/home/zeta/Pictures/11.jpg");
	btn2.click([&win](int x, int y) { win.quit(); });
	btn.click([&](int, int){ if(popup.open()) cout << "yes" << endl; else cout << "no" << endl;});
	win += btn;
	win += btn2;
	win += btn3;
	z::CheckBox chk{"option", {100, 200, 20, 20}};
	z::Image img{{200, 200, 200, 200}};
	cv::resize(mat, mat, {200, 200});
	img = mat;
	win += chk;
	win += img;
	win.show();
	cv::waitKey();
}

