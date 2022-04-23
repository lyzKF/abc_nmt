#pragma once

#include <map>
#include <iostream>
#include <string>

using namespace std;

enum IniRes
{
    Ini_success,
    Ini_error,
    Ini_openfile_error
};

typedef std::map<std::string, std::string> KeyMap;
typedef std::map<std::string, KeyMap> MainKeyMap;

class Ini
{
public:
    Ini();
    ~Ini();
    //
    std::string getStr(string mAttr, string cAttr);

    //
    IniRes readFile(string path);

    IniRes getKey(string mAttr, string cAttr, string &value);

    std::string m_szKey;
    MainKeyMap m_keymap;
};
