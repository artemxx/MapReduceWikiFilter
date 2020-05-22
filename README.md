# MapReduceWikiFilter

The task is as follows: having the url of the *wikipedia* articles, for the given words, determine in which articles they are mentioned.

For testing this, there are two data sets in the data folder, they are not very large, because if I make a lot of requests to *https://www.wikipedia.org*, i get error *too many requests* and performing requests takes many time. 

First of all, you need to preprocess data, using **preprocess** mode, it takes the path to tab-separated urls and then split it into parts. Then, using **map** mode, in each part, we concurrently run our **map** script, that work as follows:

- for each *url* we make a request and get the html-code of the page
- extract the text of the article (first *\<p\>* tag) from it
- perform lemmatization of text using *nltk* - a suite of libraries and programs for symbolic and statistical natural language processing for English written in the *Python* programming language
- for each word we can easily find in which articles it is mentioned

Now our task is to combine all the output data of the **map** stage and sort them. This is done in **sort** mode, that takes one argument - the path to script. In this task, we assume that data may be very large for in-memory sort, and it was decided to use external sorting (*sort.cpp*). This works as follows:

- we need to sort the outputs from **map** stage and then combine them
- merge these *k* sorted files using a standard algorithm with *priority queue*
- we read input in parts, at each stage we read the next line from some file and put it in our queue
- therefore, at each moment of time, we store no more than *O(k * |word|)* characters

Now we need to combine results for each word. This task is performed by **reduce** mode, it just runs a script that merge articles for each word.

It the end, to remove all temporary files, use **finish** mode, whick just removes folders for temporary files created on **map** and **reduce** stages.

A possible set of commands for performing all operations may look like this:

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
