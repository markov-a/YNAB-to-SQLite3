#include "YNABConvert.hpp"

int YNABConvert::convert(string budget_directory, string sqlite3_db)
{
  try
  {
    session sql(sqlite3, sqlite3_db);

    auto filename=get_yfull_file(budget_directory).string();
    auto p=aux::load_json(filename);

    if(p==nullptr)
    {
      throw YNABConvertException("Fatal error: Couldn't open json file on \"" + filename + "\"\n");
    }

    table_struct s;

    create_db_struct(s,*p);
    create_tables_from_struct(sql,s);
    insert_from_node(*p,sql);
  }
  catch (exception const &err)
  {
      throw YNABConvertException("Database exception on " + sqlite3_db + ":" + err.what());
  }
}


void YNABConvert::create_db_struct(table_struct &db, const ptree &p, const string& node_name, const string& table_name)
{
  if(p.begin()==p.end())
  {
    string tbl_name="master";
    if(table_name!="") tbl_name=table_name;
    if (node_name!="")
      db[tbl_name].insert(node_name);
    else
      throw YNABConvertInvalidBudgetException("Invalid YNAB4 Budget");
  }
  else
  {
    for(auto it=p.begin();it!=p.end();++it)
    {
      if(it->first=="")
      {
        if(table_name!="")
        {
          db[node_name].insert(table_name + "_id");
        }
        create_db_struct(db,it->second,it->first,node_name);
      }
      else
      {
        create_db_struct(db,it->second,it->first,table_name);
      }
    }
  }
}

void YNABConvert::create_tables_from_struct(soci::session &sql, const table_struct &db)
{
  string cur_query;

  for(const auto &e:db)
  {
    cur_query="CREATE TABLE " + e.first;
    cur_query+="(";

    for(auto it=e.second.begin();it!=e.second.end();++it)
    {
      cur_query+="`" + *it + "` VARCHAR(45) NULL" ;
      cur_query+=",";
    }

    cur_query[cur_query.length()-1]=')';
    cur_query+='\n';

    sql << cur_query;
  }
}


void YNABConvert::insert_from_node(
    const ptree &p,
    soci::session& sql,
    const string table,
    const string prev_table,
    const sqlite_api::sqlite3_int64 prev_rowid)
{

  statement st(sql);

  string query="INSERT INTO " + (table==""?"master":table) + "(";

  bool first=true;

  for(auto it=p.begin();it!=p.end();++it)
  {
    if(it->second.begin()==it->second.end())
    {
      if(first) first=false;
      else query+=",";
      query += it->first;
    }
  }

  if(prev_table.size()!=0)
  {
    query += "," + prev_table + "_id";
  }

  query += ") VALUES (";

  first=true;
  int i=1;
  for(auto it=p.begin();it!=p.end();++it)
  {
    if(it->second.begin()==it->second.end())
    {
      if(first) first=false;
      else query+=",";
      query += ":" + std::to_string(i);
      ++i;
    }
  }

  if(prev_table!="")
  {
    query += ",:" + std::to_string(i);
  }

  query += ");";

  for(auto it=p.begin();it!=p.end();++it)
  {
    if(it->second.begin()==it->second.end())
    {
      st.exchange(use(it->second.data()));
    }
  }

  if(prev_table!="")
  {
    st.exchange(use(prev_rowid));
  }

  st.alloc();
  st.prepare(query);
  st.define_and_bind();
  st.execute(true);
  auto prev_rid = sqlite3_last_rowid(sql);

  for(auto it=p.begin();it!=p.end();++it)
  {
    for(auto itr=it->second.begin();itr!=it->second.end();++itr)
    if(itr->first=="")
    {
      insert_from_node(itr->second,sql,it->first,table,prev_rid);
    }
  }
}

inline sqlite_api::sqlite3_int64 YNABConvert::sqlite3_last_rowid(soci::session& sess)
{
  return sqlite_api::sqlite3_last_insert_rowid(static_cast<sqlite3_session_backend*>(sess.get_backend())->conn_);
}

boost::filesystem::path YNABConvert::get_yfull_file(const boost::filesystem::path& budget_path)
{
  //1st step: Open Budget.ymeta
  auto cur_file=budget_path/"Budget.ymeta";
  if(!exists(cur_file))
  {
    throw YNABConvertInvalidBudgetException("Buget ymeta file doesn't exist: " + cur_file.string() + ")");
    
  }

  auto tree=aux::load_json(cur_file.string());
  if(tree==nullptr)
  {
    throw YNABConvertException("Couldn't load JSON file");
  }

  //2nd step: Get the folder where the data is.
  auto data_path=budget_path;

  for(auto it=tree->begin();it!=tree->end();++it)
  {
    if(it->first=="relativeDataFolderName") data_path/=it->second.data();
  }

  if(!exists(data_path))
  {
    throw YNABConvertInvalidBudgetException("Budget data directory doesn't exist");
  }

  //3rd step: list every file in budget_path/devices
  auto tmp_path=data_path/"devices/";
  if(!exists(tmp_path)) 
  {
    throw YNABConvertInvalidBudgetException("Budget devices directory doesn't exist");
  }
  
  vector<ptree> devices;

  struct DeviceData
  {
    YNABKnowledgeVersion    full_budget_file_version;
    YNABKnowledgeVersion    budget_version;
    string                  GUID;
    string                  short_device_id;
    bool                    has_full_knowledge;
    boost::filesystem::path filename;
  };
  
  vector<DeviceData> device_data;

  //4th step: create a vector with the data of all the devices
  for(auto &e:directory_iterator(tmp_path))
  {
    if (is_regular_file(e) && extension(e)==".ydevice")
    {
      auto device=aux::load_json(e.path().string());
      if(device!=nullptr) 
      try
      {
          DeviceData tmp;
          tmp.GUID=device->get<string>("deviceGUID");
          tmp.short_device_id=device->get<string>("shortDeviceId");
          tmp.full_budget_file_version = YNABKnowledgeVersion(device->get<string>("knowledgeInFullBudgetFile"));
          tmp.budget_version = YNABKnowledgeVersion(device->get<string>("knowledge"));
          tmp.has_full_knowledge = device->get<string>("hasFullKnowledge")=="true"?true:false;
          tmp.filename=e;

          device_data.push_back(tmp);
      }
      catch (exception &e)
      {
        throw YNABConvertException(string("JSON error: ") + e.what()); 
      }
    }
  }
  
  //5th step: Sort the devices according to the budget version and full budget version
  std::sort(device_data.begin(),device_data.end(),[](const DeviceData &l, const DeviceData &r) { return l.budget_version>r.budget_version;});
  std::stable_sort(device_data.begin(),device_data.end(),[](const DeviceData &l, const DeviceData &r) { return l.full_budget_file_version>r.full_budget_file_version;});
  
  
  if(device_data.size()!=0)
  {
    if(!device_data[0].has_full_knowledge)
    {
      throw YNABConvertException("Most up-to-date budget yfull file doesn't contain full knowledge");
    } 
    return data_path/device_data[0].GUID/"Budget.yfull";
  }
  else
  {
    throw YNABConvertException("No yfull complete budget found");
  }
}

