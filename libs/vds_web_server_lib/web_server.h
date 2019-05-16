#ifndef __VDS_WEB_SERVER_LIB_WEB_SERVER_H_
#define __VDS_WEB_SERVER_LIB_WEB_SERVER_H_
#include "service_provider.h"

/*
Copyright (c) 2017, Vadim Malyshev, lboss75@gmail.com
All rights reserved
*/


namespace vds {
  class web_server : public iservice_factory
  {
  public:
    web_server();
    ~web_server();

    expected<void> register_services(service_registrator &) override;
    expected<void> start(const service_provider *) override;
    expected<void> stop() override;
    vds::async_task<vds::expected<void>> prepare_to_stop() override;

    operator bool () const {
      return nullptr != this->impl_;
    }

    class _web_server * operator -> () const;

    uint16_t port() const {
      return this->port_;
    }

    void port(uint16_t value) {
      this->port_ = value;
    }

  private:
    uint16_t port_;
    std::shared_ptr<class _web_server> impl_;
  };
}

#endif // __VDS_WEB_SERVER_LIB_WEB_SERVER_H_