#include "KitManager.h"
#include <filesystem>

int main()
{
	std::filesystem::path data_dir = PROJECT_SOURCE_DIR;
	data_dir /= "data";

	KitManager manager;
	manager.solution(
        (data_dir / "document.txt").string(),
        (data_dir / "kit.txt").string(),
        (data_dir / "output.txt").string()
    );

	return 0;
}
