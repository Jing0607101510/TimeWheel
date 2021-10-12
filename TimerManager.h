#ifndef __TIMER_MANAGER_H__
#define __TIMER_MANAGER_H__

#include <vector>
#include <list>
#include <unordered_set>

#include "TimeWheel.h"
#include "Timer.h"

namespace timer {

    class TimerManager {
        public:
            TimerManager();
            void AddTimerAt(TimerTask task, uint64_t time, uint64_t interval=0);
            void AddTimerAfter(TimerTask task, uint64_t delay_time, uint64_t interval=0);
            void CancelTimer(uint64_t timer_id);
            void CancelTimer(TimerPtr timer_ptr);
            void AppendDrivingTimeWheel(uint64_t slot_count, uint64_t time_unit);
            void AppendOtherTimeWheel(uint64_t slot_count);
            void Tick();
        
        private:
            void AppendTimeWheel(uint64_t slot_count, uint64_t time_unit);

        private:
            std::vector<TimeWheelPtr> time_wheels_;
            std::unordered_set<uint64_t> timers_del_;
            uint64_t cur_time_wheel_time_; // 多级时间轮所代表的时间。这个时间可能滞后于真正的当前时间
    };
}

#endif