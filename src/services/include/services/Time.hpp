#pragma once
#include <cstdint>
#include <core/Service.hpp>

namespace service {

class Time : public core::Service {
public:
  inline float get_time() const noexcept;
  inline float get_delta() const noexcept;
  inline std::uintmax_t get_tick() const noexcept;

  virtual void update() = 0;
  virtual ~Time() = default;
protected:
  float time = 0.0f; //in secs
  float delta = 0.0f; //время прошедшее с предыдушего кадра
  std::uintmax_t tick = 0; //кажыдй update +1
};


//Inline methods
inline float Time::get_time() const noexcept {
  return time;
}

inline std::uintmax_t Time::get_tick() const noexcept {
  return tick;
}

inline float Time::get_delta() const noexcept {
  return delta;
}

}
