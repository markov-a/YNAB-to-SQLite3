#ifndef YNABConvert_HPP
#define YNABConvert_HPP

#include "YNABKnowledgeVersion.hpp"
#include "YNABConvertException.hpp"
#include "aux.hpp"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <sqlite3.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace soci;

/**
 * Main class to convert YNAB4 budgets to SQLite3 databases
*/
class YNABConvert
{
    /**
    The table_struct is a map with a set of strings 
    */
    using table_struct = map<string,set<string>>; 
    
    /**
    Tries to find the .yfull file with the complete budget databases
    @param budget_path Path of the YNAB4 budget directory
    @return an empty string if the complete budget wasn't found 
    */
    boost::filesystem::path get_yfull_file(const boost::filesystem::path& budget_path);
    
    /**
        Gets the last inserted rowid from a sqlite3 session 
    */
    sqlite_api::sqlite3_int64 sqlite3_last_rowid(soci::session& sess);
    
    /**
     Inserts a record in a table from the data in a ptree of a node
     @param p The node with the data
     @param sql SOCI database session
     @param table Table name
     @param prev_table Parent table, if any
     @param prev_rowid If there's a previous table, insert the previous rowid it had
    */
    void insert_from_node(
        const ptree &p,
        soci::session& sql,
        const string table="",
        const string prev_table="",
        const sqlite_api::sqlite3_int64 prev_rowid=0);
    
    /**
      Creates in the SQLite3 database all the tables in a table_struct
      @param sql SOCI database session
    */
    void create_tables_from_struct(soci::session &sql, const table_struct &db);
    
    /**
     Figures out the table structure from the property_tree created from the JSON file
     @param db Table [in/out] structure of the database
     @param p Property tree with the JSON
     @param node_name Current node name
     @param table_name Current table name
    */
    void create_db_struct(table_struct &db, const ptree &p, const string& node_name="", const string& table_name="");

public:
    /**
     Main function that converts the YNAB4 budget into a SQLite3 database
     @param budget_directory Directory with the budget (normally ends with .ynab4)
     @param sqlite3_db Path to the new SQLite3 database
    */
    int convert(string budget_directory, string sqlite3_db);
};

#endif

