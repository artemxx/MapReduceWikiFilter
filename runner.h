#pragma once

#include <bits/stdc++.h>

using namespace std;

static constexpr auto kChunkSize = 3;
static const string kMapFilesDir = "map_files";
static const string kReduceFilesDir = "reduce_files";
static const string kMapInputPattern = kMapFilesDir + "/map_input_";
static const string kReduceInputPattern = kReduceFilesDir + "/reduce_input_";
static const string kReduceOutputPattern = kReduceFilesDir + "/reduce_output_";
static const string kSortedFileName = "sorted.txt";

struct TMapReduceOptions {
    std::string type;
    std::string script_path;
    std::string urls_path;
    std::string words_path;
    std::string dst_file;
};

class TMapReduceRunner {
public:
    explicit TMapReduceRunner(TMapReduceOptions options)
        : options_(std::move(options)) {
    }

    static void RemoveTempFiles();

    void SplitDataOnFiles() const;

    void RunMap() const;

    void Sort() const;

    void RunReduce();

private:
    TMapReduceOptions options_;

    void MergeOutputs() const;

    void RunReduceJobs() const;

    static void SplitIntoJobs();

    static void GenerateReduceInput(const vector<pair<string, string>>& data, uint64_t file_num);
};