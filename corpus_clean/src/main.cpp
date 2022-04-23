#include <dirent.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <thread>
#include <vector>
#include <map>
#include <algorithm>

#include <ThreadPool.h>
#include <SentenceFilter.h>
#include <WordsSegment.hpp>
#include <Ini.h>

using namespace std;

struct TaskParam
{
    std::map<std::string, std::string> config;
    SentenceFilter sfilter;
};

void getFileNames(const std::string &filePath,
                  std::vector<std::string> &fileNames, std::string &file_suffix)
{
    struct dirent *ptr;
    DIR *dir = opendir(filePath.c_str());

    while ((ptr = readdir(dir)) != nullptr)
    {
        std::string fname = ptr->d_name;
        if (fname.substr(fname.rfind(".") + 1) == file_suffix)
        {
            fileNames.push_back(filePath + fname);
        }
    }
}

std::vector<std::string> segmentWords(const string &segFunc, std::string &line, WordsSegment &wsegment, SentenceFilter &sfilter)
{
    std::vector<std::string> seg_tokens;
    
    if(segFunc == "jieba")
    {
        seg_tokens = wsegment(line);
    }
    if(segFunc == "space")
    {
        sfilter.segmentDelim(line, ' ', seg_tokens);
    }
    return seg_tokens;
}

void readFile(void *arg, void *arg_exten)
{
    std::string path = *(std::string *)arg;
    TaskParam taskParam = *(TaskParam *)arg_exten;

    std::ifstream fin(path, std::ios::in);
    std::ofstream fout(path + ".filter", std::ios::out);

    if (!fin.is_open())
    {
        std::cout << "Cann't open the file" << std::endl;
    }
    std::string line_tmp;
    WordsSegment wsegment;
    
    while (getline(fin, line_tmp))
    {
        // whether contain the source and target line
        std::vector<std::string> sentences;
        taskParam.sfilter.segmentDelim(line_tmp, '\t', sentences);

        if (sentences.size() >= 2)
        {
            // get source len and source_seg
            std::string src_line;
            std::vector<std::string> src_tokens;
            std::string src_seg = taskParam.config["src_seg_func"];
            src_tokens = segmentWords(src_seg, sentences[0], wsegment, taskParam.sfilter);
            taskParam.sfilter.mergeSegment(src_line, src_tokens);

            // get target len and target_seg
            std::string trg_line;
            std::vector<std::string> trg_tokens;
            std::string trg_seg = taskParam.config["trg_seg_func"];
            trg_tokens = segmentWords(trg_seg, sentences[1], wsegment, taskParam.sfilter);            
            taskParam.sfilter.mergeSegment(trg_line, trg_tokens);

            // cal the same tokens rate
            float rate = taskParam.sfilter.calRepeatRate(src_tokens, trg_tokens);

            // lambda func for cal the length rate
            auto lenRate = [](int srcLen, int trgLen) -> float
            { return float(std::max(srcLen, trgLen)) / std::min(srcLen, trgLen); };

            //
            float common_token_rate = std::stof(taskParam.config["common_token_rate"]);
            float len_rate = std::stof(taskParam.config["len_rate"]);
            if (rate <= common_token_rate && lenRate(src_tokens.size(), trg_tokens.size()) <= len_rate)
            {
                fout << src_line << "\t" << trg_line << std::endl;
            }
        }
    }

    fin.close();
    fout.close();
}

int main(int argc, char *argv[])
{
    // WordSegment wsegment;
    std::vector<string> file_paths;
    std::vector<std::thread> vec_thd;

    //
    std::string path = argv[1];
    std::string path_config = argv[2];

    TaskParam taskParam;

    // read ini file
    Ini ini;
    ini.readFile(path_config);
    string len_rate_s = ini.getStr("Preprocess", "len_rate");
    string token_rate_s = ini.getStr("Preprocess", "common_token_rate");
    string src_seg_func = ini.getStr("Segment", "src_seg_func");
    string trg_seg_func = ini.getStr("Segment", "trg_seg_func");

    taskParam.config["len_rate"] = len_rate_s;
    taskParam.config["src_seg_func"] = src_seg_func;
    taskParam.config["trg_seg_func"] = trg_seg_func;
    taskParam.config["common_token_rate"] = token_rate_s;

    std::string file_suffix = ini.getStr("Preprocess", "file_suffix");
    getFileNames(path, file_paths, file_suffix);

    std::string min_thread = ini.getStr("Preprocess", "min_thread");
    std::string max_thread = ini.getStr("Preprocess", "max_thread");
    ThreadPool pool(std::stoi(min_thread), std::stoi(max_thread));
    for (int i = 0; i < int(file_paths.size()); ++i)
    {
        pool.addTask(Task(readFile, &file_paths[i], &taskParam));
    }

    while (pool.m_doneNum != int(file_paths.size()))
    {
        sleep(1);
        std::cout << "waiting......" << pool.m_doneNum << std::endl;
    }
    sleep(5);

    return 0;
}
