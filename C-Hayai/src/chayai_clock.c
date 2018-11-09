
#include "chayai_clock.h"
#include "stddef.h"

int64_t chayai_clock_resolution_measured() {
    int64_t min_time = 0x8FFFFFFFFFFFF;  // maximum value
    CHayaiTimePoint p1, p2;

    // we measure at least 1000 times to find a lower value. If this succeeds, start a new measurement loop.
    int64_t cur_time;
    for(int i=0; i < 1000; i++) {
        p1 = chayai_clock_now();
        do {
            p2 = chayai_clock_now();
        } while((cur_time = chayai_clock_duration(p1, p2)) <= 0L);

        if(cur_time < min_time) {
            min_time = cur_time;
            i = 0; // start new measurement loop
        }
    }
    return min_time;
}

// Win32
#if defined(_WIN32)


static double chayai_performance_frequency_ns()
{
    CHayaiTimePoint result;
    QueryPerformanceFrequency(&result);
    return 1e9 / (double)(result.QuadPart);
}
 
CHayaiTimePoint chayai_clock_now()
{
    CHayaiTimePoint result;
    QueryPerformanceCounter(&result);
    return result;
}

int64_t chayai_clock_duration(CHayaiTimePoint startTime, CHayaiTimePoint endTime)
{
    const static double performanceFrequencyNs =
        chayai_performance_frequency_ns();
    
    return (int64_t)((endTime.QuadPart - startTime.QuadPart)
        * performanceFrequencyNs);
}

int64_t chayai_clock_resolution() {
    int64_t resolution = (int64_t)chayai_performance_frequency_ns();
    return resolution > 0L ? resolution : 1L;
}

char* chayai_clock_type_str() {
    return "QueryPerformanceCounter";
}

// Apple    
#elif defined(__APPLE__) && defined(__MACH__)


CHayaiTimePoint chayai_clock_now()
{
    return mach_absolute_time();
}

int64_t chayai_clock_duration(CHayaiTimePoint startTime, CHayaiTimePoint endTime)
{
    mach_timebase_info_data_t time_info;
    mach_timebase_info(&time_info);
    return (endTime - startTime) * time_info.numer / time_info.denom;
}

int64_t chayai_clock_resolution() {
    return 1L; // TODO: theroretical maximal resolution
}

char* chayai_clock_type_str() {
    return "mach_absolute_time";
}

// Unix
#elif defined(__unix__) || defined(__unix) || defined(unix)

// gethrtime
#   if (defined(__hpux) || defined(hpux)) || ((defined(__sun__) || defined(__sun) || defined(sun)) && (defined(__SVR4) || defined(__svr4__)))
 
CHayaiTimePoint chayai_clock_now()
{
    return gethrtime();
}

int64_t chayai_clock_duration(CHayaiTimePoint startTime, CHayaiTimePoint endTime)
{
    return (int64_t)(endTime - startTime);
}

int64_t chayai_clock_resolution() {
    return 1L; // TODO: theroretical maximal resolution
}

char* chayai_clock_type_str() {
    return "gethrtime";
}

// clock_gettime
#   elif defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)

clockid_t chayai_clock_gettime_fetch_clockid() {
    struct timespec tmp;
#if defined(CLOCK_MONOTONIC_RAW)
    if(clock_gettime(CLOCK_MONOTONIC_RAW, &tmp) == 0) {
        return CLOCK_MONOTONIC_RAW;
    }
#endif
#if defined(CLOCK_MONOTONIC)
    if(clock_gettime(CLOCK_MONOTONIC, &tmp) == 0) {
        return CLOCK_MONOTONIC;
    }
#endif
#if defined(CLOCK_REALTIME)
    if(clock_gettime(CLOCK_REALTIME, &tmp) == 0) {
        return CLOCK_REALTIME;
    }
#endif
    return (clockid_t)-1;
}

CHayaiTimePoint chayai_clock_now()
{
    static clockid_t clk_id = -1;
    if(clk_id == -1) {
        clk_id = chayai_clock_gettime_fetch_clockid();
    }
    CHayaiTimePoint result;
    clock_gettime(clk_id, &result);
    return result;
}

int64_t chayai_clock_duration(CHayaiTimePoint startTime, CHayaiTimePoint endTime)
{
    CHayaiTimePoint timeDiff;

    timeDiff.tv_sec = endTime.tv_sec - startTime.tv_sec;
    if (endTime.tv_nsec < startTime.tv_nsec)
    {
        timeDiff.tv_nsec = endTime.tv_nsec + 1000000000L - 
            startTime.tv_nsec;
        timeDiff.tv_sec--;
    }
    else
        timeDiff.tv_nsec = endTime.tv_nsec - startTime.tv_nsec;

    return timeDiff.tv_sec * 1000000000L + timeDiff.tv_nsec;
} 

int64_t chayai_clock_resolution() {
    struct timespec res;
    clock_getres(chayai_clock_gettime_fetch_clockid(), &res);
    return res.tv_sec * 1000000000L + res.tv_nsec;
}

char* chayai_clock_type_str() {
    clockid_t clk_id = chayai_clock_gettime_fetch_clockid();

    switch(clk_id) {
#if defined(CLOCK_MONOTONIC_RAW)
    case CLOCK_MONOTONIC_RAW:
        return "clock_gettime(CLOCK_MONOTONIC_RAW)";
#endif
#if defined(CLOCK_MONOTONIC_COARSE)
    case CLOCK_MONOTONIC_COARSE:
        return "clock_gettime(CLOCK_MONOTONIC_COARSE)";
#endif
#if defined(CLOCK_MONOTONIC)
    case CLOCK_MONOTONIC:
        return "clock_gettime(CLOCK_MONOTONIC)";
#endif
#if defined(CLOCK_REALTIME_COARSE)
    case CLOCK_REALTIME_COARSE:
        return "clock_gettime(CLOCK_REALTIME_COARSE)";
#endif
#if defined(CLOCK_REALTIME)
    case CLOCK_REALTIME:
        return "clock_gettime(CLOCK_REALTIME)";
#endif
#if defined(CLOCK_BOOTTIME)
    case CLOCK_BOOTTIME:
        return "clock_gettime(CLOCK_BOOTTIME)";
#endif
#if defined(CLOCK_PROCESS_CPUTIME_ID)
    case CLOCK_PROCESS_CPUTIME_ID:
        return "clock_gettime(CLOCK_PROCESS_CPUTIME_ID)";
#endif
#if defined(CLOCK_THREAD_CPUTIME_ID)
    case CLOCK_THREAD_CPUTIME_ID:
        return "clock_gettime(CLOCK_THREAD_CPUTIME_ID)";
#endif
    default:
        return "clock_gettime(unknow)";
    }
}

// gettimeofday
#   else

CHayaiTimePoint chayai_clock_now()
{
    CHayaiTimePoint result;
    gettimeofday(&result, NULL);
    return result;
}

int64_t chayai_clock_duration(CHayaiTimePoint startTime, CHayaiTimePoint endTime)
{
    return ((endTime.tv_sec - startTime.tv_sec) * 1000000000L +
        (endTime.tv_usec - startTime.tv_usec) * 1000);
}

int64_t chayai_clock_resolution() {
    return 1000L; // TODO: theroretical maximal resolution
}

char* chayai_clock_type_str() {
    return "gettimeofday";
}

#   endif

#endif
