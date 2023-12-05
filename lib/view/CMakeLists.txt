add_library(view view.h view.cpp)

target_link_libraries(view PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(view PRIVATE ftxui::screen PRIVATE ftxui::dom)