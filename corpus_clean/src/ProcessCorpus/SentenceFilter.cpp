#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <exception>

#include <SentenceFilter.h>

SentenceFilter::SentenceFilter()
{
    //
}
SentenceFilter::~SentenceFilter()
{
    //
}

void SentenceFilter::segmentDelim(std::string &line, const char delim, std::vector<std::string> &tokens)
{
    std::istringstream iss(line);
    std::string tmp;

    while (std::getline(iss, tmp, delim))
    {
        if (tmp != "")
        {
            tokens.push_back(std::move(tmp));
        }
    }
}

float SentenceFilter::calRepeatRate(std::vector<std::string> &src_tokens, std::vector<std::string> &trg_tokens)
{
    float cnt = 0;
    for (auto token : src_tokens)
    {
        if (std::find(trg_tokens.begin(), trg_tokens.end(), token) != trg_tokens.end())
        {
            cnt++;
        }
    }
    return float(cnt) / std::min(src_tokens.size(), trg_tokens.size());
}

void SentenceFilter::mergeSegment(std::string &line, std::vector<std::string> &tokens)
{
    std::string pre_word = tokens[0];
    for (auto word : tokens)
    {
        if (pre_word != " ")
        {
            line = line + word + " ";
        }
        pre_word = word;
    }
    // 去除末尾空格
    line.erase(line.find_last_not_of(" ") + 1);
}