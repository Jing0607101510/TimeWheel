#ifndef __TIMER_H__
#define __TIMER_H__

#include <functional>

namespace timer {

    typedef std::function<void()> TimerTask;

    class Timer {
        public:
            Timer(TimerTask task, uint64_t expired_time, uint64_t interval_time=0);
            bool IsRepeat() const;
            uint64_t GetId() const;
            uint64_t GetIntervalTime() const;
            void SetIntervalTime(uint64_t interval_time);
            uint64_t GetExpiredTime() const;
            void UpdateExpiredTime();
            void SetRoundCount(uint32_t round_count);
            uint32_t GetRoundCount();
            void Run();
            
        private:
            static uint64_t timer_id_generator_;

            TimerTask task_;
            bool repeat_;
            uint64_t expired_time_;
            uint64_t interval_time_;
            uint64_t id_;  
            uint32_t round_count_;
    };
}

#endif

