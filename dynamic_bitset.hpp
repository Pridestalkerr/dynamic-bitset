#pragma once

#include <cmath>
#include <vector>
#include <iostream>

namespace bit
{

#define u64 unsigned long long int
#define u8 unsigned char

class Bitset
{
	u64 size_;
	std::vector <u64> segments_;

public:

	Bitset(const u64 size = 0)
		:
		size_(size),
		segments_(size / 64 + (((size / 64) * 64) == size ? 0 : 1))
	{};

	Bitset& set(const std::size_t pos, const bool value = true)
	{
		u64 segment = pos / 64;
		u8 bit_pos = pos % 64;

		if(value)
			segments_[segment] |= (1ULL << bit_pos);
		else
			segments_[segment] &= ~(1ULL << bit_pos);

		return *this;
	}

	friend std::ostream& operator << (std::ostream&, const Bitset&);
};

std::ostream& operator << (std::ostream &out, const Bitset &bits)
{
	for(int j = (bits.size_ % 64 == 0 ? 64 : bits.size_ % 64) - 1; j >= 0; --j)
	{
		if(bits.segments_[bits.segments_.size() - 1] & (1ULL << j))
			out << 1;
		else
			out << 0;
	}

	//out << std::endl;

	for(int i = bits.segments_.size() - 2; i >= 0; --i)
	{
		for(int j = 63; j >= 0; --j)
		{
			if(bits.segments_[i] & (1ULL << j))
				out << 1;
			else
				out << 0;
		}

		//out << std::endl;
	}

	return out;
}

}