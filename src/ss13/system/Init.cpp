#include "Init.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

namespace ss13::system {

const std::string_view Init::name() const noexcept {
  return "ss13::system::Init";
}

void Init::init() {
  std::cout << "Init::init()" << '\n';
  this->window_service = core::service::get_service<core::service::Window>();
  this->performance_service =
    core::service::get_service<core::service::PerformanceController>();

  if (!this->font.loadFromFile("resources/default.ttf")) {
    std::cout << "FONT LOAD FAIL" << '\n';
    return;
  }

  this->text.setFont(this->font);
  this->text.setCharacterSize(16);
  this->text.setFillColor(sf::Color::White);
}

void Init::update() {
  sf::RenderTarget* render = this->window_service->get_render_target();
  if(!render) {
    return;
  }
  this->string.clear();

  this->string += "Tick usage: ";
  this->string += std::to_string(this->performance_service->tick_usage() * 100);
  this->string += "%";

  this->text.setString(this->string);
  render->draw(this->text);
}


} /* ss13::system */
