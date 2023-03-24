// reproducing https://unqlite.org/intro.html

#include <unqlitepp/unqlitepp.hpp>

#include <cstdlib>

#include <iostream>

int main()
{

    auto db = unqlitepp::openLocalFile("./test.db", unqlitepp::OpenMode::Create);
    if(!db)
    {
        std::cout << "cannot open : " << db.error().code << " - " << db.error().log << std::endl;
        return -1;
    }

    // Store some records
    const auto res1 = unqlitepp::store(*db, "test", "Hello World");
    if(not res1)
    {
        std::cout << "cannot open : " << res1.error().code << " - " << res1.error().log << std::endl;
        return -2;
    }

    // Append some data
    const auto res2 = unqlitepp::append(*db, "test", ", how are you?");
    if(not res2)
    {
        std::cout << "cannot open : " << res2.error().code << " - " << res2.error().log << std::endl;
        return -2;
    }

    std::cout << "all good\n";
    return EXIT_SUCCESS;
}
