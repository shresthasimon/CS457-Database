// Author: Simon Shrestha
// Date: 3/7/20
// Name: database.h
// Purpose: The header file for the database class

//Header Files
#ifndef SIMONSHRESTHA_PA1_DATABASE_H
#define SIMONSHRESTHA_PA1_DATABASE_H

#include <iostream>
#include <fstream>
#include <vector>

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
        // get information from table
        void queryTable(int index, string value, vector<int> columns);
        // insert more data into data vector
        void insertIntoVector (vector<string> input);
        // get index of column
        int getColumnIndex(string columnName);
        // updating table
        int updateTable(int positionIndex, int setIndex, string positionValue, string setValue);
        // delete tuple from table
        int deleteValue(int positionIndex, string positionvalue, char operation);

        // write table to file
        void writeToFile();
    private:
        string tableName;
        string tableParameters;
        string Database;
        vector <vector<string>> databaseVector;

};


#endif //SIMONSHRESTHA_PA1_DATABASE_H
