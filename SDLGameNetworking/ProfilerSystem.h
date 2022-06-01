#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <SDL.h>

struct SampleData
{
	int64_t frameTime;
	int frameReferance;
};
typedef std::map<const char*, std::vector<SampleData*>> FrameMap;

class ProfilerSystem
{
public:

	static ProfilerSystem& Instance()
	{
		static ProfilerSystem INSTANCE;
		return INSTANCE;
	}

	ProfilerSystem();
	~ProfilerSystem();

	void StartFrame();
	void StoreSample(const char* name, int64_t elapsedTime);
	void EndFrame();
	void WriteDataToCSV();


    FrameMap& GetFrameDate() const
    {
		return *frameData;
	}


    int GetCurrentFrame()
	{
		return currentFrame;
	}


    FrameMap& GetLastFrameData() const
	{
		return *lastFramesFrameData;
	}


    std::vector<float>& GetFrameTimes()
    {
		return totalFrameTimes;
    }



private:
	int currentFrame;
	FrameMap* frameData;
    std::vector<float> totalFrameTimes;
	float thisFramesTotalTime = 0;
	FrameMap* thisFramesFrameData;
	FrameMap* lastFramesFrameData;

};






struct Profile
{
	Profile(const char* name)
	{
		_name = name;
		//startTime = SDL_GetTicks();
		startTime = std::clock();
	}

	~Profile()
	{
		std::clock_t endTime = std::clock();
		std::clock_t elapsedTime = endTime - startTime;
		//uint64_t endTime = SDL_GetTicks();
		//uint64_t elapsedTime = endTime - startTime;

	    ProfilerSystem::Instance().StoreSample(_name, (1000 *elapsedTime)/CLOCKS_PER_SEC);
	}

	const char* _name;
	//uint64_t startTime;
	std::clock_t startTime;
};
#define PROFILE(name) Profile p(name)