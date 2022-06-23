#ifndef SERVER_UTIL_H
#define SERVER_UTIL_H

#include <thread>

namespace Util {
    void setThreadName( const char * threadName );
    void setThreadAffinity( std::thread::native_handle_type handle, int core );
}

#endif //SERVER_UTIL_H
