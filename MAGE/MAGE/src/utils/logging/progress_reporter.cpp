//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "utils\logging\progress_reporter.hpp"
#include "utils\logging\logging.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <algorithm>

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	ProgressReporter::ProgressReporter(
		const string &title, U32 nb_work, char plus_char, U32 bar_length)
		: m_nb_work_total(nb_work), 
		m_nb_work_done(0), 
		m_nb_plusses_printed(0), 
		m_plus_char(plus_char),
		m_fout(stdout), 
		m_buffer(), 
		m_current_pos(nullptr), 
		m_timer(MakeUnique< Timer >()), 
		m_mutex() {
		
		Initialize(title, bar_length);
	}

	ProgressReporter::ProgressReporter(
		ProgressReporter &&progress_reporter) = default;

	ProgressReporter::~ProgressReporter() = default;

	void ProgressReporter::Initialize(const string &title, U32 bar_length) {

		if (0 == bar_length) {
			bar_length = ConsoleWidth() - 28;
		}

		m_nb_plusses_total = 
			std::max(2u, bar_length - static_cast< U32 >(title.size()));
		
		// Initialize progress string
		const size_t buffer_length = title.size() + m_nb_plusses_total + 64;
		m_buffer = MakeUnique< char[] >(buffer_length);
		
		// Composes a string with the same text that would be printed if format 
		// was used on printf, but instead of being printed, the content is 
		// stored in the buffer.
		snprintf(m_buffer.get(), buffer_length, "\r%s: [", title.c_str());

		// A C string is as long as the number of characters between the 
		// beginning of the string and the terminating null character (without 
		// including the terminating null character itself).
		m_current_pos = m_buffer.get() + strlen(m_buffer.get());
		char *s = m_current_pos;
		for (size_t i = 0; i < m_nb_plusses_total; ++i) {
			*s++ = ' ';
		}
		*s++ = ']';
		*s++ = ' ';
		*s++ = '\0';

		if (LoggingConfiguration::Get()->IsQuiet()) {
			// Do not output the progression in quiet mode.
			return;
		}

		// Write the buffer to the output file stream.
		fputs(m_buffer.get(), m_fout);

		// If the given stream was open for writing (or if it was open for 
		// updating and the last IO operation was an output operation) any 
		// unwritten data in its output buffer is written to the output file 
		// stream.
		fflush(m_fout);

		m_timer->Start();
	}

	void ProgressReporter::Update(U32 nb_work) {
		
		if (0 == nb_work || LoggingConfiguration::Get()->IsQuiet()) {
			// Do not output the progression in quiet mode.
			return;
		}

		const MutexLock lock(m_mutex);
		
		m_nb_work_done += nb_work;
		const F32 percent_done = static_cast< F32 >(m_nb_work_done) / m_nb_work_total;
		const U32 plusses_needed = 
			std::min(static_cast< U32 >(round(percent_done * m_nb_plusses_total)), 
					 m_nb_plusses_total);
		
		while (m_nb_plusses_printed < plusses_needed) {
			*m_current_pos = m_plus_char;
			++m_current_pos;
			++m_nb_plusses_printed;
		}

		// Write the buffer to the output file stream.
		fputs(m_buffer.get(), m_fout);
		// Update elapsed time and estimated time to completion
		const F32 seconds = static_cast< F32 >(m_timer->GetTotalDeltaTime());
		if (1.0f == percent_done) {
			// Writes the string format to the output file stream.
			fprintf(m_fout, " (%.1fs)       ", seconds);
		}
		else {
			const F32 estimation_remaining = seconds / percent_done - seconds;
			// Writes the string format to the output file stream.
			fprintf(m_fout, " (%.1fs|%.1fs)  ", 
				seconds, std::max(0.0f, estimation_remaining));
		}

		// If the given stream was open for writing (or if it was open for 
		// updating and the last IO operation was an output operation) any 
		// unwritten data in its output buffer is written to the output file 
		// stream.
		fflush(m_fout);
	}

	void ProgressReporter::Done() {
		
		if (LoggingConfiguration::Get()->IsQuiet()) {
			// Do not output the progression in quiet mode.
			return;
		}

		const MutexLock lock(m_mutex);
		
		while (m_nb_plusses_printed < m_nb_plusses_total) {
			*m_current_pos = m_plus_char;
			++m_current_pos;
			++m_nb_plusses_printed;
		}

		// Write the buffer to the output file stream.
		fputs(m_buffer.get(), m_fout);
		// Update elapsed time
		const F32 seconds = static_cast< F32 >(m_timer->GetTotalDeltaTime());
		// Writes the string format to the output file stream.
		fprintf(m_fout, " (%.1fs)       \n", seconds);
	
		// If the given stream was open for writing (or if it was open for 
		// updating and the last IO operation was an output operation) any 
		// unwritten data in its output buffer is written to the output file 
		// stream.
		fflush(m_fout);
	}
}