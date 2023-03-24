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
    const auto res = unqlitepp::store(*db, "test", "Hello World");
    if(not res)
    {
        std::cout << "cannot open : " << res.error().code << " - " << res.error().log << std::endl;
        return -2;
    }

    std::cout << "all good\n";
    return EXIT_SUCCESS;
}
