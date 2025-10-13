#include "database.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

Database::Database() {
    // Ensure main "database" folder always exists
    if (!fs::exists("database")) {
        fs::create_directory("database");
    }
}

void Database::createDatabase(const string &name)
{
    string path = "database/" + name;
    if (!fs::exists(path)) {
        fs::create_directories(path);
        cout << "Database '" << name << "' created.\n";
    } else {
        cout << "Database '" << name << "' already exists.\n";
    }
}

void Database::useDatabase(const string &name)
{
    string path = "database/" + name;
    if (fs::exists(path)) {
        currentDB = path;
        cout << "Using database '" << name << "'.\n";
    } else {
        cout << "Database not found.\n";
    }
}

void Database::createTable(const string &tableName, const vector<string> &columns)
{
    if (currentDB.empty()) {
        cout << "No database selected. Use USE <dbname> first.\n";
        return;
    }

    string filePath = currentDB + "/" + tableName + ".tbl";

    if (fs::exists(filePath)) {
        cout << "Table '" << tableName << "' already exists.\n";
        return;
    }

    ofstream file(filePath);
    if (!file) {
        cout << "Error creating table.\n";
        return;
    }

    // Write column headers
    for (size_t i = 0; i < columns.size(); ++i) {
        file << columns[i];
        if (i < columns.size() - 1) file << ",";
    }
    file << endl;
    file.close();

    cout << "Table '" << tableName << "' created successfully in " << currentDB << ".\n";
}

void Database::insertIntoTable(const string &tableName, const vector<string> &values)
{
    if (currentDB.empty()) {
        cout << "No database selected. Use USE <dbname> first.\n";
        return;
    }

    string filePath = currentDB + "/" + tableName + ".tbl";
    if (!fs::exists(filePath)) {
        cout << "Table '" << tableName << "' not found.\n";
        return;
    }

    ofstream file(filePath, ios::app);
    if (!file) {
        cout << "Error writing to table.\n";
        return;
    }

    for (size_t i = 0; i < values.size(); ++i) {
        file << values[i];
        if (i < values.size() - 1) file << ",";
    }
    file << endl;
    file.close();

    cout << "Row inserted into table '" << tableName << "'.\n";
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
        cout << "Table '" << tableName << "' not found.\n";
        return;
    }

    string line;
    cout << "\nData from table '" << tableName << "':\n";
    cout << "---------------------------------\n";
    while (getline(file, line)) {
        cout << line << endl;
    }
    cout << "---------------------------------\n";
    file.close();
}

void Database::dropTable(const string &tableName)
{
    if (currentDB.empty()) {
        cout << "No database selected. Use USE <dbname> first.\n";
        return;
    }

    string filePath = currentDB + "/" + tableName + ".tbl";
    if (fs::remove(filePath)) {
        cout << "Table '" << tableName << "' deleted.\n";
    } else {
        cout << "Table '" << tableName << "' not found.\n";
    }
}

void Database::deleteFromTable(const string& tableName, const string& condition) {
    string filePath = currentDB + "/" + tableName + ".tbl";
    ifstream inFile(filePath);
    if (!inFile) {
        cout << "Table '" << tableName << "' not found.\n";
        return;
    }

    // Read header
    string headerLine;
    getline(inFile, headerLine);
    vector<string> columns;
    string col;
    stringstream headerStream(headerLine);
    while (getline(headerStream, col, ',')) columns.push_back(col);

    // Read all rows
    vector<vector<string>> rows;
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        vector<string> row;
        string val;
        while (getline(ss, val, ',')) row.push_back(val);
        if (!row.empty()) rows.push_back(row);
    }
    inFile.close();

    // Parse condition column=value
    size_t pos = condition.find('=');
    if (pos == string::npos) {
        cout << "Invalid condition format. Use WHERE column=value.\n";
        return;
    }

    string colName = condition.substr(0, pos);
    string colValue = condition.substr(pos + 1);

    // Trim whitespace and quotes
    colName.erase(remove_if(colName.begin(), colName.end(), ::isspace), colName.end());
    colValue.erase(remove_if(colValue.begin(), colValue.end(), ::isspace), colValue.end());
    if (!colValue.empty() && (colValue.front() == '"' || colValue.front() == '\'')) colValue.erase(0, 1);
    if (!colValue.empty() && (colValue.back() == '"' || colValue.back() == '\'')) colValue.pop_back();

    // Find matching column index
    int colIndex = -1;
    for (int i = 0; i < columns.size(); ++i) {
        if (columns[i] == colName) {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1) {
        cout << "Column '" << colName << "' not found.\n";
        return;
    }

    // Filter rows
    vector<vector<string>> newRows;
    int deletedCount = 0;
    for (auto& row : rows) {
        if (row[colIndex] != colValue) {
            newRows.push_back(row);
        } else {
            deletedCount++;
        }
    }

    // Rewrite the file
    ofstream outFile(filePath);
    outFile << headerLine << "\n";
    for (auto& r : newRows) {
        for (int i = 0; i < r.size(); ++i) {
            outFile << r[i];
            if (i < r.size() - 1) outFile << ",";
        }
        outFile << "\n";
    }
    outFile.close();

    cout << deletedCount << " row(s) deleted where " << condition << ".\n";
}

void Database::updateTable(const std::string& tableName,
                           const std::string& updateColumn,
                           const std::string& newValue,
                           const std::string& whereColumn,
                           const std::string& whereValue)
{
    if (currentDB.empty()) {
        cout << "No database selected. Use USE <dbname> first.\n";
        return;
    }

    std::string filePath = currentDB + "/" + tableName + ".tbl";
    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cout << "Table '" << tableName << "' not found.\n";
        return;
    }

    // Read header
    std::string headerLine;
    getline(inFile, headerLine);
    std::vector<std::string> columns;
    std::stringstream headerStream(headerLine);
    std::string col;
    while (getline(headerStream, col, ',')) columns.push_back(col);

    // Find column indexes
    int updateColIndex = -1, whereColIndex = -1;
    for (int i = 0; i < columns.size(); ++i) {
        if (columns[i] == updateColumn) updateColIndex = i;
        if (columns[i] == whereColumn) whereColIndex = i;
    }
    if (updateColIndex == -1) { std::cout << "Column '" << updateColumn << "' not found.\n"; return; }
    if (whereColIndex == -1) { std::cout << "Column '" << whereColumn << "' not found.\n"; return; }

    // Read all rows
    std::vector<std::vector<std::string>> rows;
    std::string line;
    while (getline(inFile, line)) {
        std::stringstream ss(line);
        std::vector<std::string> row;
        std::string val;
        while (getline(ss, val, ',')) row.push_back(val);
        if (!row.empty()) rows.push_back(row);
    }
    inFile.close();

    // Update matching rows
    int updatedCount = 0;
    for (size_t i = 1; i < rows.size(); ++i) { // skip header
        if (rows[i][whereColIndex] == whereValue) {
            rows[i][updateColIndex] = newValue;
            updatedCount++;
        }
    }

    // Rewrite file
    std::ofstream outFile(filePath);
    outFile << headerLine << "\n";
    for (size_t i = 1; i < rows.size(); ++i) {
        for (size_t j = 0; j < rows[i].size(); ++j) {
            outFile << rows[i][j];
            if (j < rows[i].size() - 1) outFile << ",";
        }
        outFile << "\n";
    }
    outFile.close();

    std::cout << updatedCount << " row(s) updated in '" << tableName << "'.\n";
}




