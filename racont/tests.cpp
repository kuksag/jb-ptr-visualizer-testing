#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "racont.h"


TEST_CASE("Default init") {
    SUBCASE("Init without parameters") {
        racont::racont<int, std::mt19937_64> a;
        CHECK(a.size() == 0);
        CHECK(a.capacity() == 0);
    }
    SUBCASE("Init with custom seed") {
        racont::racont<int, std::mt19937_64> a(1337);
        CHECK(a.size() == 0);
        CHECK(a.capacity() == 0);
    }
}

TEST_CASE("Check insert method") {
    SUBCASE("Sequential inserts") {
        racont::racont<int> a;
        a.insert(0, 0);
        CHECK(a[0] == 0);
        a.insert(1, 1);
        CHECK(a[0] == 0);
        CHECK(a[1] == 1);
        a.insert(2, 2);
        CHECK(a[0] == 0);
        CHECK(a[1] == 1);
        CHECK(a[2] == 2);
        a.insert(3, 3);
        CHECK(a[0] == 0);
        CHECK(a[1] == 1);
        CHECK(a[2] == 2);
        CHECK(a[3] == 3);
    }
    SUBCASE("Random inserts") {
        racont::racont<int> a;
        a.insert(0, 0);
        CHECK(a[0] == 0);
        a.insert(0, 1);
        CHECK(a[0] == 1);
        CHECK(a[1] == 0);
        a.insert(1, 2);
        CHECK(a[0] == 1);
        CHECK(a[1] == 2);
        CHECK(a[2] == 0);
        a.insert(3, 3);
        CHECK(a[0] == 1);
        CHECK(a[1] == 2);
        CHECK(a[2] == 0);
        CHECK(a[3] == 3);
    }
}
