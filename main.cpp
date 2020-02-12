// Author: Simon Shrestha
// Date: 2/6/20
// Name: main.cpp
// Purpose: The main runner for the program

//header libraries/files
#include <iostream>
#include "database.h"
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

//Name: removeCarriageReturn
//Purpose: to remove the carriage return that appears after every line
//Parameters:
//      query: a string which is the command to remove the carriage return from
void removeCarriageReturn(string& query){
    string temp;
    //if the carriage return is not there, leave
    if(query.find('\r') == string::npos){
        return;
    }else{
        // go through the entire string until the carriage return is found
        for(unsigned int i = 0; i < query.size(); i++){
            if(query[i] == '\r'){
                continue;
            }else{
                //once the carriage return is found, delete it from the string
                temp.push_back(query[i]);
            }
        }
        query = temp;
        return;
    }
}

int main(int argc, char const *argv[]) {
    string inputQuery = "";
    string databaseName = "None";
    string tableName = "None";
    int errorChecker;
    string Database = "";
    int errorFlag = 0;
    vector<database> databaseVector;
    string attributes = "";
    // run loop until EXIT query is found
    while(inputQuery != ".EXIT"){
        //each time read a new line from the file
        getline(cin, inputQuery);
        //remove the carriage return from each line
        removeCarriageReturn(inputQuery);
        // CREATE DATABASE
        if(inputQuery.find("CREATE DATABASE") != string::npos){
            //Locate the name of the Database
            databaseName = inputQuery.substr(16, inputQuery.length() - 17);
            //Check if that database already exists, if it does, output error
            // if it doesn't then create it
            errorChecker = system(("mkdir " + databaseName).c_str());
            if(errorChecker == 0){
                // output that the database has been created
                cout << "-- Database " + databaseName + " created." << endl;
            }
          //DROP DATABASE
        }else if(inputQuery.find("DROP DATABASE") != string::npos){
            //find the name of the database
            databaseName = inputQuery.substr(14, inputQuery.length() - 15);
            //check if it already exists, if it does, output error
            //otherwise delete it
            errorChecker = system(("rmdir " + databaseName).c_str());
            if(errorChecker == 0){
                cout << "-- Database " + databaseName + " deleted." << endl;
            }
          //USE
        }else if(inputQuery.find("USE") != string::npos){
            // Find the name of the database
            Database = inputQuery.substr(4, inputQuery.length() - 5);
            //check if it already is being used, if so output error
            //otherwise use database
            errorChecker = system(("cd " + Database).c_str());
            if(errorChecker == 0){
                cout << "-- Using Database " + Database + "." << endl;
            }
          //CREATE TABLE
        }else if(inputQuery.find("CREATE TABLE") != string::npos){
            //find name of table
            tableName = inputQuery.substr(13, inputQuery.find("(") - 14);
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
        }else if(inputQuery.find("DROP TABLE") != string::npos){
            // find name of table to drop
            tableName = inputQuery.substr(11, inputQuery.length() - 12);
            // check if the table can be removed, if it can't then report error
            //otherwise remove it
            errorChecker = system(("rm " + Database + "/" + tableName + ".txt").c=_str());
            if(errorChecker == 0){
                //remove data and all associations with the table
                for(unsigned int i = 0; i < databaseVector.size(); ++i){
                    if(tableName == databaseVector[i].getTableName() && Database == databaseVector[i].getDatabase()){
                        databaseVector.erase(databaseVector.begin() + i);
                    }
                }
                cout << "-- Table " + tableName + " deleted." << endl;
            }
          //SELECT
        }else if(inputQuery.find("SELECT *") != string::npos){
            unsigned int i;
            //find the table to be printed
            tableName = inputQuery.substr(14, inputQuery.length() - 15);
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
        }else if(inputQuery.find("ALTER TABLE") != string::npos){
            unsigned int i;
            //find the table name
            tableName = inputQuery.substr(12, inputQuery.find("ADD") - 13);
            //find the data that needs to be added or deleted from table
            string addattributes = inputQuery.substr(inputQuery.find("ADD") + 4, inputQuery.length() - (inputQuery.find("ADD") + 5));
            cout << "-- Table " + tableName + " modified." << endl;
            for(i = 0; i < databaseVector.size(); ++i){
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
        }

        errorFlag = 0;
    }

    cout << "--All done!" << endl;
    return 0;
}
