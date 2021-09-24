#ifndef __TIME_WHEEL_H__
#define __TIME_WHEEL_H__

#include <memory>
#include <vector>
#include <list>

namespace timer {

    class Timer;
    class TimeWheel;
    
    typedef std::shared_ptr<Timer> TimerPtr;
    typedef std::shared_ptr<TimeWheel> TimeWheelPtr;

    class TimeWheel {
        public:
            TimeWheel(uint64_t wheel_create_time, uint32_t slot_count, uint32_t time_unit);
            uint32_t GetSlotCount();
            uint32_t GetTimeUnit();
            void SetParentTimeWheel(TimeWheelPtr time_wheel_ptr);
            void SetSubTimeWheel(TimeWheelPtr time_wheel_ptr);
            void AddTimer(TimerPtr timer_ptr);
            void TickOne();

        private:
            const uint64_t wheel_create_time_;

            uint32_t cur_idx_;
            uint32_t time_unit_;
            uint32_t slot_count_;
            
            std::shared_ptr<TimeWheel> parent_time_wheel_;
            std::weak_ptr<TimeWheel> sub_time_wheel_;

            std::vector<std::list<TimerPtr>> slots_;
    };

}

#endif 