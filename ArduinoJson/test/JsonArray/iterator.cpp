// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <typename TIterator>
static void run_iterator_test() {
  StaticJsonDocument<JSON_ARRAY_SIZE(2)> doc;
  JsonArray array = doc.to<JsonArray>();
  array.add(12);
  array.add(34);

  TIterator it = array.begin();
  TIterator end = array.end();

  REQUIRE(end != it);
  REQUIRE(12 == it->template as<int>());
  REQUIRE(12 == static_cast<int>(*it));
  ++it;
  REQUIRE(end != it);
  REQUIRE(34 == it->template as<int>());
  REQUIRE(34 == static_cast<int>(*it));
  ++it;
  REQUIRE(end == it);
}

TEST_CASE("JsonArray::begin()/end()") {
  SECTION("Mutable") {
    run_iterator_test<JsonArray::iterator>();
  }
}
