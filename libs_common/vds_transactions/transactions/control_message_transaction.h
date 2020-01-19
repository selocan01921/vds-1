#ifndef __VDS_TRANSACTIONS_CONTROL_MESSAGE_TRANSACTION_H_
#define __VDS_TRANSACTIONS_CONTROL_MESSAGE_TRANSACTION_H_

/*
Copyright (c) 2017, Vadim Malyshev, lboss75@gmail.com
All rights reserved
*/
#include <unordered_map>
#include "binary_serialize.h"
#include "channel_message_id.h"
#include "json_object.h"
#include "json_parser.h"

namespace vds {
	namespace transactions {

		class control_message_transaction {
		public:
      static const channel_message_id message_id = channel_message_id::control_message_transaction;


      std::shared_ptr<json_value> message;
      std::map<std::string, const_data_buffer> attachments;

      template <typename  visitor_type>
      void visit(visitor_type & v) {
        v(
          message,
          attachments
        );
      }

      static constexpr const char * create_wallet_type = "create_wallet";

      static expected<control_message_transaction> create_wallet_message(
        const std::string & name,
        const asymmetric_public_key & public_key,
        const asymmetric_private_key & private_key) {
        auto message = std::make_shared<json_object>();
        message->add_property("$type", create_wallet_type);
        message->add_property("name", name);

        std::map<std::string, const_data_buffer> attachments;
        GET_EXPECTED_VALUE(attachments["public_key"], public_key.der());
        GET_EXPECTED_VALUE(attachments["key"], private_key.der(std::string()));

        return message_create<transactions::control_message_transaction>(
          std::move(message),
          std::move(attachments));
      }
    };
}

}

#endif //__VDS_TRANSACTIONS_CONTROL_MESSAGE_TRANSACTION_H_