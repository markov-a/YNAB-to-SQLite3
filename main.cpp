#include "YNABConvert.hpp"

int main(int argc, char ** argv)
{
    
  if(argc!=2)
  {
    cout << "Wrong arguments" << endl <<
    "usage: " << argv[0] << " ynab_budget_folder sqlite3_db" << endl;
    return 0;
  }
  
  YNABConvert c;
  cout << c.convert(argv[1],argv[2]) << endl;
}
