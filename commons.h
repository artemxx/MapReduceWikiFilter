#pragma once

#include <string>
#include <vector>

bool StartsWith(const std::string& text, const std::string& pattern);

bool StartsWith(const std::string& text, const std::vector<std::string>& patterns);

std::vector<std::string> GetDirFilesStartsWith(
    const std::string& pattern, const std::string& dir = ".");
