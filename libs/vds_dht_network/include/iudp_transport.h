#ifndef __VDS_DHT_NETWORK_IUDP_TRANSPORT_H_
#define __VDS_DHT_NETWORK_IUDP_TRANSPORT_H_

/*
Copyright (c) 2017, Vadim Malyshev, lboss75@gmail.com
All rights reserved
*/

#include "service_provider.h"
#include "async_task.h"

namespace vds {
  class udp_datagram;

  namespace dht {
    namespace network {
      class iudp_transport : public std::enable_shared_from_this<iudp_transport> {
      public:
        virtual void start(
          const service_provider& sp,
          const certificate & node_cert,
          const asymmetric_private_key & node_key,
          uint16_t port) = 0;

        virtual void stop(const service_provider& sp) = 0;

        virtual async_task<> write_async(const service_provider& sp, const udp_datagram& datagram) = 0;
        virtual async_task<> try_handshake(const service_provider& sp, const std::string& address) = 0;
      };
    }
  }
}


#endif //__VDS_DHT_NETWORK_IUDP_TRANSPORT_H_