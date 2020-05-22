#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <utility>

#include "commons.h"

static constexpr auto MapFileDirectoty = "map_files";
static constexpr auto MapOutputPattern =  "map_files/map_output_";

using std::cout;
using std::ifstream;
using std::string;
using std::vector;

struct State {
    explicit State(string text_, std::shared_ptr<ifstream> stream_)
        : text(std::move(text_)), stream(std::move(stream_)) {
    }

    string text;
    std::shared_ptr<ifstream> stream;

    bool operator<(const State& other) const {
        return text > other.text;
    }
};

using MinHeap = std::priority_queue<State>;
using VectorPtr = vector<std::shared_ptr<ifstream>>;

VectorPtr GetInputStreams() {
    const auto files = GetDirFilesStartsWith(MapOutputPattern, MapFileDirectoty);
    VectorPtr result;
    result.reserve(files.size());
    for (const auto& file : files) {
        result.push_back(std::make_shared<ifstream>(file));
    }

    return result;
}

int main() {
    auto streams = GetInputStreams();
    MinHeap heap;
    for (auto& stream : streams) {
        string text;
        if (std::getline(*stream, text)) {
            heap.emplace(text, stream);
        }
    }

    while (!heap.empty()) {
        auto state = heap.top();
        heap.pop();
        cout << state.text << "\n";

        if (std::getline(*state.stream, state.text)) {
            heap.push(state);
        }
    }
}
