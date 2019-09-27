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

	class Iterator	//single bit encapsulation
	{
		u64 pos_;
		Bitset &array_;
		bool value_;

	public:

		Iterator(const u64, Bitset&);

		operator bool const();	//comparisons with bool type

		Iterator& operator = (const bool);	//simulating access
	};

	Bitset(const u64);

	bool get(const std::size_t);

	Bitset& set();

	Bitset& set(const std::size_t, const bool);

	Bitset& reset();

	Bitset& reset(const std::size_t);

	Bitset& flip();

	Bitset& flip(const std::size_t);

	Bitset::Iterator operator [] (const std::size_t);

	void resize(const u64);

	void resize(const u64, const bool);

	void clear();

	friend std::ostream& operator << (std::ostream&, const Bitset&);
};



//-------------------------class Iterator methods BEGIN-------------------------

Bitset::Iterator::Iterator(const u64 pos, Bitset &array)
	:
	pos_(pos),
	array_(array),
	value_(array.get(pos))
{};

Bitset::Iterator::operator bool const() 
{
	return value_;
}

Bitset::Iterator& Bitset::Iterator::operator = (const bool value)
{
	array_.set(pos_, value);

	return *this;
}

//-------------------------class Iterator methods END-------------------------



//-------------------------class Bitset methods BEGIN-------------------------

Bitset::Bitset(const u64 size = 0)
	:
	size_(size),
	segments_(size / 64 + (((size / 64) * 64) == size ? 0 : 1))
{};

bool Bitset::get(const std::size_t pos)
{
	u64 segment = pos / 64;
	u8 bit_pos = pos % 64;

	if(segments_[segment] & (1ULL << bit_pos))
		return true;
	else
		return false;
}

Bitset& Bitset::set()
{
	for(auto &itr : segments_)
		itr = ~0ULL;

	return *this;
}

Bitset& Bitset::set(const std::size_t pos, const bool value = true)
{
	u64 segment = pos / 64;
	u8 bit_pos = pos % 64;

	if(value)
		segments_[segment] |= (1ULL << bit_pos);
	else
		segments_[segment] &= ~(1ULL << bit_pos);

	return *this;
}

Bitset& Bitset::reset()
{
	for(auto &itr : segments_)
		itr = 0ULL;

	return *this;
}

Bitset& Bitset::reset(const std::size_t pos)
{
	set(pos, 0);

	return *this;
}

Bitset& Bitset::flip()
{
	for(auto &itr : segments_)
		itr = ~itr;

	return *this;
}

Bitset& Bitset::flip(const std::size_t pos)
{
	if((*this)[pos])
		(*this)[pos] = false;
	else
		(*this)[pos] = true;

	return *this;
}

Bitset::Iterator Bitset::operator [] (const std::size_t pos)
{
	return Iterator(pos, *this);
}

void Bitset::resize(const u64 size = 0)
{
	size_ = size;
	segments_.resize(size / 64 + (((size / 64) * 64) == size ? 0 : 1));
}

void Bitset::resize(const u64 size, const bool value)
{
	size_ = size;
	if(value)
		segments_.resize(size / 64 + (((size / 64) * 64) == size ? 0 : 1), ~0ULL);
	else
		segments_.resize(size / 64 + (((size / 64) * 64) == size ? 0 : 1), 0ULL);
}

void Bitset::clear()
{
	size_ = 0;
	segments_.clear();
}

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

//-------------------------class Bitset methods END-------------------------

}