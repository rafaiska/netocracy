#ifndef _AUTH_COMM_H
#define _AUTH_COMM_H

#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <string>

class AuthorizationServerComms {
private:
  std::string auth_url;
  int status = 2;

public:
  static const int STATUS_CONNECTED = 0;
  static const int STATUS_ERROR = 1;
  static const int STATUS_DISCONNECTED = 2;

  AuthorizationServerComms(const std::string _auth_url) {
    this->auth_url = _auth_url;
  }
  void connect();
  int get_status() { return status; }
  // get_session(std::string username);
};

#endif