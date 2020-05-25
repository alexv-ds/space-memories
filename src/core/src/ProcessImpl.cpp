#include <cassert>
#include "ProcessImpl.hpp"

namespace core {

ProcessImpl::ProcessImpl(std::shared_ptr<Logger> logger,
                         std::function<void()> exit,
                         std::function<void()> force_exit):
          //Init
          logger(std::move(logger)),
          fn_exit(std::move(exit)),
          fn_force_exit(std::move(force_exit))
{
  assert(fn_exit != nullptr);
  assert(fn_force_exit != nullptr);
}

void ProcessImpl::exit(std::string_view reason) {
  logger->info("Выход из программы. Причина: {}", reason);
  fn_exit();
}

void ProcessImpl::force_exit(std::string_view reason) {
  logger->critical("Принудительный выход из программы. Причина: {}", reason);
  fn_force_exit();
}

std::string_view ProcessImpl::impl_name() const noexcept {
  return "core::ProcessImpl";
}

}