#include "YNABKnowledgeVersion.hpp"

YNABKnowledgeVersion::YNABKnowledgeVersion() {}

YNABKnowledgeVersion::YNABKnowledgeVersion(const string &s)
{
    *this=s;
}

void YNABKnowledgeVersion::operator=(const string &s)
{
auto devices=aux::split(s,",");

for(auto it=devices.begin();it!=devices.end();++it)
{
    auto device=aux::split(*it,"-");
    if(device.size()==2)
    {
    m[device[0]]=boost::lexical_cast<unsigned int>(device[1]);
    }
}
}

bool YNABKnowledgeVersion::operator<(const YNABKnowledgeVersion& rhs) const
{
if(m.size()>rhs.m.size()) return false;
bool allequal=true;

for(const auto &e:m)
{
    const auto it=rhs.m.find(e.first);
    if(it==rhs.m.cend() || it->second>e.second) return false;
    else if(it->second<e.second) allequal=false;
}
if(allequal && m.size()==rhs.m.size()) return false;

return true;
}

bool YNABKnowledgeVersion::operator>(const YNABKnowledgeVersion& rhs) const
{
if(rhs.m.size()>m.size()) return false;
bool allequal=true;

for(const auto &e:rhs.m)
{
    const auto it=m.find(e.first);
    if(it==m.cend() || it->second<e.second) return false;
    else if(it->second>e.second) allequal=false;
}
if(allequal && m.size()==rhs.m.size()) return false;

return true;
}

bool YNABKnowledgeVersion::operator==(const YNABKnowledgeVersion& rhs) const
{
if(rhs.m.size()!=m.size()) return false;

for(const auto &e:m)
{
    const auto it=rhs.m.find(e.first);
    if(it==rhs.m.cend() || it->second!=e.second) return false;
}

return true;
}
bool YNABKnowledgeVersion::operator!=(const YNABKnowledgeVersion& rhs) const {return !(*this==rhs);}
bool YNABKnowledgeVersion::operator<=(const YNABKnowledgeVersion &rhs) const {return (*this == rhs || *this < rhs);}
bool YNABKnowledgeVersion::operator>=(const YNABKnowledgeVersion &rhs) const {return (*this == rhs || *this > rhs);}

string YNABKnowledgeVersion::to_string()
{
bool first=true;
string ret;
for(auto &e:m)
{
    if(first) first=false;
    else ret+=",";
    ret+= e.first + "-" + boost::lexical_cast<string>(e.second);
}
return ret;
}