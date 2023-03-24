#include <catch2/catch_test_macros.hpp>

#include <unqlitepp/unqlitepp.hpp>

using namespace std::string_literals;

TEST_CASE("Basic store and fetch workflow")
{
    auto db = unqlitepp::openInMemory(unqlitepp::OpenMode::ReadWrite);
    // auto db = unqlitepp::openLocalFile("./test.db", unqlitepp::OpenMode::Create);
    REQUIRE(db.has_value());

    const auto key1 = "key1";
    const auto data1 = "some bits of data"s;
    const auto res_store1 = unqlitepp::store(*db, key1, data1);
    REQUIRE(res_store1.has_value());

    const auto key2 = "key 2";
    const auto data2 = "some other bits of data"s;
    const auto res_store2 = unqlitepp::store(*db, key2, data2);
    REQUIRE(res_store2.has_value());

    {
        const auto res_fetch1 = unqlitepp::fetch(*db, key1);
        REQUIRE(res_fetch1.has_value());
        CHECK(*res_fetch1 == data1);

        const auto res_fetch2 = unqlitepp::fetch(*db, key2);
        REQUIRE(res_fetch2.has_value());
        CHECK(*res_fetch2 == data2);
    }

    const auto res_erase1 = unqlitepp::erase(*db, key1);
    REQUIRE(res_erase1.has_value());

    {
        const auto res_fetch1 = unqlitepp::fetch(*db, key1);
        REQUIRE_FALSE(res_fetch1.has_value());

        const auto res_fetch2 = unqlitepp::fetch(*db, key2);
        REQUIRE(res_fetch2.has_value());
        CHECK(*res_fetch2 == data2);
    }
}
