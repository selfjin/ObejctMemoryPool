
#pragma once

#include <Windows.h>
#include <string>
#include <float.h> // DBL_MAX, DBL_MIN

class CProfileTimer
{
public:
    CProfileTimer() {
        QueryPerformanceFrequency(&freq);
        start();
    }

    void start() {
        QueryPerformanceCounter(&startTime);
    }

    double stop() {
        LARGE_INTEGER endTime;
        QueryPerformanceCounter(&endTime);
        return 1.f * (endTime.QuadPart - startTime.QuadPart) / freq.QuadPart; // microseconds
    }

private:
    LARGE_INTEGER freq;
    LARGE_INTEGER startTime;
};

// �������ϸ��� ����ü
#define THRESHOLD 10
typedef struct _tagProfileData {
    std::wstring name;
    double totalTime = 0;
    double minTime[THRESHOLD];
    double maxTime[THRESHOLD];
    int callCount = 0;
    CProfileTimer timer; // Ÿ�̸�

    _tagProfileData(const std::wstring& _name)
    {
        for (UINT8 i = 0; i < THRESHOLD; ++i)
        {
            minTime[i] = DBL_MAX;
            maxTime[i] = DBL_MIN;
        }

        name = _name;
    }
}ProfileData;

ProfileData* findProfileData(const std::wstring& name);
void ProfileBegin(const std::wstring& name);
void UpdateMinTime(ProfileData* data, double elapsedTime);
void UpdateMaxTime(ProfileData* data, double elapsedTime);
void ProfileEnd(const std::wstring& name);
void ProfileDataOutText(const std::wstring& fileName);
void ProfileReset();

#define PROFILE

#ifdef PROFILE
#define PRO_BEGIN(tagName) ProfileBegin(tagName)
#define PRO_END(tagName) ProfileEnd(tagName)
#else
#define PRO_BEGIN(tagName)
#define PRO_END(tagName)
#endif

class Profile {
public:
    Profile(const std::wstring& tag) : m_tagName(tag) {
        PRO_BEGIN(m_tagName);
    }
    ~Profile() {
        PRO_END(m_tagName);
    }

private:
    std::wstring m_tagName;
};