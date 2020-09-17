// Copyright (C) 2016 The Android Open Source Project
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

#pragma once

#include <string>
#include <vector>

// Replaces |rm| with |rep| in |str|.
// Example: rm = "a", rep = "bb", str = "abcd" returns "bbbcd"
std::string replace_with(const std::string& rm,
                         const std::string& rep,
                         const std::string& str);

// Removes everything between "in" and "out" characters
// including the in/out characters themselves.
// Only cares about full matches.
// Only one level deep.
// *err = true if something went wrong during parsing,
// in which case the original string is returned.
// Example: src = "a (1 2)b(3)",
//          in = "(", out = ")"
//          returns "a b"
// Example: src = "a (1 2 (3))"
//          in = "(", out = ")"
//          returns "a )"
std::string remove_in_out(const std::string& src,
                          const std::string& in,
                          const std::string& out,
                          bool* err);

// Isolates everything between "in" and "out" characters,
// not including the in/out characters themselves.
// Only cares about full matches.
// Only one level deep.
// *err = true if something went wrong during parsing,
// in which case empty vector is returned.
// Example: src = "a (1 2)b(3)",
//          in = "(", out = ")"
//          returns vector with ["1 2", "3"]
// Example: src = "a (1 2 (3))"
//          in = "(", out = ")"
//          returns vector with ["1 2 (3"]
std::vector<std::string> isolate_in_out(
                          const std::string& src,
                          const std::string& in,
                          const std::string& out,
                          bool* err);

// Splits a string |src| delimited by |delim|.
// Example: delim = ",", src = "1,2,3"
//          returns vector with ["1","2","3"]
std::vector<std::string> split(const std::string& delim,
                               const std::string& src);

// Attempts to find any string in to_find in src.
// Returns the position of the last match, or std::string::npos
// Example: src = "abcdef", to_find = ["b", "c", "d"]
//          returns 3
size_t multi_find(const std::string& src,
                  const std::vector<std::string>& to_find);

