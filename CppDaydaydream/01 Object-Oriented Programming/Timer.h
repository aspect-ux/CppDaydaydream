/*
* @author: aspect
* @file: Timer.h
* @date: 2023/09/11
*/

#pragma once
#include <chrono>

namespace CppDaydaydream
{
	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}
		
		// 重置
		void Reset()
		{
			// 获取当前时间(不是系统的时间)
			m_Start = std::chrono::high_resolution_clock::now();
		}

		// 已经过去的时间(秒)
		float Elapsed()
		{
			// 1. 1ns = 10e9s, 返回单位为秒
			// 2. now() - now()等于duration时间段，duration_cast转换成nano,然后通过count()获取时间
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count()* 0.001f * 0.001f * 0.001f;
		}

		// 已经过去的时间(毫秒)
		float ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
	
	/*
	// Example of Timer
	template<typename Fn>
	class Timer1
	{
	public:
		Timer(const char* name, Fn&& func)
			: m_Name(name), m_Stopped(false), m_Func(func)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			// microseconds微秒
			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			m_Stopped = true;

			float duration = (end - start) * 0.001f;

			//std::cout << m_Name << " Duration: " << duration << "ms" << std::endl;

			m_Func({ m_Name,duration });
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
		Fn m_Func;
	};*/

	// []表示捕获（capture）上下文变量，[=]按值，[&]按引用，[]什么也不捕获
//#define PROFILE_SCOPE(name) Timer timer##__LINE__(name,[&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); })

}