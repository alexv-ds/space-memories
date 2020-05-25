#pragma once

#include <core/LoggerFactory.hpp>

class LoggerFactory final : public core::LoggerFactory {
  std::shared_ptr<spdlog::logger> logger = spdlog::default_logger(); 
public:
  std::shared_ptr<core::Logger> create_logger(std::string_view name) override {
    return logger->clone(std::string(name));
  }
  std::string_view impl_name() const noexcept override {
    return "main::LoggerFactory";
  }
};