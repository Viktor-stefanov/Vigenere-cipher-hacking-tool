#include "util.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

std::string frequencyAnalysis(std::string column)
{
	// create the map of english letter frequensies and initialize the map of the current column letter frequensies to 0
	std::map<char, double> enLettersFrequency = { {'a', 8.2}, {'b', 1.5}, {'c', 2.8}, {'d', 4.3}, {'e', 13}, {'f', 2.2}, {'g', 2},
												  {'h', 6.1}, {'i', 7}, {'j', .15}, {'k', .77}, {'l', 4}, {'m', 2.4}, {'n', 6.7},
												  {'o', 7.5}, {'p', 1.9}, {'q', .095}, {'r', 6}, {'s', 6.3}, {'t', 9.1}, {'u', 2.8},
												  {'v', .98}, {'w', 2.4}, {'x', .15}, {'y', 2}, {'z', .074} };
	std::map<char, double> columnLettersFrequency;
	std::pair<int, double> minDifference = { 0, INFINITY };	// store the minimal difference in letter frequnsies and the corresponding shift value in this variable	
	double letterDiff;									// see the proximity of letter frequnesies between the shifted column text and english language. Pick the shift with the highest proximity to english
	for (int shift = 0; shift < 26; ++shift)
	{
		letterDiff = 0;
		// set 0 as default value for the column with the current shift's letter frequensies
		for (char letter = 'a'; letter <= 'z'; ++letter)
			columnLettersFrequency[letter] = 0;
		// first get the letter count and then transform it into a percent, because repeated addition of floating point numbers will cause rounding errors
		for (char letter : column)
		{
			char shiftedLetter = (letter + shift) <= 'z' ? letter + shift : (letter + shift) % 123 + 97;
			columnLettersFrequency[shiftedLetter] += 1;
		}
		// compare the absolute value of the difference in frequencies of the letters in the column with the current shift and the english language then add it to letterDiff
		for (const auto& pair : columnLettersFrequency)
		{
			columnLettersFrequency[pair.first] = (pair.second / column.size()) * 100;
			letterDiff += std::abs(columnLettersFrequency[pair.first] - enLettersFrequency[pair.first]);
		}
		// compare if the column with the current shift has 'closer' letter frequency to english than the current 'closest' if so make it the new closest 
		if (letterDiff < minDifference.second)
		{
			minDifference.first = shift;
			minDifference.second = letterDiff;
		}
	}
	shiftText(column, minDifference.first);	// 'move' the text to the right with the shift value of the closest to english shift

	return column;
}