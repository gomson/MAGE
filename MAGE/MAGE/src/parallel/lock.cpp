//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "engine.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// ReadWriteMutex
	//-------------------------------------------------------------------------
	ReadWriteMutex::ReadWriteMutex() : m_nb_writers_waiting(0), m_nb_readers_waiting(0),
		m_active_writer_readers(0) {

		// Initialize a critical section object.
		InitializeCriticalSection(&m_critical_section);

		// Creates or opens an unnamed event object.
		// On success, a handle to the event object is returned.
		// 1. no security
		// 2. manual-reset event object
		// 3. non-signaled initial state
		// 4. unnamed event object
		m_ready_to_read_handle = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_ready_to_read_handle == NULL) {
			Severe("Error creating event for ReadWriteMutex: %d", GetLastError());
		}

		// Creates or opens an unnamed semaphore object.
		// 1. The returned handle cannot be inherited by child processes.
		// 2. The initial count for the semaphore object. 
		// 3. The maximum count for the semaphore object.
		// 4. The semaphore object is created without a name.
		m_ready_to_write_handle = CreateSemaphore(NULL, 0, 1, NULL);
		if (m_ready_to_write_handle == NULL) {
			const DWORD lastError = GetLastError();
			// Close the open event handle.
			CloseHandle(m_ready_to_read_handle);
			Severe("Error creating semaphore for ReadWriteMutex: %d", lastError);
		}
	}

	void ReadWriteMutex::AcquireRead() {
		bool notify_readers = false;

		// Wait for ownership of the specified critical section object. 
		// The function returns when the calling thread is granted ownership.
		EnterCriticalSection(&m_critical_section);
		
		if ((m_nb_writers_waiting > 0) || (HIWORD(m_active_writer_readers) > 0)) {
			++m_nb_readers_waiting;

			while (true) {
				// Sets the specified event object to the nonsignaled state.
				ResetEvent(m_ready_to_read_handle);

				LeaveCriticalSection(&m_critical_section);

				// Waits until the specified object is in the signaled state or the time-out interval elapses.
				// 1. The object handle.
				// 2. The function will return only when the specified object is signaled.
				WaitForSingleObject(m_ready_to_read_handle, INFINITE);
				
				// Wait for ownership of the specified critical section object. 
				// The function returns when the calling thread is granted ownership.
				EnterCriticalSection(&m_critical_section);

				// The reader is only allowed to read if there are not any writers waiting 
				// and if a writer does not own the lock.
				if ((m_nb_writers_waiting == 0) && (HIWORD(m_active_writer_readers) == 0)) {
					break;
				}
			}

			// Reader is done waiting.
			--m_nb_readers_waiting;

			// Reader can read.
			++m_active_writer_readers;
		}
		else {
			// Reader can read.
			if ((++m_active_writer_readers == 1) && (m_nb_readers_waiting != 0)) {
				// Set flag to notify other waiting readers outside of the critical section
				// so that they do not immediately block on the critical section that this thread is holding 
				// when the threads are dispatched by the scheduler.
				notify_readers = true;
			}
		}

		Assert(HIWORD(m_active_writer_readers) == 0);

		LeaveCriticalSection(&m_critical_section);

		if (notify_readers) {
			// Sets the event object to the signaled state.
			SetEvent(m_ready_to_read_handle);
		}
	}

	void ReadWriteMutex::AcquireWrite() {

		// Wait for ownership of the specified critical section object. 
		// The function returns when the calling thread is granted ownership.
		EnterCriticalSection(&m_critical_section);

		// Are there active readers?
		if (m_active_writer_readers != 0) {
			++m_nb_writers_waiting;

			LeaveCriticalSection(&m_critical_section);
			
			// Waits until the specified object is in the signaled state or the time-out interval elapses.
			// 1. The object handle.
			// 2. The function will return only when the specified object is signaled.
			WaitForSingleObject(m_ready_to_write_handle, INFINITE);

			// Upon wakeup theirs no need for the writer to acquire the critical section. 
			// It already has been transfered ownership of the lock by the signaler.
		}
		else {
			Assert(m_active_writer_readers == 0);

			// Set that the writer owns the lock.
			// Concatenates a LOW and HIGH WORD.
			m_active_writer_readers = MAKELONG(0, 1);

			LeaveCriticalSection(&m_critical_section);
		}
	}

	void ReadWriteMutex::ReleaseRead() {

		// Wait for ownership of the specified critical section object. 
		// The function returns when the calling thread is granted ownership.
		EnterCriticalSection(&m_critical_section);

		// Assert that the lock is not held by a writer.
		Assert(HIWORD(m_active_writer_readers) == 0);

		// Assert that the lock is held by readers.
		Assert(LOWORD(m_active_writer_readers > 0));

		// Decrement the number of active readers.
		--m_active_writer_readers;
		if (m_active_writer_readers == 0) {
			// Sets the specified event object to the nonsignaled state.
			ResetEvent(m_ready_to_read_handle);
		}

		// If writers are waiting and this is the last reader hand owneership over to a writer.
		if ((m_nb_writers_waiting != 0) && (m_active_writer_readers == 0)) {
			// Decrement the number of waiting writers
			--m_nb_writers_waiting;

			// Pass ownership to a writer thread.
			// Concatenates a LOW and HIGH WORD.
			m_active_writer_readers = MAKELONG(0, 1);

			// Increases the count of the specified semaphore object.
			// 1. A handle to the semaphore object.
			// 2. The amount by which the semaphore object's current count is to be increased.
			// 3. A pointer to a variable to receive the previous count for the semaphore.
			ReleaseSemaphore(m_ready_to_write_handle, 1, NULL);
		}

		LeaveCriticalSection(&m_critical_section);
	}

	void ReadWriteMutex::ReleaseWrite() {
		bool notify_writer = false;
		bool notify_readers = false;

		// Wait for ownership of the specified critical section object. 
		// The function returns when the calling thread is granted ownership.
		EnterCriticalSection(&m_critical_section);

		// Assert that the lock is owned by a writer.
		Assert(HIWORD(m_active_writer_readers) == 1);

		// Assert that the lock is not owned by one or more readers
		Assert(LOWORD(m_active_writer_readers) == 0);

		if (m_nb_writers_waiting != 0) {
			// Writers waiting, decrement the number of waiting writers and
			// release the semaphore which means ownership is passed to the thread that has been released.
			--m_nb_writers_waiting;
			notify_writer = true;
		}
		else {
			// There are not any writers waiting.
			// Release the exclusive hold on the lock.
			m_active_writer_readers = 0;

			if (m_nb_readers_waiting != 0) {
				// Readers waiting, set the flag that will cause the readers to be notified once the critical section is released.  
				// This is done so that an awakened reader won't immediately block on the critical section which is still being held by this thread.
				notify_readers = true;
			}
		}

		LeaveCriticalSection(&m_critical_section);

		if (notify_writer) {
			// Increases the count of the specified semaphore object.
			// 1. A handle to the semaphore object.
			// 2. The amount by which the semaphore object's current count is to be increased.
			// 3. A pointer to a variable to receive the previous count for the semaphore.
			ReleaseSemaphore(m_ready_to_write_handle, 1, NULL);
		}
		else if (notify_readers) {
			// Sets the event object to the signaled state.
			SetEvent(m_ready_to_read_handle);
		}
	}

	//-------------------------------------------------------------------------
	// ReadWriteMutexLock
	//-------------------------------------------------------------------------
	void ReadWriteMutexLock::UpgradeToWrite() {
		Assert(m_type == READ);
		m_mutex.ReleaseRead();
		m_mutex.AcquireWrite();
		m_type = WRITE;
	}

	void ReadWriteMutexLock::DowngradeToRead() {
		Assert(m_type == WRITE);
		m_mutex.ReleaseWrite();
		m_mutex.AcquireRead();
		m_type = READ;
	}

	//-------------------------------------------------------------------------
	// Semaphore
	//-------------------------------------------------------------------------
	Semaphore::Semaphore() {
		// Creates or opens an unnamed semaphore object.
		// 1. The returned handle cannot be inherited by child processes.
		// 2. The initial count for the semaphore object. 
		// 3. The maximum count for the semaphore object.
		// 4. The semaphore object is created without a name.
		m_handle = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
		if (!m_handle) {
			Severe("Error from CreateSemaphore: %d", GetLastError());
		}
	}

	void Semaphore::Post(uint32_t count) {
		// Increases the count of the specified semaphore object.
		// 1. A handle to the semaphore object.
		// 2. The amount by which the semaphore object's current count is to be increased.
		// 3. A pointer to a variable to receive the previous count for the semaphore.
		if (!ReleaseSemaphore(m_handle, count, NULL)) {
			Severe("Error from ReleaseSemaphore: %d", GetLastError());
		}
	}

	void Semaphore::Wait() {
		// Waits until the specified object is in the signaled state or the time-out interval elapses.
		// 1. The object handle.
		// 2. The function will return only when the specified object is signaled.
		if (WaitForSingleObject(m_handle, INFINITE) == WAIT_FAILED) {
			Severe("Error from WaitForSingleObject: %d", GetLastError());
		}
	}

	bool Semaphore::TryWait() {
		// Waits until the specified object is in the signaled state or the time-out interval elapses.
		// 1. The object handle.
		// 2. The function does not enter a wait state if the object is not signaled.
		// it always returns immediately.
		return (WaitForSingleObject(m_handle, 0L) == WAIT_OBJECT_0);
	}

	//-------------------------------------------------------------------------
	// ConditionVariable
	//-------------------------------------------------------------------------
	void ConditionVariable::Wait() {
		// Increase the number of waiters.
		EnterCriticalSection(&m_nb_waiters_mutex);
		++m_nb_waiters;
		LeaveCriticalSection(&m_nb_waiters_mutex);

		// It is ok to release the <external_mutex> here since Win32
		// manual-reset events maintain state when used with <SetEvent>.  
		// This avoids the "lost wakeup" bug...
		LeaveCriticalSection(&m_condition_mutex);

		// Wait until one or all of the specified objects are 
		// in the signaled state (available for use) or the time-out interval elapses.
		// 1. The number of object handles.
		// 2. An array of object handles.
		// 3. The function returns when the state of any one object is signaled.
		// 4. The function will return only when the specified objects are signaled.
		const int result = WaitForMultipleObjects(2, m_events, FALSE, INFINITE);

		// Decrease the number of waiters.
		EnterCriticalSection(&m_nb_waiters_mutex);
		--m_nb_waiters;
		// WAIT_OBJECT_0: The state of the specified object is signaled.
		const int last_waiter = (result == WAIT_OBJECT_0 + BROADCAST) && (m_nb_waiters == 0);
		LeaveCriticalSection(&m_nb_waiters_mutex);

		if (last_waiter) {
			// We are the last waiter to be notified or to stop waiting, so reset the manual event.
			// Sets the specified event object to the nonsignaled state.
			ResetEvent(m_events[BROADCAST]);
		}

		EnterCriticalSection(&m_condition_mutex);
	}

	void ConditionVariable::Signal() {
		// Retrieve if there are waiters.
		EnterCriticalSection(&m_nb_waiters_mutex);
		const bool has_waiters = (m_nb_waiters > 0);
		LeaveCriticalSection(&m_nb_waiters_mutex);

		if (has_waiters) {
			// Sets the SIGNAL event object to the signaled state.
			SetEvent(m_events[SIGNAL]);
		}
	}
}