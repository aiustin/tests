#include "util.h"

#ifdef _WIN32
#elif defined(__linux__)
#include <sys/prctl.h>
#include <pthread.h>
#elif defined( __APPLE__ )
#include <pthread.h>
#include <mach/mach_types.h>
#include <mach/thread_act.h>

#endif

namespace Util {

    void
    setThreadName( const char* threadName )
    {
#ifdef _WIN32
        //TODO: SetThreadDescription
#elif defined(__linux__)
        prctl( PR_SET_NAME, threadName, 0, 0, 0 );
#elif defined( __APPLE__ )
        pthread_setname_np( threadName );
#endif
    }

    void
    setThreadAffinity( std::thread::native_handle_type handle, int core )
    {
#ifdef _WIN32
        //TODO:
#elif defined(__linux__)
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core, &cpuset);
        pthread_setaffinity_np( handle, sizeof(cpu_set_t), &cpuset );
#elif defined( __APPLE__ )
        thread_port_t mach_thread;
        thread_affinity_policy_data_t policy = { core };
        mach_thread = pthread_mach_thread_np( handle );
        thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, 1);
#endif
    }
}
