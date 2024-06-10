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
			// 1. 1ns = 1e-9s, 返回单位为秒
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
	
	
	

	// []表示捕获（capture）上下文变量，[=]按值，[&]按引用，[]什么也不捕获
//#define PROFILE_SCOPE(name) Timer timer##__LINE__(name,[&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); })

}