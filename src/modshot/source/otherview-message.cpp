#include "otherview-message.h"
#include <zmqpp/zmqpp.hpp>
#include "config.h"
#include "debugwriter.h"

OtherViewMessager::OtherViewMessager(const Config &c):
    conf(c)
{
    Debug() << "Setting up otheview";
    isOtherView = c.isOtherView;
    otherViewEndpoint = "tcp://127.0.0.1:9697";
    normalEndpoint = "tcp://127.0.0.1:7536";

    Debug() << "Setting up contexts";
    if (isOtherView) {
        otherview_socket_type = zmqpp::socket_type::push;
        normal_socket_type = zmqpp::socket_type::pull;
    } else {
        otherview_socket_type = zmqpp::socket_type::pull;
        normal_socket_type = zmqpp::socket_type::push;
    }

    otherview_socket = new zmqpp::socket (otherview_context, otherview_socket_type);
    normal_socket = new zmqpp::socket (normal_context, normal_socket_type);

    Debug() << "Attempting to connect to otherview at " << otherViewEndpoint;
    Debug() << "Attempting to connect to normal at " << normalEndpoint;
    if (isOtherView) {
        otherview_socket->connect(otherViewEndpoint);
        normal_socket->connect(normalEndpoint);
    } else {
        otherview_socket->bind(otherViewEndpoint);
        normal_socket->bind(normalEndpoint);
    }
}

bool OtherViewMessager::sendMsg(const char* string)
{
    zmqpp::message message;
    message << string;
    bool ret;
    if (isOtherView) {
        ret = otherview_socket->send(message, true);
    } else {
        ret = normal_socket->send(message, true);
    }
    //Debug() << "Sent message: ";
    //Debug() << string;
    //Debug() << ret;

    return ret;
}

std::string OtherViewMessager::getMsg()
{
    zmqpp::message message;
    std::string response;
    bool ret;
    if (isOtherView) {
        ret = normal_socket->receive(message, true);
    } else {
        ret = otherview_socket->receive(message, true);
    }
    if (ret) {
        message >> response;
    } else {
        response = "";
    }
    //Debug() << "Received message: ";
    //Debug() << response;
    //Debug() << ret;
    return response;
}

void OtherViewMessager::close() 
{
    Debug() << "Closing sockets";
    otherview_socket->close();
    normal_socket->close();
    Debug() << "Deleting sockets";
    delete(otherview_socket);
    delete(normal_socket);
}