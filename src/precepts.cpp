#include<cstdlib>
#include<fstream>
#include<opencv2/imgproc.hpp>
#include"button.h"
using namespace std;

const char *pr[] = {"108-1", "108-2", "108-3", "명상", "경전", "운동", "걷기"};
struct Mywin : z::AsciiWindow
{
	Mywin() : z::AsciiWindow{R"(
		WPrecepts-------------------------------------------------
		|
		|     C0-L0-------------     C3-L3-----------------
		|     || |108-1|             || |暝想 2H|
		|     C1-L1-------------     C4-L4-------------
		|     || |108-2|             || |經典 읽기|
		|     C2-L2-------------     C5-L5-------------
		|     || |108-3|             || |운동|
		|                            C6-L6-------------
		|                            || |걷기|
		|     
		|
		|     B0-------------        B1---------------
		|     |새로운 시작|     |저장 후 종료|
		|
		|)", 20, 30}
	{
		start();
		string home{getenv("HOME")};
		B[0]->click([this]() { for(int i=0; i<7; i++) C[i]->checked(false), *this << *C[i]; });
		B[1]->click([this, home]() {
				ofstream f{home + "/Dropbox/precepts.txt"};
				for(int i=0; i<7; i++) f << C[i]->checked() << ' ' << pr[i] << endl;
				close();
			});
		bool tf;
		ifstream f{home + "/Dropbox/precepts.txt"};
		string s;
		for(int i=0; i<7; i++) f >> tf, getline(f, s), C[i]->checked(tf), *this << *C[i];
		cv::moveWindow(title_, 700, 500);
	}
};

int main()
{
	Mywin win;
	win.loop();
}
