# MiniDB (C++ Project)

MiniDB is a simple file-based database engine built in C++ that supports:
- CREATE DATABASE / TABLE
- INSERT, SELECT, DROP
- Persistent data storage using filesystem

## Run Instructions
```bash
g++ -std=c++17 main.cpp database.cpp -o main.exe
main.exe


git init
git add .
git commit -m "Deployable MiniDB with Flask API"
git branch -M main
git remote add origin https://github.com/samr8/MiniDB.git
git push -u origin main

