#include "util.hpp"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>

std::string getUserInput(const std::string& message)
{
	std::cout << message << '\n';
	std::string userInput;
	std::getline(std::cin, userInput);

	return userInput;
}

std::vector<std::string> loadDictionary()
{
	std::ifstream file("words\\words.txt");

	std::vector<std::string> dictionary;
	std::string line;
	while (std::getline(file, line))
		dictionary.push_back(line);

	return dictionary;
}

std::string splitText(const std::string& ciphertext, int& step)
{
	std::string splittedText;
	for (int i = 0; i < ciphertext.size(); i += step)
		splittedText.append(1, ciphertext[i]);

	return splittedText;
}

void replaceNthCharacters(std::string& text, const std::string& replacement, int step, int start)
{
	for (int i = 0; (i * step + start) < text.size() && i < replacement.size(); ++i)
	{
		int letterIndex = (i * step) + start;
		text[letterIndex] = replacement[i];
	}
}

void shiftText(std::string& text, int shift)
{
	for (int i = 0; i < text.size(); ++i)
	{
		char shiftedLetter = (text[i] + shift) <= 122 ? (text[i] + shift) : (text[i] + shift) % 123 + 97;
		text[i] = shiftedLetter;
	}
}

void getNGraphPositions(const std::string& ciphertext, std::string& nGraph, std::unordered_map<std::string, int>& repeatedNGraphs)
{
	int firstNGraphPos = ciphertext.find(nGraph);
	if (firstNGraphPos != std::string::npos)
	{
		int secondNGraphPos = ciphertext.find(nGraph, firstNGraphPos + nGraph.size() - 1);
		if (secondNGraphPos != std::string::npos)
			repeatedNGraphs[nGraph] = (secondNGraphPos - firstNGraphPos);
	}
}

std::unordered_map<std::string, int> getRepeatedNGraphs(const std::string& ciphertext)
{
	std::unordered_map<std::string, int> repeatedNGraphs;
	for (int i = 0; i < (ciphertext.size() - 3); ++i)
	{
		std::string threeGraph = ciphertext.substr(i, 3);
		std::string fourGraph = ciphertext.substr(i, 4);
		// if the n-graph is not in the hash map already then check for repetitions of the n-graph and if found add the distance to the map
		if (repeatedNGraphs.find(threeGraph) == repeatedNGraphs.end())
			getNGraphPositions(ciphertext, threeGraph, repeatedNGraphs);
		if (repeatedNGraphs.find(fourGraph) == repeatedNGraphs.end())
			getNGraphPositions(ciphertext, fourGraph, repeatedNGraphs);
	}

	return repeatedNGraphs;
}

void countFactors(const int& charDifference, std::unordered_map<int, int>& factorFrequency)
{
	// check if possibleFactor is a factor of the character difference between the two repeating n-graphs, and if it is increment it's value in the map
	for (int possibleFactor = 2; possibleFactor < 26; ++possibleFactor)
	{
		if (charDifference % possibleFactor == 0)
			++factorFrequency[possibleFactor];
	}
}

std::array<int, 5> getMostFrequentFactors(const std::unordered_map<std::string, int>& repeatedNGraphs)
{
	// initialize the factorFrequency map with factors from 1 to 25 and assign 0 as default value
	std::unordered_map<int, int> factorFrequency;
	for (int factor = 2; factor < 26; ++factor)
		factorFrequency[factor] = 0;
	
	for (const auto& pair : repeatedNGraphs)
		countFactors(pair.second, factorFrequency);
	// get the 5 most frequent factors from the 'counted' factors and sort them in descending order
	std::array<int, 5> mostFrequentFactors;
	for (int i = 0; i < 5; ++i)
	{
		// after finding the current most frequent factor set it to 0 so the next call won't return the same max value pair
		std::pair<int, int> pair = *std::max_element(factorFrequency.begin(), factorFrequency.end(),
													 [](const std::pair<int, int>& p1, const std::pair<int, int>& p2)
													 { return p1.second < p2.second; });
		mostFrequentFactors[i] = pair.first;
		factorFrequency[pair.first] = 0;
	}	
	// sort the most frequent factors in reverse order, because the key length probably isn't 2 or 3, so we start with the bigger key length possibilities
	std::sort(mostFrequentFactors.begin(), mostFrequentFactors.end(), std::greater<>());
	
	return mostFrequentFactors;
}