#include "Leaderboard.h"

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).
    // Check if the linked list is empty
    if(head_leaderboard_entry == nullptr) {
        head_leaderboard_entry = new_entry;
    }
    else {
        LeaderboardEntry* p2 = nullptr;
        for(LeaderboardEntry* p = head_leaderboard_entry; p != nullptr; p = p->next_leaderboard_entry) {
            // Check if there is a same player in the list
            if(new_entry->player_name == p->player_name) {
                // Check if the new entry's score is higher or lower than the previous score
                // Careful with >= or >
                if(new_entry->score >= p->score) {
                    if(new_entry->score == p->score) {
                        return;
                    }
                    // Delete previous entry and insert the new one
                    if(p == head_leaderboard_entry) {
                        head_leaderboard_entry = head_leaderboard_entry->next_leaderboard_entry;
                    }
                    else {
                        p2->next_leaderboard_entry = p->next_leaderboard_entry;
                    }
                    delete p;
                    insert_new_entry(new_entry);
                    break;
                }
            }
            p2 = p;
        }

        // After making sure there is no same player, insert the new entry
        bool inserted = false;
        LeaderboardEntry* p3 = nullptr;
        for(LeaderboardEntry* p = head_leaderboard_entry; p != nullptr; p = p->next_leaderboard_entry) {
            if(new_entry->score > p->score) {
                if(p == head_leaderboard_entry) {
                    head_leaderboard_entry = new_entry;
                    head_leaderboard_entry->next_leaderboard_entry = p;
                }
                else {
                    p3->next_leaderboard_entry = new_entry;
                    new_entry -> next_leaderboard_entry = p;
                }
                inserted = true;
                break;
            }
            p3 = p;
        }
        if(!inserted) {
            p3->next_leaderboard_entry = new_entry;
            new_entry->next_leaderboard_entry = nullptr;
        }

        // Count list elements to determine size, if it is bigger than 11 delete last one
        int size = 0;
        for(LeaderboardEntry* p = head_leaderboard_entry; p != nullptr; p = p->next_leaderboard_entry) {
            size++;
        }

        if(size > 10) {
            LeaderboardEntry* p4 = nullptr;
            LeaderboardEntry* p_ = head_leaderboard_entry;
            for(;p_->next_leaderboard_entry != nullptr; p_ = p_->next_leaderboard_entry) {
                p4 = p_;
            }
            p4->next_leaderboard_entry = nullptr;
            delete p_;
        }
    }
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions

    ofstream oFile(filename, std::ios::out | std::ios::trunc);
    if(!oFile.is_open()) {
        cerr << "Error opening the file: " << filename << endl;
        return;
    }

    if(head_leaderboard_entry == nullptr) {
        return;
    }

    for(LeaderboardEntry* p = head_leaderboard_entry; p != nullptr; p = p->next_leaderboard_entry) {
        oFile << p->score << " " << p->last_played << " " << p->player_name;
        if(p->next_leaderboard_entry != nullptr) {
            oFile << endl;
        }
    }
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.

    ifstream iFile(filename);
    if(iFile.is_open()) {
        // Check if the file is empty
        iFile.seekg(0, std::ios::end);
        if(iFile.tellg() == 0) {
            return;
        }
        iFile.seekg(0, std::ios::beg);
        // Read from the file
        vector<vector<string>> leaderboardVec;
        string line;
        while(getline(iFile, line)) {
            if(line.empty()) {
                break;
            }
            istringstream iss(line);
            string element;
            vector<string> row;
            while(getline(iss, element, ' ')) {
                row.push_back(element);
            }
            leaderboardVec.push_back(row);
        }

        head_leaderboard_entry = new LeaderboardEntry(
                                    stoul(leaderboardVec[0][0]),
                                    stoi(leaderboardVec[0][1]),
                                    leaderboardVec[0][2]);
        LeaderboardEntry* p = head_leaderboard_entry;
        for(int i = 1; i < leaderboardVec.size(); i++) {
            LeaderboardEntry* nextEntry = new LeaderboardEntry(
                    stoul(leaderboardVec[i][0]),
                    stoi(leaderboardVec[i][1]),
                    leaderboardVec[i][2]);
            p->next_leaderboard_entry = nextEntry;
            p = p->next_leaderboard_entry;
        }
    }
    else {
        ofstream oFile(filename);
        oFile.close();
    }
}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    cout << "Leaderboard" << endl;
    cout << "-----------" << endl;
    if(head_leaderboard_entry == nullptr) {
        return;
    }

    int i = 1;
    for(LeaderboardEntry* p = head_leaderboard_entry; p != nullptr; p = p->next_leaderboard_entry) {
        char formattedTime[100];
        std::strftime(formattedTime, sizeof(formattedTime), "%H:%M:%S/%d.%m.%Y", std::localtime(&p->last_played));
        cout << i << ". " << p->player_name << " " << p->score << " " << formattedTime << endl;
        i++;
    }
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    // Free main blocks
    if(head_leaderboard_entry != nullptr) {
        while(head_leaderboard_entry->next_leaderboard_entry != nullptr) {
            LeaderboardEntry* temp = head_leaderboard_entry->next_leaderboard_entry;
            head_leaderboard_entry->next_leaderboard_entry = temp->next_leaderboard_entry;
            delete temp;
        }
        delete head_leaderboard_entry;
    }
}
