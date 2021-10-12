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
            TimeWheel(uint32_t slot_count, uint32_t time_unit);
            uint32_t GetSlotCount();
            uint32_t GetTimeUnit();
            std::list<TimerPtr> GetAndClearCurrentSlot();
            void SetParentTimeWheel(TimeWheelPtr time_wheel_ptr);
            void SetSubTimeWheel(TimeWheelPtr time_wheel_ptr);
            void AddTimer(TimerPtr timer_ptr, uint64_t cur_time_wheel_time);
            void TickOne(uint64_t new_cur_time_wheel_time);
            uint64_t GetCurrentTime();
            void AddTimerAtCurrentSlot(TimerPtr timer_ptr);

        private:
            uint32_t cur_idx_;
            uint32_t time_unit_;
            uint32_t slot_count_;
            const uint32_t total_time_scale_;
            
            std::shared_ptr<TimeWheel> parent_time_wheel_;
            std::weak_ptr<TimeWheel> sub_time_wheel_;

            std::vector<std::list<TimerPtr>> slots_;
    };

}

#endif 