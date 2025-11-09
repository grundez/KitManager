add_executable (KitManager 
	"KitManager.cpp" "KitManager.h"
	"main.cpp")

target_compile_definitions(KitManager PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET KitManager PROPERTY CXX_STANDARD 20)
endif()