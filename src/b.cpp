#include<iostream>
#include<opencv2/highgui.hpp>
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
	z::Window win{"test", {0, 0, 500, 500}};
	z::Button btn{"Ok", {100, 100, 50, 30}};
	z::Button btn2{"cancel", {200, 100, 100, 30}};
	cv::Mat mat = cv::imread("/home/zeta/Pictures/11.jpg");
	btn2.click([&win](int x, int y) { win.quit(); });
	btn.click([&](int, int){ if(popup.open()) cout << "yes" << endl; else cout << "no" << endl;});
	win.add(btn);
	win.add(btn2);
	z::CheckBox chk{"option", {100, 200, 20, 20}};
	win.add(chk);
	win.show();
	cv::waitKey();
}

