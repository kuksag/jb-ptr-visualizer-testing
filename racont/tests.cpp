#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "racont.h"
#include <set>
#include <vector>

class test_gen {
    int a = 3;
    int b = 7;
    int x;
public:
    test_gen(int seed = 1) : x(seed) {}

    int operator()() {
        return x = a * x + b;
    }
};

class sequent_gen {
    int i = 0;
public:
    sequent_gen(int) {}
    int operator()() {
        return i++;
    }
};

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
    SUBCASE("Init with custom generator") {
        racont::racont<int, test_gen> a(210918);
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

TEST_CASE("Test random access") {
    SUBCASE("Random access on first element") {
        racont::racont<int> a;
        a.insert(0);
        CHECK(a() == 0);
    }
    SUBCASE("Random access to all elements") {
        racont::racont<int> a;
        a.insert(0);
        a.insert(1);
        a.insert(2);
        a.insert(3);
        a.insert(4);
        std::set<int> pool;
        while (pool.size() != a.size()) {
            pool.insert(a());
        }
    }
    SUBCASE("Sequential random") {
        racont::racont<int, sequent_gen> a;
        a.insert(0);
        a.insert(1);
        a.insert(2);
        a.insert(3);
        a.insert(4);
        std::vector<int> b;
        for (std::size_t i = 0; i < 2 * a.size(); i++) {
            b.push_back(a());
        }
        CHECK((b == std::vector<int>{0, 1, 2, 3, 4, 0, 1, 2, 3, 4}));
    }
}
