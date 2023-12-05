add_executable(main main.cpp)

target_link_libraries(main PRIVATE controller)
target_link_libraries(main PRIVATE cpr::cpr)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(main PRIVATE ftxui::screen PRIVATE ftxui::dom )
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR})
