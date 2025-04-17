#include "user_comm.hpp"
#include "socket_addr_factory.hpp"

ncy::UserCommunication::UserCommunication(std::string &ip_address) 
{
    this->broadcast_address = SocketAddressFactory::CreateIPv4FromString(ip_address);
}
