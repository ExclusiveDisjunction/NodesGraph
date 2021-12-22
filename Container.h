#pragma once

#include <iostream>

#ifndef Aaron_Container
#define Aaron_Container

template<typename T>
class VectorIterator;
template<typename T>
class Vector;

template<typename T>
class ContainerSegment
{
private:
	ContainerSegment() : Next(nullptr), Last(nullptr) {}
	ContainerSegment(const T& Value) : Value(Value), Next(nullptr), Last(nullptr) {}

	ContainerSegment<T>* Next = nullptr;
	ContainerSegment<T>* Last = nullptr;

	T Value = T();

	friend Vector<T>;
	friend VectorIterator<T>;
};

using uint = unsigned int;

template<typename T>
class VectorIterator
{
private:
	ContainerSegment<T>* Current;
public:
	using iterator_category = std::forward_iterator_tag;
	using diffrence_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T&;

	VectorIterator(ContainerSegment<T>* This) : Current(This) {}

	reference operator*() const { return Current->Value; }
	pointer operator->() { return &Current->Value; }

	// Prefix increment
	VectorIterator& operator++() { Current = Current->Next; return *this; }

	// Postfix increment
	VectorIterator operator++(int) { VectorIterator tmp = *this; ++(*this); return tmp; }

	friend bool operator== (const VectorIterator& a, const VectorIterator& b) { return a.Current == b.Current; };
	friend bool operator!= (const VectorIterator& a, const VectorIterator& b) { return a.Current != b.Current; };
};

template<typename T>
class Vector
{
private:
	using ThisType = Vector<T>;
	using seg = ContainerSegment<T>*;
	using uint = unsigned int;

	seg First;
	seg Last;
	uint _Size;

	seg GetAtIndex(uint Index) const
	{
		if (Index >= _Size)
			return nullptr;

		if (Index == 0 && _Size >= 1)
			return First;
		else if (Index == (_Size - 1) && _Size >= 1)
			return Last;

		Index++;
		seg Current = First;
		for (uint i = 1; i < Index; i++)
			Current = Current->Next;

		return Current;
	}
	seg AddNewSegment(const T& Value)
	{
		if (_Size == 0)
		{
			First = Last = new ContainerSegment<T>(Value);
			_Size++;
			return First;
		}
		else if (_Size == 1)
		{
			Last = new ContainerSegment<T>(Value);
			First->Next = Last;
			Last->Last = First;

			_Size++;
			return Last;
		}

		seg Last = this->Last;
		seg New = new ContainerSegment<T>(Value);

		New->Last = Last;

		if (_Size == 0)
			First = New;
		else
			Last->Next = New;

		_Size++;
		this->Last = New;
		return New;
	}

public:
	Vector()
	{
		First = 0;
		Last = 0;
		_Size = 0;
	}
	Vector(const Vector<T>& Other) : Vector()
	{
		for (uint i = 0; i < Other._Size; i++)
			this->Add(Other[i]);
	}
	Vector(const T* List, uint Len) : Vector()
	{
		for (uint i = 0; i < Len; i++)
			this->Add(List[i]);
	}
	Vector(const std::initializer_list<T>& List) : Vector()
	{
		for (T Item : List)
			this->Add(Item);
	}
	Vector(uint Len) : Vector()
	{
		for (uint i = 0u; i < Len; i++)
			Add(T());
	}
	Vector(T& Obj) : Vector()
	{
		AddNewSegment(Obj);
	}
	Vector(const T& Obj) : Vector()
	{
		AddNewSegment(Obj);
	}
	Vector(const T& Obj, uint Len) : Vector()
	{
		if (Len == 0)
			return;

		seg Current = First = new ContainerSegment<T>(Obj);
		for (uint i = 1; i < Len; i++)
		{
			seg Next = new ContainerSegment<T>(Obj);
			Current->Next = Next;
			Next->Last = Current;
			Current = Next;
		}

		Last = Current;
	}
	~Vector()
	{
		Clear();
	}

#ifdef _VECTOR_
	Vector(const std::vector<T>& Other) : Vector()
	{
		for (T&& Obj : Other)
			Add(Obj);
	}
#endif

	T& operator[](uint Index) const
	{
		if (Index >= _Size)
			throw "Out of range";

		return GetAtIndex(Index)->Value;
	}
	const uint& Size = _Size;

	void Add(const T& Obj)
	{
		AddNewSegment(Obj);
	}
	void Add(const Vector<T>& Other)
	{
		for (uint i = 0; i < Other._Size; i++)
			this->AddNewSegment(Other[i]);
	}
	void InsertAt(uint Index, const T& Value)
	{
		if (Index >= _Size)
			return;

		seg New = new ContainerSegment<T>();
		New->Value = Value;

		if (Index == 0)
		{
			New->Next = First;
			New->Last = nullptr;
			First->Last = New;

			First = New;
			_Size++;

			return;
		}

		seg At = GetAtIndex(Index);
		if (At->Next == nullptr)
		{
			At->Next = New;
			New->Last = At;
			New->Next = nullptr;

			_Size++;
			return;
		}

		seg Next = At->Next;
		At->Next = New;
		Next->Last = New;

		New->Next = Next;
		New->Last = At;
		_Size++;
	}
	bool Remove(const T& Obj)
	{
		return RemoveAt(IndexOf(Obj));
	}
	bool RemoveAt(uint Index)
	{
		if (Index >= _Size)
			return false;

		if (Index == 0) //Delete First Index
		{
			seg Second = First->Next;
			delete First;
			if (Second)
				Second->Last = 0;

			First = Second;
			_Size--;
			if (_Size == 0)
				First = Last = nullptr;
			return true;
		}
		else if (Index == (_Size - 1)) //Delete Last Index
		{
			seg SecondLast = Last->Last;
			delete Last;
			if (SecondLast)
				SecondLast->Next = 0;

			Last = SecondLast;
			_Size--;

			if (_Size == 0)
				First = Last = nullptr;
			return true;
		}

		seg Target = GetAtIndex(Index);
		seg Next = Target->Next;
		seg Last = Target->Last;

		if (Last)
			Last->Next = Next;
		Next->Last = Last;

		delete Target;
		_Size--;

		if (_Size == 0)
			First = this->Last = nullptr;

		return true;
	}
	bool RemoveAllOf(T& Condition)
	{
		seg Current = First;
		bool Return = true;
		for (uint i = 0; i < _Size && Current; i++)
		{
			if (Current->Value == Condition)
			{
				seg Last = Current->Last;
				seg Next = Current->Next;

				delete Current;
				if (Last == nullptr)
					First = Next;
				else if (Next == nullptr)
					this->Last = Last;

				if (Last)
					Last->Next = Next;
				if (Next)
					Next->Last = Last;

				Current = Next;
				_Size--;
				continue;
			}
			Current = Current->Next;
		}

		return Return;
	}
	bool RemoveDuplicates()
	{
		Vector<T> Refined;
		for (uint i = 0; i < _Size; i++)
		{
			T& Item = operator[](i);
			if (!Refined.Contains(Item))
				Refined.Add(Item);
		}

		this->operator=(Refined);

		return true;
	}

	bool Contains(const T& Obj) const
	{
		seg Current = First;

		for (uint i = 0; i < _Size; i++)
		{
			if (Obj == Current->Value)
				return true;

			Current = Current->Next;
		}

		return false;
	}
	template<typename Condition>
	bool Search(Condition Cond) const
	{
		if (_Size == 0)
			return false;
		else if (_Size == 1)
			return Cond(First->Value);

		for (seg Current = First; Current != nullptr; Current = Current->Next)
		{
			if (Cond(Current->Value))
				return true;
		}

		return false;
	}
	template<typename Condition>
	T Find(Condition Cond) const
	{
		if (_Size == 0)
			return T();

		if (_Size == 1)
		{
			if (Cond(First->Value))
				return First->Value;
			else
				return T();
		}

		seg Current = First;
		for (uint i = 0; i < _Size; i++)
		{
			if (Cond(Current->Value))
				return Current->Value;

			Current = Current->Next;
		}

		return T();
	}
	uint IndexOf(const T& Obj) const
	{
		for (uint i = 0; i < _Size; i++)
		{
			if (Obj == operator[](i))
				return i;
		}

		return (uint)4294967295;
	}

	void Resize(uint NewSize)
	{
		if (NewSize == 0)
		{
			Clear();
			return;
		}
		
		if (NewSize == _Size)
			return;

		Clear();
		seg Current = First = new ContainerSegment<T>(), Last = nullptr;
		for (uint i = 1; i < NewSize; i++)
		{
			seg This = new ContainerSegment<T>();
			if (Current)
				Current->Next = This;
			if (Last)
				Last->Next = Current;
			This->Last = Current;

			Last = Current;
			Current = This;
		}
		this->Last = Current;
		_Size = NewSize;
	}
	void Reverse()
	{
		if (_Size == 0 || _Size == 1) //For Single Size lists, and empty lists, do nothing.
			return;
		if (_Size == 2)
		{
			seg Temp = First;
			First = Last;
			Last = Temp;

			First->Last = nullptr;
			First->Next = Last;
			Last->Next = nullptr;
			Last->Last = First;

			return;
		}
		
		seg LastItem = nullptr;
		seg NewCurrent = nullptr;
		seg Current = Last;

		for (uint i = 0; i < _Size; i++)
		{
			seg Temp = NewCurrent;
			NewCurrent = new ContainerSegment<T>(Current->Value);
			LastItem = Temp;

			if (LastItem)
				LastItem->Next = NewCurrent;
			NewCurrent->Last = LastItem;

			Current = Current->Last;
		}

		Last = NewCurrent;

		Current = NewCurrent;
		for (uint i = 0; i < _Size-1; i++)
			Current = Current->Last;

		First = Current;
	}
	void Clear()
	{
		if (_Size == 0 || !First || !Last)
			return;

		if (_Size == 1)
		{
			delete First;

			_Size = 0;
			First = nullptr;
			Last = nullptr;
			return;
		}
		else if (_Size == 2)
		{
			delete First;
			delete Last;

			_Size = 0;
			First = nullptr;
			Last = nullptr;
			return;
		}

		seg Current = this->First;
		for (uint i = 0; i < _Size; i++)
		{
			seg Temp = Current->Next;
			delete Current;
			Current = Temp;
		}

		_Size = 0;
		First = nullptr;
		Last = nullptr;
	}
	bool IsEmpty() const
	{
		return _Size == 0;
	}

	Vector<T> SubVector(uint BeginIndex) const
	{
		if (_Size == 0)
			return Vector<T>();

		return SubVector(BeginIndex, _Size - 1);
	}
	Vector<T> SubVector(uint BeginIndex, uint EndIndex) const
	{
		if (_Size == 0 || BeginIndex > EndIndex)
			return Vector<T>();

		if (BeginIndex >= _Size || EndIndex >= _Size)
			return Vector<T>(*this);

		uint Size = (EndIndex - BeginIndex) + 1;

		seg Begin = GetAtIndex(BeginIndex);

		seg NewFirst = new ContainerSegment<T>();
		NewFirst->Next = NewFirst->Last = 0;
		NewFirst->Value = Begin->Value;
		seg Last = NewFirst;
		seg Current = 0;
		for (uint i = 1, j = BeginIndex + 1; i < Size; i++, j++)
		{
			Current = new ContainerSegment<T>();
			Current->Last = Last;
			Current->Next = 0;
			Current->Value = operator[](j);
			Last->Next = Current;

			Last = Current;
		}

		Vector<T> Return = Vector<T>();
		Return._Size = Size;
		Return.First = NewFirst;
		Return.Last = Current;

		return Return;
	}
	template<typename ReturnT, typename Conversion>
	Vector<ReturnT> ConvertVector(Conversion Conv)
	{
		Vector<ReturnT> Return;
		seg Current = First;
		for (uint i = 0; i < _Size; i++)
		{
			Return.Add(Conv(Current->Value));
			Current = Current->Next;
		}
	}

	VectorIterator<T> begin() { return !First ? end() : VectorIterator<T>(First); }
	VectorIterator<T> end() { return VectorIterator<T>(!Last ? nullptr : Last->Next); }
	const VectorIterator<T> begin() const { return !First ? end() : VectorIterator<T>(First); }
	const VectorIterator<T> end() const { return VectorIterator<T>(!Last ? nullptr : Last->Next); }

	Vector<T>& operator=(Vector<T>& Other)
	{
		if (this->_Size != 0)
			Clear();

		if (Other._Size > 0)
		{
			for (uint i = 0; i < Other._Size; i++)
				Add(Other[i]);
		}

		return *this;
	}
	Vector<T>& operator=(const Vector<T>& Other)
	{
		if (this->_Size != 0)
			Clear();

		if (Other._Size > 0)
		{
			for (uint i = 0; i < Other._Size; i++)
				Add(Other[i]);
		}

		return *this;
	}
	Vector<T>& operator=(const T& Obj)
	{
		Clear();
		AddNewSegment(Obj);

		return *this;
	}

	bool operator==(const Vector<T>& Other) const
	{
		Vector<T> One = *this;
		Vector<T> Two = Other;

		if (One._Size != Two._Size)
			return false;

		if (One._Size == 0 && Two._Size == 0)
			return true;

		bool Return = true;
		for (unsigned int i = 0; i < One._Size; i++)
			Return &= One[i] == Two[i];

		return Return;
	}
	bool operator!=(const Vector<T>& Other) const
	{
		Vector<T> One = *this;
		Vector<T> Two = Other;

		return !(One == Two);
	}

#ifdef _VECTOR_
	operator std::vector<T>() const
	{
		std::vector<T> Return;
		for (uint i = 0; i < _Size; i++)
			Return.push_back(operator[](i));

		return Return;
	}

	Vector<T>& operator=(const std::vector<T>& Other)
	{
		this->Clear();

		for (T Item : Other)
			this->Add(Item);

		return *this;
	}
#endif
};

using IntList = Vector<int>;
using LongList = Vector<long long>;
template<typename T>
using List = Vector<T>;

#endif