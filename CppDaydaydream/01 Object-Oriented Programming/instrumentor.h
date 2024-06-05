#pragma once
#include <chrono>

namespace CppDaydaydream
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>; // ΢��

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

			// 1. time_since_epoch��ȡ*this��ʱ�ӵ�ʱ���duration
			// 2. FloatingPointMicroseconds��duration��ʱ�䵥λת����΢�룬��������ת����double
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


	// []��ʾ����capture�������ı�����[=]��ֵ��[&]�����ã�[]ʲôҲ������
#define AS_PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
	//,[&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); }
}