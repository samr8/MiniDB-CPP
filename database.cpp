#include "database.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs= filesystem;

void Database::createDatabase(const string &name)
{
    string path="database/" + name;
    if(!fs::exists("databases")) fs::create_directory("databases");

    if(!fs::exists("path"))
    {
        fs::create_directory(path);
        cout<<"Database '"<< name << "' created.\n";
    } else {
        cout<<" Database already exists .\n";
    }
}

void Database::useDatabase(const string& name)
{
    string path="databases/"+name;
    if(fs::exists(path)){
        currentDB=path;
        cout<<"Using database '"<<name<<"' .\n";
    } else {
        cout<<"Database not found ";
    }
}

void Database::createTable(const string &tableName, const vector <string> &columns)
{
    if(currentDB.empty())
    {
        cout<<"No database selected. .\n";
        return;
    }

    string filePath=currentDB+"/"+tableName+".tbl";
    ofstream file(filePath);
    if(!file)
    {
        cout<<"Error creating table .\n";
        return;
    }

    for(size_t i=0; i<columns.size(); ++i)
    {
        file<<columns[i];
        if(i<columns.size()-1) file<<",";
    }
    file<<endl;
    cout<<"Table '"<<tableName <<" 'created in "<<currentDB<<".\n";
}

void Database::insertRow(const string &tableName,const vector<string> &values)
{
    if(currentDB.empty())
    {
        cout<<"No database selected .\n";
        return;
    }
    string filePath = currentDB + "/" + tableName + ".tbl";
    ofstream file(filePath, ios::app);
    if (!file) {
        cout << "Table not found.\n";
        return;
    }
    for (size_t i = 0; i < values.size(); ++i) {
        file << values[i];
        if (i < values.size() - 1) file << ",";
    }
    file << endl;
    cout << "Row inserted into '" << tableName << "'.\n";
}

void Database::selectAll(const string &tableName)
{
    if (currentDB.empty()) {
        cout << "No database selected. Use USE <dbname> first.\n";
        return;
    }
    string filePath = currentDB + "/" + tableName + ".tbl";
    ifstream file(filePath);
    if (!file) {
        cout << "Table not found.\n";
        return;
    }
    string line;
    cout<<"Data from table '"<<tableName<<"':\n";
    while(getline(file, line))
    {
        cout<<line<<endl;
    }
}

void Database::dropTable(const string &tableName)
{
    if (currentDB.empty()) {
        cout << "No database selected. Use USE <dbname> first.\n";
        return;
    }
    string filePath = currentDB + "/" + tableName + ".tbl";
    if (fs::remove(filePath))
        cout << "Table '" << tableName << "' deleted.\n";
    else
        cout << "Table not found.\n";
}