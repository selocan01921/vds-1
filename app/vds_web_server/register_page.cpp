/*
Copyright (c) 2017, Vadim Malyshev, lboss75@gmail.com
All rights reserved
*/

#include "stdafx.h"
#include "private/register_page.h"
#include "http_response.h"
#include "http_simple_form_parser.h"
#include "private/web_server_p.h"
#include "private/auth_session.h"

vds::async_task<vds::http_message> vds::register_page::post(
  const service_provider& sp,
  const std::shared_ptr<_web_server> & owner,
  const http_message& message) {
  auto parser = std::make_shared<http::simple_form_parser>();

  return parser->parse(sp, message).then([sp, owner, parser]() -> async_task<http_message>{
    auto invitation = parser->values().find("inputInvitation");
    auto login = parser->values().find("inputEmail");
    auto password = parser->values().find("inputPassword");

    if (
        invitation == parser->values().end()
        || login == parser->values().end()
        || password == parser->values().end()) {
      return async_task<http_message>::result(
          http_response::redirect(sp, "/error/?code=InvalidRegister"));
    }

    auto session_id = guid::new_guid().str();
    auto session = std::make_shared<auth_session>(login->second, password->second);
    return session->create_user(sp).then([sp, owner, session_id, session]() {
      owner->add_auth_session(session_id, session);

      http_response response(302, "Found");
      response.add_header("Location", "/");
      response.add_header("Set-Cookie", "Auth=" + session_id);
      auto result = response.create_message(sp);
      result.body()->write_async(nullptr, 0).execute(
          [sp](const std::shared_ptr<std::exception> &ex) {
            if (ex) {
              sp.unhandled_exception(ex);
            }
          });

      return result;
    });
  });
}