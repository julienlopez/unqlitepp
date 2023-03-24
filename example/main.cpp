// reproducing https://unqlite.org/intro.html

#include <unqlitepp/unqlitepp.hpp>

#include <cstdlib>

int main()
{
    auto db = unqlitepp::openLocalFile("./test.db", unqlitepp::OpenMode::Create);

    return EXIT_SUCCESS;
}
