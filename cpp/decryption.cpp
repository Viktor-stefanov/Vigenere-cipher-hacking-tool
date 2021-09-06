#include "frequencyAnalysis.hpp"
#include "detectEnglish.hpp"
#include "decryption.hpp"
#include "util.hpp"
#include <conio.h>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <array>

std::string keyDecrypt(const std::string& ciphertext, const std::string& key, const bool& punctuation, const bool& reversed)
{
	std::string decipheredText;
	for (int i = 0; i < ciphertext.length(); ++i)
	{
		int charValue = std::tolower(ciphertext[i]) - 97;
		if (charValue >= 0 && charValue < 26)
		{
			int keyValue = key[i % key.length()] - 97;
			if (reversed)
				decipheredText.append(1, static_cast<char>((charValue + keyValue) % 26 + 97));
			else
				decipheredText.append(1, static_cast<char>((charValue - keyValue) >= 0 ? (charValue - keyValue) + 97 : (charValue - keyValue) + 26 + 97));
		}
		else if (punctuation)
			decipheredText.append(1, ciphertext[i]);
	}

	return decipheredText;
}

void dictionaryAtack(const std::string& ciphertext, const bool& punctuation, const bool& reversed)
{
	std::vector<std::string> dictionary = loadDictionary();
	for (std::string& possibleKey : dictionary)
	{
		std::string decipheredMsg = keyDecrypt(ciphertext, possibleKey, punctuation, reversed);
		if (isEnglish(decipheredMsg, dictionary))
		{
			std::cout << "Possible decryption with key - " << possibleKey << ". If this is the message press ENTER, otherwise press any other key : " << "\n";
			std::cout << decipheredMsg << "\n";

			char input = _getch();
			if (std::isspace(input))
				return;
		}
	}
}

void decrypt(std::string& ciphertext, const std::string& key, const bool& punctuation, const bool& reversed, const bool& dictAtack)
{
	if (key.size() == 0)
	{
		std::string decryptedMessage = keyDecrypt(ciphertext, key, punctuation, reversed);
		std::cout << decryptedMessage << "\n";
	}
	else if (dictAtack)
	{
		std::cout << "attempting dictionary atack" << "\n";
		dictionaryAtack(ciphertext, punctuation, reversed);
	}
	else
	{
		/* kasiski examination method steps. Goal - make an educated guess about the length of the key and then brute force it
		   1. Find repeated sequences (n-graphs) in the ciphertext of length 3, 4 and 5 and save
			  the distance between the occurences
		   2. Based on the distance of the occurences we can infer the length of the key. The assumption is that if we see
		      a repeated sequence that means that the same part of the message has been encrypted with the same part of the key.
			  So the length of the key must be a factor of the distance between the occurences.
		   3. We calculate the most frequent factors of n-graphs and guess that the key length must be the biggest of the 
		      x ( a random number we choose ) most frequent factors.
		   4. Option(1) Brute force the keys with a cartesian-product strategy. i.e. try all possible letter combinations of the
		      current factor length [bad strategy]
		   5. Option(2) Separate the ciphertext into length-of-key columns and do frequncy analysis on each one.
		      Treat the problem like separate caesar cipher's. (this works on longer ciphertexts, because on shorter one+s
			  frequncy analysis won't be effective) */
		std::transform(ciphertext.begin(), ciphertext.end(), ciphertext.begin(), std::tolower);	// transform the ciphertext to lowercase to ease work with it
		std::cout << "finding the character difference between repeated sequences in the ciphertext..." << "\n";
		std::unordered_map<std::string, int> repeatedNGraphs = getRepeatedNGraphs(ciphertext);
		std::cout << "getting the most frequent factors of the character differences" << "\n";
		std::array<int, 5> mostFrequentFactors = getMostFrequentFactors(repeatedNGraphs);
		// split the text into mostFrequentFactors[i] columns
		for (int i = 0; i < mostFrequentFactors.size(); ++i)
		{
			int factor = mostFrequentFactors[i];	// for readability
			std::cout << "assuming key size of " << factor << " and performing frequency analysis" << "\n";
			for (int j = 0; j < factor; ++j)
			{
				std::string column = splitText(ciphertext.substr(j, std::string::npos), factor);
				// before doing frequency analysis I have to 'clean' the string of any punctuation
				if (punctuation)
				{
					;
				}
				std::string bestMatch = frequencyAnalysis(column);		// get the column, whose letter frequency is the closest to english
				replaceNthCharacters(ciphertext, bestMatch, factor, j);	// and replace every factor-th character in the ciphertext with the column's
			}
			std::cout << ciphertext << "\n";
			std::cout << "If this is the original message press ENTER, otherwise press any other key to try a new key length: ";
			char userInput = _getch();
			if (std::isspace(userInput))
				return;
		}
	}
}