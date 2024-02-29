#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {
public:
    Network();
    ~Network();

    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port,
                     const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename);

    //Helper functions
    stack<Packet*> makeFrame(vector<Client> &clients, string &sender_id,
                             string &receiver_id, string &message,
                             const string &sender_port, const string &receiver_port, int frame_num);
    void fragMessageNQueue(vector<Client> &clients, string &sender_id,
                           string &receiver_id, string &message,
                           const string &sender_port, const string &receiver_port, int message_limit);
    bool clientExistByID(vector<Client> &clients, string &client_id);
    Client& findClientByID(vector<Client> &clients, std::string &client_id);
    Client& findClientByMAC(vector<Client> &clients, std::string &client_mac);
    void sendAllFrames(vector<Client> &clients);
    void receiveAllFrames(vector<Client> &clients);

    void stdoutFrameInfo(stack<Packet*> frame);
};

#endif  // NETWORK_H
