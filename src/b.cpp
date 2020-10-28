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
	z::CheckBox chk{"option", {100, 200, 20, 20}};
	z::TextInput input{{100, 300, 150, 30}};
	cv::Mat mat = cv::imread("/home/zeta/Pictures/11.jpg");
	btn2.click([&win](int x, int y) { win.quit(); });
	btn.click([&](int, int){ if(popup.open()) cout << "yes" << endl; else cout << "no" << endl;});
	win += btn;
	win += btn2;
	win += btn3;
	win += chk;
	z::Image img{{250, 200, 200, 200}};
	z::Slider sl{{10, 10, 300, 30}, 0, 100, 1};
	sl.value(75);
	cv::resize(mat, mat, {200, 200});
	img = mat;
	win += img;//set image position and copy
	win += input;
	win += sl;
	win.show();
	return win.loop();
}

