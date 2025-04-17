#ifndef _SOCKET_ADDR_FACTORY_H
#define _SOCKET_ADDR_FACTORY_H

#include "socket_addr.hpp"
#include <netdb.h>

namespace ncy {
class SocketAddressFactory {
public:
  static SocketAddressPtr CreateIPv4FromString(const std::string &inString);
};
} // namespace ncy

#endif