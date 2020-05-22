#include "commons.h"

#define BOOST_SYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;

bool StartsWith(const string& text, const string& pattern) {
    if (text.size() < pattern.size()) {
        return false;
    }

    for (size_t i = 0; i < pattern.size(); ++i) {
        if (text[i] != pattern[i]) {
            return false;
        }
    }

    return true;
}

bool StartsWith(const string& text, const vector<string>& patterns) {
    for (const auto& pattern : patterns) {
        if (StartsWith(text, pattern)) {
            return true;
        }
    }

    return false;
}

vector<string> GetDirFilesStartsWith(const string& pattern, const string& dir) {
    vector<string> result;
    for (const auto& file : bf::directory_iterator(dir)) {
        const auto& path = file.path();
        if (StartsWith(path.string(), pattern)) {
            result.push_back(path.string());
        }
    }

    std::sort(result.begin(), result.end());

    return result;
}
