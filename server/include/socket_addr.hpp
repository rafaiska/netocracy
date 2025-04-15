#ifndef _SOCKET_ADDR_H
#define _SOCKET_ADDR_H

#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <stdint.h>
#include <sys/socket.h>

namespace ncy {
class SocketAddress {
public:
  SocketAddress(uint32_t inAddress, uint16_t inPort) {
    GetAsSockAddrIn()->sin_family = AF_INET;
    GetAsSockAddrIn()->sin_addr.s_addr = htonl(inAddress);
    GetAsSockAddrIn()->sin_port = htons(inPort);
  }
  SocketAddress(const sockaddr &inSockAddr) {
    memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
  }
  size_t GetSize() const { return sizeof(sockaddr); }

private:
  sockaddr mSockAddr;
  sockaddr_in *GetAsSockAddrIn() {
    return reinterpret_cast<sockaddr_in *>(&mSockAddr);
  }
};
typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

} // namespace ncy

#endif