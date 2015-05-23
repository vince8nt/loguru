/*
Loguru logging library for C++, by Emil Ernerfeldt.
www.github.com/emilk/loguru
License: use, abuse, give credit if you like.

* Version 0.1 - 2015-03-22 - Works great on Mac.


# Usage:
LOG(INFO, "I'm hungry for some %.3f!", 3.14159);
CHECK(0 < x, "Expected positive integer, got %d", x);
Use LOG_SCOPE for temporal grouping and to measure durations.
Calling loguru::init is optional, but useful to timestamp the start of the log.


# TODO:
* Set file output.
* Customize log-level.
* argc/argv parsing.
* Port to Windows.
* Remove dependency on boost::posix_time.
* Log on atexit?
*/

#pragma once

// Helper macro for declaring functions as having similar signature to printf.
// This allows the compiler to catch format errors at compile-time.
#define LOGURU_PRINTF_LIKE(fmtarg, firstvararg) \
		__attribute__((__format__ (__printf__, fmtarg, firstvararg)))

namespace loguru
{
	enum class LogType
	{
		// Value is the verbosity level one must pass.
		// Negative numbers go to stderr and cannot be skipped.
		FATAL   = -3,
		ERROR   = -2,
		WARNING = -1,
		INFO    =  0, // Normal messages.
		SPAM    = +1  // Goes to file, but not to screen.
	};

	// Musn't throw!
	typedef void (*log_handler_t)(void* user_data, LogType type, const char* text);
	typedef void (*fatal_handler_t)();

	// Musn't be called, but it's nice if you do.
	void init(int argc, char* argv[]);

	/* Will be called right before abort().
	   This can be used to print a callstack.
	   Feel free to call LOG:ing function from this, but not FATAL ones!
	*/
	void set_fatal_handler(fatal_handler_t handler);

	/* Will be called on each log messages that passes verbocity tests etc.
	   Useful for displaying messages on-screen in a game, for eample. */
	void add_callback(const char* id, log_handler_t callback, void* user_data);
	void remove_callback(const char* id);

	// Actual logging function. Use the LOG macro instead of calling this directly.
	void log(LogType type, const char* file, unsigned line, const char* format, ...) LOGURU_PRINTF_LIKE(4, 5);

	// Helper class for LOG_SCOPE
	class LogScopeRAII
	{
	public:
		LogScopeRAII(const char* file, unsigned line, const char* format, ...) LOGURU_PRINTF_LIKE(4, 5);
		~LogScopeRAII();

	private:
		LogScopeRAII(LogScopeRAII&);
		LogScopeRAII(LogScopeRAII&&);
		LogScopeRAII& operator=(LogScopeRAII&);
		LogScopeRAII& operator=(LogScopeRAII&&);

		const char* _file;
		unsigned    _line;
		long long   _start_time_ns;
	};

	// Marked as 'noreturn' for the benefit of the static analyzer and optimizer.
	void on_assertion_failed(const char* expr, const char* file, unsigned line, const char* format, ...) LOGURU_PRINTF_LIKE(4, 5) __attribute__((noreturn));
} // namespace loguru

// --------------------------------------------------------------------
// Macros!

// LOG(INFO, "Foo: %d", some_number);
#define LOG(level, ...) loguru::log(loguru::LogType:: level, __FILE__, __LINE__, __VA_ARGS__)

// Use to book-end a scope. Affects logging on all threads.
#define JOIN_STRINGS(a, b) a ## b
#define LOG_SCOPE(...) loguru::LogScopeRAII JOIN_STRINGS(error_context_RAII_, __LINE__)(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_SCOPE_FUNCTION() LOG_SCOPE(__PRETTY_FUNCTION__)


/* Checked at runtime too. Will print error, then call abort_handler (if any), then 'abort'.
   Note that the test must be boolean.
   CHECK(ptr, ...); will not compile, but CHECK(ptr != nullptr, ...); will.
*/
#define CHECK(test, ...) if ((test) == true) {} else do { loguru::on_assertion_failed("ASSERTION FAILED: " #test, __FILE__, __LINE__, __VA_ARGS__); } while (false)

#define ASSERT(test) CHECK(test, "(developer too lazy to add proper error message)")


#ifndef NDEBUG
#  define DLOG(level, ...)  LOG(level, __VA_ARGS__)
#  define DCHECK(test, ...) CHECK(test, __VA_ARGS__)
#  define DASSERT(test)     ASSERT(test)
#else
#  define DLOG(level, ...)
#  define DCHECK(test, ...)
#  define DASSERT(test)
#endif

#undef assert
#define assert(test) ASSERT(test) // HACK
