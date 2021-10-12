#include <iostream>
#include <unistd.h>

#include "../TimerManager.h"
#include "../Utils.h"

int main(void) {
    timer::TimerManager timer_manager;
    timer_manager.AppendDrivingTimeWheel(1000, 1);  // 一个slot为1ms
    timer_manager.AppendOtherTimeWheel(60); // 一个slot为1s
    // timer_manager.AppendOtherTimeWheel(60); // 一个slot为1min
    // timer_manager.AppendOtherTimeWheel(24); // 一个slot为1hour

    uint64_t now = timer::GetTimeStampNow();
    timer_manager.AddTimerAt([](){
        std::cout << "Hello World1" << std::endl;
    }, now);   // 时间单位是ms

    timer_manager.AddTimerAt([](){
        std::cout << "Hello World2" << std::endl;
    }, now + 1000);   // 时间单位是ms

    timer_manager.AddTimerAfter([](){
        std::cout << "Hello World3" << std::endl;
    }, 1000 * 2);   // 时间单位是ms

    timer_manager.AddTimerAfter([](){
        std::cout << "Hello World4" << std::endl;
    }, 1000 * 10);   // 时间单位是ms

    timer_manager.AddTimerAfter([](){
        std::cout << "Hello World5" << std::endl;
    }, 1000 * 20);   // 时间单位是ms

    sleep(3);

    timer_manager.Tick();
    // std::cout << "hello" << std::endl;
    sleep(10);
    timer_manager.Tick();
    // // std::cout << "world" << std::endl;
    sleep(8);
    timer_manager.Tick();

    timer_manager.AddTimerAfter([](){
        std::cout << "Hello World6" << std::endl;
    }, 1000 * 30);
    sleep(31);
    timer_manager.Tick();

    return 0;
}