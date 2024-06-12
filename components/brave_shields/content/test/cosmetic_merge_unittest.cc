// Copyright (c) 2020 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <optional>

#include "base/json/json_reader.h"
#include "brave/components/brave_shields/core/browser/ad_block_service_helper.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace brave_shields {

using ::testing::_;

class CosmeticResourceMergeTest : public testing::Test {
 public:
  CosmeticResourceMergeTest() = default;
  ~CosmeticResourceMergeTest() override = default;

  void CompareMergeFromStrings(const std::string& a,
                               const std::string& b,
                               bool force_hide,
                               const std::string& expected) {
    std::optional<base::Value> a_val = base::JSONReader::Read(a);
    ASSERT_TRUE(a_val);

    std::optional<base::Value> b_val = base::JSONReader::Read(b);
    ASSERT_TRUE(b_val);

    const std::optional<base::Value> expected_val =
        base::JSONReader::Read(expected);
    ASSERT_TRUE(expected_val);

    MergeResourcesInto(std::move(b_val->GetDict()), *a_val->GetIfDict(),
                       force_hide);

    ASSERT_EQ(*a_val, *expected_val);
  }

 protected:
  void SetUp() override {}

  void TearDown() override {}
};

const char EMPTY_RESOURCES[] =
    "{"
    "\"hide_selectors\": [], "
    "\"style_selectors\": {}, "
    "\"exceptions\": [], "
    "\"remove_selectors\": [], "
    "\"remove_attrs\": {}, "
    "\"remove_classes\": {}, "
    "\"injected_script\": \"\", "
    "\"generichide\": false"
    "}";

const char NONEMPTY_RESOURCES[] =
    "{"
    "\"hide_selectors\": [\"a\", \"b\"], "
    "\"style_selectors\": {"
    "\"c\": [\"color: #fff\"], "
    "\"d\": [\"color: #000\"]"
    "}, "
    "\"exceptions\": [\"e\", \"f\"], "
    "\"remove_selectors\": [\"x\", \"y\"], "
    "\"remove_attrs\": {"
    "\"v\": [\"href\", \"id\"], "
    "\"w\": [\"attr\"]"
    "}, "
    "\"remove_classes\": {"
    "\"t\": [\"class-one\", \"class-two\"], "
    "\"u\": [\"class-three\"]"
    "}, "
    "\"injected_script\": \"console.log('g')\", "
    "\"generichide\": false"
    "}";

TEST_F(CosmeticResourceMergeTest, MergeTwoEmptyResources) {
  const std::string a = EMPTY_RESOURCES;
  const std::string b = EMPTY_RESOURCES;

  // Same as EMPTY_RESOURCES, but with an additional newline in the
  // injected_script
  const std::string expected =
      "{"
      "\"hide_selectors\": [], "
      "\"style_selectors\": {}, "
      "\"exceptions\": [], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {}, "
      "\"remove_classes\": {}, "
      "\"injected_script\": \"\n\", "
      "\"generichide\": false"
      "}";

  CompareMergeFromStrings(a, b, false, expected);
}

TEST_F(CosmeticResourceMergeTest, MergeEmptyIntoNonEmpty) {
  const std::string a = NONEMPTY_RESOURCES;
  const std::string b = EMPTY_RESOURCES;

  // Same as a, but with an additional newline at the end of the
  // injected_script
  const std::string expected =
      "{"
      "\"hide_selectors\": [\"a\", \"b\"], "
      "\"style_selectors\": {"
      "\"c\": [\"color: #fff\"], "
      "\"d\": [\"color: #000\"]"
      "}, "
      "\"exceptions\": [\"e\", \"f\"], "
      "\"remove_selectors\": [\"x\", \"y\"], "
      "\"remove_attrs\": {"
      "\"v\": [\"href\", \"id\"], "
      "\"w\": [\"attr\"]"
      "}, "
      "\"remove_classes\": {"
      "\"t\": [\"class-one\", \"class-two\"], "
      "\"u\": [\"class-three\"]"
      "}, "
      "\"injected_script\": \"console.log('g')\n\", "
      "\"generichide\": false"
      "}";

  CompareMergeFromStrings(a, b, false, expected);
}

TEST_F(CosmeticResourceMergeTest, MergeNonEmptyIntoEmpty) {
  const std::string a = EMPTY_RESOURCES;
  const std::string b = NONEMPTY_RESOURCES;

  // Same as b, but with an additional newline at the beginning of the
  // injected_script
  const std::string expected =
      "{"
      "\"hide_selectors\": [\"a\", \"b\"],"
      "\"style_selectors\": {"
      "\"c\": [\"color: #fff\"], "
      "\"d\": [\"color: #000\"]"
      "}, "
      "\"exceptions\": [\"e\", \"f\"], "
      "\"remove_selectors\": [\"x\", \"y\"], "
      "\"remove_attrs\": {"
      "\"v\": [\"href\", \"id\"], "
      "\"w\": [\"attr\"]"
      "}, "
      "\"remove_classes\": {"
      "\"t\": [\"class-one\", \"class-two\"], "
      "\"u\": [\"class-three\"]"
      "}, "
      "\"injected_script\": \"\nconsole.log('g')\", "
      "\"generichide\": false"
      "}";

  CompareMergeFromStrings(a, b, false, expected);
}

TEST_F(CosmeticResourceMergeTest, MergeNonEmptyIntoNonEmpty) {
  const std::string a = NONEMPTY_RESOURCES;
  const std::string b =
      "{"
      "\"hide_selectors\": [\"h\", \"i\"], "
      "\"style_selectors\": {"
      "\"j\": [\"color: #eee\"], "
      "\"k\": [\"color: #111\"]"
      "}, "
      "\"exceptions\": [\"l\", \"m\"], "
      "\"remove_selectors\": [\"k\"], "
      "\"remove_attrs\": {"
      "\"s\": [\"attr\"]"
      "}, "
      "\"remove_classes\": {"
      "\"r\": [\"class-six\"]"
      "}, "
      "\"injected_script\": \"console.log('n')\", "
      "\"generichide\": false"
      "}";

  const std::string expected =
      "{"
      "\"hide_selectors\": [\"a\", \"b\", \"h\", \"i\"], "
      "\"style_selectors\": {"
      "\"c\": [\"color: #fff\"], "
      "\"d\": [\"color: #000\"], "
      "\"j\": [\"color: #eee\"], "
      "\"k\": [\"color: #111\"]"
      "}, "
      "\"exceptions\": [\"e\", \"f\", \"l\", \"m\"], "
      "\"remove_selectors\": [\"x\", \"y\", \"k\"], "
      "\"remove_attrs\": {"
      "\"v\": [\"href\", \"id\"], "
      "\"w\": [\"attr\"], "
      "\"s\": [\"attr\"]"
      "}, "
      "\"remove_classes\": {"
      "\"t\": [\"class-one\", \"class-two\"], "
      "\"u\": [\"class-three\"], "
      "\"r\": [\"class-six\"]"
      "}, "
      "\"injected_script\": \"console.log('g')\nconsole.log('n')\", "
      "\"generichide\": false"
      "}";

  CompareMergeFromStrings(a, b, false, expected);
}

TEST_F(CosmeticResourceMergeTest, MergeEmptyForceHide) {
  const std::string a = EMPTY_RESOURCES;
  const std::string b = EMPTY_RESOURCES;

  // Same as EMPTY_RESOURCES, but with an additional newline in the
  // injected_script and a new empty `force_hide_selectors` array
  const std::string expected =
      "{"
      "\"hide_selectors\": [], "
      "\"style_selectors\": {}, "
      "\"exceptions\": [], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {}, "
      "\"remove_classes\": {}, "
      "\"injected_script\": \"\n\","
      "\"generichide\": false, "
      "\"force_hide_selectors\": []"
      "}";

  CompareMergeFromStrings(a, b, true, expected);
}

TEST_F(CosmeticResourceMergeTest, MergeNonEmptyForceHide) {
  const std::string a = NONEMPTY_RESOURCES;
  const std::string b =
      "{"
      "\"hide_selectors\": [\"h\", \"i\"], "
      "\"style_selectors\": {"
      "\"j\": [\"color: #eee\"], "
      "\"k\": [\"color: #111\"]"
      "}, "
      "\"exceptions\": [\"l\", \"m\"], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {}, "
      "\"remove_classes\": {}, "
      "\"injected_script\": \"console.log('n')\", "
      "\"generichide\": false"
      "}";

  const std::string expected =
      "{"
      "\"hide_selectors\": [\"a\", \"b\"], "
      "\"style_selectors\": {"
      "\"c\": [\"color: #fff\"], "
      "\"d\": [\"color: #000\"], "
      "\"j\": [\"color: #eee\"], "
      "\"k\": [\"color: #111\"]"
      "}, "
      "\"exceptions\": [\"e\", \"f\", \"l\", \"m\"], "
      "\"remove_selectors\": [\"x\", \"y\"], "
      "\"remove_attrs\": {"
      "\"v\": [\"href\", \"id\"], "
      "\"w\": [\"attr\"]"
      "}, "
      "\"remove_classes\": {"
      "\"t\": [\"class-one\", \"class-two\"], "
      "\"u\": [\"class-three\"]"
      "}, "
      "\"injected_script\": \"console.log('g')\nconsole.log('n')\","
      "\"generichide\": false, "
      "\"force_hide_selectors\": [\"h\", \"i\"]"
      "}";

  CompareMergeFromStrings(a, b, true, expected);
}

TEST_F(CosmeticResourceMergeTest, MergeNonGenerichideIntoGenerichide) {
  const std::string a =
      "{"
      "\"hide_selectors\": [], "
      "\"style_selectors\": {}, "
      "\"exceptions\": [], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {}, "
      "\"remove_classes\": {}, "
      "\"injected_script\": \"\n\", "
      "\"generichide\": true"
      "}";
  const std::string b = EMPTY_RESOURCES;

  const std::string expected =
      "{"
      "\"hide_selectors\": [], "
      "\"style_selectors\": {}, "
      "\"exceptions\": [], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {}, "
      "\"remove_classes\": {}, "
      "\"injected_script\": \"\n\n\", "
      "\"generichide\": true"
      "}";

  CompareMergeFromStrings(a, b, false, expected);
}

TEST_F(CosmeticResourceMergeTest, MergeGenerichideIntoNonGenerichide) {
  const std::string a = NONEMPTY_RESOURCES;
  const std::string b =
      "{"
      "\"hide_selectors\": [\"h\", \"i\"], "
      "\"style_selectors\": {"
      "\"j\": [\"color: #eee\"], "
      "\"k\": [\"color: #111\"]"
      "}, "
      "\"exceptions\": [\"l\", \"m\"], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {}, "
      "\"remove_classes\": {}, "
      "\"injected_script\": \"console.log('n')\", "
      "\"generichide\": true"
      "}";

  const std::string expected =
      "{"
      "\"hide_selectors\": [\"a\", \"b\", \"h\", \"i\"], "
      "\"style_selectors\": {"
      "\"c\": [\"color: #fff\"], "
      "\"d\": [\"color: #000\"], "
      "\"j\": [\"color: #eee\"], "
      "\"k\": [\"color: #111\"]"
      "}, "
      "\"exceptions\": [\"e\", \"f\", \"l\", \"m\"], "
      "\"remove_selectors\": [\"x\", \"y\"], "
      "\"remove_attrs\": {"
      "\"v\": [\"href\", \"id\"], "
      "\"w\": [\"attr\"]"
      "}, "
      "\"remove_classes\": {"
      "\"t\": [\"class-one\", \"class-two\"], "
      "\"u\": [\"class-three\"]"
      "}, "
      "\"injected_script\": \"console.log('g')\nconsole.log('n')\", "
      "\"generichide\": true"
      "}";

  CompareMergeFromStrings(a, b, false, expected);
}

TEST_F(CosmeticResourceMergeTest, MergeGenerichideIntoGenerichide) {
  const std::string a =
      "{"
      "\"hide_selectors\": [], "
      "\"style_selectors\": {}, "
      "\"exceptions\": [], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {}, "
      "\"remove_classes\": {}, "
      "\"injected_script\": \"\", "
      "\"generichide\": true"
      "}";

  const std::string expected =
      "{"
      "\"hide_selectors\": [], "
      "\"style_selectors\": {}, "
      "\"exceptions\": [], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {}, "
      "\"remove_classes\": {}, "
      "\"injected_script\": \"\n\", "
      "\"generichide\": true"
      "}";

  CompareMergeFromStrings(a, a, false, expected);
}

TEST_F(CosmeticResourceMergeTest, MergeStyles) {
  const std::string a =
      "{"
      "\"hide_selectors\": [], "
      "\"style_selectors\": {"
      "\".a\": [\"color: #eee\"], "
      "\".b\": [\"color: #111\"], "
      "\".d\": [\"padding: 0\"]"
      "}, "
      "\"exceptions\": [], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {"
      "\"v\": [\"href\", \"id\"], "
      "\"w\": [\"attr\"]"
      "}, "
      "\"remove_classes\": {"
      "\"t\": [\"class-one\", \"class-two\"], "
      "\"u\": [\"class-three\"]"
      "}, "
      "\"injected_script\": \"\", "
      "\"generichide\": false"
      "}";
  const std::string b =
      "{"
      "\"hide_selectors\": [], "
      "\"style_selectors\": {"
      "\".c\": [\"margin: 0\"], "
      "\".b\": [\"background: #000\"], "
      "\".a\": [\"background: #fff\"]"
      "}, "
      "\"exceptions\": [], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {"
      "\"v\": [\"class\", \"data-no\"], "
      "\"s\": [\"attr\"]"
      "}, "
      "\"remove_classes\": {"
      "\"t\": [\"class-four\", \"class-five\"], "
      "\"r\": [\"class-six\"]"
      "}, "
      "\"injected_script\": \"\", "
      "\"generichide\": false"
      "}";

  const std::string expected =
      "{"
      "\"hide_selectors\": [], "
      "\"style_selectors\": {"
      "\".a\": [\"color: #eee\", \"background: #fff\"], "
      "\".b\": [\"color: #111\", \"background: #000\"], "
      "\".c\": [\"margin: 0\"], "
      "\".d\": [\"padding: 0\"] "
      "}, "
      "\"exceptions\": [], "
      "\"remove_selectors\": [], "
      "\"remove_attrs\": {"
      "\"v\": [\"href\", \"id\", \"class\", \"data-no\"], "
      "\"w\": [\"attr\"], "
      "\"s\": [\"attr\"]"
      "}, "
      "\"remove_classes\": {"
      "\"t\": [\"class-one\", \"class-two\", \"class-four\", \"class-five\"], "
      "\"u\": [\"class-three\"], "
      "\"r\": [\"class-six\"]"
      "}, "
      "\"injected_script\": \"\n\", "
      "\"generichide\": false"
      "}";

  CompareMergeFromStrings(a, b, false, expected);
}

}  // namespace brave_shields
