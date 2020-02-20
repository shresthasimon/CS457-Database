// Author: Simon Shrestha
// Date: 2/5/20
// Name: database.h
// Purpose: The header file for the database class

//Header Files
#ifndef SIMONSHRESTHA_PA1_DATABASE_H
#define SIMONSHRESTHA_PA1_DATABASE_H

#include <iostream>
#include <fstream>

using namespace std;

//Class: database
//Purpose: creates tables and holds values
class database {
    public:
        //parameterized constructor
        database(string tableName, string attributes, string database);
        //getter functions
        string getTableName() const;
        string getAttributes() const;
        string getDatabase() const;
        //set function
        void setAttributes(string query);
        //helper function
        void printTable();
        //destructor
        ~database();
    private:
        string tableName;
        string tableParameters;
        string Database;

};


#endif //SIMONSHRESTHA_PA1_DATABASE_H
