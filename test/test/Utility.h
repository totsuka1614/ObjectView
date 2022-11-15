#ifndef UTILITY_H_
#define UTILITY_H_

#include <vector>
#include <string>
#include <list>

void Split(char split_char, char* buffer, std::vector<std::string>& out);
void Split(char split_char, char* buffer, std::list<std::string>& out);
void Replace(char search_char, char replace_char, char* buffer);

#endif
