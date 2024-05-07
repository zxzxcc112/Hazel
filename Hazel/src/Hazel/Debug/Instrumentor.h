#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>

#include <thread>

struct ProfileResult
{
	const char* Name;
	long long Start, End;
	std::thread::id ThreadID;
};

struct InstrmentationSession
{
	std::string Name;
};

namespace Hazel
{
	class Instrumentor
	{
	public:
		Instrumentor()
			:m_CurrentSession(nullptr), m_ProfileCount(0)
		{
		}

		void BeginSession(const char* name, const std::string& filepath = "result.json")
		{
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrmentationSession{ name };
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteHeader()
		{
			m_OutputStream << R"({"traceEvents":[)" << "\n";
			m_OutputStream.flush();
		}

		void WriteProfile(const ProfileResult& profile)
		{
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",\n";

			std::string name = profile.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"cat\":\"Function\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"ts\":" << profile.Start << ",";
			m_OutputStream << "\"dur\":" << (profile.End - profile.Start) << ",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << profile.ThreadID;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		std::ofstream m_OutputStream;
		InstrmentationSession* m_CurrentSession;
		int m_ProfileCount;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			:m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();
			
			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			std::thread::id threadId =  std::this_thread::get_id();
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadId });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}

#define HZ_PROFILE 1
#ifdef HZ_PROFILE
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath) ::Hazel::Instrumentor::Get().BeginSession(name, filepath);
	#define HZ_PROFILE_END_SESSION() ::Hazel::Instrumentor::Get().EndSession();
	#define HZ_PROFILE_SCOPE(name) ::Hazel::InstrumentationTimer timer##__LINE__(name);
	#define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__FUNCSIG__);
#else
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath)
	#define HZ_PROFILE_END_SESSION()
	#define HZ_PROFILE_SCOPE(name)
	#define HZ_PROFILE_FUNCTION() 
#endif