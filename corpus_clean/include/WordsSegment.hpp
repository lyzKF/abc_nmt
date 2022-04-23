#include <iostream>
#include <string>
#include <vector>
#include <Jieba.hpp>

const std::string &DictPath = "../third_party/cppjieba/dict/jieba.dict.utf8";
const std::string &ModelPath = "../third_party/cppjieba/dict/hmm_model.utf8";
const std::string &UserDictPath = "../third_party/cppjieba/dict/user.dict.utf8";
const std::string &IdfPath = "../third_party/cppjieba/dict/idf.utf8";
const std::string &StopWordsPath = "../third_party/cppjieba/dict/stop_words.utf8";

class WordsSegment
{
public:
    WordsSegment() : _jieba(DictPath, ModelPath, UserDictPath, IdfPath, StopWordsPath)
    {
        //std::cout << "Initialize jieba......" << std::endl;
    }

    //
    std::vector<std::string> operator()(const std::string &line)
    {
        std::vector<std::string> words;
        try
        {
            _jieba.Cut(line, words);
        }
        catch (std::bad_exception)
        {
            std::cout << "fail to segment the words......" << std::endl;
        }
        return words;
    }

public:
    cppjieba::Jieba _jieba;
};
