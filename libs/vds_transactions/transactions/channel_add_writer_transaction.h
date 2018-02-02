#ifndef __VDS_TRANSACTIONS_CHANNEL_ADD_WRITER_TRANSACTION_H_
#define __VDS_TRANSACTIONS_CHANNEL_ADD_WRITER_TRANSACTION_H_
/*
Copyright (c) 2017, Vadim Malyshev, lboss75@gmail.com
All rights reserved
*/

#include "cert_control.h"
#include "guid.h"
#include "asymmetriccrypto.h"
#include "transaction_log.h"
#include "symmetriccrypto.h"
#include "channel_message_transaction.h"

namespace vds {
  namespace transactions {
    class channel_add_writer_transaction : public channel_message_transaction {
    public:
      channel_add_writer_transaction(
          const guid & channel_id,
          const certificate & target_cert,
          const certificate & sing_cert,
          const asymmetric_private_key & sing_cert_private_key,
		  const guid & target_channel_id,
		  const std::string & name,
          const certificate & read_cert,
          const certificate & write_cert,
          const asymmetric_private_key & write_private_key)
      : channel_message_transaction(
          channel_message_id::channel_add_writer_transaction,
          channel_id,
          target_cert,
		  cert_control::get_id(sing_cert),
          sing_cert_private_key,
          (
            binary_serializer()
			    << target_channel_id
			    << name
                << read_cert.der()
                << write_cert.der()
                << write_private_key.der(std::string())
          ).data()) {
      }

      channel_add_writer_transaction(binary_deserializer & s)
          : channel_message_transaction(s){
      }

	  template <typename target>
	  static void parse_message(binary_deserializer &data_stream, target t) {

		  std::string name;
		  guid target_channel_id;
		  certificate read_cert;
		  certificate write_cert;
		  const_data_buffer write_private_key_der;

		  data_stream
      		>> target_channel_id 
      		>> name
      		>> read_cert
      		>> write_cert
      		>> write_private_key_der;

		  t(
			  target_channel_id,
			  name,
			read_cert,
			write_cert,
			asymmetric_private_key::parse_der(write_private_key_der, std::string()));

	  }
    };
  }
}


#endif //__VDS_TRANSACTIONS_CHANNEL_ADD_WRITER_TRANSACTION_H_