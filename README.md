#YNAB to SQLite3 converter 

##What does it do?

It converts .ynab budget folders into sqlite3 databases

##How does it work?

First it searches for the most up-to-date .yfull budget in the folder, then it processes the JSON file.

The JSON file processing is done in two steps: First it tries to figure out the database structure from the JSON structure in order to create all the tables, then it re-reads the JSON file to grab the data.

##Will it work in every case?

No. If the .yfull file doesn't contain the entire budget, it will not be able to do it.

##Compiling

You'll need a C++11 compiler, Boost 1.61 (boost\_filesystem and boost\_system), SQLite3 and SOCI compiled for SQLite3.

I tested this in Linux with GCC and you can compile it with the following command:

    g++ -O3 -std=c++0x main.cpp YNABKnowledgeVersion.cpp aux.cpp YNABConvert.cpp -lboost_system -lboost_filesystem -lsoci_core -lsoci_sqlite3 -lsqlite3

It's probably possible to compile with LLVM and for other operating systems, but beware the you'll need to compile the libs with LLVM if you want to compile the code with LLVM.

