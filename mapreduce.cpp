#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "runner.h"

namespace po = boost::program_options;

using std::cout;
using std::endl;
using std::string;

void Fail(const string& message) {
    cout << message << endl;
    cout << "Try \'./mapreduce --help\' for more information." << endl;
    exit(1);
}

int main(int num_args, char* arg_values[]) {
    po::options_description desc("General options");
    desc.add_options()
        ("help,h", "produce help message")
        ("type", po::value<string>(), "select preprocess, map, sort, reduce or finish")
        ("script", po::value<string>(), "path to the script to be executed")
        ("urls", po::value<string>(), "path to wikipedia urls")
        ("words", po::value<string>(), "path to words for which search will be performed")
        ("dst_file", po::value<string>(), "path to results");

    po::variables_map vm;
    po::store(po::parse_command_line(num_args, arg_values, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    if (!vm.count("type")) {
        Fail("You must specify the type");
    }

    TMapReduceOptions options;
    options.type = vm["type"].as<string>();

    // Required and optional options are made very ugly in the boost,
    // so let's write a couple of ifs
    if (options.type == "preprocess") {
        if (!vm.count("urls")) {
            Fail("You must specify urls for --type=preprocess");
        }
        options.urls_path = vm["urls"].as<string>();
    } else if (options.type == "map") {
        if (!vm.count("script") || !vm.count("words")) {
            Fail("You must specify script and words for --type=map");
        }
        options.script_path = vm["script"].as<string>();
        options.words_path = vm["words"].as<string>();
    } else if (options.type == "sort") {
        if (!vm.count("script")) {
            Fail("You must specify script for --type=sort");
        }
        options.script_path = vm["script"].as<string>();
    } else if (options.type == "reduce") {
        if (!vm.count("script") || !vm.count("dst_file")) {
            Fail("You must specify script and dst_file for --type=reduce");
        }
        options.script_path = vm["script"].as<string>();
        options.dst_file = vm["dst_file"].as<string>();
    } else if (options.type != "finish") {
        Fail("Unrecognized type: " + options.type);
    }

    TMapReduceRunner runner(options);
    if (options.type == "preprocess") {
        runner.SplitDataOnFiles();
    } else if (options.type == "map") {
        runner.RunMap();
    } else if (options.type == "sort") {
        runner.Sort();
    } else if (options.type == "reduce") {
        runner.RunReduce();
    } else if (options.type == "finish") {
        TMapReduceRunner::RemoveTempFiles();
    } else {
        throw std::runtime_error("unreachable code");
    }

    return 0;
}
