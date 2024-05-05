#ifndef FIST_H
#define FIST_H

#include "../include/Weapon.h"

class Fist : public Weapon {
public:
  Fist();
  ~Fist();

  virtual void fire();

private:
};

#endif // FIST_H