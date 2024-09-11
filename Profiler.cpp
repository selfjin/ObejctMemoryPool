
#include "Profiler.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

#define PRECISION 8

std::vector<ProfileData> profileDatas;

ProfileData* findProfileData(const std::wstring& name) {
    for (auto& sample : profileDatas) {
        if (sample.name == name) {
            return &sample;
        }
    }
    return nullptr;
}

// �������ϸ� ����
void ProfileBegin(const std::wstring& name) {
    // ������ ����
    ProfileData* data = findProfileData(name);

    // ���ٸ� �߰�
    if (!data) {
        profileDatas.push_back(ProfileData(name));
        data = &profileDatas.back();
    }

    // Ÿ�̸� ����
    data->timer.start();
}

void UpdateMinTime(ProfileData* data, double elapsedTime) {
    for (UINT8 i = 0; i < THRESHOLD; ++i) {
        if (elapsedTime < data->minTime[i]) {
            for (UINT8 j = THRESHOLD - 1; j > i; --j) {
                data->minTime[j] = data->minTime[j - 1];
            }
            data->minTime[i] = elapsedTime;
            break;
        }
    }
}

void UpdateMaxTime(ProfileData* data, double elapsedTime) {
    for (UINT8 i = 0; i < THRESHOLD; ++i) {
        if (elapsedTime > data->maxTime[i]) {
            for (UINT8 j = THRESHOLD - 1; j > i; --j) {
                data->maxTime[j] = data->maxTime[j - 1];
            }
            data->maxTime[i] = elapsedTime;
            break;
        }
    }
}

// �������ϸ� ��
void ProfileEnd(const std::wstring& name) {
    // �����͸� ã��
    ProfileData* data = findProfileData(name);

    // �����Ͱ� �����Ѵٸ�
    // ������ ���� ���� �����Ϳ� �߰�
    if (data) {
        // ���� �ð� ����
        double elapsedTime = data->timer.stop();

        // ��ü �ð��� ���ϱ�
        data->totalTime += elapsedTime;

        // �ְ� �ð� �� ���� �ð� ����
        UpdateMinTime(data, elapsedTime);
        UpdateMaxTime(data, elapsedTime);

        // �Լ� ȣ�� Ƚ�� ����
        data->callCount++;
    }
}

void ProfileDataOutText(const std::wstring& fileName) {
    std::wofstream file(fileName);
    file << L"\tName\t|\tAverage\t|\tMin\t|\tMax\t|\tCall\n";
    file << L"----------------------------------------------------------------------------------------------------\n";

    double averageTime;

    for (const auto& data : profileDatas) {

        // ȣ���� Ƚ���� ������ ���ܹ��ֺ��� �� ���ٸ� ǥ���������� ��� �����͸� ���� �� ��� ���ϱ�
        if (data.callCount > THRESHOLD * 2)
        {
            int originalCallCnt = data.callCount;
            double originalTotalTime = data.totalTime;

            // totalTime���� ���ϰ� Ư�Ⳮ ������ ����
            for (UINT8 i = 0; i < THRESHOLD; ++i)
            {
                originalTotalTime -= data.minTime[i];
                originalTotalTime -= data.maxTime[i];

                originalCallCnt -= 2;
            }

            averageTime = originalTotalTime / originalCallCnt;
        }
        // �ƴ϶�� �ٷ� ��� ���ϱ�
        else
        {
            averageTime = data.totalTime / data.callCount;
        }

        file << std::left << "\t" << data.name
            << std::fixed << std::setprecision(PRECISION) << "\t" << averageTime
            << std::setw(15) << std::fixed << std::setprecision(PRECISION) << "\t" << data.minTime[0]
            << std::setw(15) << std::fixed << std::setprecision(PRECISION) << "\t" << data.maxTime[0]
            << std::setw(15) << "\t" << data.callCount
            << L'\n';
    }
    file.close();
}

void ProfileReset() {
    profileDatas.clear();
}