#include "YNABConvert.hpp"

int main(int argc, char ** argv)
{
    
  if(argc!=3)
  {
    cout << "Invalid arguments" << endl <<
    "usage: " << argv[0] << " ynab_budget_folder sqlite3_db" << endl;
    return 0;
  }
  
  try
  {
    YNABConvert c;
    cout << c.convert(argv[1],argv[2]) << endl;
        
  }
  catch (YNABConvertInvalidBudgetException &e)
  {
      cout << "Invalid budget error: (" << e.what() << ")" << endl;
      return EXIT_FAILURE;
  }
  catch (YNABConvertException &e)
  {
      cerr << "Error: " << e.what() << endl;
      return EXIT_FAILURE;
  }
  
  
  return EXIT_SUCCESS;
  
}
