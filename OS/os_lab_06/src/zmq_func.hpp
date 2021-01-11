#include "zmq.hpp"
#include "avl.h"
#include <string>

#define CLIENTNAME "./client"
   
const int WAIT_TIME = 1000;
const int PORT_BASE  = 4040;

bool send_message(zmq::socket_t&,const std::string&);
std::string recieve_message(zmq::socket_t&);
void create_node(int, int);
std::string get_port_name(const int port);
