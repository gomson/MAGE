//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "logging\timer.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	Timer::Timer() 
		: m_initial_time(0.0), m_last_time(0.0), 
		m_elapsed_time(0.0), m_running(false) {
		// Retrieve the frequency of the performance counter. 
		// The frequency of the performance counter is fixed at system boot 
		// and is consistent across all processors.
		QueryPerformanceFrequency(&m_performance_frequency);
		// Calculate the period of the performance counter.
		m_performance_period = 1.0 / (static_cast< double >(m_performance_frequency.QuadPart));
	}

	void Timer::Start() {
		if (m_running) {
			return;
		}

		m_running = true;
		
		// Set the initial time stamp.
		m_initial_time = GetTime();
		// Reset the elapsed time.
		m_elapsed_time = 0.0;
		// Reset the last time stamp.
		m_last_time = m_initial_time;
	}

	void Timer::Stop() {
		if (!m_running) {
			return;
		}

		m_running = false;
		
		// Set the elapsed time.
		m_elapsed_time = GetTime() - m_last_time;
		// Set the last time stamp.
		m_last_time = GetTime();
	}

	void Timer::Restart() {
		m_running = false;
		Start();
	}

	void Timer::Resume() {
		if (m_running) {
			return;
		}

		m_running = true;

		// Set the last time stamp.
		m_last_time = GetTime();
	}

	double Timer::GetElapsedTime() const {
		if (m_running) {
			// Set the elapsed time.
			m_elapsed_time = GetTime() - m_last_time;
			// Set the last time stamp.
			m_last_time = GetTime();
		}

		return m_elapsed_time;
	}

	double Timer::GetTime() const {
		// Retrieve the current value of the performance counter, 
		// which is a high resolution (< 1 �s) time stamp 
		// that can be used for time-interval measurements.
		QueryPerformanceCounter(&m_performance_counter);
		return static_cast< double >(m_performance_counter.QuadPart * m_performance_period);
	}
}