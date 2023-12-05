add_library(model model.h model.cpp)

target_link_libraries(model PRIVATE nlohmann_json::nlohmann_json)