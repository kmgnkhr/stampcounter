#include "timercounter.h"
#include <Arduino.h>

TimerCounter::TimerCounter(unsigned long timeout) : timeout_(timeout) {

}

void TimerCounter::begin() {
  reset();
}

bool TimerCounter::isTimeOut() {
  return (::millis() - last_idle_millis_) > timeout_;
}

void TimerCounter::reset() {
  last_idle_millis_ = ::millis();
}
