#pragma once
#include <iostream>
#include <fstream>
#include "exception.h"
#include <string>
using namespace std;
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>
template <class T>
class DynamicArray
{
private:
	T* _arr;
	int _logicalSize;
	int _physicalSize;


public:
	DynamicArray(int size = 2) : _logicalSize(0), _physicalSize(size)
	{
		if (size > 0)
		_arr = new T[size];
		else
		_arr = nullptr;
	
	}

	DynamicArray(const DynamicArray& other) : _arr(nullptr) {
		*this = other;
	}
	~DynamicArray() {
		delete[] _arr;
	}

	const DynamicArray& operator=(const DynamicArray& other) {
		if (this != &other) {
			_logicalSize = other._logicalSize;
			_physicalSize = other._physicalSize;
			delete[] _arr;
			_arr = new T[_physicalSize];
			for (int i = 0; i < _logicalSize; i++)
				_arr[i] = other._arr[i];
		}
		return *this;
	}

	const T& operator[](int i) const { return _arr[i]; }
	T& operator[](int i) { return _arr[i]; }

	void push_back(const T& value) {
		if (_logicalSize == _physicalSize)
			resize();
		_arr[_logicalSize++] = value;
	}

	const T& front()    const { return _arr[0]; }
	int      size()     const { return _logicalSize; }
	int      capacity() const { return _physicalSize; }
	bool     empty()    const { return _logicalSize == 0; }
	void     clear() { _logicalSize = 0; }

	class iterator
	{
	private:
		DynamicArray* _da;
		int				_i;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using different_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		iterator(DynamicArray& arr, int i) : _da(&arr), _i(i) {}
		iterator(const iterator& other) : _da(other._da), _i(other._i) {}
		friend class const_iterator;

		const iterator& operator=(const iterator& other) {
			_da = other._da;
			_i = other._i;
			return *this;
		}


		// comparison with another iterator
		bool operator==(const iterator& other) const {
			return (_da == other._da) && (_i == other._i);
		}
		bool operator!=(const iterator& other) const {
			return !(*this == other);
		}

		// smart-pointer iterator methods
		T& operator*() {
			return _da->_arr[_i];
		}
		T* operator->() {
			return &_da->_arr[_i];
		}

		// increment-decrement iterator methods
		iterator& operator++() {
			++_i;
			return *this;
		}
		iterator operator++(int) {
			iterator temp(*this);
			++_i;
			return temp;
		}
		iterator& operator--() {
			--_i;
			return *this;
		}
		iterator operator--(int) {
			iterator temp(*this);
			--_i;
			return temp;
		}
	};

//------------------------------------------------------------------------------------------
	class const_iterator
	{
	private:
		const DynamicArray* _da;
		int				_i;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using different_type = std::ptrdiff_t;
		using value_type = const T;//
		using pointer = T*;
		using reference = const T&;//

		const_iterator(const DynamicArray& arr, int i) : _da(&arr), _i(i) {}
		const_iterator(const const_iterator& other) : _da(other._da), _i(other._i) {}
		const_iterator(const iterator& other) : _da(other._da), _i(other._i) {}

		const const_iterator& operator=(const iterator& other) {
			_da = other._da;
			_i = other._i;
			return *this;
		}
		const const_iterator& operator=(const const_iterator& other) {
			_da = other._da;
			_i = other._i;
			return *this;
		}

		// comparison with another iterator
		bool operator==(const const_iterator& other) const {
			return (_da == other._da) && (_i == other._i);
		}
		bool operator!=(const const_iterator& other) const {
			return !(*this == other);
		}

		// smart-pointer iterator methods
		const T& operator*() {
			return _da->_arr[_i];
		}
		T* operator->() {
			return &_da->_arr[_i];
		}

		// increment-decrement iterator methods
		const_iterator& operator++() {
			++_i;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator temp(*this);
			++_i;
			return temp;
		}
		const_iterator& operator--() {
			--_i;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator temp(*this);
			--_i;
			return temp;
		}
	};


	//------------------------------------------------------------------------------------------
	
	class reverse_iterator 
	{
	private:
		DynamicArray* _da;
		int	_i;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using different_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		reverse_iterator(DynamicArray& arr, int i) : _da(&arr), _i(i) {}
		reverse_iterator(const reverse_iterator& other) : _da(other._da), _i(other._i) {}



		const reverse_iterator& operator=(const reverse_iterator& other) {
			_da = other._da;
			_i = other._i;
			return *this;
		}

		// comparison with another iterator
		bool operator==(const reverse_iterator& other) const {
			return (_da == other._da) && (_i == other._i);
		}
		bool operator!=(const reverse_iterator& other) const {
			return !(*this == other);
		}

		// smart-pointer iterator methods
		T& operator*() {
			return _da->_arr[_i];
		}
		T* operator->() {
			return &_da->_arr[_i];
		}

		// increment-decrement iterator methods
		reverse_iterator& operator++() {
			--_i;
			return *this;
		}
		reverse_iterator operator++(int) {
			reverse_iterator temp(*this);
			--_i;
			return temp;
		}
		reverse_iterator& operator--() {
			++_i;
			return *this;
		}
		reverse_iterator operator--(int) {
			reverse_iterator temp(*this);
			++_i;
			return temp;
		}

	}; 



	//------------------------------------------------------------------------------------------





	void print() const {
		for (int i = 0; i < _logicalSize; i++)
			cout << _arr[i] << " ";
		cout << endl;
	}
	friend ostream& operator<<(ostream& os, const DynamicArray& arr)
	{
		for (int i = 0; i < arr.size(); i++)
			os <<i<<". " <<arr[i];
		return os;
	}

	void insert(const iterator& pos, const T& val) {
		if (_logicalSize == _physicalSize)
			resize();

		iterator itrEnd = end();
		iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
		while (itrCurrent != pos)
		{
			*itrCurrent = *itrPrev;
			itrCurrent = itrPrev--;
		}

		iterator p = pos;
		*p = val;
		++_logicalSize;
	}

	const iterator& erase(const iterator& iter)
	{

		iterator itrbeg = begin();
		iterator itrnext = ++begin();

		while (itrbeg != iter && itrbeg != end())
		{
			itrbeg = itrnext++;
		}
		if (itrbeg == iter)
		{
			while (itrnext != end())
			{
				*itrbeg = *itrnext;
				itrbeg++;
				itrnext++;
			}
			_logicalSize--;
		}
		else
		{
			return begin();
		}
		itrbeg = iter;
		itrbeg--;
		return (itrbeg);
	}
	const iterator& erase(const iterator& first, const iterator& last)
	{
		int counter = 0;
		iterator itrfirst = first;
		iterator itrlast = last;
		while (itrlast != end())
		{
		
			*itrfirst = *itrlast;
			++itrfirst;
			++itrlast;
		}
		for(iterator itr=first;itr!=last;itr++)
				counter++;
		_logicalSize = _logicalSize - counter;
		itrfirst = first;
		itrfirst--;
		return (itrfirst);
	}
	void save(ostream& out) 
	{
		
		out.write(rcastcc(&_logicalSize), sizeof(_logicalSize));
		if (!out.good())
			throw Saveexception();
		for (auto itr = begin(); itr != end(); ++itr)
		{
			(*itr).save(out);
		}
	}


private:
	void resize() {
		_physicalSize *= 2;
		T* temp = new T[_physicalSize];
		for (int i = 0; i < _logicalSize; i++)
			temp[i] = _arr[i];

		delete[] _arr;
		_arr = temp;
	}
		public:
	iterator begin() {
		return iterator(*this, 0);
	}
	iterator end() {
		return iterator(*this, _logicalSize);
	}
	reverse_iterator rbegin() {
		return reverse_iterator(*this, _logicalSize);
	}
	reverse_iterator rend() {
		return reverse_iterator(*this, 0);
	}
	iterator cbegin() {
		return  const_iterator(*this, 0);
	}
	iterator cend() {
		return  const_iterator(*this, _logicalSize);
	}

};

