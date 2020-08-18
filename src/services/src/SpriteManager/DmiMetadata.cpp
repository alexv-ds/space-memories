#include <regex>
#include "DmiMetadata.hpp"

void split_string(std::vector<std::string>& out, const std::string_view str, const std::string_view delim) {
  size_t prev = 0;
  size_t pos = 0;
  do {
    pos = str.find(delim, prev);
    if (pos == std::string_view::npos) {
      pos = str.length();
    }
    std::string_view substr = str.substr(prev, pos - prev);
    out.emplace_back(substr.begin(), substr.end());
    prev = pos + delim.length();
  } while(pos < str.length() && prev < str.length());
}

DmiMetadata::DmiMetadata(std::string_view desc) {
  static thread_local std::regex regex_width("^\twidth\\s=\\s(\\d+)$");
  static thread_local std::regex regex_height("^\theight\\s=\\s(\\d+)$");
  static thread_local std::regex regex_state("^state\\s=\\s\"(.*)\"$");
  static thread_local std::regex regex_dirs("^\tdirs\\s=\\s(\\d+)$");
  static thread_local std::regex regex_frames("^\tframes\\s=\\s(\\d+)$");
  static thread_local std::regex regex_delay("^\tdelay\\s=\\s(.*)$");
  static thread_local std::regex regex_loop("^\tloop\\s=\\s(\\d+)$");
  static thread_local std::regex regex_hotspot("^\thotspot\\s=\\s\"(.*)\"$");

  std::vector<std::string> lines;
  split_string(lines, desc, "\n");
  if (lines.size() < 6) {
    throw std::logic_error("Метаданные слишком короткие");
  }
  if (lines[0] != "# BEGIN DMI") {
    throw std::logic_error("Ожидается в начале: # BEGIN DMI");
  }
  if (lines[lines.size() - 1] != "# END DMI") {
    throw std::logic_error("Ожидается в конце: # END DMI");
  }
  if (lines[1] != "version = 4.0") {
    throw std::logic_error("Неизвестная версия. Умеем только в 4.0");
  }
  std::smatch match_results;
  if(std::regex_match(lines[2], match_results, regex_width)) {
    width = std::stoi(match_results[1]);
  }
  if (std::regex_match(lines[3], match_results, regex_height)) {
    height = std::stoi(match_results[1]);
  }
  size_t current_line = 4;
  while (current_line < lines.size() - 1) {
    DmiIconState icon_state;
    if(!std::regex_match(lines[current_line], match_results, regex_state)) {
      throw std::logic_error("Не найден state");
    }
    icon_state.name = match_results[1];
    ++current_line;
    while (lines[current_line].size() != 0 && lines[current_line][0] == '\t') {
      //dirs
      if (std::regex_match(lines[current_line], match_results, regex_dirs)) {
        icon_state.dirs = std::stoi(match_results[1]);
      //frames
      } else if (std::regex_match(lines[current_line], match_results, regex_frames)) {
        icon_state.frames = std::stoi(match_results[1]);
      //delay
      } else if (std::regex_match(lines[current_line], match_results, regex_delay)) {
        std::vector<std::string> splitted_delays;
        split_string(splitted_delays, match_results[1].str(), ",");
        for (std::string& str_delay : splitted_delays) {
          icon_state.delays.push_back(std::stof(str_delay)); //!!!найти замену stof!!! (locale)
        }
      //rewind
      } else if (lines[current_line] == "\trewind = 1") {
        icon_state.rewind = true;
      //loop
      } else if (std::regex_match(lines[current_line], match_results, regex_loop)) {
        icon_state.loop = std::stoi(match_results[1]);
      //hotspot
      } else if (std::regex_match(lines[current_line], match_results, regex_hotspot)) {
        icon_state.hotspot = match_results[1];
      //tmovement
      } else if (lines[current_line] == "\tmovement = 1") {
        icon_state.movement = true;
      } else {
        throw std::logic_error("Неизвестная строка");
      }
      ++current_line;
    } //end of while
    validate_icon_state(icon_state);
    states.push_back(std::move(icon_state));
  } //end of while
}

void DmiMetadata::validate_icon_state(const DmiIconState& state) {
  if (!(state.dirs == 1 || state.dirs == 4 || state.dirs == 8)) {
    throw std::logic_error("Ошибка валидации: dirs может быть только 1, 4 или 8");
  }
  if (state.frames == 1) {
    if (state.delays.size() != 0) {
      throw std::logic_error("Ошибка валидации: если frames == 1, то delays.size() должен быть равен 0");
    }
  } else {
    if (state.frames != state.delays.size()) {
      throw std::logic_error("Ошибка валидации: frames != delays.size()");
    }
  }
}
