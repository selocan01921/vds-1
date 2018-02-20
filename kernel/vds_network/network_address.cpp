#include "stdafx.h"
#include "network_address.h"

vds::network_address::network_address()
: addr_size_(sizeof(this->addr_)) {
  memset((char *)&this->addr_, 0, sizeof(this->addr_));
}

vds::network_address::network_address(sa_family_t af, const std::string &server, uint16_t port) {
  memset((char *)&this->addr_, 0, sizeof(this->addr_));

  switch(af) {
    case AF_INET: {
      auto addr = (sockaddr_in *) &this->addr_;
      addr->sin_family = af;
      addr->sin_port = htons(port);
      addr->sin_addr.s_addr = inet_addr(server.c_str());

      this->addr_size_ = sizeof(sockaddr_in);
      break;
    }
    case AF_INET6: {
      auto addr = (sockaddr_in6 *) &this->addr_;
      addr->sin6_family = af;
      addr->sin6_port = htons(port);
      inet_pton(AF_INET6, server.c_str(), &addr->sin6_addr);

      this->addr_size_ = sizeof(sockaddr_in6);
      break;
    }
    default:
      throw std::runtime_error("Invalid error");
  }
}

std::string vds::network_address::to_string() const {
  return this->server() + ":" + std::to_string(this->port());
}

std::string vds::network_address::server() const {
  char buffer[512];

  auto addr = (sockaddr *)&this->addr_;
  inet_ntop(addr->sa_family, addr, buffer, sizeof(buffer));

  return buffer;
}

uint16_t vds::network_address::port() const {
  auto addr = (sockaddr *)&this->addr_;
  switch(addr->sa_family) {
    case AF_INET:{
      return ntohs(reinterpret_cast<const sockaddr_in *>(&this->addr_)->sin_port);
    }
    case AF_INET6:{
      return ntohs(reinterpret_cast<const sockaddr_in6 *>(&this->addr_)->sin6_port);
    }
    default: {
      throw std::runtime_error("Invalid error");
    }
  }
}

vds::network_address::network_address(sa_family_t af, uint16_t port) {
  memset((char *)&this->addr_, 0, sizeof(this->addr_));

  switch(af) {
    case AF_INET: {
      auto addr = (sockaddr_in *) &this->addr_;
      addr->sin_family = af;
      addr->sin_port = htons(port);
      addr->sin_addr.s_addr = INADDR_ANY;

      this->addr_size_ = sizeof(sockaddr_in);
      break;
    }
    case AF_INET6: {
      auto addr = (sockaddr_in6 *) &this->addr_;
      addr->sin6_family = af;
      addr->sin6_port = htons(port);
      addr->sin6_addr = IN6ADDR_ANY_INIT;

      this->addr_size_ = sizeof(sockaddr_in6);
      break;
    }
    default:
      throw std::runtime_error("Invalid error");
  }

}

bool vds::network_address::is_martian() const {
  auto addr = (const sockaddr *)&this->addr_;
  switch(addr->sa_family) {
    case AF_INET: {
      struct sockaddr_in *sin = (struct sockaddr_in*)&this->addr_;
      const unsigned char *address = (const unsigned char*)&sin->sin_addr;
      return sin->sin_port == 0 ||
             (address[0] == 0) ||
             (address[0] == 127) ||
             ((address[0] & 0xE0) == 0xE0);
    }
    case AF_INET6: {
      static const unsigned char zeroes[20] = {0};
      static const unsigned char v4prefix[16] = {
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0
      };
      struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)&this->addr_;
      const unsigned char *address = (const unsigned char*)&sin6->sin6_addr;
      return sin6->sin6_port == 0 ||
             (address[0] == 0xFF) ||
             (address[0] == 0xFE && (address[1] & 0xC0) == 0x80) ||
             (memcmp(address, zeroes, 15) == 0 &&
              (address[15] == 0 || address[15] == 1)) ||
             (memcmp(address, v4prefix, 12) == 0);
    }
    default:
      throw std::runtime_error("Invalid state");
  }
}

