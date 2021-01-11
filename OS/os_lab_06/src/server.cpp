#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <signal.h>
#include "zmq.hpp"
#include <string>
#include "zmq_func.hpp"
#include "avl.h"
#include <unistd.h>

using namespace std;

bool is_number(string val) {
    try{
        int tmp = stoi(val);
        return true;
    }
    catch(std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return false;
    }
}

void create(AvlTreeId& ids, zmq::socket_t& main_socket, int& child_pid, int& child_id){
    size_t node_id = 0;
    string str = "";
    string result = "";
    cin >> str;
    if(!is_number(str)) { return; }
    node_id = stoi(str);


    if(child_pid == 0) {
        main_socket.bind(get_port_name(PORT_BASE + node_id));
        child_pid = fork();
        if(child_pid == -1) {
            cout << "Unable to create first worker node\n";
            child_pid = 0;
            exit(1);
        } 
        else if(child_pid == 0) {   //дочерний процесс
            create_node(node_id, PORT_BASE + node_id);
        }
        else {      //родительский
            child_id = node_id;
            send_message(main_socket,"pid");
            result = recieve_message(main_socket);
        }
    } 

    else {
        string msg_s = "create " + std::to_string(node_id);
        send_message(main_socket, msg_s);
        result = recieve_message(main_socket);
    }

    if (result.substr(0,2) == "Ok") { ids.insert(node_id); }
    cout << result << "\n";
}



void remove(AvlTreeId& ids, zmq::socket_t& main_socket, int& child_pid, int& child_id){
    int node_id = 0;
    string str = "";
    cin >> str;
    if(!is_number(str)) { return; }
    node_id = stoi(str);
    if(ids.find(node_id) == nullptr) {
        cout << "Error: id doesn't exist\n";
        return;
    }
    if(child_pid == 0) {
        cout << "Error: Not found\n";
        return;
    }
    if(node_id == child_id) {
        kill(child_pid, SIGTERM);
        kill(child_pid, SIGKILL);
        child_id = 0;
        child_pid = 0;
        ids.erase(node_id);
        cout << "Ok\n";
        return;
    }
    string message_string = "remove " + to_string(node_id);
    send_message(main_socket, message_string);
    string recieved_message = recieve_message(main_socket);
    if(recieved_message.substr(0, min<int>(recieved_message.size(), 2)) == "Ok") {
        ids.erase(node_id);
    }
    cout << recieved_message << "\n";
}

void exec(AvlTreeId& ids, zmq::socket_t& main_socket,  int& child_pid, int& child_id) {
    string id_str = "";
    string n_str = "";
    int id = 0; 
    int n = 0;
    cin >> id_str >> n_str;
    if (!is_number(id_str) || !is_number(n_str)) { return; }
    id = stoi(id_str);
    n =  stoi(n_str);

    if(ids.find(id) == nullptr){
        cout << "Error: id doesn't exist\n";
        return;
    }
    if(n <= 0){
        cout << "Count mus't be > 0\n";
        return;
    }
    string numb_str = "";
    vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        cin >> numb_str;
        if(!is_number(numb_str)) { return; }
        numbers[i] = stoi(numb_str);
    }

    string message_string = "exec " + to_string(id) + " ";
    message_string += to_string(n);
    for (int i = 0; i < n; ++i) { 
        message_string += " " + to_string(numbers[i]);
    }

    send_message(main_socket, message_string);
    string recieved_message = recieve_message(main_socket);
    cout << recieved_message << "\n";
}

void pingall(AvlTreeId& ids, zmq::socket_t& main_socket, int& child_pid, int& child_id) {
            send_message(main_socket,"pingall");
            string recieved = recieve_message(main_socket);
            istringstream is;
            
            if (recieved.substr(0,std::min<int>(recieved.size(), 5)) == "Error") {
                is = istringstream("");
            } else {
                is = istringstream(recieved);
            }
            
            set<int> recieved_ids;
            int rec_id;
            while (is >> rec_id) {
                recieved_ids.insert(rec_id);
            }
            vector<int> from_tree = ids.get_nodes();
            auto part_it = partition(from_tree.begin(), from_tree.end(), [&recieved_ids] (int a) {  //разделение по условию
                return recieved_ids.count(a) == 0;
            });

            if (part_it == from_tree.begin()) {
                cout << "Ok:-1\n";
            } else {
                cout << "Ok:";
                for (auto it = from_tree.begin(); it != part_it; ++it) {
                    cout  << *it << " ";
                }
                cout << "\n";
            }
}

void help(){
    cout << "create <id> \n"  
         << "exec <id> <n> <k0...kn> \n"
         << "remove <id> \n"
         << "pingall\n"
         << "help\n"
         << "exit\n\n";
}

int main() {
    AvlTreeId ids;
    string command = "";
    int child_pid = 0;
    int child_id = 0;
    int linger = 0;
    zmq::context_t context(1);
    zmq::socket_t main_socket(context, ZMQ_REQ);
    main_socket.setsockopt(ZMQ_SNDTIMEO, WAIT_TIME);
    main_socket.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    //main_socket.bind(get_port_name(PORT_BASE));
 
    help();

    while(cin >> command){
        if(command == "create") {
            create(ids, main_socket, child_pid, child_id);
        } 
        else if(command == "remove") {
            remove(ids, main_socket, child_pid, child_id);
        } 
        else if(command == "exec") {
            exec(ids, main_socket, child_pid, child_id);
        }
        else if(command == "pingall"){
            pingall(ids, main_socket, child_pid, child_id);    
        }
        else if(command == "help"){
            help();
        }
        else if(command == "exit") {
            system("killall client");
            break; 
        }
    }
    return 0;
}
