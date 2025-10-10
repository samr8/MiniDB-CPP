#include "database.h"
#include <iostream>
#include <sstream>
#include <algorithm> // for std::remove

using namespace std;


int main() {
    Database db;
    string command;

    cout << "MiniDB (type EXIT to quit)\nMiniDB> ";

    while (getline(cin, command)) {
        stringstream ss(command);
        string word;
        ss >> word;

        if (word == "CREATE") {
            string type; ss >> type;
            if (type == "DATABASE") {
                string dbName; ss >> dbName;
                db.createDatabase(dbName);
            } else if (type == "TABLE") {
                string tableName; ss >> tableName;
                vector<string> cols;
                string col;
                while (ss >> col) cols.push_back(col);
                db.createTable(tableName, cols);
            }
        }
        else if (word == "USE") {
            string dbName; ss >> dbName;
            db.useDatabase(dbName);
        }
        else if (word == "INSERT") {
            string into, tableName; ss >> into >> tableName;
            vector<string> vals;
            string val;
            while (ss >> val) vals.push_back(val);
            db.insertIntoTable(tableName, vals);
        }
        else if (word == "SELECT") {
            string star, from, tableName; ss >> star >> from >> tableName;
            db.selectAll(tableName);
        }
        else if (word == "DROP") {
            string type, name; ss >> type >> name;
            if (type == "TABLE") db.dropTable(name);
        }
        else if (word == "EXIT" || word =="exit") {
            break;
        }
        else if (word == "DELETE") {
    string from, tableName, where;
    ss >> from >> tableName >> where; // DELETE FROM table WHERE
    string condition;
    getline(ss, condition);           // rest of line contains "column=value"
    condition.erase(0, condition.find_first_not_of(" ")); // trim leading spaces
    db.deleteFromTable(tableName, condition);
}


        else {
            cout << "Unknown command.\n";
        }
        cout << "MiniDB> ";
    }
    return 0;
}

