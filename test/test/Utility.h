/******************************************************************************
* 
* @file      Utility.h
* @brief     文字列配列分割
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
#ifndef UTILITY_H_
#define UTILITY_H_
//インクルード部
#include <vector>
#include <string>
#include <list>

void Split(char split_char, char* buffer, std::vector<std::string>& out);
void Split(char split_char, char* buffer, std::list<std::string>& out);
void Replace(char search_char, char replace_char, char* buffer);

#endif
