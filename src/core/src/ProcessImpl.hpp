#pragma once

#include <functional>
#include <core/Process.hpp>
#include <core/Logger.hpp>


namespace core {

class ProcessImpl final : public Process {
public:
  ProcessImpl(std::shared_ptr<Logger> logger,
              std::function<void()> exit,
              std::function<void()> force_exit);
  
  void exit(std::string_view reason) override;
  void force_exit(std::string_view reason) override;
  std::string_view impl_name() const noexcept override;
private:
  std::shared_ptr<Logger> logger;
  std::function<void()> fn_exit;
  std::function<void()> fn_force_exit;
};

}