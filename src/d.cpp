#include<fstream>
#include<opencv2/imgproc.hpp>
#include"button.h"
using namespace std;

struct Mywin : z::AsciiWindow
{
	Mywin() : z::AsciiWindow{R"(
		WPrecepts-------------------------------------------------
		|
		|     C0-L0-------------     C3-L3-----------------
		|     || |108-1|             || |Meditation 2H|
		|     C1-L1-------------     C4-L4-------------
		|     || |108-2|             || |Sutra|
		|     C2-L2-------------     C5-L5-------------
		|     || |108-3|             || |Exercise|
		|                            C6-L6-------------
		|                            || |Walk|
		|     
		|
		|     B0-------------        B1-------------
		|     |Restart|              |Quit|
		|
		|)"}
	{
		B[0]->click([this]() { 
				for(int i=0; i<7; i++) C[i]->checked(false), *this << *C[i];
				});
		B[1]->click([this]() {
				close();
				});
		start();
	}
};

int main()
{
	Mywin win;
	win.loop();
}
