#include <chrono>

#include "Utils.h"

namespace timer {

    uint64_t GetTimeStampNow() {
        std::chrono::system_clock::time_point time_point_now = std::chrono::system_clock::now();
        std::chrono::system_clock::duration duration = time_point_now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

}
