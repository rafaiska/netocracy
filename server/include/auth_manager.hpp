#ifndef _AUTH_MANAGER_H
#define _AUTH_MANAGER_H

#include "user_session.hpp"
#include <string>
#include <unordered_map>

class AuthorizationManager {
private:
  std::unordered_map<std::string, ncy::UserSession> sessions;

public:
};

#endif