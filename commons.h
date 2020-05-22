#pragma once

#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;

bool StartsWith(const string& text, const string& pattern);

bool StartsWith(const string& text, const std::vector<string>& patterns);

vector<string> GetDirFilesStartsWith(const string& pattern, const string& dir = ".");
