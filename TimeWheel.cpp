#include "TimeWheel.h"
#include "Timer.h"

namespace timer {
    TimeWheel::TimeWheel(uint64_t wheel_create_time, uint32_t slot_count, uint32_t time_unit) 
        : wheel_create_time_(wheel_create_time), 
            slot_count_(slot_count), 
            time_unit_(time_unit), 
            cur_idx_(0),
            slots_(slot_count_) {
    }

    uint32_t TimeWheel::GetSlotCount() {
        return slot_count_;
    }

    uint32_t TimeWheel::GetTimeUnit() {
        return time_unit_;
    }

    void TimeWheel::SetParentTimeWheel(TimeWheelPtr time_wheel_ptr) {
        parent_time_wheel_ = time_wheel_ptr;
    }

    void TimeWheel::SetSubTimeWheel(TimeWheelPtr time_wheel_ptr) {
        sub_time_wheel_ = time_wheel_ptr;
    }

    void TimeWheel::AddTimer(TimerPtr timer_ptr) {
        // TODO: 
    }

    void TimeWheel::TickOne() {
        // TODO:
    }

    const uint64_t wheel_create_time_;
    
    uint32_t cur_idx_;
    uint32_t time_unit_;
    uint32_t slot_count_;
    
    std::shared_ptr<TimeWheel> parent_time_wheel;
    std::weak_ptr<TimeWheel> sub_time_wheel;

    std::vector<std::list<TimerPtr>> slots_;
}