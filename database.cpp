// Author: Simon Shrestha
// Date: 2/5/20
// Name: database.cpp
// Purpose: The implementation file for the database class

#include "database.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//parameterized constructor
// Parameters:
//      tableName: the name of the table
//      attributes: the data of the table (for now)
//      database: the database in which the table will be located
database::database(string tableName, string attributes, string database) {
    this->tableName = tableName;
    this->tableParameters = attributes;
    this->Database = database;
}

//destructor
database::~database() {
    tableName = "";
    tableParameters = "";
    Database = "";
}

//getter function
string database::getTableName() const {
    return tableName;
}

string database::getAttributes() const {
    return tableParameters;
}

string database::getDatabase() const {
    return Database;
}

//setter function
void database::setAttributes(string query) {
    tableParameters = query;
}

//helper print function
void database::printTable() {
    // make the line the input query
    string line = tableParameters;
    //find the position of the , in the parameters for the CREATE query
    int position = line.find(',');
    // while the comma is found, replace it with a |
    while(position > 0){
        line.replace(position, 2, " | ");
        position = line.find(',');
    }
    //formalities
    cout << "-- " + line << endl;
}















