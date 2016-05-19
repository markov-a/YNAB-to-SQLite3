#ifndef AUX_HPP
#define AUX_HPP

#include <string>
#include <vector>
#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using namespace boost::property_tree;

namespace aux
{
  bool is_numeric(std::string &s);
  std::vector<std::string> split(const std::string& str, const std::string& delimiter);
  shared_ptr<ptree> load_json(const string& filename);
}

#endif
