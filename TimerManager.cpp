#include <memory>

#include "TimerManager.h"
#include "Timer.h"
#include "Utils.h"
#include "TimeWheelException.h"

namespace timer {

    TimerManager::TimerManager()
        : cur_time_wheel_time_(timer::GetTimeStampNow()) {
        }

    void TimerManager::AddTimerAt(TimerTask task, uint64_t time, uint64_t interval) {
        std::shared_ptr<Timer> timer_ptr = std::make_shared<Timer>(task, time, interval);
        if(time_wheels_.empty()) {
            throw TimeWheelNotExistException();
        }
        else {
            time_wheels_.back()->AddTimer(timer_ptr, cur_time_wheel_time_);
        }
    }

    void TimerManager::AddTimerAfter(TimerTask task, uint64_t delay_time, uint64_t interval) {
        uint64_t time = GetTimeStampNow() + delay_time;
        AddTimerAt(task, time, interval);
    }

    void TimerManager::CancelTimer(uint64_t timer_id) {
        timers_del_.insert(timer_id);
    }

    void TimerManager::CancelTimer(TimerPtr timer_ptr) {
        if(timer_ptr != nullptr) {
            CancelTimer(timer_ptr->GetId());
        }
    }

    void TimerManager::AppendDrivingTimeWheel(uint64_t slot_count, uint64_t time_unit) {
        if(time_wheels_.empty()) {
            AppendTimeWheel(slot_count, time_unit);
        }
        else {
            throw RepeatAppendDrivingTimeWheelException();
        }
    }

    void TimerManager::AppendOtherTimeWheel(uint64_t slot_count) {
        if(time_wheels_.empty()) {
            throw FirstAppendOtherTimeWheelException();
        }
        else {
            TimeWheelPtr last_time_wheel_ptr = time_wheels_.back();
            uint64_t time_unit = last_time_wheel_ptr->GetSlotCount() * last_time_wheel_ptr->GetTimeUnit();
            AppendTimeWheel(slot_count, time_unit);
        }
    }

    void TimerManager::AppendTimeWheel(uint64_t slot_count, uint64_t time_unit) {
        TimeWheelPtr time_wheel_ptr = std::make_shared<TimeWheel>(slot_count, time_unit);
        
        if(!time_wheels_.empty()) {
            TimeWheelPtr last_time_wheel_ptr = time_wheels_.back();
            last_time_wheel_ptr->SetParentTimeWheel(time_wheel_ptr);
            time_wheel_ptr->SetSubTimeWheel(last_time_wheel_ptr);
        }
        
        time_wheels_.emplace_back(time_wheel_ptr);
    }

    void TimerManager::Tick() {
        if(time_wheels_.empty()) {
            throw TimeWheelNotExistException();
        }

        uint64_t now = GetTimeStampNow();
        TimeWheelPtr driving_time_wheel_ptr = time_wheels_.front();
        uint32_t driving_tw_time_unit = driving_time_wheel_ptr->GetTimeUnit();

        uint64_t check_time = cur_time_wheel_time_ + driving_tw_time_unit;

        while(check_time <= now) {
            for(const TimerPtr& timer_ptr : driving_time_wheel_ptr->GetAndClearCurrentSlot()) {
                if(timers_del_.find(timer_ptr->GetId()) != timers_del_.end()) {
                    timers_del_.erase(timer_ptr->GetId());
                    continue;
                }
                
                timer_ptr->Run();

                if(timer_ptr->IsRepeat()) {
                    timer_ptr->UpdateExpiredTime();
                    time_wheels_.back()->AddTimer(timer_ptr, cur_time_wheel_time_);
                }
            }

            cur_time_wheel_time_ += driving_tw_time_unit;
            check_time += driving_tw_time_unit;
            driving_time_wheel_ptr->TickOne(cur_time_wheel_time_);
        }
    }
}