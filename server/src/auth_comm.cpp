#include "auth_comm.hpp"

void AuthorizationServerComms::connect() {
  try {
    curlpp::Easy request;

    request.setOpt(new curlpp::options::Url(this->auth_url));
    request.setOpt(new curlpp::options::Verbose(true));

    std::list<std::string> header;
    header.push_back("Content-Type: application/octet-stream");

    request.setOpt(new curlpp::options::HttpHeader(header));

    request.setOpt(new curlpp::options::PostFields("abcd"));
    request.setOpt(new curlpp::options::PostFieldSize(5));

    request.perform();
  } catch (curlpp::LogicError &e) {
    std::cout << e.what() << std::endl;
    this->status = STATUS_ERROR;
  } catch (curlpp::RuntimeError &e) {
    std::cout << e.what() << std::endl;
    this->status = STATUS_ERROR;
  }
  this->status = STATUS_CONNECTED;
}