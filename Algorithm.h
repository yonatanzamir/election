#pragma once
#include <iostream>
using namespace std;
template<class T,class k>
 k* Myfind(T t1, T t2, k num)//template algorithm
{
	 bool is_found = false;
	 while (t1 != t2 && is_found == false)
	 {
		 if ((*t1)==num)
			 is_found = true;
		 else
			 t1++;
	 }
	 if (is_found == false)
		 return nullptr;
	 return &(*t1);
}

	


