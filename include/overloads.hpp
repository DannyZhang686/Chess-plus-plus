#ifndef _OVERLOADS_HPP_
#define _OVERLOADS_HPP_

#include <map>
#include <vector>
#include "main.hpp"

std::ostream& operator<<(std::ostream&, std::map<std::string, Cell>&);
std::ostream& operator<<(std::ostream&, std::vector<std::string>&);

#endif //_OVERLOADS_HPP_