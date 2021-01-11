#include <iostream>
#include "zmq_func.hpp"
#include <unistd.h>

using namespace std;

bool send_message(zmq::socket_t& socket, const string& message_string) {
    zmq::message_t message(message_string.size());
    memcpy(message.data(), message_string.c_str(), message_string.size());
    return socket.send(message);
}

string recieve_message(zmq::socket_t& socket) {
    zmq::message_t message;
    bool ok = false;
    try {
         ok = socket.recv(&message); 
    } 
    catch(...) {
         ok = false; 
    }
    string recieved_message(static_cast<char*>(message.data()), message.size());
    if(recieved_message.empty() || !ok) {
         return "Error: Node is not available"; 
    }
    return recieved_message;
}

void create_node(int id, int port) {
    char* arg0 = strdup(CLIENTNAME);
    char* arg1 = strdup((to_string(id)).c_str());
    char* arg2 = strdup((to_string(port)).c_str());
    char* args[] = {arg0, arg1, arg2, NULL};
    execv(CLIENTNAME, args);
}
string get_port_name(const int port) {
     return "tcp://127.0.0.1:" + to_string(port); 
}