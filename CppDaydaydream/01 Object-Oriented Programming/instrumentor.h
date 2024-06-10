#pragma once
#include <chrono>

namespace CppDaydaydream
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>; // 微秒

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();

			// 1. time_since_epoch获取*this和时钟的时间的duration
			// 2. FloatingPointMicroseconds将duration的时间单位转换成微秒，数据类型转换成double
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() }; 
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			// TODO: repair
			//Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });


			std::cout << m_Name << " Duration: " << elapsedTime.count() * 0.001f<< "ms" << std::endl;

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};


	// []表示捕获（capture）上下文变量，[=]按值，[&]按引用，[]什么也不捕获
#define AS_PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
	//,[&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); }
}