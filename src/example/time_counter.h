
#pragma once

#include <chrono>
#include <string>
#include <memory>
#include <iostream>

class CTimeCounterDefaultPrinter
{
	void print(const ::std::chrono::nanoseconds&) {}
};

class CTimeCounterStdOutPrinter
{
	using nanoseconds = ::std::chrono::nanoseconds;
	using rep = typename nanoseconds::rep;

	template<typename... Args>
	inline ::std::string string_format(const char* format, Args... args)
	{
		size_t size = ::std::snprintf(nullptr, 0, format, args...) + 1;
		::std::unique_ptr<char[]> buf(new char[size]);
		::std::snprintf(buf.get(), size, format, args...);
		return ::std::string(buf.get(), buf.get() + size - 1);
	}

public:
	void print(const nanoseconds& time)
	{
		rep ns = time.count();
		double elapsedSeconds = static_cast<double>(ns) / 1000000000.0;
		double elapsedMilliseconds = static_cast<double>(ns) / 1000.0;
		::std::cout << string_format("Elapsed time in seconds: [%.3f] in milliseconds: [%.3f]",
			elapsedSeconds, elapsedMilliseconds) << ::std::endl;
	}
};

template <class time_unit, class printer>
class CTimeCounter : public printer
{
public:
	using time_point = ::std::chrono::time_point<::std::chrono::steady_clock>;
	using clock = ::std::chrono::high_resolution_clock;
	using duration = typename time_point::duration;
	using rep = typename time_unit::rep;

	CTimeCounter()
		: m_startTime(clock::now())
		, m_stopTime(time_point())
	{}

	inline void start()
	{
		m_startTime = clock::now();
		m_stopTime = time_point();
	}

	inline time_unit stop()
	{
		m_stopTime = clock::now();
		time_unit elapsedTime = getElapsedTime();
		print(::std::chrono::duration_cast<::std::chrono::nanoseconds>(elapsedTime));
		return elapsedTime;
	}

	inline time_unit restart()
	{
		time_unit time = stop();
		start();
		return time;
	}

	inline time_unit getElapsedTime() {
		bool wasStopped = (m_stopTime != time_point());
		time_point stopTime = wasStopped ? m_stopTime : clock::now();
		return ::std::chrono::duration_cast<time_unit, rep>(stopTime - m_startTime);
	}

private:
	time_point m_startTime;
	time_point m_stopTime;
};
