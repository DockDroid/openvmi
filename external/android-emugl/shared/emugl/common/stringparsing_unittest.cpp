// Copyright (C) 2014 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "emugl/common/stringparsing.h"

#include <gtest/gtest.h>

namespace emugl {

TEST(StringParsing, Empty) {
    std::string e;
    std::string replace_res = replace_with(std::string("a"),
                                           std::string("b"),
                                           e);
    EXPECT_STREQ("", replace_res.c_str());

    bool err = false;
    std::string remove_inout_res =
        remove_in_out(e, e, e, &err);
    EXPECT_FALSE(err);

    EXPECT_STREQ("", remove_inout_res.c_str());

    std::vector<std::string> isolate_inout_res =
        isolate_in_out(e, e, e, &err);
    EXPECT_FALSE(err);

    EXPECT_EQ(0U, isolate_inout_res.size());

    std::vector<std::string> split_res =
        split(e, e);
    EXPECT_EQ(0U, split_res.size());

    std::vector<std::string> split_nonempty_delim_res =
        split(std::string("("), e);
    EXPECT_EQ(0U, split_res.size());
}

TEST(StringParsing, Simple) {
    std::string e("a b (1 2 3) (4 5) (6 7)");
    std::string replace_res = replace_with(std::string("a"),
                                           std::string("b"),
                                           e);
    EXPECT_STREQ("b", replace_res.substr(0,1).c_str());

    // Replace with something longer than the string itself
    replace_res = replace_with(std::string("2"),
                               std::string("asdfasdfasdfasdfasdfasdfasdf"),
                               e);
    EXPECT_STREQ("a b (1 asdfasdfasdfasdfasdfasdfasdf 3) (4 5) (6 7)",
                 replace_res.c_str());


    bool err = false;
    std::string remove_inout_res =
        remove_in_out(e, "(", ")", &err);
    EXPECT_FALSE(err);
    EXPECT_STREQ("a b   ", remove_inout_res.c_str());

    std::vector<std::string> isolate_inout_res =
        isolate_in_out(e, "(", ")", &err);
    EXPECT_FALSE(err);
    EXPECT_EQ(3U, isolate_inout_res.size());
    EXPECT_STREQ("1 2 3", isolate_inout_res[0].c_str());
    EXPECT_STREQ("4 5", isolate_inout_res[1].c_str());
    EXPECT_STREQ("6 7", isolate_inout_res[2].c_str());

    std::vector<std::string> split_res =
        split(" ", e);
    EXPECT_EQ(9U, split_res.size());
    EXPECT_STREQ("a", split_res[0].c_str());
    EXPECT_STREQ("b", split_res[1].c_str());
    EXPECT_STREQ("(1", split_res[2].c_str());
    EXPECT_STREQ("2", split_res[3].c_str());
    EXPECT_STREQ("3)", split_res[4].c_str());
    EXPECT_STREQ("(4", split_res[5].c_str());
    EXPECT_STREQ("5)", split_res[6].c_str());
    EXPECT_STREQ("(6", split_res[7].c_str());
    EXPECT_STREQ("7)", split_res[8].c_str());

    // multi_find returns position of the last one
    size_t match_res = multi_find(e, split_res);
    EXPECT_EQ(21U, match_res);

    // multi_find returns std::string::npos
    // if there aren't any matches
    std::vector<std::string> nomatches;
    nomatches.push_back("{");
    nomatches.push_back("}");
    nomatches.push_back("8");
    nomatches.push_back("asdf");
    EXPECT_EQ(std::string::npos, multi_find(e, nomatches));

    // Split with trailing delimiter
    split_res = split(" ", std::string("1 2 3 "));
    EXPECT_EQ(3U, split_res.size());

    // Split with multiple trailing delimiters
    split_res = split(" ", std::string("1 2 3  "));
    EXPECT_EQ(4U, split_res.size());
    EXPECT_STREQ("", split_res[3].c_str());

    // Split with multiple delimiters
    split_res = split(" ", std::string("1 2  3  "));
    EXPECT_EQ(5U, split_res.size());
    EXPECT_STREQ("", split_res[4].c_str());

    // Cases where we expect err = true:
    // 1. Only one level deep is supported
    // 2. Partial in/out matches
    std::string f0("a b (1 2 3 (4 5) (6 7))");
    std::string f1("(a b");
    std::string f2("a b)");

    err = false;
    remove_inout_res = remove_in_out(f0, "(", ")", &err);
    EXPECT_TRUE(err);
    EXPECT_STREQ(f0.c_str(), remove_inout_res.c_str());

    err = false;
    isolate_inout_res = isolate_in_out(f0, "(", ")", &err);
    EXPECT_TRUE(err);
    EXPECT_EQ(0U, isolate_inout_res.size());

    err = false;
    remove_inout_res = remove_in_out(f1, "(", ")", &err);
    EXPECT_TRUE(err);

    err = false;
    isolate_inout_res = isolate_in_out(f1, "(", ")", &err);
    EXPECT_TRUE(err);

    err = false;
    remove_inout_res = remove_in_out(f2, "(", ")", &err);
    EXPECT_TRUE(err);

    err = false;
    isolate_inout_res = isolate_in_out(f2, "(", ")", &err);
    EXPECT_TRUE(err);
}

}  // namespace emugl
