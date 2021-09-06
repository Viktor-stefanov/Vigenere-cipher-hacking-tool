#include "detectEnglish.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

double getEnWordsPercent(std::string &text, std::vector<std::string> &dictionary)
{
	int enWords = 0;
	int textWords = 0;

	std::string word;
	std::istringstream iss(text);
	while (std::getline(iss, word, ' '))
	{
		word.erase(std::remove_if(word.begin(), word.end(), std::ispunct));
		if (std::find(dictionary.begin(), dictionary.end(), word) != dictionary.end())
		{
			++enWords;
		}
		++textWords;
	}

	return  static_cast<double>(enWords) / textWords;
}

double getEnLettersPercent(std::string& text)
{
	int enLetters = 0;
	int letters = 0;
	for (int i = 0; i < text.size(); ++i)
	{
		if (std::isalpha(text[i]))
			++enLetters;
		else if (std::isspace(text[i]) || std::ispunct(text[i]))
			continue;
		++letters;
	}

	return enLetters / static_cast<double>( letters );
}

bool isEnglish(std::string &text, std::vector<std::string> &dictionary)
{
	/* check if at least wordsPercentage percent of the words
	   in the text are english and if at least lettersPercentage percent
	   of the letters in the text are english ( latin ) */
	constexpr int wordsPercentage = 45;
	constexpr int lettersPercentage = 75;
	bool wordsAreEnglish = (getEnWordsPercent(text, dictionary) * 100) >= wordsPercentage;	
	bool lettersAreEnglish = (getEnLettersPercent(text) * 100) >= lettersPercentage;

	return wordsAreEnglish && lettersAreEnglish;
}