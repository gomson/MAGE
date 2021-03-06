#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "utils\type\types.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of CPU timers.
	 */
	class CPUTimer final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a CPU timer.
		 */
		CPUTimer();

		/**
		 Constructs a CPU timer from the given CPU timer.

		 @param[in]		timer
						A reference to the CPU timer to copy.
		 */
		CPUTimer(const CPUTimer &timer) = default;

		/**
		 Constructs a CPU timer by moving the given CPU timer.

		 @param[in]		timer
						A reference to the CPU timer to move.
		 */
		CPUTimer(CPUTimer &&timer) = default;

		/**
		 Destructs this CPU timer.
		 */
		~CPUTimer() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------	

		/**
		 Copies the given CPU timer to this CPU timer.

		 @param[in]		timer
						A reference to the CPU timer to copy.
		 @return		A reference to the copy of the given CPU timer (i.e. 
						this CPU timer).
		 */
		CPUTimer &operator=(const CPUTimer &timer) = default;

		/**
		 Moves the given CPU timer to this CPU timer.

		 @param[in]		timer
						A reference to the CPU timer to move.
		 @return		A reference to the moved CPU timer (i.e. this CPU 
						timer).
		 */
		CPUTimer &operator=(CPUTimer &&timer) = default;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Starts this CPU timer.
		 */
		void Start() noexcept;

		/**
		 Stops this CPU timer.
		 */
		void Stop() noexcept;

		/**
		 Restarts this CPU timer.
		 */
		void Restart() noexcept;

		/**
		 Resumes this CPU timer.
		 */
		void Resume() noexcept;

		//---------------------------------------------------------------------
		// Member Methods: Delta Time
		//---------------------------------------------------------------------

		/**
		 Returns the core delta time (in seconds) per processing core of this 
		 CPU timer's process.

		 @return		The core delta time (in seconds) per processing core 
						of this CPU timer's process.
		 */
		F64 GetCoreDeltaTimePerCore() const noexcept {
			return GetCoreDeltaTime() / m_nb_processor_cores;
		}

		/**
		 Returns the core delta time (in seconds) of this CPU timer's process.

		 @return		The core delta time (in seconds) of this CPU timer's 
						process.
		 */
		F64 GetCoreDeltaTime() const noexcept;

		/**
		 Returns the kernel mode delta time (in seconds) per processing core 
		 of this CPU timer's process.

		 @return		The kernel mode delta time (in seconds) per processing 
						core of this CPU timer's process.
		 */
		F64 GetKernelModeDeltaTimePerCore() const noexcept {
			return GetKernelModeDeltaTime() / m_nb_processor_cores;
		}

		/**
		 Returns the kernel mode delta time (in seconds) of this CPU timer's 
		 process.

		 @return		The kernel mode delta time (in seconds) of this CPU 
						timer's process.
		 */
		F64 GetKernelModeDeltaTime() const noexcept;

		/**
		 Returns the user mode delta time (in seconds) per processing core 
		 of this CPU timer's process.

		 @return		The user mode delta time (in seconds) per processing 
						core of this CPU timer's process.
		 */
		F64 GetUserModeDeltaTimePerCore() const noexcept {
			return GetUserModeDeltaTime() / m_nb_processor_cores;
		}

		/**
		 Returns the user mode delta time (in seconds) of this CPU timer's 
		 process.

		 @return		The user mode delta time (in seconds) of this CPU 
						timer's process.
		 */
		F64 GetUserModeDeltaTime() const noexcept;

		//---------------------------------------------------------------------
		// Member Methods: Total Delta Time
		//---------------------------------------------------------------------

		/**
		 Returns the total core delta time (in seconds) per processing core of 
		 this CPU timer's process.

		 @return		The total core delta time (in seconds) per processing 
						core of this CPU timer's process.
		 */
		F64 GetTotalCoreDeltaTimePerCore() const noexcept {
			return GetTotalCoreDeltaTime() / m_nb_processor_cores;
		}

		/**
		 Returns the total core delta time (in seconds) of this CPU timer's 
		 process.

		 @return		The total core delta time (in seconds) of this CPU 
						timer's process.
		 */
		F64 GetTotalCoreDeltaTime() const noexcept;

		/**
		 Returns the total kernel mode delta time (in seconds) per processing 
		 core of this CPU timer's process.

		 @return		The total kernel mode delta time (in seconds) per 
						processing core of this CPU timer's process.
		 */
		F64 GetTotalKernelModeDeltaTimePerCore() const noexcept {
			return GetTotalKernelModeDeltaTime() / m_nb_processor_cores;
		}

		/**
		 Returns the total kernel mode delta time (in seconds) of this CPU 
		 timer's process.

		 @return		The total kernel mode delta time (in seconds) of this CPU 
						timer's process.
		 */
		F64 GetTotalKernelModeDeltaTime() const noexcept;

		/**
		 Returns the total user mode delta time (in seconds) per processing 
		 core of this CPU timer's process.

		 @return		The total user mode delta time (in seconds) per 
						processing core of this CPU timer's process.
		 */
		F64 GetTotalUserModeDeltaTimePerCore() const noexcept {
			return GetTotalUserModeDeltaTime() / m_nb_processor_cores;
		}

		/**
		 Returns the total user mode delta time (in seconds) of this CPU 
		 timer's process.

		 @return		The total user mode delta time (in seconds) of this CPU 
						timer's process.
		 */
		F64 GetTotalUserModeDeltaTime() const noexcept;

	private:

		//---------------------------------------------------------------------
		// Class Member Methods
		//---------------------------------------------------------------------

		/**
		 An enumeration of the different modes of this CPU timer.
		 */
		enum struct ModeIndex { 
			Kernel = 0,
			User   = 1,
			Count  = 2
		};

		/**
		 Returns the kernel mode index of CPU timers.

		 @return		The kernel mode index of CPU timers.
		 */
		static constexpr size_t GetKernelModeIndex() noexcept {
			return static_cast< size_t >(ModeIndex::Kernel);
		}

		/**
		 Returns the user mode index of CPU timers.

		 @return		The user mode index of CPU timers.
		 */
		static constexpr size_t GetUserModeIndex() noexcept {
			return static_cast< size_t >(ModeIndex::User);
		}

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Updates the modes' last timestamps 
		 of this CPU timer's process.
		 */
		void UpdateLastTimestamp() const noexcept;

		/**
		 Resets the modes' delta times, total delta times and last timestamps 
		 of this CPU timer's process.
		 */
		void ResetDeltaTime() const noexcept;

		/**
		 Updates the modes' delta times, total delta times and last timestamps 
		 of this CPU timer's process.
		 */
		void UpdateDeltaTime() const noexcept;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The process handle.
		 */
		HANDLE m_handle;

		/**
		 The number of processor cores.
		 */
		size_t m_nb_processor_cores;
		
		/**
		 The modes' last timestamps of this CPU timer's process.
		 */
		mutable U64 m_last_timestamp[static_cast< size_t >(ModeIndex::Count)];
		
		/**
		 The modes' delta times (in seconds) of this CPU timer's process.
		 */
		mutable U64 m_delta_time[static_cast< size_t >(ModeIndex::Count)];

		/**
		 The modes' total delta times (in seconds) of this CPU timer's process.
		 */
		mutable U64 m_total_delta_time[static_cast< size_t >(ModeIndex::Count)];

		/**
		 Flag indicating whether this CPU timer is running.
		 */
		bool m_running;

		//---------------------------------------------------------------------
		// Class Member Variables
		//---------------------------------------------------------------------

		/**
		 The time period of CPU timers.
		 */
		static const F64 s_time_period;
	};
}