#include "ObjectMemoryPool.h"



template<typename T>
inline ObjectMemoryPool<T>::ObjectMemoryPool(int initCapacity) : _top(nullptr), _size(0)
{
	ObjectPoolNode* pNode = new ObjectPoolNode<T>;
	pNode->next = nullptr;
	_top = pNode;
	
	_size++;
	_availableSize++;
	for (int i = 0; i < initCapacity - 1; i++)
	{
		ObjectPoolNode* newNode = new ObjectPoolNode<T>;
		
		newNode->next = _top;
		_top = newNode;
		_size++;
		_availableSize++;
	}

}

template<typename T>
T* ObjectMemoryPool<T>::Alloc()
{
	if (_availableSize == 0)
	{
		ObjectPoolNode* newNode = new ObjectPoolNode<T>;
		
		newNode->next = nullptr;
		
		_top = newNode;
		_size++;
		return &(newNode->data);
	}
	else
	{
		T* returnPtr = &(_top->_data);

		_top = _top->next;
		_availableSize--;

		return returnPtr;
	}
}

template<typename T>
T* ObjectMemoryPool<T>::Alloc_Constructor()
{
	if (_availableSize == 0)
	{
		ObjectPoolNode* newNode = new ObjectPoolNode<T>;

		newNode->next = nullptr;

		_top = newNode;
		_size++;
		return &(newNode->data);
	}
	else
	{
		T* returnPtr =  new (&(_top->_data)) T;

		_top = _top->next;
		_availableSize--;

		return returnPtr;
	}
}

template<typename T>
bool ObjectMemoryPool<T>::Free(T* object)
{	

#ifdef _DEBUG
	// object는 _data를 가리키고 있으므로 ObjectPoolNode<T>로 변환
	ObjectPoolNode<T>* node = reinterpret_cast<ObjectPoolNode<T>*>(reinterpret_cast<char*>(object) - offsetof(ObjectPoolNode<T>, _data));

	// 가드 값 검사
	if (node->guard1 != GUARD1 || node->guard2 != GUARD2)
	{
		DebugBreak(); // 가드 값이 맞지 않으면 디버그 브레이크
		return false;
	}

#else
	// RELEASE 모드에서 object는 _data를 가리킴
	ObjectPoolNode<T>* node = reinterpret_cast<ObjectPoolNode<T>*>(reinterpret_cast<char*>(object) - offsetof(ObjectPoolNode<T>, _data));

	// object의 메모리를 풀이 가리키는 _top으로 돌려놓음
	node->next = _top;
	_top = node;

#endif
}


