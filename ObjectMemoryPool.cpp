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
	// object�� _data�� ����Ű�� �����Ƿ� ObjectPoolNode<T>�� ��ȯ
	ObjectPoolNode<T>* node = reinterpret_cast<ObjectPoolNode<T>*>(reinterpret_cast<char*>(object) - offsetof(ObjectPoolNode<T>, _data));

	// ���� �� �˻�
	if (node->guard1 != GUARD1 || node->guard2 != GUARD2)
	{
		DebugBreak(); // ���� ���� ���� ������ ����� �극��ũ
		return false;
	}

#else
	// RELEASE ��忡�� object�� _data�� ����Ŵ
	ObjectPoolNode<T>* node = reinterpret_cast<ObjectPoolNode<T>*>(reinterpret_cast<char*>(object) - offsetof(ObjectPoolNode<T>, _data));

	// object�� �޸𸮸� Ǯ�� ����Ű�� _top���� ��������
	node->next = _top;
	_top = node;

#endif
}


