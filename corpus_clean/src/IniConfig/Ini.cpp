#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <Ini.h>
using namespace std;

Ini::Ini()
{
    //
}

Ini::~Ini()
{
    //
    m_keymap.clear();
}

IniRes Ini::getKey(string mAttr, string cAttr, string &value)
{
    KeyMap mKey = m_keymap[mAttr];
    value = mKey[cAttr];
    return Ini_success;
}

std::string Ini::getStr(string mAttr, string cAttr)
{
    std::string result = "";
    if (Ini_success == getKey(mAttr, cAttr, m_szKey))
    {
        result = m_szKey;
    }
    return result;
}

IniRes Ini::readFile(string path)
{
    std::string szLine, szMainKey, szLastMainKey, szSubKey;

    KeyMap mLastMap;
    size_t indexPos = -1;
    size_t leftPos = -1;
    size_t rightPos = -1;
    //
    std::ifstream fin(path, std::ios::in);

    if (!fin.is_open())
    {
        std::cout << "Cann't open the file" << std::endl;
        return Ini_openfile_error;
    }
    //

    while (getline(fin, szLine))
    {
        leftPos = szLine.find("[");
        rightPos = szLine.find("]");
        if (leftPos != std::string::npos && rightPos != std::string::npos)
        {
            szLine.erase(leftPos, 1);
            rightPos--;
            szLine.erase(rightPos, 1);

            mLastMap.clear();
            szLastMainKey = szLine;
        }
        else if (indexPos = szLine.find("="), indexPos != std::string::npos)
        {
            std::string szSubKey, szSubValue;
            szSubKey = szLine.substr(0, indexPos);
            szSubValue = szLine.substr(indexPos + 1);
            mLastMap[szSubKey] = szSubValue;
        }
        else
        {
            m_keymap[szLastMainKey] = mLastMap;
        }
    }
    m_keymap[szLastMainKey] = mLastMap;
    return Ini_success;
}