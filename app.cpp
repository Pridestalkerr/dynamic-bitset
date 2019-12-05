#include <iostream>
#include "dynamic_bitset.hpp"
#include <vector>

int main()
{
	bit::Bitset <unsigned char> b({true,true,false,false,true,false});
	//b.set(0);
	std::cout << b << std::endl;
	b.clear();
	std::cout << b << std::endl;
	b.resize(3);
	std::cout << b << std::endl;
	b.resize(20, true);
	std::cout << b << std::endl;
	b.resize(24, false); //can be used as fill lol
	std::cout << b << std::endl;
	b.resize(24, true); //can be used as fill lol
	std::cout << b << std::endl;
	b.resize(25, true); //can be used as fill lol
	std::cout << b << std::endl;
	std::cout << b.to_string('f', 't') << std::endl;
	b.set();
	std::cout << b << std::endl;
	b.set(0, false);
	std::cout << b << std::endl;
	b.set(0, true);
	std::cout << b << std::endl;
	b.set(0, false);
	std::cout << b << std::endl;
	b.set(2, false);
	std::cout << b << std::endl;
	b.set(7, false);
	std::cout << b << std::endl;
	b.set(9, false);
	std::cout << b << std::endl;
	b.set(24, false);
	std::cout << b << std::endl;
	b.reset();
	std::cout << b << std::endl;
	b.reset();
	std::cout << b << std::endl;
	b.set(0);
	std::cout << b << std::endl;
	b.set();
	std::cout << b << std::endl;
	b.reset(0);
	std::cout << b << std::endl;
	b.reset(7);
	std::cout << b << std::endl;
	b.reset(9);
	std::cout << b << std::endl;
	b.flip();
	std::cout << b << std::endl;
	b.flip();
	std::cout << b << std::endl;
	b.flip(0);
	std::cout << b << std::endl;
	b.flip(7);
	std::cout << b << std::endl;
	b.flip(9);
	std::cout << b << std::endl;
	b.flip(0);
	std::cout << b << std::endl;
	b.flip(7);
	std::cout << b << std::endl;
	b.flip(9);
	std::cout << b << std::endl;

	//b.resize(24, true);
	b.set();
	std::cout << b << std::endl;
	//b <<= 24;
	std::cout << (b>>24) << std::endl;

	bit::Bitset <unsigned char> a (32);
	//TEST IN PLACE OPERATORS



	b.set();
	std::cout << b << std::endl;
	std::cout << a << std::endl;
	a ^= b;
	std::cout << b << std::endl;
	std::cout << a << std::endl;

	std::cout << std::endl;

	std::cout << b << std::endl;
	b.set(24, 0);
	b.set(23, 0);
	std::cout << b.at(24) << std::endl;
	std::cout << b.at(23) << std::endl;
	b.at(23) = b.at(0);
	std::cout << b.at(0) << std::endl;

	std::cout << b << std::endl;
	b.at(23) &= b.at(24);
	std::cout << b << std::endl;

	std::vector <int> wtf({1,2,3,4,5});

	for(auto &itr : wtf)
	{
		itr = 3;
		std::cout << itr << ' ';
	}

	for(auto itr : wtf)
	{
		std::cout << itr << ' ';
	}

	/*std::cout << (b << 63) << std::endl;
	b[63] = false;
	b[8] = false;
	b[16] = false;
	b[32] = false;
	b[33] = false;
	b[61] = false;
	std::cout << b << std::endl;
	b = b << 0;
	std::cout << b << std::endl;
	b.set();
	std::cout << b << std::endl;
	b <<= 32;
	std::cout << b << std::endl;
	b = b >> 32;
	std::cout << b << std::endl;
	b >>= 16;
	std::cout << b << std::endl;
	b.test(333);*/
	return 0;
}