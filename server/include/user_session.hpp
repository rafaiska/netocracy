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
  UserSession(std::string _username, long _session_id, std::string _key_base64,
              long _expiration_time)
      : username(_username), session_id(_session_id), key_base64(_key_base64),
        expiration_time(_expiration_time) {}
};
} // namespace ncy

#endif