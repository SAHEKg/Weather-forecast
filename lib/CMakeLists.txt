#add_library(weather controller/controller.h controller/controller.cpp)
#
#target_link_libraries(weather PRIVATE cpr::cpr)
#target_link_libraries(weather PRIVATE nlohmann_json::nlohmann_json)

link_directories(controller)

add_subdirectory(controller)
add_subdirectory(model)
add_subdirectory(view)
