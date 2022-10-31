﻿#ifndef UTILITY_H_
#define UTILITY_H_

#include <vector>
#include <string>

void Split(char split_char, char* buffer, std::vector<std::string>& out);
void Replace(char search_char, char replace_char, char* buffer);

#endif
