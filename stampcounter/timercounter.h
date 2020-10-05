#ifndef STAMPCOUNTER_TIMERCOUNTER_H_
#define STAMPCOUNTER_TIMERCOUNTER_H_

class TimerCounter {
 public:
  explicit TimerCounter(unsigned long timeout);
  void begin();
  bool isTimeOut();
  void reset();
 private:
  unsigned long last_idle_millis_;
  const unsigned long timeout_;
};
#endif  // STAMPCOUNTER_TIMERCOUNTER_H_
