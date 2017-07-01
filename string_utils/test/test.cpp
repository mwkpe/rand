#include "../string_utils.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


TEST_CASE("ascii") {
  using namespace mpxe::string;

  SUBCASE("to_upper") {
    auto s = std::string{"abc_XYZ,-$"};
    ascii::to_upper(s);
    CHECK(s == "ABC_XYZ,-$");

    s = std::string{""};
    ascii::to_upper(s);
    CHECK(s == "");
  }

  SUBCASE("to_lower") {
    auto s = std::string{"abc_XYZ,-$"};
    ascii::to_lower(s);
    CHECK(s == "abc_xyz,-$");

    s = std::string{""};
    ascii::to_lower(s);
    CHECK(s == "");
  }

  SUBCASE("as_upper") {
    CHECK(ascii::as_upper("abc_XYZ,-$") == "ABC_XYZ,-$");
    CHECK(ascii::as_upper("") == "");
  }

  SUBCASE("as_lower") {
    CHECK(ascii::as_lower("abc_XYZ,-$") == "abc_xyz,-$");
    CHECK(ascii::as_lower("") == "");
  }
}


TEST_CASE("starts_with") {
  using namespace mpxe::string;

  CHECK(starts_with("hello world", "hello") == true);
  CHECK(starts_with("hello world", "h") == true);
  CHECK(starts_with("hello world", "hello w") == true);
  CHECK(starts_with("hello world", "hello world") == true);
  CHECK(starts_with(u8"初音ミク", u8"初音") == true);

  CHECK(starts_with("hello world", "world") == false);
  CHECK(starts_with("hello world", "") == false);
  CHECK(starts_with("", "world") == false);
  CHECK(starts_with("hello world", "hello world ") == false);
  CHECK(starts_with("", "") == false);
  CHECK(starts_with(u8"初音ミク", u8"音") == false);
}


TEST_CASE("split_keep_empty") {
  using namespace mpxe::string;

  SUBCASE("1") {
    auto v = split_keep_empty("a,b,c", ',');

    CHECK(v.size() == 3);
    CHECK(v[0] == "a");
    CHECK(v[1] == "b");
    CHECK(v[2] == "c");
  }

  SUBCASE("2") {
    auto v = split_keep_empty("", ',');

    CHECK(v.size() == 1);
    CHECK(v[0].empty());
  }

  SUBCASE("3") {
    auto v = split_keep_empty(",", ',');

    CHECK(v.size() == 2);
    CHECK(v[0].empty());
    CHECK(v[1].empty());
  }

  SUBCASE("4") {
    auto v = split_keep_empty(",,", ',');

    CHECK(v.size() == 3);
    CHECK(v[0].empty());
    CHECK(v[1].empty());
    CHECK(v[2].empty());
  }

  SUBCASE("5") {
    auto v = split_keep_empty("a,", ',');

    CHECK(v.size() == 2);
    CHECK(v[0] == "a");
    CHECK(v[1].empty());
  }

  SUBCASE("6") {
    auto v = split_keep_empty(",$", ',');

    CHECK(v.size() == 2);
    CHECK(v[0].empty());
    CHECK(v[1] == "$");
  }

  SUBCASE("7") {
    auto v = split_keep_empty("ab:::b:ccc", ':');

    CHECK(v.size() == 5);
    CHECK(v[0] == "ab");
    CHECK(v[1].empty());
    CHECK(v[2].empty());
    CHECK(v[3] == "b");
    CHECK(v[4] == "ccc");
  }

  SUBCASE("8") {
    auto v = split_keep_empty(",alpha-beta,1-", '-');

    CHECK(v.size() == 3);
    CHECK(v[0] == ",alpha");
    CHECK(v[1] == "beta,1");
    CHECK(v[2].empty());
  }
}


TEST_CASE("split_ignore_empty") {
  using namespace mpxe::string;

  SUBCASE("1") {
    auto v = split_ignore_empty("a,b,c", ',');

    CHECK(v.size() == 3);
    CHECK(v[0] == "a");
    CHECK(v[1] == "b");
    CHECK(v[2] == "c");
  }

  SUBCASE("2") {
    auto v = split_ignore_empty("", ',');

    CHECK(v.size() == 0);
  }

  SUBCASE("3") {
    auto v = split_ignore_empty(",", ',');

    CHECK(v.size() == 0);
  }

  SUBCASE("4") {
    auto v = split_ignore_empty(",,", ',');

    CHECK(v.size() == 0);
  }

  SUBCASE("5") {
    auto v = split_ignore_empty("a,", ',');

    CHECK(v.size() == 1);
    CHECK(v[0] == "a");
  }

  SUBCASE("6") {
    auto v = split_ignore_empty(",$", ',');

    CHECK(v.size() == 1);
    CHECK(v[0] == "$");
  }

  SUBCASE("7") {
    auto v = split_ignore_empty("ab:::b:ccc", ':');

    CHECK(v.size() == 3);
    CHECK(v[0] == "ab");
    CHECK(v[1] == "b");
    CHECK(v[2] == "ccc");
  }

  SUBCASE("8") {
    auto v = split_ignore_empty(",alpha-beta,1-", '-');

    CHECK(v.size() == 2);
    CHECK(v[0] == ",alpha");
    CHECK(v[1] == "beta,1");
  }
}
