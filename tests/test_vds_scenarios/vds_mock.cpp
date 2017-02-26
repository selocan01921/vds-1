/*
Copyright (c) 2017, Vadim Malyshev, lboss75@gmail.com
All rights reserved
*/
#include "stdafx.h"
#include "vds_mock.h"

void vds_mock::start(int client_count, int server_count)
{
  auto root_password = generate_password();
  int first_port = 8050;

  mock_client client(0);
  client.init_root(root_password, first_port);

  mock_server server(0, first_port);
  server.start();
  
  mock_client client1(1);
  client1.init_server(root_password, "127.0.0.1", first_port + 1);
  
}

std::string vds_mock::generate_password(size_t min_len, size_t max_len)
{
  size_t password_len = 0;
  while (password_len < min_len || password_len > max_len) {
    password_len = ((size_t)std::rand() % (max_len + 1));
  }

  std::string result;
  while (result.length() < password_len) {
    result += '0' + ((size_t)std::rand() % ('z' - '0'));
  }

  return result;
}

mock_client::mock_client(int index)
  : index_(index)
{
}

void mock_client::init_root(const std::string & root_password, int port)
{
  this->start_vds(false, [root_password, port](const vds::service_provider&sp) {

    vds::storage_log log(sp);
    log.reset(root_password, "127.0.0.1:" + std::to_string(port));

  });
}

void mock_client::init_server(
  const std::string& root_password,
  const std::server& address,
  int port)
{
  this->start_vds(true, [root_password, address, port](const vds::service_provider&sp) {
    sp.get<vds::iclient>().init_server(root_password, address, port);
  });
}


void mock_client::start_vds(bool full_client, const std::function<void(const vds::service_provider&sp)> & handler)
{
  vds::service_registrator registrator;

  vds::network_service network_service;
  vds::console_logger console_logger(vds::ll_trace);
  vds::crypto_service crypto_service;
  vds::client client;
  
  auto folder = vds::foldername(vds::filename::current_process().contains_folder(), std::to_string(this->index_));
  registrator.set_root_folders(folder, folder);

  registrator.add(console_logger);
  registrator.add(network_service);
  registrator.add(crypto_service);
  
  if(full_client){
    registrator.add(client);
  }

  auto sp = registrator.build();

  handler(sp);

  registrator.shutdown();
}

mock_server::mock_server(int index, int port)
  : index_(index),
  port_(port),
  console_logger_(vds::ll_trace)
{
}

void mock_server::start()
{
  auto folder = vds::foldername(vds::filename::current_process().contains_folder(), std::to_string(this->index_));
  this->registrator_.set_root_folders(folder, folder);
  
  this->registrator_.add(this->console_logger_);
  this->registrator_.add(this->network_service_);
  this->registrator_.add(this->task_manager_);
  this->registrator_.add(this->crypto_service_);
  this->registrator_.add(this->server_);

  auto sp = this->registrator_.build();
  
  sp.get<vds::iserver>().start_http_server("127.0.0.1", 8050 + this->index_);
}

void mock_server::stop()
{
  this->registrator_.shutdown();
}