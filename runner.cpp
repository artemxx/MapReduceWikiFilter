#include <boost/filesystem.hpp>
#include <boost/process.hpp>

#include "commons.h"
#include "runner.h"

namespace bf = boost::filesystem;
namespace bp = boost::process;

using std::pair;
using std::string;
using std::vector;

string GetNumber(uint64_t number) {
    auto res = std::to_string(number);
    while (res.size() < 3) {
        res = "0" + res;
    }

    return res;
}

string GetFileName(const string& operation, const string& type, uint64_t file_num) {
    return operation + "_" + type + "_" + GetNumber(file_num) + ".txt";
}

void PrintBatchToFile(vector <string>& urls, uint64_t file_num) {
    const auto file_name = GetFileName("map", "input", file_num);
    bf::create_directory(kMapFilesDir);
    std::ofstream out(kMapFilesDir + "/" + file_name);
    for (const auto& url : urls) {
        out << url << "\t\n";
    }
}

TMapReduceRunner::TMapReduceRunner(TMapReduceOptions options)
    : options_(std::move(options)) {
}

void TMapReduceRunner::RemoveTempFiles() {
    bf::remove_all(kMapFilesDir);
    bf::remove_all(kReduceFilesDir);
}

void TMapReduceRunner::SplitDataOnFiles() const {
    std::ifstream in(options_.urls_path.data());
    std::vector<std::string> url_batch;
    url_batch.reserve(kChunkSize);
    std::string url;
    uint64_t file_num = 0;
    while (getline(in, url, '\t')) {
        url_batch.push_back(url);
        if (url_batch.size() == kChunkSize) {
            PrintBatchToFile(url_batch, file_num++);
            url_batch.clear();
        }
        in.get();
    }

    if (!url_batch.empty()) {
        PrintBatchToFile(url_batch, file_num);
    }
}

void TMapReduceRunner::RunMap() const {
    const auto inputs = GetDirFilesStartsWith(kMapInputPattern, kMapFilesDir);

    vector <bp::child> processes;
    processes.reserve(inputs.size());
    uint64_t file_num = 0;
    for (const auto& input : inputs) {
        const auto file_name = kMapFilesDir + "/" + GetFileName("map", "output", file_num++);
        auto child = bp::child(options_.script_path + " " + options_.words_path,
                               bp::std_in < input, bp::std_out > file_name);
        processes.push_back(std::move(child));
    }

    for (auto& process : processes) {
        process.wait();
    }
}

void TMapReduceRunner::Sort() const {
    bf::create_directory(kReduceFilesDir);
    bp::system(options_.script_path,
               bp::std_out > kReduceFilesDir + "/" + kSortedFileName);
}

void TMapReduceRunner::RunReduce() {
    SplitIntoJobs();
    RunReduceJobs();
    MergeOutputs();
}

void TMapReduceRunner::MergeOutputs() const {
    const auto files = GetDirFilesStartsWith(kReduceOutputPattern, kReduceFilesDir);
    assert(!files.empty());

    // cat file1.txt file2.txt ... > results.txt
    string command = "cat";
    for (const auto& file : files) {
        command += " " + file;
    }

    bp::system(command, bp::std_out > options_.dst_file);
}

void TMapReduceRunner::RunReduceJobs() const {
    const auto inputs = GetDirFilesStartsWith(kReduceInputPattern, kReduceFilesDir);
    vector <bp::child> processes;
    processes.reserve(inputs.size());
    uint64_t file_num = 0;
    for (const auto& input : inputs) {
        const auto file_name = kReduceFilesDir + "/" + GetFileName("reduce", "output", file_num++);
        auto child = bp::child(options_.script_path, bp::std_in < input, bp::std_out > file_name);
        processes.push_back(std::move(child));
    }

    for (auto& process : processes) {
        process.wait();
    }
}

void TMapReduceRunner::SplitIntoJobs() {
    std::ifstream in(kReduceFilesDir + "/" + kSortedFileName);
    string text;
    vector <pair<string, string>> cur;
    uint64_t file_num = 0;
    while (getline(in, text)) {
        const auto word = text.substr(0, text.find('\t'));
        if (cur.empty() || cur.back().second == word) {
            cur.emplace_back(text, word);
        } else {
            GenerateReduceInput(cur, file_num++);
            cur.clear();
            cur.emplace_back(text, word);
        }
    }

    GenerateReduceInput(cur, file_num);
}

void TMapReduceRunner::GenerateReduceInput(const vector <pair<string, string>>& data, uint64_t file_num) {
    const auto file_name = kReduceFilesDir + "/" + GetFileName("reduce", "input", file_num);
    std::ofstream out(file_name);
    for (const auto& it : data) {
        out << it.first << "\n";
    }
}
