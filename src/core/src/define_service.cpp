#include <core/define_service.hpp>

#include <iostream>

class Singleton {
private: 
    Singleton() {}
    Singleton( const Singleton&);  
    Singleton& operator=( Singleton& );
public:
    std::vector<core::ServiceBuilder*> builders;
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }    
}; 

/*namespace {
  static std::vector<core::ServiceBuilder*> g_builders;
  int counter = 0;
}*/
namespace core {

void register_service_builder(ServiceBuilder* builder) {
  get_registered_service_builders().push_back(builder);
}
std::vector<ServiceBuilder*>& get_registered_service_builders() {
  //std::cout << "SIZE: " << Singleton::getInstance().builders.size() << std::endl;
  static std::vector<ServiceBuilder*> builders;
  return builders;
}

}
