#ifndef DATABASE_H
#define DATABASE_H

#include<string>
#include<vector>

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

};

#endif