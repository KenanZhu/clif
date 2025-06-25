/**
    \file funcs.hpp
    \brief functions declaration and implementation, which are used as an
           example for "CLIF".
 */

#include <string>
#include <vector>
#include <iostream>

/**
    \note :
    The functions below are solely for CLIF argument validation purposes.
    They contain minimal stubs for parameter parsing and are not
    full implementations.
 */

/// req, receive 'std::string', 'std::string', 'int'
void req(std::string type, std::string host, int port)
{
    std::cout << "Request from : '"
            + type + "://"
            + host + ":"
            + std::to_string(port)
            + "'."
            << std::endl;
}

/// map, receive 'std::map<std::string, int>'.
void map(std::map<std::string, int> m)
{
    std::cout << "Map size = " 
            + std::to_string((int)m.size())
            << std::endl;

    for (const auto &it : m)
    {
        std::cout << "Key = " + it.first
                + ", Value = " + std::to_string(it.second)
                << std::endl;
    }
}

/// matrix, receive 'std::vector<std::vector<int>>'.
void matrix(std::vector<std::vector<int>> mat)
{
    for (const auto &row : mat) {
        for (const auto &col : row) {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    }
}