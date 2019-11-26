#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>
#include <memory>
#include <cctype>
#include <cxxabi.h>

/*template<typename T, typename = void>
inline constexpr bool is_arithmetic_v = std::false_type{};

template<typename T>
inline constexpr bool is_arithmetic_v<
    T, 
    std::void_t<decltype(std::declval<T>() + std::declval<T>())>
> = std::true_type{};*/

namespace bit
{

template <typename Chunk_T = unsigned long long, typename Allocator_T = std::allocator <Chunk_T>> //Chunk_T should be unsigned, gotta assert
class Bitset
{
private:

	static_assert(std::is_unsigned <Chunk_T>::value, "Non arithmetic unsigned type passed as template parameter.");

	std::size_t size_;
	std::size_t chunk_size_;
	std::vector <Chunk_T> chunks_;

	class Iterator	//single bit encapsulation
	{
		std::size_t pos_;
		Bitset <Chunk_T, Allocator_T> &array_;
		bool value_;

	public:

		Iterator(const std::size_t, Bitset <Chunk_T, Allocator_T>&);

		operator bool const();	//comparisons with bool type

		Iterator& operator = (const bool);	//simulating access
	};

public:

	Bitset(const std::size_t = 0);

	~Bitset() = default;

//-------------------------ELEMENT ACCESS-------------------------

	bool test(const std::size_t) const;

	bool all() const;//
	bool any() const;//
	bool none() const;//

	std::size_t count() const;//

	typename Bitset <Chunk_T, Allocator_T>::Iterator operator [] (const std::size_t);

	typename Bitset <Chunk_T, Allocator_T>::Iterator at(const std::size_t); //

	typename Bitset <Chunk_T, Allocator_T>::Iterator front(); //

	typename Bitset <Chunk_T, Allocator_T>::Iterator back(); //

	Chunk_T* data(); //

//-------------------------ITERATORS-------------------------

//-------------------------CAPACITY-------------------------

	std::size_t size() const;

	bool empty() const; //

	std::size_t max_size() const; //

	void reserve(const std::size_t); //

	std::size_t capacity() const; //

	void shrink_to_fit(); //

//-------------------------MODIFIERS-------------------------

	Bitset <Chunk_T, Allocator_T>& clear();

	Bitset <Chunk_T, Allocator_T>& set();
	Bitset <Chunk_T, Allocator_T>& set(const std::size_t, const bool = true);

	Bitset <Chunk_T, Allocator_T>& reset();
	Bitset <Chunk_T, Allocator_T>& reset(const std::size_t);

	Bitset <Chunk_T, Allocator_T>& flip();
	Bitset <Chunk_T, Allocator_T>& flip(const std::size_t);

	Bitset <Chunk_T, Allocator_T>& resize(const std::size_t = 0);
	Bitset <Chunk_T, Allocator_T>& resize(const std::size_t, const bool);

	Bitset <Chunk_T, Allocator_T>& operator &= (const Bitset <Chunk_T, Allocator_T>&);
	Bitset <Chunk_T, Allocator_T>& operator |= (const Bitset <Chunk_T, Allocator_T>&);
	Bitset <Chunk_T, Allocator_T>& operator ^= (const Bitset <Chunk_T, Allocator_T>&);
	Bitset <Chunk_T, Allocator_T> operator ~ () const;

	Bitset <Chunk_T, Allocator_T> operator << (const std::size_t) const;
	Bitset <Chunk_T, Allocator_T>& operator <<= (const std::size_t);
	Bitset <Chunk_T, Allocator_T> operator >> (const std::size_t) const;
	Bitset <Chunk_T, Allocator_T>& operator >>= (const std::size_t);

//-------------------------CONVERSIONS-------------------------

	template <typename Char_T, typename Traits_T, typename Alloc_T>
	std::basic_string <Char_T, Traits_T, Alloc_T> to_string(const Char_T, const Char_T) const;//

//-------------------------NON MEMBER FUNCTIOS-------------------------

	template <typename Chunk_FT, typename Allocator_FT, typename Char_T, typename Traits_T>
	friend std::basic_ostream <Char_T, Traits_T>& operator << (std::basic_ostream <Char_T, Traits_T>&, const Bitset <Chunk_FT, Allocator_FT>&);
};



//-------------------------class Iterator methods BEGIN-------------------------

template <typename Chunk_T, typename Allocator_T> //non pure
Bitset <Chunk_T, Allocator_T>::Iterator::Iterator(const std::size_t pos, Bitset <Chunk_T, Allocator_T> &array)
	:
	pos_(pos),
	array_(array),
	value_(array.test(pos))
{};

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>::Iterator::operator bool const() 
{
	return value_;
}

template <typename Chunk_T, typename Allocator_T> //non pure
typename Bitset <Chunk_T, Allocator_T>::Iterator& Bitset <Chunk_T, Allocator_T>::Iterator::operator = (const bool value)
{
	array_.set(pos_, value);

	return *this;
}

//-------------------------class Iterator methods END-------------------------


/*============================================================================
--------------------------class Bitset methods BEGIN--------------------------
============================================================================*/

template <typename Chunk_T, typename Allocator_T>
Bitset <Chunk_T, Allocator_T>::Bitset(const std::size_t size)
	:
	size_(size),
	chunk_size_(sizeof(Chunk_T) * CHAR_BIT),
	chunks_((size_ - 1) / chunk_size_ + 1, Allocator_T())
{};

template <typename Chunk_T, typename Allocator_T> //pure
std::size_t Bitset <Chunk_T, Allocator_T>::size() const
{
	return size_;
}

template <typename Chunk_T, typename Allocator_T>
template <typename Char_T, typename Traits_T, typename Alloc_T>
std::basic_string <Char_T, Traits_T, Alloc_T> Bitset<Chunk_T, Allocator_T>::to_string(const Char_T zero, const Char_T one) const
{
	std::basic_string <Char_T, Traits_T, Alloc_T> tmp(size_, zero);

	std::size_t pos = size_;
	for(auto chunk : chunks_)
		for(std::size_t jtr = chunk_size_; jtr-- > 0;)
			if(chunk & (static_cast <Chunk_T>(1) << jtr))
				Traits_T::assign(tmp[--pos], one);

	return tmp;
}

template <typename Chunk_T, typename Allocator_T> //pure
bool Bitset <Chunk_T, Allocator_T>::test(const std::size_t pos) const
{
	if(pos >= size_)
	{
		/*throw std::out_of_range(	"Bitset< " 
									+ std::string(abi::__cxa_demangle(typeid(Chunk_T).name(), 0, 0, 0)) 
									+ " , " 
									+ std::string(abi::__cxa_demangle(typeid(Allocator_T).name(), 0, 0, 0)) 
									+ " >::test(const std::size_t) : index = "
									+ std::to_string(pos)
									+ " vs size = "
									+ std::to_string(size_)
		);*/
		throw std::out_of_range(
									"bit::Bitset::test(): pos (which is " 
									+ std::to_string(pos) 
									+ ") > size() (which is " 
									+ std::to_string(size_) 
									+ ")"
		);
	}

	std::size_t chunk_pos = pos / chunk_size_;
	std::size_t bit_pos = pos % chunk_size_;

	if(chunks_[chunk_pos] & (static_cast <Chunk_T>(1) << bit_pos))
		return true;
	else
		return false;
}

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::set()
{
	for(auto &itr : chunks_)
		itr = ~static_cast <Chunk_T>(0);

	return *this;
}

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::set(const std::size_t pos, const bool value)
{
	std::size_t chunk_pos = pos / chunk_size_;
	std::size_t bit_pos = pos % chunk_size_;

	if(value)
		chunks_[chunk_pos] |= (static_cast <Chunk_T>(1) << bit_pos);
	else
		chunks_[chunk_pos] &= ~(static_cast <Chunk_T>(1) << bit_pos);

	return *this;
}

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::reset()
{
	for(auto &itr : chunks_)
		itr = 0;

	return *this;
}

template <typename Chunk_T, typename Allocator_T> //recursively pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::reset(const std::size_t pos)
{
	set(pos, 0);

	return *this;
}

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::flip()
{
	for(auto &itr : chunks_)
		itr = ~itr;

	return *this;
}

template <typename Chunk_T, typename Allocator_T> //pure 
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::flip(const std::size_t pos)
{
	std::size_t chunk_pos = pos / chunk_size_;
	std::size_t bit_pos = pos % chunk_size_;

	chunks_[chunk_pos] ^= (static_cast <Chunk_T>(1) << bit_pos);

	return *this;
}

template <typename Chunk_T, typename Allocator_T> //pure
typename Bitset <Chunk_T, Allocator_T>::Iterator Bitset <Chunk_T, Allocator_T>::operator [] (const std::size_t pos)
{
	return Iterator(pos, *this);
}

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::resize(const std::size_t size)
{
	size_ = size;
	chunks_.resize((size_ - 1) / chunk_size_ + 1);
}

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::resize(const std::size_t size, const bool value)
{
	size_ = size;
	chunks_.resize((size_ - 1) / chunk_size_ + 1, value ? ~static_cast <Chunk_T>(0) : 0);
}

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::clear()
{
	size_ = 0;
	chunks_.clear();
}

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::operator &= (const Bitset <Chunk_T, Allocator_T> &rhs)
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

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::operator |= (const Bitset <Chunk_T, Allocator_T> &rhs)
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

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::operator ^= (const Bitset <Chunk_T, Allocator_T> &rhs)
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

template <typename Chunk_T, typename Allocator_T> //pure
Bitset <Chunk_T, Allocator_T> Bitset <Chunk_T, Allocator_T>::operator ~ () const
{
	Bitset <Chunk_T, Allocator_T> flip(size_);

	for(std::size_t itr = 0; itr < chunks_.size(); ++itr)
		flip.chunks_[itr] = ~chunks_[itr];

	return flip;
}

template <typename Chunk_T, typename Allocator_T>
Bitset <Chunk_T, Allocator_T> Bitset <Chunk_T, Allocator_T>::operator << (const std::size_t pos) const
{
	//shift chunks to right (increase value)
	if(!pos)
		return *this;

	if(pos >= size_)
		return Bitset <Chunk_T, Allocator_T>(size_);

	Bitset <Chunk_T, Allocator_T> shifted(size_); //empty bitset

	const std::size_t left_offset = pos % chunk_size_; 				//left part (smaller) of the chunk will become the right (larger) part of the new chunk
	const std::size_t right_offset = chunk_size_ - left_offset; 	//right part (larger) of the chunk will become the left (smallest) part of the new chunk
	const std::size_t chunk_offset = pos / chunk_size_;				//first chunk to be set in the new bitset

	if(pos % chunk_size_)
	{
		//we will set every chunk AFTER chunk_offset, we will deal with chunk_offset separately as it will only need part of it set
		for(std::size_t itr = chunks_.size() - 1; itr > chunk_offset; --itr)
			shifted.chunks_[itr] = (chunks_[itr - chunk_offset] << left_offset) | (chunks_[itr - chunk_offset - 1] >> right_offset);

		shifted.chunks_[chunk_offset] = chunks_[0] << left_offset;
	}
	else
	{
		//full chunk shift
		for(std::size_t itr = chunks_.size() - 1; itr >= chunk_offset; --itr)
			shifted.chunks_[itr] = chunks_[itr - chunk_offset];
	}

	return shifted;
}

template <typename Chunk_T, typename Allocator_T>
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::operator <<= (const std::size_t pos)
{
	if(!pos)
		return *this;

	if(pos >= size_)
	{
		std::fill(chunks_.begin(), chunks_.end(), static_cast <Chunk_T>(0));
		return *this;
	}

	const std::size_t left_offset = pos % chunk_size_; 				//left part (smaller) of the chunk will become the right (larger) part of the new chunk
	const std::size_t right_offset = chunk_size_ - left_offset; 	//right part (larger) of the chunk will become the left (smallest) part of the new chunk
	const std::size_t chunk_offset = pos / chunk_size_;				//first chunk to be set in the new bitset

	if(pos % chunk_size_)
	{
		//we will set every chunk AFTER chunk_offset, we will deal with chunk_offset separately as it will only need part of it set
		for(std::size_t itr = chunks_.size() - 1; itr > chunk_offset; --itr)
			chunks_[itr] = (chunks_[itr - chunk_offset] << left_offset) | (chunks_[itr - chunk_offset - 1] >> right_offset);

		chunks_[chunk_offset] = chunks_[0] << left_offset;
	}
	else
	{
		//full chunk shift
		for(std::size_t itr = chunks_.size() - 1; itr >= chunk_offset; --itr)
			chunks_[itr] = chunks_[itr - chunk_offset];
	}

	std::fill(chunks_.begin(), chunks_.begin() + chunk_offset, static_cast <Chunk_T>(0));

	return *this;
}

template <typename Chunk_T, typename Allocator_T>
Bitset <Chunk_T, Allocator_T> Bitset <Chunk_T, Allocator_T>::operator >> (const std::size_t pos) const
{
	//shift chunks to left (decrease value)
	if(!pos)
		return *this;

	if(pos >= size_)
		return Bitset <Chunk_T, Allocator_T>(size_);

	Bitset <Chunk_T, Allocator_T> shifted(size_); //empty bitset

	const std::size_t left_offset = pos % chunk_size_; 				//left part (smaller) of the chunk will become the right (larger) part of the new chunk
	const std::size_t right_offset = chunk_size_ - left_offset; 	//right part (larger) of the chunk will become the left (smallest) part of the new chunk
	const std::size_t chunk_offset = pos / chunk_size_;				//last chunk to be set in the new bitset

	if(pos % chunk_size_)
	{
		//we will set every chunk AFTER chunk_offset, we will deal with chunk_offset separately as it will only need part of it set
		for(std::size_t itr = 0; itr < chunks_.size() - chunk_offset - 1; ++itr)
			shifted.chunks_[itr] = (chunks_[itr + chunk_offset] >> left_offset) | (chunks_[itr + chunk_offset + 1] << right_offset);

		shifted.chunks_[chunks_.size() - chunk_offset - 1] = chunks_[chunks_.size() - 1] >> left_offset;
	}
	else
	{
		//full chunk shift
		for(std::size_t itr = 0; itr <= chunks_.size() - chunk_offset - 1; ++itr)
			shifted.chunks_[itr] = chunks_[itr + chunk_offset];
	}

	return shifted;
}

template <typename Chunk_T, typename Allocator_T>
Bitset <Chunk_T, Allocator_T>& Bitset <Chunk_T, Allocator_T>::operator >>= (const std::size_t pos)
{
	//shift chunks to left (decrease value)
	if(!pos)
		return *this;

	if(pos >= size_)
	{
		std::fill(chunks_.begin(), chunks_.end(), static_cast <Chunk_T>(0));
		return *this;
	}

	const std::size_t left_offset = pos % chunk_size_; 				//left part (smaller) of the chunk will become the right (larger) part of the new chunk
	const std::size_t right_offset = chunk_size_ - left_offset; 	//right part (larger) of the chunk will become the left (smallest) part of the new chunk
	const std::size_t chunk_offset = pos / chunk_size_;				//last chunk to be set in the new bitset

	if(pos % chunk_size_)
	{
		//we will set every chunk AFTER chunk_offset, we will deal with chunk_offset separately as it will only need part of it set
		for(std::size_t itr = 0; itr < chunks_.size() - chunk_offset - 1; ++itr)
			chunks_[itr] = (chunks_[itr + chunk_offset] >> left_offset) | (chunks_[itr + chunk_offset + 1] << right_offset);

		chunks_[chunks_.size() - chunk_offset - 1] = chunks_[chunks_.size() - 1] >> left_offset;
	}
	else
	{
		//full chunk shift
		for(std::size_t itr = 0; itr <= chunks_.size() - chunk_offset - 1; ++itr)
			chunks_[itr] = chunks_[itr + chunk_offset];
	}

	std::fill(chunks_.begin() + chunks_.size() - chunk_offset, chunks_.end(), static_cast <Chunk_T>(0));

	return *this;
}

template <typename Chunk_T = unsigned long long, typename Allocator_T = std::allocator <Chunk_T>, typename Char_T, typename Traits_T> //pure
std::basic_ostream <Char_T, Traits_T>& operator << (std::basic_ostream <Char_T, Traits_T>&out, const Bitset <Chunk_T, Allocator_T> &mask)
{
	if(!mask.size_)
		return out;

	Char_T zero = std::use_facet <std::ctype <Char_T>>(out.getloc()).widen('0');
	Char_T one = std::use_facet <std::ctype <Char_T>>(out.getloc()).widen('1');

	return out << mask.template to_string <Char_T, Traits_T, std::allocator <Char_T>> (zero, one);
}

//-------------------------class Bitset methods END-------------------------

}