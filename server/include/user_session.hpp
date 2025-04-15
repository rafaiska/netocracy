#ifndef _USER_SESSION_H_
#define _USER_SESSION_H_

#include <string>

namespace ncy {

class UserSession {
private:
  std::string username;
  long session_id;
  std::string key_base64;
  long expiration_time;

public:
};
} // namespace ncy

#endif