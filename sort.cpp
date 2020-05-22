#include <bits/stdc++.h>

#include <utility>

#include "commons.h"

using namespace std;

static constexpr auto MapFileDirectoty = "map_files";
static constexpr auto MapOutputPattern =  "map_files/map_output_";

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
using VectorPtr = vector<shared_ptr<ifstream>>;

VectorPtr GetInputStreams() {
    const auto files = GetDirFilesStartsWith(MapOutputPattern, MapFileDirectoty);
    VectorPtr result;
    result.reserve(files.size());
    for (const auto& file : files) {
        result.push_back(make_shared<ifstream>(file));
    }

    return result;
}

int main() {
    auto streams = GetInputStreams();
    MinHeap heap;
    for (auto& stream : streams) {
        string text;
        if (getline(*stream, text)) {
            heap.emplace(text, stream);
        }
    }

    while (!heap.empty()) {
        auto state = heap.top();
        heap.pop();
        cout << state.text << "\n";

        if (getline(*state.stream, state.text)) {
            heap.push(state);
        }
    }
}
