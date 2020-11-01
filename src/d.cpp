#include<iostream>
#include"asyncq.h"
using namespace std;

int main()
{
	WQueue<int, 100> q{[](int i) { cout << i << endl;}};
	thread th{[&]() { for(int i=0; ; i++) q.push_back(i); }};
	while(1);
}

