#pragma once
#include <map>
#include <vector>
#include <string>

namespace component {

struct UseDebugMenus {};
struct DebugSystemMenu {};
struct DebugMenusList {}; //C++ only
struct DebugImguiDemoMenu {}; //C++ only

//C++ only
//При изменении этого компонента можно не вызывать update триггер
struct DebugSystemPerformance {
  struct TimeData { //обновляются каждый кадр
    double max = 0;
    double total = 0;
    int collect_times = 0;
    double current = 0;
  };
  struct ForVisualiseData { //Обновляются каждую секунду
    double max = 0;
    double avg = 0;
    std::string name;
  };
  TimeData all_systems_data; //Тоже самое как и в мапе, только для общего времени
  ForVisualiseData all_systems_visualise; //Тоже самое как и в векторе, только для общего времени, кроме имени, оно пустое
  std::map<std::string, TimeData> systems;
  std::vector<ForVisualiseData> for_visualise;
  float current_real_time = 0; //При достижении секунды обновляtется вектор
  float start_real_time = 0;
};

//C++ only
struct DebugMetricsWindow {};

}