#include <iostream>

#include "TimeWheel.h"
#include "Timer.h"
#include "Utils.h"

namespace timer {
    TimeWheel::TimeWheel(uint32_t slot_count, uint32_t time_unit) 
        : slot_count_(slot_count), 
            time_unit_(time_unit), 
            cur_idx_(0),
            slots_(slot_count_),
            total_time_scale_(time_unit_ * slot_count_) {
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

    void TimeWheel::AddTimer(TimerPtr timer_ptr, uint64_t cur_time_wheel_time) {
        if(timer_ptr) {
            timer_ptr->SetRoundCount(0);

            uint64_t expired_time = timer_ptr->GetExpiredTime();

            if(expired_time <= cur_time_wheel_time) {
                if(!sub_time_wheel_.expired()) {
                    TimeWheelPtr sub_time_wheel_ptr = sub_time_wheel_.lock();
                    sub_time_wheel_ptr->AddTimer(timer_ptr, cur_time_wheel_time);
                }
                else {
                    slots_[cur_idx_].push_back(timer_ptr);
                }
                return;
            }

            uint64_t time_diff = expired_time - cur_time_wheel_time;
            uint64_t sub_timewheel_current_time = 0;
            if(!sub_time_wheel_.expired()) {
                sub_timewheel_current_time = sub_time_wheel_.lock()->GetCurrentTime();
            }

            uint64_t relative_time = time_diff + sub_timewheel_current_time;
            if(relative_time >= time_unit_) {
                uint32_t round_count = relative_time / total_time_scale_;
                uint32_t slot_position = (cur_idx_ + relative_time / time_unit_) % slot_count_;
                // bug has fixed, reason: 非驱动轮（即刻度单位最小的轮）的cur_idx_所指向的slot上
                // 的所有timer已经被分配到下一级的时间轮中了。也就是这个时间轮上的cur_idx_所指向的slot已经被处理，
                // 如果此时刚好有一个新的timer需要插入到cur_idx_所指向的slot，那么它的round_count就应该减去1，
                // 因为cur_idx_在指向该slot的时候，slot上的timer的被处理，那么新插入的timer的round_count对应减去1
                // 如果新的timer插入的位置不是cur_idx_，那么它的round_count不受影响
                if(slot_position == cur_idx_ && !sub_time_wheel_.expired()) {
                    round_count -= 1;
                }
                timer_ptr->SetRoundCount(round_count);
                slots_[slot_position].push_back(timer_ptr);
                return;
            }

            else {
                if(!sub_time_wheel_.expired()) {
                    sub_time_wheel_.lock()->AddTimer(timer_ptr, cur_time_wheel_time);
                }
                else {
                    slots_[cur_idx_].push_back(timer_ptr);
                }
            }
        }
    }

    std::list<TimerPtr> TimeWheel::GetAndClearCurrentSlot() {
        std::list<TimerPtr> timers;
        timers.swap(slots_[cur_idx_]);
        return timers;
    }

    void TimeWheel::TickOne(uint64_t new_cur_time_wheel_time) {
        cur_idx_ += 1;
        if(cur_idx_ == slot_count_) {
            cur_idx_ = 0;
            if(parent_time_wheel_) {
                parent_time_wheel_->TickOne(new_cur_time_wheel_time);
            }
        }
        
        if(!sub_time_wheel_.expired()) {
            TimeWheelPtr sub_time_wheel_ptr = sub_time_wheel_.lock();
            std::list<TimerPtr> timer_ptrs = GetAndClearCurrentSlot();
            for(const TimerPtr& timer_ptr : timer_ptrs) {
                if(timer_ptr->GetRoundCount() == 0) {
                    sub_time_wheel_ptr->AddTimer(timer_ptr, new_cur_time_wheel_time);
                }
                else {
                    timer_ptr->SetRoundCount(timer_ptr->GetRoundCount() - 1);
                    slots_[cur_idx_].push_back(timer_ptr);
                }
            }
        }
    }

    uint64_t TimeWheel::GetCurrentTime() {
        uint64_t current_time = cur_idx_ * time_unit_;
        if(!sub_time_wheel_.expired()) {
            current_time += sub_time_wheel_.lock()->GetCurrentTime();
        }
        return current_time;
    }

    void TimeWheel::AddTimerAtCurrentSlot(TimerPtr timer_ptr) {
        slots_[cur_idx_].push_back(timer_ptr);
    }
}