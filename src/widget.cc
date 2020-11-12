#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

cv::Ptr<cv::freetype::FreeType2> z::Widget::ft2_;

z::Widget::Widget(cv::Rect_<int> r)
	: Rect_<int>{r}
	, mat_(r.height, r.width)
{
	mat_ = background_color_;
	if(!ft2_) {
		ft2_ = cv::freetype::createFreeType2();
    ft2_->loadFontData("/usr/share/fonts/truetype/nanum/NanumBarunGothic.ttf", 0);
	}
}

bool z::Widget::focus() {
	return focus_;
}
void z::Widget::focus(bool tf) {
	focus_ = tf;
}
