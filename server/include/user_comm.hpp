#ifndef _USER_COMM_H_
#define _USER_COMM_H_

#include "socket_addr.hpp"

namespace ncy {

class UserCommunication {
private:
  ncy::SocketAddressPtr broadcast_address;

public:
  UserCommunication(char *ip_address);
};

} // namespace ncy

#endif