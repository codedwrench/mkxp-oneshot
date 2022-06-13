#include "etc.h"
#include <string>
#include <zmqpp/zmqpp.hpp>

struct Config;

class OtherViewMessager {
    const Config &conf;
    bool isOtherView;
    std::string otherViewEndpoint;
    std::string normalEndpoint;

    zmqpp::context otherview_context;
    zmqpp::context normal_context;
    zmqpp::socket_type otherview_socket_type;
    zmqpp::socket_type normal_socket_type;

    zmqpp::socket *otherview_socket;
    zmqpp::socket *normal_socket;
public:
    OtherViewMessager(const Config &c);
    bool sendMsg(const char* message);
    std::string getMsg();
    void close();
};