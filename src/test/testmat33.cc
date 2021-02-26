#include "catch.hpp"
#include "engine/mat33.h"
#include "engine/mat44.h"
#include "test.h"

using namespace simple;

TEST_CASE("TestColumnMajor")
{
    mat33 m = mat33::FromColumnMajor(mat33::FA{0, 1, 2, 3, 4, 5, 6, 7, 8});
    CHECK(m(0, 0) == Approx(0));
    CHECK(m(1, 0) == Approx(1));
    CHECK(m(2, 0) == Approx(2));

    CHECK(m(0, 1) == Approx(3));
    CHECK(m(1, 1) == Approx(4));
    CHECK(m(2, 1) == Approx(5));

    CHECK(m(0, 2) == Approx(6));
    CHECK(m(1, 2) == Approx(7));
    CHECK(m(2, 2) == Approx(8));
}

TEST_CASE("TestRowMajor")
{
    mat33 m = mat33::FromRowMajor(mat33::FA{0, 1, 2, 3, 4, 5, 6, 7, 8});
    CHECK(m(0, 0) == Approx(0));
    CHECK(m(0, 1) == Approx(1));
    CHECK(m(0, 2) == Approx(2));

    CHECK(m(1, 0) == Approx(3));
    CHECK(m(1, 1) == Approx(4));
    CHECK(m(1, 2) == Approx(5));

    CHECK(m(2, 0) == Approx(6));
    CHECK(m(2, 1) == Approx(7));
    CHECK(m(2, 2) == Approx(8));
}

TEST_CASE("TestFromToMat44")
{
    CHECK(AreEqual(mat33::Identity().AsMat44().AsMat33(), mat33::Identity()));
}
