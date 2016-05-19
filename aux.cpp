#include "aux.hpp"

namespace aux
{
  bool is_numeric(std::string &s)
  {
    for(auto &e:s)
    {
      if(e<'0' || e>'9') return false;
    }
    return true;
  }

  std::vector<std::string> split(const std::string& str, const std::string& delimiter)
  {
    size_t pos = 0, lpos=0;

    std::string token;
    std::vector<std::string> ret;

    if((pos = str.find(delimiter,lpos))!=string::npos)
    {
      do
      {
        ret.push_back(str.substr(lpos,pos-lpos));
        lpos=pos+delimiter.length();
      }
      while ((pos = str.find(delimiter,lpos)) != string::npos );
    }

    ret.push_back(str.substr(lpos,str.length()-lpos));

    return ret;
  }


  shared_ptr<ptree> load_json(const string& filename)
  {
    ptree ret;
    std::ifstream is(filename);

    try
    {
      read_json(is,ret);
      return shared_ptr<ptree>(new ptree(ret));

      is.close();
    }
    catch(const json_parser_error &e)
    {
      is.close();
      return nullptr;
    }
  }
}


