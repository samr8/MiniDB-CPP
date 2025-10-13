#include "database.h"
#include <iostream>
#include <sstream>
#include<vector>
#include <algorithm> // for std::remove

using namespace std;


int main() {
    Database db;
    string command;

    cout << "MiniDB\nType EXIT to quit or HELP to see what you can do!\nMiniDB> ";

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
}else if (word == "UPDATE") {
    string tableName, setWord, updateAssign, whereWord, whereAssign;
    ss >> tableName >> setWord >> updateAssign >> whereWord >> whereAssign;

    if (setWord != "SET" || whereWord != "WHERE") {
        cout << "Invalid UPDATE syntax.\n";
        continue;
    }

    // Split column=value
    size_t eqPos = updateAssign.find('=');
    string updateColumn = updateAssign.substr(0, eqPos);
    string newValue = updateAssign.substr(eqPos + 1);

    eqPos = whereAssign.find('=');
    string whereColumn = whereAssign.substr(0, eqPos);
    string whereValue = whereAssign.substr(eqPos + 1);

    auto trimQuotes = [](string &s) {
        if (!s.empty() && (s.front() == '"' || s.front() == '\'')) s.erase(0,1);
        if (!s.empty() && (s.back() == '"' || s.back() == '\'')) s.pop_back();
    };
    trimQuotes(newValue);
    trimQuotes(whereValue);

    db.updateTable(tableName, updateColumn, newValue, whereColumn, whereValue);
}
else if (word == "HELP") {
    cout << "\nMiniDB Supported Commands:\n";
    cout << "1. CREATE DATABASE <dbname>\n";
    cout << "2. USE <dbname>\n";
    cout << "3. CREATE TABLE <tablename> <col1> <col2> ...\n";
    cout << "4. INSERT INTO <tablename> <val1> <val2> ...\n";
    cout << "5. SELECT * FROM <tablename>\n";
    cout << "6. UPDATE <tablename> SET <column>=<new_value> WHERE <column>=<value>\n";
    cout << "7. DELETE FROM <tablename> WHERE <column>=<value>\n";
    cout << "8. DROP TABLE <tablename>\n";
    cout << "9. EXIT\n";
    cout << "Type HELP to see this list again.\n\n";
}
        else {
            cout << "Unknown command.\n";
        }
        cout << "MiniDB> ";
    }
    return 0;
}

