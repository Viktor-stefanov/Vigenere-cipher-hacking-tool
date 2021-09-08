#include "util.hpp"
#include "decryption.hpp"
#include "encryption.hpp"
#include <iostream>
#include <string>
//#include <Windows.h>
//#include <debugapi.h>

int main(int argc, char* argv[])
{
	bool encryption = false;
	bool decryption = false;
	bool reversed = false;
	bool punctuation = false;
	bool dictionaryAtack = false;
	for (int i = 1; i < argc; ++i)
	{
		std::string argument = argv[i];
		if (argument.substr(0, 2).find_first_of('e') != std::string::npos)
			encryption = true;
		else if (argument.substr(0, 2).find_first_of('r') != std::string::npos)
			reversed = true;
		else if (argument.substr(0, 2).find_first_of('p') != std::string::npos)
			punctuation = true;
		else if (argument.substr(0, 3).find("da") != std::string::npos)
			dictionaryAtack = true;
		else if (argument.substr(0, 2).find_first_of('d') != std::string::npos)
			decryption = true;
	}

	//if (!IsDebuggerPresent())
	//{
	//	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
	//	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	//}

	if (encryption)
	{
		std::string message = getUserInput("Enter the message for encryption: ");
		std::string key = getUserInput("Enter a key to use in encryption: ");
		encrypt(message, key, punctuation, reversed);
	}

	if (decryption)
	{
		std::string ciphertext = getUserInput("Enter the message for decryption: ");
		std::string key = getUserInput("If the key is known enter it: ");
		decrypt(ciphertext, key, punctuation, reversed, dictionaryAtack);
	}
}