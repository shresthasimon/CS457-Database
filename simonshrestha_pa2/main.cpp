// Author: Simon Shrestha
// Date: 3/7/20
// Name: main.cpp
// Purpose: The main runner for the program

//header libraries/files
#include <iostream>
#include "database.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;
/*
 * Name: inputVector
 * Purpose: a temporary vector for the tables that will be pushed into the actual 2-D vector
 * Parameters:
 *          input: the data that is being inputed
 *          loadVector: the vector that contains that data
 */
void inputVector(string input, vector<string>& loadVector){
    int originalPosition = input.find(',');
    loadVector.push_back(input.substr(0,originalPosition));
    int newPosition = input.find(',', originalPosition + 1);
    while (newPosition > 0){
        //puch into the vector
        loadVector.push_back(input.substr(originalPosition + 1, newPosition - originalPosition - 1));

        // look for next position
        originalPosition = newPosition;
        newPosition = input.find(',', originalPosition + 1);
    }
    loadVector.push_back(input.substr(originalPosition + 1, input.length() - originalPosition));
}
int main() {
    string inputQuery = "";
    string databaseName = "None";
    string tableName = "None";
    int errorChecker;
    string Database = "";
    int errorFlag = 0;
    vector<database> databaseVector;
    string attributes = "";
    string appendCommand = "";

    cout << "Enter some commands, type .EXIT to exit the program." << endl;
    // run loop until EXIT query is found
    while(inputQuery != ".EXIT" || inputQuery != ".exit"){
        //each time read a new line from the file
        getline(cin, inputQuery);
        // append to query until semicolon found
        while(inputQuery.find(";") == string::npos && inputQuery != "" && inputQuery.find('.') != 0){
            getline(cin, appendCommand);
            if(appendCommand != ""){
                inputQuery += appendCommand;
            }
        }

        // CREATE DATABASE
        if(inputQuery.find("CREATE DATABASE") != string::npos || inputQuery.find("create database") != string::npos ){
            //Locate the name of the Database
            databaseName = inputQuery.substr(16, inputQuery.length() - 17);
            //Check if that database already exists, if it does, output error
            // if it doesn't then create it
            errorChecker = system(("mkdir " + databaseName).c_str());

            if(errorChecker == 0){
                // output that the database has been created
                cout << "-- Database " + databaseName + " created." << endl;
            }else{
                cout << "--!Failed to create Database " + databaseName + " because it already exists"  << endl;
            }
          //DROP DATABASE
        }else if(inputQuery.find("DROP DATABASE") != string::npos || inputQuery.find("drop database") != string::npos ){
            //find the name of the database
            databaseName = inputQuery.substr(14, inputQuery.length() - 15);
            //check if it already exists, if it does, output error
            //otherwise delete it
            errorChecker = system(("rmdir " + databaseName).c_str());

            if(errorChecker == 0){
                cout << "-- Database " + databaseName + " deleted." << endl;
            }else{
                cout << "--!Failed to delete Database " + databaseName + " because it doesn't exist"  << endl;
            }
          //USE
        }else if(inputQuery.find("USE") != string::npos || inputQuery.find("use") != string::npos){
            // Find the name of the database
            Database = inputQuery.substr(4, inputQuery.length() - 5);
            //check if it already is being used, if so output error
            //otherwise use database
            errorChecker = system(("cd " + Database).c_str());

            if(errorChecker == 0){
                cout << "-- Using Database " + Database + "." << endl;
            }else{
                cout << "--!Failed to access Database " + databaseName + " because it doesn't exist"  << endl;
            }
          //CREATE TABLE
        }else if(inputQuery.find("CREATE TABLE") != string::npos || inputQuery.find("create table") != string::npos  ){
            //find name of table
            tableName = inputQuery.substr(13, inputQuery.find("(") - 14);
            transform(tableName.begin(), tableName.end(), tableName.begin(), ::toupper);
            //search through database to check if the table already exists
            for(unsigned int i = 0; i < databaseVector.size(); i ++){
                if(tableName == databaseVector[i].getTableName() && Database == databaseVector[i].getDatabase()){
                    cerr << "--!Failed to create table " + tableName + " because it already exists." << endl;
                    errorFlag = 1;
                    break;
                }
            }
            //if the table does not already exist in the database
            if(errorFlag == 0){
                if(Database != ""){
                    // as long as a valid database is being used then create the table
                    errorChecker = system(("touch " + Database + "/" + tableName + ".txt").c_str());
                    if(errorChecker == 0){
                        //find what data should be inserted into the table
                        attributes = inputQuery.substr(inputQuery.find("(") + 1, inputQuery.length() - 3 - inputQuery.find("("));
                        //allocate space for table data
                        database* insert = new database(tableName, attributes, Database);
                        //insert that data
                        databaseVector.push_back(*insert);
                        cout << "-- Table " + tableName + " created." << endl;
                    }
                }else{
                    cout << "--!Failed to create " + tableName + " because no database selected." << endl;
                }
            }
          //DROP TABLE
        }else if(inputQuery.find("DROP TABLE") != string::npos || inputQuery.find("drop table") != string::npos ){
            // find name of table to drop
            tableName = inputQuery.substr(11, inputQuery.length() - 12);
            transform(tableName.begin(), tableName.end(), tableName.begin(), ::toupper);
            // check if the table can be removed, if it can't then report error
            //otherwise remove it
            errorChecker = system(("rm " + Database + "/" + tableName + ".txt").c_str());
            if(errorChecker == 0){
                //remove data and all associations with the table
                for(unsigned int i = 0; i < databaseVector.size(); ++i){
                    if(tableName == databaseVector[i].getTableName() && Database == databaseVector[i].getDatabase()){
                        databaseVector.erase(databaseVector.begin() + i);
                    }
                }
                cout << "-- Table " + tableName + " deleted." << endl;
            }else{
                cout << "--!Failed to delete " + tableName + " because it does not exist." << endl;
            }
          //SELECT
        }else if(inputQuery.find("SELECT *") != string::npos || inputQuery.find("select *") != string::npos){
            unsigned int i;
            //find the table to be printed
            tableName = inputQuery.substr(14, inputQuery.length() - 15);
            transform(tableName.begin(), tableName.end(), tableName.begin(), ::toupper);
            for(i = 0; i < databaseVector.size(); i++){
                //if the table is found within the database then print it
                if(tableName == databaseVector[i].getTableName() && Database == databaseVector[i].getDatabase()){
                    databaseVector[i].printTable();
                    break;
                }
            }
            //otherwise output error that no table could be found
            if(i == databaseVector.size()){
                cout << "--!Failed to query table " + tableName + " because it does not exist." << endl;
            }
          //ALTER TABLE
        }else if(inputQuery.find("ALTER TABLE") != string::npos || inputQuery.find("alter table") != string::npos ){
            unsigned int i;
            //find the table name
            tableName = inputQuery.substr(12, inputQuery.find("ADD") - 13);
            transform(tableName.begin(), tableName.end(), tableName.begin(), ::toupper);
            //find the data that needs to be added or deleted from table
            string addattributes = inputQuery.substr(inputQuery.find("ADD") + 4, inputQuery.length() - (inputQuery.find("ADD") + 5));
            cout << "-- Table " + tableName + " modified." << endl;
            for(i = 0; i < databaseVector.size(); i++){
                // find the table in the database and start adding values
                if(tableName == databaseVector[i].getTableName() && Database == databaseVector[i].getDatabase()){
                    databaseVector[i].setAttributes(databaseVector[i].getAttributes() + ", " + addattributes);
                    break;
                }
            }
            //if the table does not exist then output error
            if(i == databaseVector.size()){
                cout << "--!Failed to query table " + tableName + " because it does not exist." << endl;
            }
            //INSERT NEW ROWS
        }else if (inputQuery.find("INSERT INTO") != string::npos || inputQuery.find("insert into") != string::npos){
            unsigned int i;
            //get table name
            tableName = inputQuery.substr(12, inputQuery.find("values") - 13);
            //change case
            transform(tableName.begin(), tableName.end(), tableName.begin(), ::toupper);
            //check if valid
            for(i = 0; i < databaseVector.size(); i++){
                if(tableName == databaseVector[i].getTableName() && Database == databaseVector[i].getDatabase()){
                    break;
                }
            }
            // find the attributes that need to be added
            if(i != databaseVector.size()){
                vector<string> test;
                int position = inputQuery.find("(");
                attributes = inputQuery.substr(position + 1, inputQuery.length() - 3 - position);
                attributes.erase(remove_if(attributes.begin(), attributes.end(), ::isspace), attributes.end());
                //input the attributes into temperary vector
                inputVector(attributes, test);
                databaseVector[i].insertIntoVector(test);

                cout << "-- 1 new record inserted." << endl;
            }else{
                cout << " --!Failed, invalid table." << endl;
            }
            //UPDATE table by replacing values
        }else if(inputQuery.find("UPDATE") != string::npos || inputQuery.find("update") != string::npos){
            unsigned int i = 0;
            //find table name
            tableName = inputQuery.substr(7, inputQuery.find("set") - 7);
            //change case
            transform(tableName.begin(), tableName.end(), tableName.begin(), ::toupper);
//            cout << tableName << endl;
            for(i = 0; i < databaseVector.size(); i++){
                if(tableName == databaseVector[i].getTableName() && Database == databaseVector[i].getDatabase()){
                    break;
                }
            }
            //Find the where and set values in the query command
            if(i != databaseVector.size()){
                string whereKey = inputQuery.substr(inputQuery.find("where") + 6, inputQuery.find_last_of("=") - 6 - inputQuery.find("where"));
//                cout << whereKey << endl;
                string whereValue = inputQuery.substr(inputQuery.find_last_of("=") + 2, inputQuery.find(";") - 2 - inputQuery.find_last_of("="));
//                cout << whereValue << endl;
                string setKey = inputQuery.substr(inputQuery.find("set") + 4, inputQuery.find("=") - 5 - inputQuery.find("set"));
//                cout << setKey << endl;
                string setValue = inputQuery.substr(inputQuery.find("=") + 2, inputQuery.find("where") - 2 - inputQuery.find("="));
//                cout << setValue << endl;
                //find the column location of  the where and set keys
                int whereIndex = databaseVector[i].getColumnIndex(whereKey);
                int setIndex = databaseVector[i].getColumnIndex(setKey);
                //update the vector based on the values of where and set
                cout << "-- " << databaseVector[i].updateTable(whereIndex, setIndex, whereValue, setValue) << " record(s) modified." << endl;
            }else{
                cout << "-- !Not a valid table" << endl;
            }
            //DELETE rows from the table
        }else if(inputQuery.find("DELETE") != string::npos || inputQuery.find("delete") != string::npos){
            unsigned int i = 0;
            //find the name of the the table
            tableName = inputQuery.substr(12, inputQuery.find("where") - inputQuery.find("from") - 5);
            //change case
            transform(tableName.begin(), tableName.end(), tableName.begin(), ::toupper);
//            cout << tableName << endl;
            // check if the table is valid
            for(i = 0; i < databaseVector.size(); i++){
                if(tableName == databaseVector[i].getTableName() && Database == databaseVector[i].getDatabase()){
                    break;
                }
            }

            if(i != databaseVector.size()){
                string whereKey = "";
                string whereValue = "";
                //when looking for values that equal something
                if(inputQuery.find("=") != string::npos){
                    //Find out the attribute such as price or name that is being deleted
                    whereKey = inputQuery.substr(inputQuery.find("where") + 6, inputQuery.find_last_of("=") - 7 - inputQuery.find("where"));
//                    cout << whereKey << endl;
                    whereValue = inputQuery.substr(inputQuery.find_last_of("=") + 2, inputQuery.find(";") - 2 - inputQuery.find_last_of("="));
//                    cout << whereValue << endl;
                    int whereIndex = databaseVector[i].getColumnIndex(whereKey);
                    char operation = 'e';
                    // delete the row
                    cout << "-- " << databaseVector[i].deleteValue(whereIndex, whereValue, operation) << " record(s) deleted." << endl;
                    //when looking for values less than
                }else if(inputQuery.find("<") != string::npos){
                    whereKey = inputQuery.substr(inputQuery.find("where") + 6, inputQuery.find_last_of("<") - 7 - inputQuery.find("where"));
//                    cout << whereKey << endl;
                    whereValue = inputQuery.substr(inputQuery.find_last_of("<") + 2, inputQuery.find(";") - 2 - inputQuery.find_last_of("<"));
//                    cout << whereValue << endl;
                    int whereIndex = databaseVector[i].getColumnIndex(whereKey);
                    char operation = 'l';
                    cout << "-- " << databaseVector[i].deleteValue(whereIndex, whereValue, operation) << " record(s) deleted." << endl;
                    //when finding values greater than
                }else if(inputQuery.find(">") != string::npos){
                    whereKey = inputQuery.substr(inputQuery.find("where") + 6, inputQuery.find_last_of(">") - 7 - inputQuery.find("where"));
//                    cout << whereKey << endl;
                    whereValue = inputQuery.substr(inputQuery.find_last_of(">") + 2, inputQuery.find(";") - 2 - inputQuery.find_last_of(">"));
//                    cout << whereValue << endl;
                    int whereIndex = databaseVector[i].getColumnIndex(whereKey);
                    char operation = 'g';
                    cout << "-- " << databaseVector[i].deleteValue(whereIndex, whereValue, operation) << " record(s) deleted." << endl;
                }else{
                    cout <<" -- Invalid Operator" << endl;
                }
            }else{
                cout << " -- !Invalid Table" << endl;
            }
            //SELECT to output only certain rows and attributes of the table
        }else if(inputQuery.find("SELECT") != string::npos || inputQuery.find("select") != string::npos){
            unsigned int i = 0;
            //find table name
            tableName = inputQuery.substr(inputQuery.find("from") + 5, inputQuery.find("where") - inputQuery.find("from") - 5);
            //change case
            transform(tableName.begin(), tableName.end(), tableName.begin(), ::toupper);
//            cout << tableName << endl;
            // check if the table exists
            for(i = 0; i < databaseVector.size(); i++){
                if(tableName == databaseVector[i].getTableName() && Database == databaseVector[i].getDatabase()){
                    break;
                }
            }

            if(i != databaseVector.size()){
                if(inputQuery.find("!=") != string::npos){
                    //find the attribute that is being desired and what values it is being compared to
                    string whereKey = inputQuery.substr(inputQuery.find("where") + 6, inputQuery.find_last_of("!=") - 8 - inputQuery.find("where"));
//                    cout << whereKey << endl;
                    string whereVal = inputQuery.substr(inputQuery.find_last_of("!=") + 2, inputQuery.find(";") - 2 - inputQuery.find_last_of("!="));
//                    cout << whereVal << endl;
                    int whereIndex = databaseVector[i].getColumnIndex(whereKey);
                    //find the desired columns to be printed
                    string columNames = inputQuery.substr(7, inputQuery.find("from") - 7);
//                    cout << columNames << endl;
                    vector <int> columns;
                    // push desired column headers into a temporary vector
                    columns.push_back(databaseVector[i].getColumnIndex(columNames.substr(0, columNames.find(",") - 1)));
                    columns.push_back(databaseVector[i].getColumnIndex(columNames.substr(columNames.find(",") + 1, columNames.length() - columNames.find(",") - 1)));
                    //query table with the desired columns
                    databaseVector[i].queryTable(whereIndex, whereVal, columns);
                }else{
                    cout << "-- invalid operator" << endl;
                }
            }
        }else if(inputQuery == ".EXIT" || inputQuery == ".exit"){
            break;
        }
        else{
            cout << "--!Not a correct command, try again." << endl;
        }

        errorFlag = 0;
    }
    for( unsigned int i = 0; i < databaseVector.size(); i++)
    {
        databaseVector[i].writeToFile();
    }
    cout << "--All done!" << endl;
    return 0;
}
