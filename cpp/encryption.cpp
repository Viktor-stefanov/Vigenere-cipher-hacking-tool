#include "encryption.hpp"
#include <iostream>
#include <string>
#include <random>

void encrypt(std::string message, std::string key, bool punctuation, bool reversed)
{
	// if the user has not specified a key for encryption generate a pseudo-random one
	if (key.size() == 0)
	{
		std::random_device rd;
		std::default_random_engine eng(rd());
		std::uniform_int_distribution<int> distr(97, 122);

		int keyLength = (distr(eng) % 10) + 5;
		for (int i = 0; i < keyLength; ++i)
		{
			key += distr(eng);
		}
	}

	int keyIndex = 0;
	for (int i = 0; i < message.length(); ++i)
	{
		int charValue = std::tolower(message[i]) - 97;
		if (charValue >= 0 && charValue < 26)
		{
			int keyValue = key[keyIndex % key.length()] - 97;
			if (reversed)
				std::cout << char( (charValue - keyValue >= 0) ? (charValue - keyValue) + 97 : (charValue - keyValue) + 26 + 97 );
			else
				std::cout << char ( (charValue + keyValue) % 26 + 97 );
			++keyIndex;
		}
		else if (punctuation)
		{
			std::cout << message[keyIndex];
			++keyIndex;
		}
	}

	std::cout << "\n";
}
