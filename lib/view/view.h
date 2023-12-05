#pragma once

#include "nlohmann/json.hpp"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <map>

enum Input {
    NEXT,
    PREV,
    PLUS,
    MINUS,
    ESC
};

class WeatherView {
public:
    WeatherView() = default;

    std::string GetWindDirection(int degrees);
    std::vector<ftxui::Element> MakeInfo(const nlohmann::json& json, const std::string& day, const std::string& time_of_day);
    void DisplayForecast(const nlohmann::json& json);
    void DisplayRetry();
    void DisplayNoCity(const std::string& city);

    bool CheckInput();
    Input ReadInput();

private:
    std::map<int, std::vector<std::string>> pictures = {
            {0, {
                    "    \\   /      Clear sky    ",
                    "     .-.       ",
                    "  ― (   ) ―    ",
                    "     `-’       ",
                    "    /   \\      "
            }},
            {1, {
                    "   \\  /        Mainly clear ",
                    " _ /\"\".-.      ",
                    "   \\_(   ).    ",
                    "   /(___(__)   ",
                    "               "
            }},
            {2, {
                    "   \\  /        Partly cloudy",
                    " _ /\"\".-.      ",
                    "   \\_(   ).    ",
                    "   /(___(__)   ",
                    "               "
            }},
            {3, {
                    "   \\  /        Overcast     ",
                    " _ /\"\".-.      ",
                    "   \\_(   ).    ",
                    "   /(___(__)   ",
                    "               "
            }},
            {45, {
                    "               Fog          ",
                    " _ - _ - _ -   ",
                    "  _ - _ - _    ",
                    " _ - _ - _ -   ",
                    "               "
            }},
            {48, {
                    "               Rime fog      ",
                    " _ - _ - _ -   ",
                    "  _ - _ - _    ",
                    " _ - _ - _ -   ",
                    "               "
            }},
            {51, {
                    "     .-.       Light drizzle ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    '  '  '    ",
                    "   '  '  '     "
            }},
            {53, {
                    "     .-.       Medium drizzle",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    '  '  '    ",
                    "   '  '  '     "
            }},
            {55, {
                    "     .-.       Dense drizzle ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    '  '  '    ",
                    "   '  '  '     "
            }},
            {56, {
                    "     .-.       Light freezing drizzle",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    *  '  *    ",
                    "   '  *  '     "
            }},
            {57, {
                    "     .-.       Dense freezing drizzle",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    *  '  *    ",
                    "   '  *  '     "
            }},
            {61, {
                    "     .-.       Light rain    ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    \"  \"  \"    ",
                    "   \"  \"  \"     "
            }},
            {63, {
                    "     .-.       Medium rain   ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    \"  \"  \"    ",
                    "   \"  \"  \"     "
            }},
            {65, {
                    "     .-.       Heavy rain    ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    \"  \"  \"    ",
                    "   \"  \"  \"     "
            }},
            {66, {
                    "     .-.      Light freezing rain   ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    *  \"  *    ",
                    "   \"  *  \"     "
            }},
            {67, {
                    "     .-.       Heavy freezing rain   ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    *  \"  *    ",
                    "   \"  *  \"     "
            }},
            {71, {
                    "     .-.       Light snowfall ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    *  *  *    ",
                    "   *  *  *     "
            }},
            {73, {
                    "     .-.       Medium snowfall",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    *  *  *    ",
                    "   *  *  *     "
            }},
            {75, {
                    "     .-.       Heavy snowfall ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    *  *  *    ",
                    "   *  *  *     "
            }},
            {77, {
                    "     .-.       Snow grains    ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    °  °  °    ",
                    "   °  °  °     "
            }},
            {80, {
                    "     .-.       Light showers  ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    \"\"  \"\"     ",
                    "   \"\" \"\" \"\"    "
            }},
            {81, {
                    "     .-.       Medium showers ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    \"\"  \"\"     ",
                    "   \"\" \"\" \"\"    "
            }},
            {82, {
                    "     .-.       Heavy showers  ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    \"\"  \"\"     ",
                    "   \"\" \"\" \"\"    "
            }},
            {85, {
                    "     .-.       Light snow showers ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    **  **     ",
                    "   ** ** **    "
            }},
            {86, {
                    "     .-.       Heavy snow showers ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    **  **     ",
                    "   ** ** **    "
            }},
            {95, {
                    "     .-.       Thunderstorm  ",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    _/ _/      ",
                    "   /  /        "
            }},
            {96, {
                    "     .-.       Thunderstorm with light hail",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    ' _/ '     ",
                    "   ' / '  '    "
            }},
            {99, {
                    "     .-.       Thunderstorm with heavy hail",
                    "    (   ).     ",
                    "   (___(__)    ",
                    "    \" _/ \"     ",
                    "   \" / \"  \"    "
            }},
    };
};
