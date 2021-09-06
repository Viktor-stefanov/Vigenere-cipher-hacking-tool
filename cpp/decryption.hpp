#pragma once
#include <string>

void decrypt(std::string& message, const std::string& key = "", 
	const bool& punctuation = false, const bool& reversed = false, const bool& dictAtack = false);