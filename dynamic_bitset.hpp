#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>

namespace bit
{

template <typename Chunk_T = unsigned long long> //Chunk_T should be unsigned, gotta assert
class Bitset
{
	static_assert(std::is_unsigned <Chunk_T>::value, "Non arithmetic unsigned type passed as template parameter.");

	std::size_t size_;
	std::size_t chunk_size_;
	std::vector <Chunk_T> chunks_;

	class Iterator	//single bit encapsulation
	{
		std::size_t pos_;
		Bitset <Chunk_T> &array_;
		bool value_;

	public:

		Iterator(const std::size_t, Bitset <Chunk_T>&);

		operator bool const();	//comparisons with bool type

		Iterator& operator = (const bool);	//simulating access
	};

public:

	Bitset(const std::size_t = 0);

	bool get(const std::size_t) const;

	std::size_t size() const;

	Bitset <Chunk_T>& set();

	Bitset <Chunk_T>& set(const std::size_t, const bool = true);

	Bitset <Chunk_T>& reset();

	Bitset <Chunk_T>& reset(const std::size_t);

	Bitset <Chunk_T>& flip();

	Bitset <Chunk_T>& flip(const std::size_t);

	Bitset <Chunk_T>::Iterator operator [] (const std::size_t);

	Bitset <Chunk_T>& resize(const std::size_t = 0);

	Bitset <Chunk_T>& resize(const std::size_t, const bool);

	Bitset <Chunk_T>& clear();

	Bitset <Chunk_T>& operator &= (const Bitset <Chunk_T>&);

	Bitset <Chunk_T>& operator |= (const Bitset <Chunk_T>&);

	Bitset <Chunk_T>& operator ^= (const Bitset <Chunk_T>&);

	Bitset <Chunk_T> operator ~ () const;

	Bitset <Chunk_T> operator << (const std::size_t) const;

	Bitset <Chunk_T>& operator <<= (const std::size_t);

	Bitset <Chunk_T> operator >> (const std::size_t) const;

	Bitset <Chunk_T>& operator >>= (const std::size_t);

	template <typename Chunk_FT>
	friend std::ostream& operator << (std::ostream&, const Bitset <Chunk_FT>&);
};



//-------------------------class Iterator methods BEGIN-------------------------

template <typename Chunk_T> //non pure
Bitset <Chunk_T>::Iterator::Iterator(const std::size_t pos, Bitset &array)
	:
	pos_(pos),
	array_(array),
	value_(array.get(pos))
{};

template <typename Chunk_T> //pure
Bitset <Chunk_T>::Iterator::operator bool const() 
{
	return value_;
}

template <typename Chunk_T> //non pure
typename Bitset <Chunk_T>::Iterator& Bitset <Chunk_T>::Iterator::operator = (const bool value)
{
	array_.set(pos_, value);

	return *this;
}

//-------------------------class Iterator methods END-------------------------


/*============================================================================
--------------------------class Bitset methods BEGIN--------------------------
============================================================================*/

template <typename Chunk_T>
Bitset <Chunk_T>::Bitset(const std::size_t size)
	:
	size_(size),
	chunk_size_(sizeof(Chunk_T) * CHAR_BIT),
	chunks_((size_ - 1) / chunk_size_ + 1)
{};

template <typename Chunk_T> //pure
std::size_t Bitset <Chunk_T>::size() const
{
	return size_;
}

template <typename Chunk_T> //pure
bool Bitset <Chunk_T>::get(const std::size_t pos) const
{
	std::size_t chunk_pos = pos / chunk_size_;
	std::size_t bit_pos = pos % chunk_size_;

	if(chunks_[chunk_pos] & (static_cast <Chunk_T>(1) << bit_pos)) //gotta make sure that chunk_t cast is fine...
		return true;
	else
		return false;
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::set()
{
	for(auto &itr : chunks_)
		itr = ~static_cast <Chunk_T>(0);

	return *this;
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::set(const std::size_t pos, const bool value)
{
	std::size_t chunk_pos = pos / chunk_size_;
	std::size_t bit_pos = pos % chunk_size_;

	if(value)
		chunks_[chunk_pos] |= (static_cast <Chunk_T>(1) << bit_pos);
	else
		chunks_[chunk_pos] &= ~(static_cast <Chunk_T>(1) << bit_pos);

	return *this;
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::reset()
{
	for(auto &itr : chunks_)
		itr = 0;

	return *this;
}

template <typename Chunk_T> //recursively pure
Bitset <Chunk_T>& Bitset <Chunk_T>::reset(const std::size_t pos)
{
	set(pos, 0);

	return *this;
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::flip()
{
	for(auto &itr : chunks_)
		itr = ~itr;

	return *this;
}

template <typename Chunk_T> //pure 
Bitset <Chunk_T>& Bitset <Chunk_T>::flip(const std::size_t pos)
{
	std::size_t chunk_pos = pos / chunk_size_;
	std::size_t bit_pos = pos % chunk_size_;

	chunks_[chunk_pos] ^= (static_cast <Chunk_T>(1) << bit_pos);

	return *this;
}

template <typename Chunk_T> //pure
typename Bitset <Chunk_T>::Iterator Bitset <Chunk_T>::operator [] (const std::size_t pos)
{
	return Iterator(pos, *this);
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::resize(const std::size_t size)
{
	size_ = size;
	chunks_.resize((size_ - 1) / chunk_size_ + 1);
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::resize(const std::size_t size, const bool value)
{
	size_ = size;
	chunks_.resize((size_ - 1) / chunk_size_ + 1, value ? ~static_cast <Chunk_T>(0) : 0);
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::clear()
{
	size_ = 0;
	chunks_.clear();
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::operator &= (const Bitset <Chunk_T> &rhs)
{
	std::size_t length;

	if(size_ >= rhs.size_)
		length = rhs.size_;
	else
		length = size_;

	for(std::size_t itr = 0; itr < length; ++itr)
		chunks_[itr] &= rhs.chunks_[itr];

	return *this;
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::operator |= (const Bitset <Chunk_T> &rhs)
{
	std::size_t length;

	if(size_ >= rhs.size_)
		length = rhs.size_;
	else
		length = size_;

	for(std::size_t itr = 0; itr < length; ++itr)
		chunks_[itr] |= rhs.chunks_[itr];

	return *this;
}

template <typename Chunk_T> //pure
Bitset <Chunk_T>& Bitset <Chunk_T>::operator ^= (const Bitset <Chunk_T> &rhs)
{
	std::size_t length;

	if(size_ >= rhs.size_)
		length = rhs.size_;
	else
		length = size_;

	for(std::size_t itr = 0; itr < length; ++itr)
		chunks_[itr] ^= rhs.chunks_[itr];

	return *this;
}

template <typename Chunk_T> //pure
Bitset <Chunk_T> Bitset <Chunk_T>::operator ~ () const
{
	Bitset <Chunk_T> flip(size_);

	for(std::size_t itr = 0; itr < chunks_.size(); ++itr)
		flip.chunks_[itr] = ~chunks_[itr];

	return flip;
}

template <typename Chunk_T>
Bitset <Chunk_T> Bitset <Chunk_T>::operator << (const std::size_t pos) const
{
	//shift chunks to right (increase value)
	if(!pos)
		return *this;

	if(pos >= size_)
		return Bitset <Chunk_T>(size_);

	Bitset <Chunk_T> shifted(size_); //empty bitset

	const std::size_t left = pos % chunk_size_;
	const std::size_t right = chunk_size_ - left;
	std::size_t chunk_pos = pos / chunk_size_;
	std::size_t itr = 0;

	if(pos % chunk_size_)
	{
		//split chunk shift
		while(chunk_pos + 1 < chunks_.size())
		{
			shifted.chunks_[chunk_pos] |= chunks_[itr] << left;
			shifted.chunks_[chunk_pos + 1] |= chunks_[itr] >> right;

			++chunk_pos;
			++itr;
		}
		shifted.chunks_[chunk_pos] |= chunks_[itr] << left; //this goes out of bounds retard
	}
	else
		//full chunk shift
		for(std::size_t itr = 0; itr + chunk_pos < chunks_.size(); ++itr)
			shifted.chunks_[itr + chunk_pos] = chunks_[itr];

	return shifted;
}

template <typename Chunk_T>
Bitset <Chunk_T>& Bitset <Chunk_T>::operator <<= (const std::size_t pos)
{
	if(!pos)
		return this;

	if(pos >= size_)
	{
		std::fill(chunks_.begin(), chunks_.end(), static_cast <Chunk_T>(0));
		return this;
	}

	const std::size_t left = pos % chunk_size_; //shifting with left gives you rightside
	const std::size_t right = chunk_size_ - left; //same
	std::size_t chunk_pos = chunks_.size() - pos / chunk_size_;
	std::size_t itr = chunks_.size();

	//we do it in reverse so we dont waste time (we can also create another bitset and then move it)
	if(pos % chunk_size_)
	{
		chunks_[itr] = chunks_[chunk_pos] << left;
		while(chunk_pos >= 0)
		{	
			--chunk_pos;
			--itr;

			
		}
	}
	else
	{

	}

	return this;
}

template <typename Chunk_T>
Bitset <Chunk_T> Bitset <Chunk_T>::operator >> (const std::size_t pos) const
{

}

template <typename Chunk_T> //pure
std::ostream& operator << (std::ostream &out, const Bitset <Chunk_T> &mask)
{
	if(!mask.size_)
		return out;

	for(std::size_t itr = (mask.size_ % mask.chunk_size_ == 0 ? mask.chunk_size_ : mask.size_ % mask.chunk_size_); itr-- > 0;)
	{
		if(mask.chunks_[mask.chunks_.size() - 1] & (static_cast <Chunk_T>(1) << itr))
			out << 1;
		else
			out << 0;
	}

	for(std::size_t itr = mask.chunks_.size() - 1; itr-- > 0;)
		for(std::size_t jtr = mask.chunk_size_; jtr-- > 0;)
			if(mask.chunks_[itr] & (static_cast <Chunk_T>(1) << jtr))
				out << 1;
			else
				out << 0;

	return out;
}

//-------------------------class Bitset methods END-------------------------

}