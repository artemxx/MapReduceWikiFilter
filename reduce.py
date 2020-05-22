#!/usr/bin/env python3

import sys

word, result = '', []
for line in sys.stdin.read().strip().split('\t\n'):
    words = line.split('\t')
    assert len(words) == 2
    if not word:
        word = words[0]
    else:
        assert word == words[0]
    result.append(words[1])
print(word + '\t' + '#'.join(result) + '\t\n', end='')