#pragma once

#include <string>
#include <vector>
#include <utility>

static constexpr auto kChunkSize = 3;
static const std::string kMapFilesDir = "map_files";
static const std::string kReduceFilesDir = "reduce_files";
static const std::string kMapInputPattern = kMapFilesDir + "/map_input_";
static const std::string kReduceInputPattern = kReduceFilesDir + "/reduce_input_";
static const std::string kReduceOutputPattern = kReduceFilesDir + "/reduce_output_";
static const std::string kSortedFileName = "sorted.txt";

struct TMapReduceOptions {
    std::string type;
    std::string script_path;
    std::string urls_path;
    std::string words_path;
    std::string dst_file;
};

class TMapReduceRunner {
public:
    explicit TMapReduceRunner(TMapReduceOptions options);

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

    static void GenerateReduceInput(
        const std::vector<std::pair<std::string, std::string>>& data, uint64_t file_num);
};