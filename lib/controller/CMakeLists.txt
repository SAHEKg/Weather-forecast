add_library(controller controller.h controller.cpp)

target_link_libraries(controller PRIVATE model)
target_link_libraries(controller PRIVATE view)
target_link_libraries(controller PRIVATE cpr::cpr)
target_link_libraries(controller PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(controller PRIVATE ftxui::screen PRIVATE ftxui::dom)