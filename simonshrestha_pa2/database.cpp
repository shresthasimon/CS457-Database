// Author: Simon Shrestha
// Date: 3/7/20
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
database::database(string tableName, string attributes, string database){
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
    cout << "-- " << line << endl;

    for(unsigned int i = 0; i < databaseVector.size(); i++){
        cout <<  "-- " << databaseVector[i][0] << '|' << databaseVector[i][1] << "|" << databaseVector[i][2] << endl;
    }
}
// writing table to file
void database::writeToFile() {
    ofstream fout;
    fout.clear();
    // open the table file
    fout.open(Database + '/' + tableName + ".txt");
    string query = tableParameters;
    int position = query.find(',');
    //replace all commas with '|'
    while(position > 0){
        query.replace(position, 2, " | ");
        position = query.find(',');
    }
    fout << query << endl;
    //write out all rows and columns onto the file
    for (unsigned int i = 0; i < databaseVector.size(); i++){
        fout << "-- " << databaseVector[i][0] << '|' << databaseVector[i][1] << '|' << databaseVector[i][2] << endl;
    }

    fout.close();
}
/* Name: queryTable
 * Purpose: Get specific information from table
 * Parameters:
 *      index: what is the value being looked at
 *      value: what is value that that index is being compared to
 *      indices: which columns are specifically being requested
 */
void database::queryTable(int index, string value, vector<int> columns) {
    string query = tableParameters;
    int position = query.find(',');
    //replace all commas with '|'
    while(position > 0){
        query.replace(position, 2, " | ");
        position = query.find(',');
    }
    // print out header column descriptions
    query = query.substr(query.find("|") + 2, query.length() - query.find("|") - 1);
    cout << "-- " << query << endl;
    // print out the requested columns for each for each row
    for(unsigned int i = 0; i < databaseVector.size(); i++){
        if(databaseVector[i][index] != value){
            cout <<  "-- " << databaseVector[i][columns[0]] << " | " << databaseVector[i][columns[1]] << endl;
        }
    }
}
//push extra values into the vector
void database::insertIntoVector(vector<string> input) {
    databaseVector.push_back(input);
}
/*
 * Name: getIndex
 * Purpose: getting the index number of the desired column name
 * Parameters:
 *          columnName: the name of the column
 */
int database::getColumnIndex(string columnName) {
    int position = tableParameters.find(columnName);
    int commaposition = tableParameters.find(",");
    int index = 0;
    //increase index until position is found
    while(position > commaposition && commaposition != -1){
        index++;
        commaposition = tableParameters.find(",", commaposition + 1);
    }

    return index;
}
/*
 * Name: update
 * Purpose: updating information on table with new information
 * Parameters:
 *          positionIndex: what is the position of the value being replaced
 *          setIndex: the index of the new value
 *          positionValue: the value being replaced
 *          setValue: the new value
 */
int database::updateTable(int positionIndex, int setIndex, string positionValue, string setValue) {
    int counter = 0;
    //go through each row until value is found and replace it
    for(unsigned int i = 0; i < databaseVector.size(); i++){
        if(databaseVector[i][positionIndex] == positionValue){
            databaseVector[i][setIndex] = setValue;
            counter++;
        }
    }
    return counter;
}
/*
 * Name: deleteValue
 * Purpose: delete a row or rows from the table
 * Parameters:
 *          positionIndex: the characteristic ( this can be the name, price, ...)
 *          positionValue: the value
 *          operation: delete rows greater than, less than, or equal to
 *
 */
int database::deleteValue(int positionIndex, string positionvalue, char operation) {
        int counter = 0;
        // delete rows equal to
        if(operation == 'e'){
            //go through each row and each column until index equals the value
            for(unsigned int i = 0; i < databaseVector.size(); i++){
                if(databaseVector[i][positionIndex] == positionvalue){
                    // erase the row
                    databaseVector.erase(databaseVector.begin() + i);
                    counter++;
                }
            }
        }
        // delete rows greater than
        if(operation == 'g'){
            for(unsigned int i = 0; i < databaseVector.size(); i++){
                //find all rows with a greater value and delete
                if(stoi(databaseVector[i][positionIndex]) > stoi(positionvalue)){
                    databaseVector.erase(databaseVector.begin() + i);
                    counter++;
                }
            }
        }
        //delete rows less than
        if(operation == 'l'){
            for(unsigned int i = 0; i < databaseVector.size(); i++){
                if(stoi(databaseVector[i][positionIndex]) < stoi(positionvalue)){
                    databaseVector.erase(databaseVector.begin() + i);
                    counter++;
                }
            }
        }
        return counter;
}
