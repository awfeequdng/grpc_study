#include <iostream>

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloResponse;
using helloworld::Greeter; 

class GreeterClient {
    public:
    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) {}
    
    std::string SayHello(const std::string &user) {
        HelloRequest request;
        request.set_name(user);

        HelloResponse response;

        ClientContext context;
        Status status = stub_->SayHello(&context, request, &response);

        if (status.ok()) {
            return response.message();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }
    }
    private:
        std::unique_ptr<Greeter::Stub> stub_;
};


int main(int argc, char **argv) {
    std::string target_str;
    std::string arg_str("--target");

    if (argc > 1) {
        std::string arg_val = argv[1];
        size_t start_pos = arg_val.find(arg_str);
        if (start_pos != std::string::npos) {
            start_pos += arg_str.size();
            if (arg_val[start_pos] == '=') {
                target_str = arg_val.substr(start_pos + 1);
            } else {
                std::cout<< "The only corrent argument syntax is --target=" << std::endl;
                return 0;
            }
        } else {
            std::cout << "The only acceptable argument is --target=" << std::endl;
            return 0;
        }
    } else {
        target_str = "localhost:50051";
    }
    GreeterClient greeter(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    std::string user("world");
    std::string response = greeter.SayHello(user);
    std::cout << "Greeter received: " << response << std::endl;
}