#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <thread>
#include <algorithm>
#include <vector>

// struct for parsing document
struct DocumentItem {
	size_t position;
	size_t count;
	std::vector<std::string> catalogs;
};

// struct for parsing kit
struct KitItem {
	std::string catalog;
	size_t required;
};

// struct for make result
struct Result {
	size_t position;
	size_t count;
	std::string catalog;
};

class KitManager {
	// variables for algorithm
	std::vector<DocumentItem> documentItems;
	std::vector<KitItem> kitItems;
	std::vector<Result> result;

public:
	void solution(const std::string& documentFileName, const std::string& kitFileName, const std::string& outputFileName);
	void readDocument(const std::string& documentFileName);
	void readKit(const std::string& kitFileName);
	void outputResult(const std::string& outputFileName, bool isSolved);
};
