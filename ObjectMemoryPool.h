//#pragma once
//#include <stddef.h>
//#include <Windows.h>
//
//
//
////////////////////////////////////////////////////
//
//// 1. DEBUG ������ ��, �޸� ����(GUARD) �߰�
//
//
//#define GUARD1 0xFFFFFFFF
//#define GUARD2 0xFFFFFFFF
//
////////////////////////////////////////////////////
//
//template <typename T>
//struct ObjectPoolNode
//{
//#ifdef _DEBUG
//	int guard1 = 0xffffffff;
//	T _data;
//	int guard2 = 0xffffffff;
//	ObjectPoolNode* next;
//#else
//	T _data;
//	ObjectPoolNode* next;
//#endif
//	
//};
//
//
//
//template <typename T>
//class ObjectMemoryPool
//{
//public:
//	ObjectMemoryPool(int capacity);
//	~ObjectMemoryPool();
//
//	// Not constructor, Only Memory
//	T* Alloc();
//
//	// Auto constructor
//	T* Alloc_Constructor();
//
//
//	// Not destructor, Only Memory return
//	bool Free(T* object);
//
//	bool Free_Destructor(T* object);
//
//	// Auto 
//
//	int getSize() { return _size; }
//
//private:
//	ObjectPoolNode<T> _top;
//	int _size;
//	int _availableSize;
//
//	/////////////////////////////////////////////
//
//	
//
//	/////////////////////////////////////////////
//};
//
//
//
////////////////////////////			������		///////////////////////////////////////
//
//
//
//template<typename T>
//ObjectMemoryPool<T>::ObjectMemoryPool(int initCapacity) : _top(nullptr), _size(0)
//{
//	ObjectPoolNode* pNode = new ObjectPoolNode<T>;
//	pNode->next = nullptr;
//	_top = pNode;
//
//	_size++;
//	_availableSize++;
//	for (int i = 0; i < initCapacity - 1; i++)
//	{
//		ObjectPoolNode* newNode = new ObjectPoolNode<T>;
//
//		newNode->next = _top;
//		_top = newNode;
//		_size++;
//		_availableSize++;
//	}
//
//}
//
//template<typename T>
//T* ObjectMemoryPool<T>::Alloc()
//{
//	if (_availableSize == 0)
//	{
//		ObjectPoolNode* newNode = new ObjectPoolNode<T>;
//
//		newNode->next = nullptr;
//
//		_top = newNode;
//		_size++;
//		return &(newNode->data);
//	}
//	else
//	{
//		T* returnPtr = &(_top->_data);
//
//		_top = _top->next;
//		_availableSize--;
//
//		return returnPtr;
//	}
//}
//
//template<typename T>
//T* ObjectMemoryPool<T>::Alloc_Constructor()
//{
//	if (_availableSize == 0)
//	{
//		ObjectPoolNode* newNode = new ObjectPoolNode<T>;
//
//		newNode->next = nullptr;
//
//		_top = newNode;
//		_size++;
//		return &(newNode->data);
//	}
//	else
//	{
//		T* returnPtr = new (&(_top->_data)) T;
//
//		_top = _top->next;
//		_availableSize--;
//
//		return returnPtr;
//	}
//}
//
//template<typename T>
//bool ObjectMemoryPool<T>::Free(T* object)
//{
//
//#ifdef _DEBUG
//	// object�� _data�� ����Ű�� �����Ƿ� ObjectPoolNode<T>�� ��ȯ
//	ObjectPoolNode<T>* node = reinterpret_cast<ObjectPoolNode<T>*>(reinterpret_cast<char*>(object) - offsetof(ObjectPoolNode<T>, _data));
//
//	// ���� �� �˻�
//	if (node->guard1 != GUARD1 || node->guard2 != GUARD2)
//	{
//		DebugBreak(); // ���� ���� ���� ������ ����� �극��ũ
//		return false;
//	}
//
//#else
//	// RELEASE ��忡�� object�� _data�� ����Ŵ
//	ObjectPoolNode<T>* node = reinterpret_cast<ObjectPoolNode<T>*>(reinterpret_cast<char*>(object) - offsetof(ObjectPoolNode<T>, _data));
//
//	// 
//
//	// object�� �޸𸮸� Ǯ�� ����Ű�� _top���� ��������
//	node->next = _top;
//	_top = node;
//
//#endif
//}
//
//



#pragma once
#include <stddef.h>
#include <Windows.h>

//////////////////////////////////////////////////
// 1. DEBUG ������ ��, �޸� ����(GUARD) �߰�
#define GUARD1 0xFFFFFFFF
#define GUARD2 0xFFFFFFFF
//////////////////////////////////////////////////

template <typename T>
struct ObjectPoolNode
{
#ifdef _DEBUG
    int guard1 = GUARD1;
    T _data;
    int guard2 = GUARD2;
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
    ObjectMemoryPool(int capacity = 10000);
    ~ObjectMemoryPool();

    // �޸𸮸� �Ҵ� (�����ڴ� ȣ�� ����)
    T* Alloc();

    // �޸� �Ҵ�� ���ÿ� ������ ȣ��
    T* Alloc_Constructor();

    // �޸� ��ȯ (�Ҹ��ڴ� ȣ�� ����)
    bool Free(T* object);

    // �޸� ��ȯ (�Ҹ��� ȣ��)
    bool Free_Destructor(T* object);

    int getSize() { return _size; }

private:
    ObjectPoolNode<T>* _top;
    int _size;
    int _availableSize;
};

//////////////////////////////////////////////////
// ���ø� Ŭ������ ������
//////////////////////////////////////////////////

template<typename T>
ObjectMemoryPool<T>::ObjectMemoryPool(int initCapacity) : _top(nullptr), _size(0), _availableSize(0)
{
 
#ifdef _DEBUG
    // �ʱ� capacity��ŭ�� ��带 �̸� �Ҵ�
    for (int i = 0; i < initCapacity; i++)
    {
        ObjectPoolNode<T>* newNode = new ObjectPoolNode<T>;
        newNode->next = _top;
        _top = newNode;
        _size++;
        _availableSize++;
    }
#else
    for (int i = 0; i < initCapacity; i++)
    {
        ObjectPoolNode<T>* newNode = new ObjectPoolNode<T>;
        newNode->next = _top;
        _top = newNode;
    }
#endif
}

template<typename T>
ObjectMemoryPool<T>::~ObjectMemoryPool()
{
    // Ǯ�� ���� �ִ� ��� ������ ����
    while (_top)
    {
        ObjectPoolNode<T>* node = _top;
        _top = _top->next;
        delete node;
    }
}

template<typename T>
T* ObjectMemoryPool<T>::Alloc()
{
#ifdef _DEBUG
    if (_top->next == nullptr)
    {
        // ��� ������ ��尡 ������ ���� ����
        ObjectPoolNode<T>* newNode = new ObjectPoolNode<T>;
        newNode->next = nullptr;
        _top = newNode;
        _size++;
        return &(newNode->_data);
    }
    else
    {
        // ��� ������ ��尡 ������ ��ȯ
        T* returnPtr = &(_top->_data);
        _top = _top->next;
        _availableSize--;
        return returnPtr;
    }
#else
    if (_top->next == nullptr)
    {
        // ��� ������ ��尡 ������ ���� ����
        ObjectPoolNode<T>* newNode = (ObjectPoolNode<T>*)malloc(sizeof(ObjectPoolNode<T>));
        newNode->next = nullptr;
        _top = newNode;
        return &(newNode->_data);
    }
    else
    {
        // ��� ������ ��尡 ������ ��ȯ
        T* returnPtr = &(_top->_data);
        _top = _top->next;
        return returnPtr;
    }
#endif
    
}



//////////////////////////////////////////// �̿ϼ� ////////////////////////////////////////////
template<typename T>
T* ObjectMemoryPool<T>::Alloc_Constructor()
{
    if (_availableSize == 0)
    {
        // ��� ������ ��尡 ������ ���� ����
        ObjectPoolNode<T>* newNode = new ObjectPoolNode<T>;
        newNode->next = nullptr;
        _top = newNode;
        _size++;
        return &(newNode->_data);
    }
    else
    {
        // ��带 ��Ȱ���ϸ鼭, �����ڸ� ȣ��
        T* returnPtr = new (&(_top->_data)) T; // placement new
        _top = _top->next;
        _availableSize--;
        return returnPtr;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////


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
    // object�� �޸𸮸� Ǯ�� ����Ű�� _top���� ��������
    ObjectPoolNode<T>* node = reinterpret_cast<ObjectPoolNode<T>*>(reinterpret_cast<char*>(object) - offsetof(ObjectPoolNode<T>, _data));
    node->next = _top;
    _top = node;
    return true;
#endif

    
}


//////////////////////////////////////////// �̿ϼ� ////////////////////////////////////////////
template<typename T>
bool ObjectMemoryPool<T>::Free_Destructor(T* object)
{
    // �Ҹ��ڸ� ȣ���ϰ� �޸𸮸� ��ȯ
    object->~T();
    
    // �۾� �ؾ� �� ~
}
//////////////////////////////////////////// �̿ϼ� ////////////////////////////////////////////



//