#ifndef __TIME_WHEEL_EXCEPTION_H__
#define __TIME_WHEEL_EXCEPTION_H__

#include <exception>

namespace timer {

    class RepeatAppendDrivingTimeWheelException : public std::exception {
        public:
            const char* what() const throw() {
                return "You Cannot Repeat Append Driving TimeWheel into TimeManager!";
            }
    };


    class FirstAppendOtherTimeWheelException : public std::exception {
        public:
            const char* what() const throw() {
                return "You cannot Append Other TimeWheel into TimeManager Before appending a Driving TimeWheel!";
            }
    };


    class TimeWheelNotExistException : public std::exception {
        public:
            const char* what() const throw() {
                return "There is no time wheels in the timer Manager!";
            }
    };

}

#endif