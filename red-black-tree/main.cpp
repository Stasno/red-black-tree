
#include <iostream>
#include <chrono>
#include <map>

using namespace std;

#include "simple_tree.h"
#include "red_black_tree.h"

int main()
{
	{
		std::map<int, int> mp;
		my::red_black_tree<int, int> a;

		auto x = mp.find(1);

		//
		for (int i{}; i < 10; i++)
		{
			a.insert(i, 0);
			mp.emplace(i, 0);
		}
		a.print(mp.end()._Ptr->_Parent);

		a.erase(3);
		mp.erase(3);
		a.print(mp.end()._Ptr->_Parent);
		a.erase(0);
		mp.erase(0);
		a.print(mp.end()._Ptr->_Parent);

		cout << "\n ||||| \n";

		for (auto i = a.begin(); i != a.end(); i++)
		{
			cout << i->first << '\n';
		}

		cout << "\n ||||| \n";

		for (auto i = a.rbegin(); i != a.rend(); i--)
		{
			cout << i->first << '\n';
		}
	}





	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();
}