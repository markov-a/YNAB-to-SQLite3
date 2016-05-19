#ifndef YNABConvert_HPP
#define YNABConvert_HPP

#include "YNABKnowledgeVersion.hpp"
#include "aux.hpp"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <sqlite3.h>

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include <set>
#include <algorithm>



using namespace std;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace soci;

class YNABConvert
{
    using table_struct = map<string,set<string>>;
    string last_error;
    string last_error_code;
    
    int process_budget_dir(const boost::filesystem::path& budget_path);
    boost::filesystem::path get_yfull_file(const boost::filesystem::path& budget_path);
    
    sqlite_api::sqlite3_int64 sqlite3_last_rowid(soci::session& sess);
    void insert_from_node(
        const ptree &p,
        soci::session& sql,
        const string table="",
        const string prev_table="",
        const sqlite_api::sqlite3_int64 prev_rowid=0);
        
    void create_tables_from_struct(soci::session &sql, const table_struct &db);
    void create_db_struct(table_struct &db, const ptree &p, const string& node_name="", const string& table_name="");
    

public:
    int convert(string budget_directory, string sqlite3_db);

};

#endif

