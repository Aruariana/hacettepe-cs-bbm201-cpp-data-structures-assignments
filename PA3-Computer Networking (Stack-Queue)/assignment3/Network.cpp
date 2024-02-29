#include "Network.h"

Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */

    // Main loop
    for(int i = 0; i < commands.size(); i++) {
        string line = commands[i];
        string strips;
        for(int j = 0; j < line.size()+9; j++) {
            strips += "-";
        }
        cout << strips << endl;
        cout << "Command: " << line << endl;
        cout << strips << endl;
        istringstream iss(line);
        string command;
        getline(iss, command, ' ');

        if(command == "MESSAGE") {
            string sender_id;
            getline(iss, sender_id, ' ');
            string receiver_id;
            getline(iss, receiver_id, ' ');
            string message;
            getline(iss, message);
            message = message.substr(1, message.size()-2);
            cout << "Message to be sent: \"" << message << "\"" << endl << endl;
            fragMessageNQueue(clients, sender_id, receiver_id, message, sender_port, receiver_port, message_limit);
        }
        else if(command == "SEND") {
            sendAllFrames(clients);
        }
        else if(command == "RECEIVE") {
            receiveAllFrames(clients);
        }
    }
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.
    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "Error opening the file: " << filename << endl;
    }
    else {
        // Be careful with empty file
        string strSize;
        getline(file, strSize);
        int size = stoi(strSize);
        string line;
        for(int i = 0; i < size; i++) {
            getline(file, line);
            vector<string> items;
            istringstream iss(line);
            string item;
            while(getline(iss, item, ' ')) {
                items.push_back(item);
            }
            Client aClient(items[0], items[1], items[2]);
            clients.push_back(aClient);
        }
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "Error opening the file: " << filename << endl;
        return;
    }
    int idx = 0;
    string line;
    while(getline(file, line)) {
        if(line == "-") {
            idx++;
            continue;
        }
        pair<string, string> keyNValue("", "");
        istringstream iss(line);
        string item;
        bool isKey = true;
        while(getline(iss, item, ' ')) {
            if(isKey) {
                keyNValue.first = item;
                isKey = false;
            }
            else {
                keyNValue.second = item;
            }
        }
        clients[idx].routing_table.insert(keyNValue);
    }
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.
    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "Error opening the file: " << filename << endl;
    }
    else {
        string strSize;
        getline(file, strSize);
        int size = stoi(strSize);
        for(int i = 0; i < size; i++) {
            string line;
            getline(file, line);
            commands.push_back(line);
        }
    }
    return commands;
}

// Helper functions
stack<Packet*> Network::makeFrame(vector<Client> &clients, std::string &sender_id,
                                  std::string &receiver_id, std::string &message,
                                  const std::string &sender_port, const std::string &receiver_port, int frame_num) {
    stack<Packet*> frame;
    Client& sender = findClientByID(clients, sender_id);
    Client& receiver = findClientByID(clients, receiver_id);

    // Application layer
    frame.push(new ApplicationLayerPacket(0, sender.client_id, receiver.client_id, message));

    // Transport Layer
    frame.push(new TransportLayerPacket(1, sender_port, receiver_port));

    // Network Layer
    frame.push(new NetworkLayerPacket(2, sender.client_ip, receiver.client_ip));

    // Physical Layer
    Client& nextHopClient = findClientByID(clients, sender.routing_table[receiver.client_id]);
    PhysicalLayerPacket* plp = new PhysicalLayerPacket(3, sender.client_mac, nextHopClient.client_mac);
    plp->frame_num = frame_num;
    frame.push(plp);

    return frame;
}

void Network::fragMessageNQueue(vector<Client> &clients, std::string &sender_id, std::string &receiver_id,
                                std::string &message, const std::string &sender_port,
                                const std::string &receiver_port, int message_limit) {
    vector<string> frags;
    Client& sender = findClientByID(clients, sender_id);
    for(int i = 0; i < message.size(); i += message_limit) {
        frags.push_back(message.substr(i, message_limit));
    }

    for(int i = 0; i < frags.size(); i++) {
        stack<Packet*> frame = makeFrame(clients, sender_id, receiver_id, frags[i], sender_port, receiver_port, i+1);
        cout << "Frame #" << i+1 << endl;
        stdoutFrameInfo(frame);
        cout << "--------" << endl;
        sender.outgoing_queue.push(frame);
    }
    ActivityType activity = ActivityType::MESSAGE_SENT;
    Log log(to_string(time(nullptr)), message, frags.size(), 0,
            sender_id, receiver_id, true, activity);
    sender.log_entries.push_back(log);
}

Client& Network::findClientByID(vector<Client> &clients, std::string &client_id) {
    for(int i = 0; i < clients.size(); i++) {
        if(clients[i].client_id == client_id) {
            return clients[i];
        }
    }
    // Consider if there isn't a client
}

Client& Network::findClientByMAC(vector<Client> &clients, std::string &client_mac) {
    for(int i = 0; i < clients.size(); i++) {
        if(clients[i].client_mac == client_mac) {
            return clients[i];
        }
    }
}

bool Network::clientExistByID(vector<Client> &clients, std::string &client_id) {
    for(int i = 0; i < clients.size(); i++) {
        if(clients[i].client_id == client_id) {
            return true;
        }
    }
    return false;
}

void Network::sendAllFrames(vector<Client> &clients) {
    for(int i = 0; i < clients.size(); i++) {
        Client& current_client = clients[i];
        while(!current_client.outgoing_queue.empty()) {
            stack<Packet*> front = current_client.outgoing_queue.front();
            PhysicalLayerPacket* plp = dynamic_cast<PhysicalLayerPacket*>(front.top());
            Client& receiver = findClientByMAC(clients, plp->receiver_MAC_address);
            cout << "Client " << current_client.client_id << " sending frame #";
            cout << plp->frame_num << " to client " << receiver.client_id << endl;
            (plp->number_of_hops)++;
            stdoutFrameInfo(front);
            cout << "--------" << endl;
            receiver.incoming_queue.push(front);
            current_client.outgoing_queue.pop();
        }
    }
}

void Network::receiveAllFrames(vector<Client> &clients) {
    for(int i = 0; i < clients.size(); i++) {
        Client& current_client = clients[i];
        string message;
        while(!current_client.incoming_queue.empty()) {
            // Check if it reached final destination
            stack<Packet*> front = current_client.incoming_queue.front();
            PhysicalLayerPacket* plp = dynamic_cast<PhysicalLayerPacket*>(front.top());

            stack<Packet*> temp;
            for(int j = 0; j < 3; j++) {
                temp.push(front.top());
                front.pop();
            }
            ApplicationLayerPacket* alp = dynamic_cast<ApplicationLayerPacket*>(front.top());
            string sender_id = alp->sender_ID;
            string receiver_id = alp->receiver_ID;
            int hops = plp->number_of_hops;
            for(int j = 0; j < 3; j++) {
                front.push(temp.top());
                temp.pop();
            }

            if(current_client.client_id == receiver_id) {

                cout << "Client " << current_client.client_id << " receiving frame #" << plp->frame_num;
                cout << " from client " << findClientByMAC(clients, plp->sender_MAC_address).client_id;
                cout << " originating from client " << sender_id << endl;
                stdoutFrameInfo(front);
                current_client.incoming_queue.pop();

                // Message assembly and log
                message.append(alp->message_data);
                if(!current_client.incoming_queue.empty()) {
                    PhysicalLayerPacket* nextPlp = dynamic_cast<PhysicalLayerPacket*>(current_client.incoming_queue.front().top());
                    if(nextPlp->frame_num == 1) {
                        cout << "--------" << endl;
                        cout << "Client " << current_client.client_id << " received the message \"";
                        cout << message << "\" from client " << sender_id << "." << endl;
                        cout << "--------" << endl;
                        message = "";
                        ActivityType activity = ActivityType::MESSAGE_RECEIVED;
                        Log log(to_string(time(nullptr)), message, plp->frame_num, hops, sender_id,
                                receiver_id, true, activity);
                        current_client.log_entries.push_back(log);
                    }
                }
                else {
                    cout << "--------" << endl;
                    cout << "Client " << current_client.client_id << " received the message \"";
                    cout << message << "\" from client " << sender_id << "." << endl;
                    cout << "--------" << endl;
                    ActivityType activity = ActivityType::MESSAGE_RECEIVED;
                    Log log(to_string(time(nullptr)), message, plp->frame_num, hops, sender_id,
                            receiver_id, true, activity);
                    current_client.log_entries.push_back(log);
                }

                while(!front.empty()) {
                    Packet* temp2 = front.top();
                    front.pop();
                    delete temp2;
                }

            }
            // Forwarding
            else {
                // Check if it needs to be dropped
                if((current_client.routing_table.count(receiver_id) == 0) ||
                    !clientExistByID(clients, current_client.routing_table[receiver_id])) {
                    PhysicalLayerPacket* nextPlp = nullptr;
                    do {
                        front = current_client.incoming_queue.front();
                        plp = dynamic_cast<PhysicalLayerPacket*>(front.top());

                        for(int j = 0; j < 3; j++) {
                            temp.push(front.top());
                            front.pop();
                        }
                        alp = dynamic_cast<ApplicationLayerPacket*>(front.top());
                        sender_id = alp->sender_ID;
                        receiver_id = alp->receiver_ID;
                        hops = plp->number_of_hops;
                        for(int j = 0; j < 3; j++) {
                            front.push(temp.top());
                            temp.pop();
                        }

                        cout << "Client " << current_client.client_id << " receiving frame #" << plp->frame_num;
                        cout << " from client " << findClientByMAC(clients, plp->sender_MAC_address).client_id;
                        cout << ", but intended for client " << receiver_id << ". Forwarding..." << endl;
                        cout << "Error: Unreachable destination. Packets are dropped after " << hops << " hops!" << endl;
                        current_client.incoming_queue.pop();
                        if(!current_client.incoming_queue.empty()) {
                            nextPlp = dynamic_cast<PhysicalLayerPacket*>(current_client.incoming_queue.front().top());
                        }
                        if(nextPlp != nullptr) {
                            if(nextPlp->frame_num == 1) {
                                ActivityType activity = ActivityType::MESSAGE_DROPPED;
                                Log log(to_string(time(nullptr)), "", plp->frame_num, hops, sender_id,
                                        receiver_id, false, activity);
                                current_client.log_entries.push_back(log);
                                break;
                            }
                        }
                        if(current_client.incoming_queue.empty()) {
                            ActivityType activity = ActivityType::MESSAGE_DROPPED;
                            Log log(to_string(time(nullptr)), "", plp->frame_num, hops, sender_id,
                                    receiver_id, false, activity);
                            current_client.log_entries.push_back(log);
                        }
                        while(!front.empty()) {
                            Packet* temp2 = front.top();
                            front.pop();
                            delete temp2;
                        }
                    } while(!current_client.incoming_queue.empty());
                }
                else {
                    PhysicalLayerPacket* nextPlp = nullptr;
                    cout << "Client " << current_client.client_id << " receiving a message from client " << sender_id;
                    cout << ", but intended for client " << receiver_id << ". Forwarding..." << endl;
                    do {
                        front = current_client.incoming_queue.front();
                        plp = dynamic_cast<PhysicalLayerPacket*>(front.top());

                        for(int j = 0; j < 3; j++) {
                            temp.push(front.top());
                            front.pop();
                        }
                        alp = dynamic_cast<ApplicationLayerPacket*>(front.top());
                        sender_id = alp->sender_ID;
                        receiver_id = alp->receiver_ID;
                        hops = plp->number_of_hops;
                        for(int j = 0; j < 3; j++) {
                            front.push(temp.top());
                            temp.pop();
                        }

                        Client& newReceiver = findClientByID(clients, current_client.routing_table[receiver_id]);
                        plp->sender_MAC_address = current_client.client_mac;
                        plp->receiver_MAC_address = newReceiver.client_mac;
                        cout << "Frame #" << plp->frame_num << " MAC address change: New sender MAC " << current_client.client_mac;
                        cout << ", new receiver MAC " << newReceiver.client_mac << endl;
                        current_client.outgoing_queue.push(front);
                        current_client.incoming_queue.pop();
                        if(!current_client.incoming_queue.empty()) {
                            nextPlp = dynamic_cast<PhysicalLayerPacket*>(current_client.incoming_queue.front().top());
                        }
                        if(nextPlp != nullptr) {
                            if(nextPlp->frame_num == 1) {
                                ActivityType activity = ActivityType::MESSAGE_FORWARDED;
                                Log log(to_string(time(nullptr)), "", plp->frame_num, hops, sender_id,
                                        receiver_id, true, activity);
                                current_client.log_entries.push_back(log);
                                break;
                            }
                        }
                        if(current_client.incoming_queue.empty()) {
                            ActivityType activity = ActivityType::MESSAGE_DROPPED;
                            Log log(to_string(time(nullptr)), "", plp->frame_num, hops, sender_id,
                                    receiver_id, false, activity);
                            current_client.log_entries.push_back(log);
                        }
                    } while(!current_client.incoming_queue.empty());
                    cout << "--------" << endl;
                }
            }
            cout << "--------" << endl;
        }
    }
}

void Network::stdoutFrameInfo(stack<Packet*> frame) {
    stack<Packet*> temp;
    int number_of_hops = dynamic_cast<PhysicalLayerPacket*>(frame.top())->number_of_hops;
    for(int i = 0; i < 4; i++) {
        frame.top()->print();
        temp.push(frame.top());
        frame.pop();
    }
    cout << "Message chunk carried: \"" << dynamic_cast<ApplicationLayerPacket*>(temp.top())->message_data << "\"" << endl;
    cout << "Number of hops so far: " << number_of_hops << endl;

    for(int i = 0; i < 4; i++) {
        frame.push(temp.top());
        temp.pop();
    }
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}
