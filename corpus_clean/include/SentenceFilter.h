#pragma once
#include <iostream>
#include <string>
#include <vector>

class SentenceFilter
{
public:
    SentenceFilter();
    ~SentenceFilter();

    // split the sentence based on the delim
    void segmentDelim(std::string &line, const char delim, std::vector<std::string> &tokens);

    // cal the rate of same tokens in source and target.
    float calRepeatRate(std::vector<std::string> &src_tokens, std::vector<std::string> &trg_tokens);

    // merge segment words and remove redundancy space
    void mergeSegment(std::string &line, std::vector<std::string> &tokens);
};