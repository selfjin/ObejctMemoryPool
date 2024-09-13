


#include <iostream>
#include <cstddef>
#include "ObjectMemoryPool.h"
#include "Profiler.h"


ObjectMemoryPool<int> g_Pool;


struct Example1
{
    int a;
    short b;
    long long c;
    long long bddd;
    char str[128];
};


inline void origin()
{
    int* ptr = new int;
    delete ptr;
}    

inline void pool()
{
    int* ptr = g_Pool.Alloc();
    g_Pool.Free(ptr);
}
    


int main()
{
    ProfileReset();
    CProfileTimer Pro;

    ObjectMemoryPool<int> gang(100000);


    
    //////////////////////////////////////
    {
        Profile p{ L"origin" };
        for (int i = 0; i < 1000000; i++)
        {
            origin();
        }
    }
    

    {
        Profile p{ L"pool" };
        for (int i = 0; i < 1000000; i++)
        {
            pool();
        }
    }
    

    
    //////////////////////////////////////

    
    ProfileDataOutText(L"pro.txt");

	return 0;
}





// 1. Free List 여러 개를 들고 있고 64 단위로

// 2. 메모리 관리자 하나 만들고

// 3. 성능 비교를 어떻게 할건디?

// 4. 할당된 크기 별로 넣어서 성능 측정

// 해보면 안다라고 하심, 뭔가 있다.
// 4 - 1. 원인이 있다?
// 4 - 2. 추측이 아닌
// 4 - 3. 명확한 원인을 찾아야 한다라


// 가설을 세우고 찾아야 함, 데이터를 속이지 말고

// 5. 






/*



스크립트 언어 (인터프리터 언어)는 런타임에 텍스트를 읽어서 돌리다보니 
컴파일 언어와 달리 변수 이름이 존재한다. 그래서 Key를 변수 이름으로 만들어서 각 객체 (ex 플레이어)에 
대한 정보들을 Json으로 저장하고 불러올 수 있다. 바이너리 데이터에 비해 용량은 크지만, 
편의성 호환성 안정성이 높기 때문에 표준적으로 많이 사용된다. 또, Key 기준으로 검색을 하기 
때문에 데이터 하나가 없어지거나, 순서가 틀어지거나, 추가되어도 쉽게 대응할 수 있다. 
Json과 바이너리의 중간 격인 MessagePack도 있다. key는 text로 유지하되 value는 바이너리로 가서 Json의 장점을 가져오되 속도와 용량을 절약하는 것이다.

스크립트 언어와 달리 c, c++ 같은 컴파일 언어는 일일이 파싱해주어야 하기 때문에 성능저하 및 트래픽 용량 증가 
등이 문제가 되지만, 그럼에도 호환을 위해 Json을 많이 사용한다. c++에서 쓸 땐 보통 오픈 소스를 사용하며 
Rapid Json이 많이 쓰인다. 최근까지 업데이트가 되고 있으며 header로만 이루어져 있어서 재빌드 할 때 편하다.
utf-16은 윈도우에서만 많이 쓰는 거고 대부분의 영어권은 utf-8을 많이 사용하기 때문에 Rapid Json도 utf-8를 
기본으로 사용하며 utf-16을 쓰려면 따로 설정해주어야 한다.

Rapid Json은 Value &value = Doc[“key”] 와 같이 적을 경우 key에 해당하는 value를 value에 저장해준다.
이때, 어떤 자료형인지 모르기 때문에 어떤 자료형이든 받을 수 있는 Value 자료형을 제공해준다. 이때 key가 없는 key라면 예외를 던진다. 안전하게 하려면 iterator를 반환해주는 find를 거치고 사용하는 것이 좋다. 컴파일 언어다보니 GetInt, GetString 등의 노가다가 필요하다. 이때, Json을 굳이 txt로 저장할 일은 없겠지만 그렇게 되어있다면 BOM 때문에 Parse에서 크래시가 난다.














*/