/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef GRPC_TEST_H
#define GRPC_TEST_H
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
// 
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
// #include <helloworld.grpc.pb.h>
#include "helloworld.grpc.pb.h"
// #include <proto_generated_file/helloworld.grpc.pb.h>

namespace ns3 {

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;
/* ... */

}

#endif /* GRPC_TEST_H */

