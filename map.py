#!/usr/bin/env python3

import sys
from collections import defaultdict
import requests
from bs4 import BeautifulSoup
from nltk import word_tokenize
from enum import IntEnum


def get_parsed_text(text):
    return [word for word in word_tokenize(text.lower()) if len(word) > 3]


def extract_text(link):
    req = requests.get(link)
    soup = BeautifulSoup(req.text, 'lxml')
    for tag in soup.find_all('p'):
        if len(tag.text) > 1:
            return get_parsed_text(tag.text)
    return []


class MapReduceArgs(IntEnum):
    WORDS_PATH = 1
    # SRC_PATH = 2
    # DST_PATH = 3


words_path = sys.argv[MapReduceArgs.WORDS_PATH]

words_file = open(words_path, 'r')
search_words = set(words_file.read().strip().split('\t\n'))

result = defaultdict(list)
for link in sys.stdin.read().strip().split('\t\n'):
    words = set(extract_text(link))
    for word in search_words:
        if word in words:
            result[word].append(link)


for k, v in sorted(result.items()):
    print('{}\t'.format(k) + '#'.join(v) + '\t\n', end='')
