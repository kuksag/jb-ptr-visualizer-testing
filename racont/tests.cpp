#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "racont.h"
#include <set>
#include <vector>

using namespace NRacont;

class test_gen {
    int a = 3;
    int b = 7;
    int mod = 1e8 + 7;
    int x;
public:
    test_gen(int seed = 1) : x(seed) {}

    int operator()() {
        return x = (a * x + b) % mod;
    }
};

class sequent_gen {
    int i;
public:
    sequent_gen(int seed = 0) : i(seed) {}

    int operator()() {
        return i++;
    }
};

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

const int ITERATIONS = 1000;

std::vector<int> get_permutation(int n) {
    std::vector<int> order;
    for (int i = 0; i < n; i++) {
        order.push_back(i);
    }
    std::shuffle(order.begin(), order.end(), rng);
    return order;
}


TEST_CASE("Default init") {
    SUBCASE("Init without parameters") {
        NRacont::TRacont<int, std::mt19937_64> a;
        CHECK(a.size() == 0);
    }
    SUBCASE("Init with custom seed") {
        NRacont::TRacont<int, std::mt19937_64> a(1337);
        CHECK(a.size() == 0);
    }
    SUBCASE("Init with custom generator") {
        NRacont::TRacont<int, test_gen> a(210918);
        CHECK(a.size() == 0);
    }
}

TEST_CASE("Check insert method") {
    SUBCASE("One insert") {
        NRacont::TRacont<int> a;
        a.insert(0);
        CHECK(a.size() == 1);
    }
    SUBCASE("Many inserts") {
        NRacont::TRacont<int> a;
        for (int i = 0; i < ITERATIONS; i++) {
            a.insert(i);
        }
        CHECK(a.size() == ITERATIONS);
    }
    SUBCASE("Many inserts") {
        NRacont::TRacont<int> a;
        for (int i = 0; i < ITERATIONS; i++) {
            a.insert(i);
        }
        CHECK(a.size() == ITERATIONS);
    }
    SUBCASE("Many random inserts") {
        NRacont::TRacont<int> a;
        for (int v: get_permutation(ITERATIONS)) {
            a.insert(v);
        }
        CHECK(a.size() == ITERATIONS);
    }
    SUBCASE("Insert multiple times the same value") {
        NRacont::TRacont<int> a;
        for (int i = 0; i < ITERATIONS; i++) {
            a.insert(0);
        }
        CHECK(a.size() == ITERATIONS);
    }
}

TEST_CASE("Check erase method") {
    SUBCASE("Small erase") {
        NRacont::TRacont<int> a;
        a.insert(0);
        a.erase(0);
        a.insert(1);
        a.erase(1);
        a.insert(2);
        a.insert(3);
        a.erase(3);
        a.erase(2);
    }
    SUBCASE("Erase all") {
        NRacont::TRacont<int> a;
        for (int i = 0; i < ITERATIONS; i++) {
            a.insert(i);
        }
        for (int i = 0; i < ITERATIONS; i++) {
            a.erase(i);
        }
        CHECK(a.size() == 0);
    }
    SUBCASE("Erase all in random order") {
        NRacont::TRacont<int> a;
        for (int i = 0; i < ITERATIONS; i++) {
            a.insert(i);
        }
        for (int v: get_permutation(ITERATIONS)) {
            a.erase(v);
        }
        CHECK(a.size() == 0);
    }
    SUBCASE("Insert and erase multiple times same values") {
        NRacont::TRacont<int> a;
        for (int i = 0; i < ITERATIONS; i++) {
            a.insert(0);
            a.insert(1);
        }
        CHECK(a.size() == 2 * ITERATIONS);
        a.erase(0);
        CHECK(a.size() == 2 * ITERATIONS - 1);
        a.erase(0);
        CHECK(a.size() == 2 * ITERATIONS - 2);
        a.erase(1);
        CHECK(a.size() == 2 * ITERATIONS - 3);
        a.erase(1);
    }
}

TEST_CASE("Check random-get") {
    SUBCASE("1 insert & 1 get") {
        NRacont::TRacont<int> a;
        a.insert(0);
        CHECK(a() == 0);
    }
    SUBCASE("Use of custom generator") {
        NRacont::TRacont<int, test_gen> a;
        for (int v: get_permutation(ITERATIONS)) {
            a.insert(v);
        }
    }
    SUBCASE("Get all values with random-get") {
        NRacont::TRacont<int> a;
        for (int v: get_permutation(10)) {
            a.insert(v);
        }
        std::set<int> validator;
        for (int i = 0; i < ITERATIONS; i++) {
            validator.insert(a());
        }
        CHECK(validator.size() == a.size());
    }
    SUBCASE("Use of sequent generator") {
        NRacont::TRacont<int, sequent_gen> a;
        for (int i = 0; i < ITERATIONS; i++) {
            a.insert(i);
        }
        for (int i = 0; i < ITERATIONS; i++) {
            CHECK(i == a());
        }
    }
    SUBCASE("Use of sequent generator with seed") {
        NRacont::TRacont<int, sequent_gen> a(1);
        for (int i = 0; i < ITERATIONS; i++) {
            a.insert(i);
        }
        for (int i = 1; i < ITERATIONS + 1; i++) {
            CHECK(i % ITERATIONS == a());
        }
    }
}
