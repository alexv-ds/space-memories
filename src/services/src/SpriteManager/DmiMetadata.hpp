#pragma once
#include <vector>
#include <string>
#include <string_view>

struct DmiIconState {
  std::string name;
  int dirs = 1;
  int frames = 1;
  std::vector<float> delays;
  bool rewind = false;
  int loop = -1;
  std::string hotspot;
  bool movement = false;
};

class DmiMetadata {
public:
  int width = 32;
  int height = 32;
  std::vector<DmiIconState> states;
  DmiMetadata(std::string_view desc);
  void validate_icon_state(const DmiIconState& state); //throw в случае ошибки
};
