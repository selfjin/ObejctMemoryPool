#pragma once
#include <stddef.h>
#include <Windows.h>

//////////////////////////////////////////////////

// 1. DEBUG 빌드일 때, 메모리 범퍼(GUARD) 추가


#define GUARD1 0xFFFFFFFF
#define GUARD2 0xFFFFFFFF

//////////////////////////////////////////////////

template <typename T>
struct ObjectPoolNode
{
#ifdef _DEBUG
	int guard1 = 0xffffffff;
	T _data;
	int guard2 = 0xffffffff;
	ObjectPoolNode* next;
#else
	T _data;
	ObjectPoolNode* next;
#endif
	
};



template <typename T>
class ObjectMemoryPool
{
public:
	ObjectMemoryPool(int capacity);
	~ObjectMemoryPool();

	// Not constructor, Only Memory
	T* Alloc();

	// Auto constructor
	T* Alloc_Constructor();


	// Not destructor, Only Memory return
	bool Free(T* object);

	bool Free_Destructor(T* object);

	// Auto 

	int getSize() { return _size; }

private:
	ObjectPoolNode<T> _top;
	int _size;
	int _availableSize;

	/////////////////////////////////////////////

	

	/////////////////////////////////////////////
};

