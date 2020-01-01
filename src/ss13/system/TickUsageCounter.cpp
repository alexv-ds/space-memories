#include "TickUsageCounter.hpp"

namespace ss13::system {

const std::string_view TickUsageCounter::name() const noexcept {
  return "ss13::system::TickUsageCounter";
}

void TickUsageCounter::init() {
  this->window = core::service::get_service<core::service::Window>();
  this->performance =
    core::service::get_service<core::service::PerformanceController>();

  if (!this->font.loadFromFile("resources/default.ttf")) {
    return;
  }

  this->text.setFont(this->font);
  this->text.setCharacterSize(16);
  this->text.setFillColor(sf::Color::White);
}

void TickUsageCounter::update() {
  sf::RenderWindow* render = this->window->get_render_window();
  if(!render) {
    return;
  }

  if (this->counter == 5) {
    this->buffer[0] = 0;
    std::snprintf(
      this->buffer, sizeof(this->buffer), "Tick Usage: %.1f%%",
      this->accumulated_tick_usage / this->counter
    );
    this->counter = 0;
    this->accumulated_tick_usage = 0;
    this->text.setString(this->buffer);
  }

  ++(this->counter);
  this->accumulated_tick_usage += this->performance->tick_usage() * 100;
  render->draw(this->text);
}



} /* ss13::system */
