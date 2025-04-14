#include "include/auth_comm.hpp"
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {
  AuthorizationServerComms comms("localhost:8000/authorize_login_node");
  comms.connect();
  std::cout << comms.get_status() << '\n';

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }
}
