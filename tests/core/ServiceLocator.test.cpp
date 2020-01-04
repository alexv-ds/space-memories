#include <catch2/catch.hpp>
#include <core/ServiceLocator.hpp>

class TestService1 final : public core::IService {};

TEST_CASE("ServiceLocator::set возвращает true, если "
          "сервис устанавливается 1й раз","[core][ServiceLocator]" )
{
  core::ServiceLocator locator;
  std::shared_ptr<TestService1> service = std::make_shared<TestService1>();
  REQUIRE(locator.set<TestService1>(service) == true);
}

TEST_CASE("ServiceLocator::set возвращает false, если "
          "сервис устанавливается не в 1й раз","[core][ServiceLocator]" )
{
  core::ServiceLocator locator;
  std::shared_ptr<TestService1> service = std::make_shared<TestService1>();
  locator.set<TestService1>(service);
  REQUIRE(locator.set<TestService1>(service) == false);
}

TEST_CASE("ServiceLocator::get возвращает корректный указатель","[core][ServiceLocator]" )
{
  core::ServiceLocator locator;
  std::shared_ptr<TestService1> service = std::make_shared<TestService1>();
  locator.set<TestService1>(service);
  std::shared_ptr<TestService1> returned_service = locator.get<TestService1>();
  REQUIRE(service == returned_service);
}

TEST_CASE("ServiceLocator::get возвращает nullptr "
          "если запрашиваемый сервис не был установлен","[core][ServiceLocator]" )
{
  core::ServiceLocator locator;
  std::shared_ptr<TestService1> returned_service = locator.get<TestService1>();
  REQUIRE(returned_service == nullptr);
}

TEST_CASE("ServiceLocator::reset удаляет сервис","[core][ServiceLocator]" )
{
  core::ServiceLocator locator;
  locator.set<TestService1>(std::make_shared<TestService1>());
  locator.reset<TestService1>();
  REQUIRE(locator.get<TestService1>() == nullptr);
}

TEST_CASE("ServiceLocator::reset возвращает true, если "
          "запрашиваемый сервис существует","[core][ServiceLocator]" )
{
  core::ServiceLocator locator;
  locator.set<TestService1>(std::make_shared<TestService1>());
  REQUIRE(locator.reset<TestService1>() == true);
}

TEST_CASE("ServiceLocator::reset возвращает false, если "
          "запрашиваемого сервиса не существует","[core][ServiceLocator]" )
{
  core::ServiceLocator locator;
  REQUIRE(locator.reset<TestService1>() == false);
}
