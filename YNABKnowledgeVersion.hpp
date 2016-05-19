#ifndef YNABKnowledgeVersion_HPP
#define YNABKnowledgeVersion_HPP

#include <map>
#include <string>
#include <boost/lexical_cast.hpp>

#include "aux.hpp"

using namespace std;

class YNABKnowledgeVersion
{
  map<string,unsigned int> m;

public:
  YNABKnowledgeVersion();
  YNABKnowledgeVersion(const string&);

  void operator=(const string&);
  bool operator<(const YNABKnowledgeVersion&) const;
  bool operator>(const YNABKnowledgeVersion&) const;
  bool operator==(const YNABKnowledgeVersion&) const;
  bool operator!=(const YNABKnowledgeVersion&) const;
  bool operator<=(const YNABKnowledgeVersion&) const;
  bool operator>=(const YNABKnowledgeVersion&) const;

  string to_string();
};

#endif
