#ifndef YNABKnowledgeVersion_HPP
#define YNABKnowledgeVersion_HPP

#include "aux.hpp"
#include "YNABConvertException.hpp"

#include <boost/lexical_cast.hpp>

#include <map>
#include <string>

using namespace std;

/**
 * This class is used to parse and compare YNAB4 budget versions
 * that are in the following format:
 * A-10,B-13,C-15
 * In which the letters A, B and C are the short name for the device
 * and the numbers are the budget versions.
 * This class is able to compare if a budget version is more recent than other
 * with the operators <, >, <=, >=, == and !=
 */
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
