#ifndef __VDS_HTTP_HTTP_REQUEST_SERIALIZER_H_
#define __VDS_HTTP_HTTP_REQUEST_SERIALIZER_H_

/*
Copyright (c) 2017, Vadim Malyshev, lboss75@gmail.com
All rights reserved
*/
#include "http_stream_reader.h"

namespace vds {
  class http_response;
  class http_outgoing_stream;

  class http_request_serializer
  {
  public:
    template<
      typename context_type
    >
    class handler : public sequence_step<
        context_type,
        void (
          const void * data,
          size_t len
        )
      >
    {
      using base_class = sequence_step<
        context_type,
        void (
          const void * data,
          size_t len
        )
      >;
    public:
      handler(
        const context_type & context,
        const http_request_serializer & args)
        : base_class(context)
      {
      }

      void operator()(
        http_request & request,
        http_outgoing_stream & outgoing_stream
        )
      {
        if (request.empty()) {
          this->next(nullptr, 0);
        }
        else {
          std::stringstream stream;
          stream << request.method() << " " << request.url() << " " << request.agent() << "\n";
          for (auto & header : request.headers()) {
            stream << header << "\n";
          }
          
          stream << "Content-Length:" << outgoing_stream.size() << "\n\n";
          
          if(outgoing_stream.is_simple()) {
            stream << outgoing_stream.body();
          }

          this->header_ = stream.str();

          this->next(
            this->header_.c_str(),
            this->header_.size());
        }
      }

    private:
      std::string header_;
    };
  };
}

#endif // __VDS_HTTP_HTTP_REQUEST_SERIALIZER_H_