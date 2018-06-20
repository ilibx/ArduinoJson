// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void eraseString(std::string &str) {
  char *p = const_cast<char *>(str.c_str());
  while (*p) *p++ = '*';
}

TEST_CASE("std::string") {
  DynamicJsonDocument doc;

  SECTION("operator[]") {
    char json[] = "{\"key\":\"value\"}";

    deserializeJson(doc, json);
    JsonObjectRef obj = doc.as<JsonObjectRef>();

    REQUIRE(std::string("value") == obj[std::string("key")]);
  }

  SECTION("operator[] const") {
    char json[] = "{\"key\":\"value\"}";

    deserializeJson(doc, json);
    JsonObjectRef obj = doc.as<JsonObjectRef>();

    REQUIRE(std::string("value") == obj[std::string("key")]);
  }

  SECTION("set(key)") {
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    std::string key("hello");
    obj.set(key, "world");
    eraseString(key);
    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("set(value)") {
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    std::string value("world");
    obj.set("hello", value);
    eraseString(value);
    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("set(key,value)") {
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    std::string key("hello");
    std::string value("world");
    obj.set(key, value);
    eraseString(key);
    eraseString(value);
    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("set(JsonArraySubscript)") {
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    DynamicJsonDocument doc2;
    JsonArray &arr = doc2.to<JsonArray>();
    arr.add("world");

    obj.set(std::string("hello"), arr[0]);

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("set(JsonObjectSubscript)") {
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    DynamicJsonDocument doc2;
    JsonObjectRef obj2 = doc2.to<JsonObjectRef>();
    obj2.set("x", "world");

    obj.set(std::string("hello"), obj2["x"]);

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("get<T>()") {
    char json[] = "{\"key\":\"value\"}";
    deserializeJson(doc, json);
    JsonObjectRef obj = doc.as<JsonObjectRef>();

    REQUIRE(std::string("value") == obj.get<const char *>(std::string("key")));
  }

  SECTION("is<T>()") {
    char json[] = "{\"key\":\"value\"}";
    deserializeJson(doc, json);
    JsonObjectRef obj = doc.as<JsonObjectRef>();

    REQUIRE(true == obj.is<const char *>(std::string("key")));
  }

  SECTION("createNestedObject()") {
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    std::string key = "key";
    char json[64];
    obj.createNestedObject(key);
    eraseString(key);
    serializeJson(doc, json, sizeof(json));
    REQUIRE(std::string("{\"key\":{}}") == json);
  }

  SECTION("createNestedArray()") {
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    std::string key = "key";
    char json[64];
    obj.createNestedArray(key);
    eraseString(key);
    serializeJson(doc, json, sizeof(json));
    REQUIRE(std::string("{\"key\":[]}") == json);
  }

  SECTION("containsKey()") {
    char json[] = "{\"key\":\"value\"}";
    deserializeJson(doc, json);
    JsonObjectRef obj = doc.as<JsonObjectRef>();
    REQUIRE(true == obj.containsKey(std::string("key")));
  }

  SECTION("remove()") {
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    obj["key"] = "value";

    obj.remove(std::string("key"));

    REQUIRE(0 == obj.size());
  }

  SECTION("operator[], set key") {
    std::string key("hello");
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    obj[key] = "world";
    eraseString(key);
    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("operator[], set value") {
    std::string value("world");
    JsonObjectRef obj = doc.to<JsonObjectRef>();
    obj["hello"] = value;
    eraseString(value);
    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("memoryUsage() increases when adding a new key") {
    std::string key1("hello"), key2("world");
    JsonObjectRef obj = doc.to<JsonObjectRef>();

    obj[key1] = 1;
    size_t sizeBefore = doc.memoryUsage();
    obj[key2] = 2;
    size_t sizeAfter = doc.memoryUsage();

    REQUIRE(sizeAfter - sizeBefore >= key2.size());
  }

  SECTION("memoryUsage() remains when adding the same key") {
    std::string key("hello");
    JsonObjectRef obj = doc.to<JsonObjectRef>();

    obj[key] = 1;
    size_t sizeBefore = doc.memoryUsage();
    obj[key] = 2;
    size_t sizeAfter = doc.memoryUsage();

    REQUIRE(sizeBefore == sizeAfter);
  }
}
