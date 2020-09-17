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

#include "emugl/common/stringparsing.h"

#include <algorithm>

// Replaces |rm| with |rep| in |str|.
// Example: rm = "a", rep = "bb", str = "abcd" returns "bbbcd"
std::string replace_with(const std::string& rm,
                         const std::string& rep,
                         const std::string& str) {
    std::string res;
    size_t p = 0;
    while (p < str.size()) {
        size_t match_pos = str.find(rm,  p);
        if (match_pos != std::string::npos) {
            res.append(str.substr(p, match_pos - p));
            res.append(rep);
            p = match_pos + rm.size();
        } else {
            res.append(str.substr(p, str.size() - p));
            p = str.size();
        }
    }
    return res;
}

// Removes everything between "in" and "out" characters
// including the in/out characters themselves.
// Only cares about full matches.
// Only one level deep.
// Example: src = "a (1 2)b(3)",
//          in = "(", out = ")"
//          returns "a b"
// Example: src = "a (1 2 (3))"
//          in = "(", out = ")"
//          returns "a )"
std::string remove_in_out(const std::string& src,
                          const std::string& in,
                          const std::string& out,
                          bool* err) {
    std::string res;
    size_t p = 0;

    while (p < src.size()) {
        bool found_match = false;
        size_t in_pos = src.find(in, p);
        size_t out_pos;
        if (in_pos != std::string::npos) {
            out_pos = src.find(out, in_pos + in.size());
            if (out_pos != std::string::npos) {
                found_match = true;
            } else {
                *err = true;
                return src;
            }
        } else {
            out_pos = src.find(out, p);
            if (out_pos != std::string::npos) {
                *err = true;
                return src;
            }
        }

        if (found_match) {
            res.append(src.substr(p, in_pos - p));
            p = out_pos + out.size();
        } else {
            res.append(src.substr(p, src.size() - p));
            p = src.size();
        }
    }

    return res;
}

// Isolates everything between "in" and "out" characters,
// not including the in/out characters themselves.
// Only cares about full matches.
// Only one level deep.
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
                          bool* err) {
    std::vector<std::string> err_res;
    std::vector<std::string> res;
    size_t p = 0;

    while (p < src.size()) {
        bool found_match = false;
        size_t in_pos = src.find(in, p);
        size_t out_pos;
        if (in_pos != std::string::npos) {
            out_pos = src.find(out, in_pos + in.size());
            if (out_pos != std::string::npos) {
                found_match = true;
                res.push_back(src.substr(in_pos + in.size(), out_pos - in_pos - in.size()));
            } else {
                *err = true;
                return err_res;
            }
        } else {
            out_pos = src.find(out, p);
            if (out_pos != std::string::npos) {
                *err = true;
                return err_res;
            }
        }

        if (found_match) {
            p = out_pos + out.size();
        } else {
            p = src.size();
        }
    }

    return res;
}

// Splits a string |src| delimited by |delim|.
// Example: delim = ",", src = "1,2,3"
//          returns vector with ["1","2","3"]
std::vector<std::string> split(const std::string& delim,
                               const std::string& src) {
    std::vector<std::string> res;
    const size_t notfound = std::string::npos;
    size_t delim_size = delim.size();
    size_t p = 0;
    size_t delim_loc = src.size();

    while (p < src.size()) {
        delim_loc = src.find(delim, p);
        if (delim_loc != notfound) {
            res.push_back(src.substr(p, delim_loc - p));
            p = delim_loc + delim_size;
            delim_loc = p;
        } else {
            res.push_back(src.substr(p, src.size() - p));
            p = src.size();
        }
    }

    return res;
}

// Attempts to find any string in to_find in src.
// Returns the position of the last match, or std::string::npos
// Example: src = "abcdef", to_find = ["b", "c", "d"]
//          returns 3
size_t multi_find(const std::string& src,
                  const std::vector<std::string>& to_find) {

    size_t res = std::string::npos;
    for (size_t i = 0; i < to_find.size(); i++) {
        size_t p = src.find(to_find[i]);
        if (p != std::string::npos) {
            res = p;
        }
    }
    return res;
}
