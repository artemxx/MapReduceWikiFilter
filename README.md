# MapReduceWikiFilter

```
g++ -c commons.cpp -o commons.o
g++ -c sort.cpp -o sort.o
g++ -c mapreduce.cpp -o mapreduce.o
g++ -c runner.cpp -o runner.o
g++ mapreduce.o commons.o runner.o -o mapreduce -lboost_system -lboost_filesystem -lboost_program_options
g++ commons.o sort.o -o sort -lboost_filesystem -lboost_system

chmod +x map.py reduce.py

./mapreduce --type preprocess --urls data/test_wiki_urls.txt
./mapreduce --type map --script ./map.py --words data/test_words.txt
./mapreduce --type sort --script ./sort
./mapreduce --type reduce --script ./reduce.py --dst_file test_result.txt
./mapreduce --type finish
```
