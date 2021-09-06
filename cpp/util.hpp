#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <array>

std::string getUserInput(const std::string& message);
std::vector<std::string> loadDictionary();
std::unordered_map<std::string, int> getRepeatedNGraphs(const std::string& ciphertext);
std::array<int, 5> getMostFrequentFactors(const std::unordered_map<std::string, int>& repeatedNGraphs);
std::string splitText(const std::string& ciphertext, int& step);
void replaceNthCharacters(std::string& text, const std::string& replacement, int step, int start);
void shiftText(std::string& text, int shift);