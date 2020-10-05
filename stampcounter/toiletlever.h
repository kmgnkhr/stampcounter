#ifndef STAMPCOUNTER_TOILETLEVER_H_
#define STAMPCOUNTER_TOILETLEVER_H_

class ToiletLever {
 public:
  explicit ToiletLever(int pin);
  void begin();
  void update();
  bool isFlushing();
  bool wasChanged();
 private:
  const int pin_;
  bool changed_;
  bool last_;
};

#endif  // STAMPCOUNTER_TOILETLEVER_H_
