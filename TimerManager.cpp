#include <memory>

#include "TimerManager.h"
#include "Timer.h"
#include "Utils.h"
#include "TimeWheelException.h"

namespace timer {

    TimerManager::TimerManager() : create_time_(GetTimeStampNow()) {}

    void TimerManager::AddTimerAt(TimerTask task, uint64_t time, uint64_t interval) {
        std::shared_ptr<Timer> timer_ptr = std::make_shared<Timer>(task, time, interval);
        if(time_wheels_.empty()) {
            throw TimeWheelNotExistException();
        }
        else {
            time_wheels_.back()->AddTimer(timer_ptr);
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
        TimeWheelPtr time_wheel_ptr = std::make_shared<TimeWheel>(create_time_, slot_count, time_unit);
        
        if(!time_wheels_.empty()) {
            TimeWheelPtr last_time_wheel_ptr = time_wheels_.back();
            last_time_wheel_ptr->SetParentTimeWheel(time_wheel_ptr);
            time_wheel_ptr->SetSubTimeWheel(last_time_wheel_ptr);
        }
        
        time_wheels_.emplace_back(time_wheel_ptr);
    }

    void Tick() {
        if(time_wheels_.empty()) {
            throw TimeWheelNotExistException();
        }

        uint64_t now = GetTimeStampNow();
        TimeWheelPtr driving_time_wheel_ptr = time_wheels_.front();
        uint32_t driving_tw_time_unit = driving_time_wheel_ptr->GetTimeUnit();

        while(true) {
            // TODO:
        }
    }

    std::vector<TimeWheelPtr> time_wheels_;
    std::unordered_set<uint64_t> timers_del_;
}