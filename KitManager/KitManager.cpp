#include "KitManager.h"

void KitManager::solution(const std::string& documentFileName, const std::string& kitFileName, const std::string& outputFileName)
{
	// without multithread
	//this->readDocument(documentFileName);
	//this->readKit(kitFileName);

	// make some parallelism
	std::thread docThread(&KitManager::readDocument, this, documentFileName);
	std::thread kitThread(&KitManager::readKit, this, kitFileName);

	// catch threads
	docThread.join();
	kitThread.join();

	// delete position without catalogs
	documentItems.erase(std::remove_if(documentItems.begin(), documentItems.end(), 
		[](const DocumentItem& item){ return item.catalogs.empty(); }), documentItems.end());


	// get available counts per catalog in document
	std::unordered_map<std::string, size_t> catalogsAvailable;
	for (const auto& item : documentItems) {
		for (const auto& catalog : item.catalogs) {
			catalogsAvailable[catalog] += item.count;
		}
	}

	// vector for update catalogs count state in document
	std::vector<size_t> remainingDocumentCatalogs;
	for (const auto& item : documentItems) {
		remainingDocumentCatalogs.push_back(item.count);
	}

	// unordered map for update kits remaining state
	std::unordered_map<std::string, size_t> remainingNeededKit;
	for (const auto& item : kitItems) {
		remainingNeededKit[item.catalog] = item.required;
	}

	// make priority list by count of catalogs
	std::vector<std::pair<std::string, size_t>> priorityList;
	for (const auto& [catalog, count] : catalogsAvailable) {
		priorityList.push_back({ catalog, count });
	}
	// sorting to make priority by counts
	std::sort(priorityList.begin(), priorityList.end(), [](const auto& a, const auto& b) { return a.second < b.second; });


	/////// MAIN SECTION OF ALOGRITHM
	bool solvedFlag = true;

	// corner case: check situation then kit didnt contains in document
	for (const auto& item : kitItems) {
		if (catalogsAvailable[item.catalog] < item.required) {
			//std::cout << "Cant reach full kit!" << std::endl;
			solvedFlag = false;
			break;
		}
	}

	// algorithm for building kit
	for (const auto& [catalog, count] : priorityList) { // iteration in priority vector to close deficit positions
		if (remainingNeededKit.count(catalog) == 0) continue;

		for (size_t i = 0; i < documentItems.size(); ++i) { 
			if (remainingDocumentCatalogs[i] == 0) continue;

			// searching in document required catalog
			if (std::find(documentItems[i].catalogs.begin(), documentItems[i].catalogs.end(), catalog) != documentItems[i].catalogs.end()) {
				size_t takedCount = std::min(remainingNeededKit[catalog], remainingDocumentCatalogs[i]); // take min value to optimize our choose for other position
				result.push_back({ documentItems[i].position, takedCount, catalog }); 
				remainingDocumentCatalogs[i] -= takedCount; // decrease our stock
				remainingNeededKit[catalog] -= takedCount; // decrease our needed

				if (remainingNeededKit[catalog] == 0) break; //stop searching catalogs if we are reached target
			}
		}

		// if we cant close our needs - so we cant build this kit from document
		if (remainingNeededKit[catalog] > 0) {
			//std::cout << "Cant reach full kit!" << std::endl;
			solvedFlag = false; // didnt solved
			return;
		}
	}

	this->outputResult(outputFileName, solvedFlag);
}


void KitManager::readDocument(const std::string& documentFileName) {
	std::ifstream document(documentFileName);

	if (!document.is_open()) {
		std::cout << "Failed to open " << documentFileName;
		exit(0);
	}

	// document parser
	std::string currentLine;
	std::getline(document, currentLine); // 1st line header skip
	while (std::getline(document, currentLine)) {
		if (currentLine.empty()) continue;
		std::istringstream sstream(currentLine);

		size_t pos, count;
		sstream >> pos >> count; // get pos and counts of item

		// parsing catalogs part
		std::string catalog;
		std::string catalogsLine;
		std::vector<std::string> catalogs;
		std::getline(sstream, catalogsLine);

		std::istringstream catalogStream(catalogsLine);
		while (std::getline(catalogStream, catalog, ',')) {
			// delete chars: , \t " and space
			catalog.erase(std::remove(catalog.begin(), catalog.end(), ' '), catalog.end());
			catalog.erase(std::remove(catalog.begin(), catalog.end(), '\t'), catalog.end());
			catalog.erase(std::remove(catalog.begin(), catalog.end(), '\"'), catalog.end());

			if (!catalog.empty()) {
				catalogs.push_back(catalog);
			}
		}

		// write results	
		documentItems.push_back({ pos, count, catalogs });
	}
	
	/*
	std::cout << "Pos" << "\tCount" << "\tCatalogs" << std::endl;
	for (size_t i = 0; i < documentItems.size(); ++i) {
		std::cout  << i+1 << "\t" << documentItems[i].count << "\t";
		for (const auto& catalog : documentItems[i].catalogs) {
			std::cout << catalog << " ";
		}
		std::cout << std::endl;
	}
	*/
}

void KitManager::readKit(const std::string& kitFileName)
{
	std::ifstream kit(kitFileName);
	if (!kit.is_open()) {
		std::cout << "\nFailed to open " << kitFileName;
		exit(0);
	}

	// kit parser
	std::string currentLine;
	std::getline(kit, currentLine); // 1st line header skip
	while (std::getline(kit, currentLine)) {
		if (currentLine.empty()) continue;
		std::istringstream sstream(currentLine);

		std::string catalog;
		size_t count;
		sstream >> catalog >> count; // get catalog and counts of item

		// delete "" from catalog names
		catalog.erase(std::remove(catalog.begin(), catalog.end(), '\"'), catalog.end());

		// write results
		kitItems.push_back({ catalog, count });
	}

	/*
	std::cout << "\nCatalog\tRequired\n";
	for (const auto& [catalog, count] : kitItems) {
		std::cout << catalog << "\t" << count << std::endl;
	}
	*/
}

void KitManager::outputResult(const std::string& outputFileName, bool isSolved)
{
	std::cout << "Check result in output.txt";
	std::ofstream out(outputFileName);
	if (isSolved) {
		std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {return a.position < b.position;});

		out << "Kit contains in document!\nComposition:\n";
		out << "Pos\tCount\tCatalog\n";
		for (const auto& res : result) {
			out << res.position << "\t" << res.count << "\t" << res.catalog << std::endl;
		}
	}
	else {
		out << "Document didnt contains kit";
	}

}

