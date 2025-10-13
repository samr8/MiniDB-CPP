#ifndef DATABASE_H
#define DATABASE_H

#include<string>
#include<vector>
#include<sstream>
#include<fstream>
#include<iostream>
#include<filesystem>
#include<algorithm>
using namespace std;

vector<string> split(const string& s, char delimiter);


class Database
{
    std:: string currentDB;

    public:
    Database();
    void createDatabase(const std::string &name);
    void useDatabase(const std:: string &name);
    void createTable(const std:: string&tableName, const std::vector<std::string> &columns);
    void insertIntoTable(const std:: string &tableName, const std::vector<std::string> &values);
    void selectAll(const std::string &tableName);
    void dropTable(const std:: string &tableName);
    void deleteFromTable(const std::string &tableName, const std::string &condition);
void updateTable(const std::string& tableName,
                 const std::string& updateColumn,
                 const std::string& newValue,
                 const std::string& whereColumn,
                 const std::string& whereValue);

};

#endif