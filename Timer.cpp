#include "Timer.h"

namespace timer {

    uint64_t Timer::timer_id_generator_ = 0;

    Timer::Timer(TimerTask task, uint64_t expired_time, uint64_t interval_time=0) {
        task_ = task;
        expired_time_ = expired_time;
        interval_time_ = interval_time;

        timer_id_generator_++;
        id_ = timer_id_generator_;

        if(interval_time_ > 0) {
            repeat_ = true;
        }
        else {
            repeat_ = false;
        }
    }
    
    bool Timer::IsRepeat() const {
        return repeat_;
    }

    uint64_t Timer::GetId() const {
        return id_;   
    }

    uint64_t Timer::GetIntervalTime() const {
        return interval_time_;
    }

    void Timer::SetIntervalTime(uint64_t interval_time) {
        interval_time_ = interval_time;
    }

    uint64_t Timer::GetExpiredTime() const {
        return expired_time_;
    }

    void Timer::UpdateExpiredTime() {
        expired_time_ += interval_time_;
    }
}