#include "view.h"

std::string WeatherView::GetWindDirection(int degrees) {
    if (degrees > 338 || degrees < 22)
        return "N ";
    if (degrees > 21 && degrees < 69)
        return "NE";
    if (degrees > 68 && degrees < 112)
        return "E ";
    if (degrees > 111 && degrees < 159)
        return "SE";
    if (degrees > 158 && degrees < 202)
        return "S ";
    if (degrees > 201 && degrees < 249)
        return "SW";
    if (degrees > 248 && degrees < 292)
        return "W ";
    if (degrees > 291 && degrees < 339)
        return "NW";
}

std::vector<ftxui::Element> WeatherView::MakeInfo(const nlohmann::json &json, const std::string& day,
                                                         const std::string &time_of_day) {
    std::vector<std::string> wind_directions = {"N ", "NE", "E ", "SE", "S ", "SW", "W ", "NW"};

    int weather_code = json[day][time_of_day]["weathercode"].get<int>();
    std::string max_temp;
    if (json[day][time_of_day]["max_temp"].get<int>() > 0) {
        max_temp += "+";
    }
    max_temp += std::to_string(json[day][time_of_day]["max_temp"].get<int>());
    std::string min_temp = std::to_string(json[day][time_of_day]["min_temp"].get<int>());
    int wind_direction = json[day][time_of_day]["wind_direction"].get<int>();
    std::string max_wind_speed = std::to_string(json[day][time_of_day]["max_wind_speed"].get<int>());
    std::string min_wind_speed = std::to_string(json[day][time_of_day]["min_wind_speed"].get<int>());
    std::string visibility = std::to_string(json[day][time_of_day]["visibility"].get<int>());
    std::string precipitation = std::to_string(json[day][time_of_day]["precipitation"].get<float>());
    precipitation = precipitation.substr(0, 4);
    std::string precipitation_probability = std::to_string(json[day][time_of_day]["precipitation_probability"].get<int>());
    precipitation_probability = precipitation_probability.substr(0, 4);

    using namespace ftxui;
    std::vector<Element> result = {
            text(pictures[weather_code][0]),
            text(pictures[weather_code][1] + max_temp + "(" + min_temp + ") °C"),
            text(pictures[weather_code][2] + GetWindDirection(wind_direction) + " " + min_wind_speed + "-" + max_wind_speed + " km/h"),
            text(pictures[weather_code][3] + visibility + " km"),
            text(pictures[weather_code][4] + precipitation + "mm | " + precipitation_probability + "%")
    };

    return result;
}

void WeatherView::DisplayForecast(const nlohmann::json& json) {
    system("CLS");
    using namespace ftxui;
    std::cout << "Weather forecast: " << json["city"].get<std::string>() << std::endl;
    std::cout << std::endl;

    std::string current_day = json["current_weather"]["date"];
    std::string period = json["current_weather"]["time"];
    auto day_info = vbox(MakeInfo(json, current_day, period));
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(day_info));
    Render(screen, day_info);
    screen.Print();

    std::cout << std::endl << std::endl;
    for (auto& elem : json.items()) {
        if (elem.key() == "city" || elem.key() == "current_weather")
            continue;
        std::string day = elem.key();
        day_info = window(text(elem.key()) | bold | hcenter,
                           hbox({
                                        vbox({
                                            text("Morning") | hcenter | flex,
                                            separator(),
                                            vbox(MakeInfo(json, day, "morning"))
                                        }) | flex,
                                        separator(),
                                        vbox({
                                            text("Day") | hcenter | flex,
                                            separator(),
                                            vbox(MakeInfo(json, day, "day"))
                                            }) | flex,
                                        separator(),
                                        vbox({
                                            text("Evening") | hcenter | flex,
                                            separator(),
                                            vbox(MakeInfo(json, day, "evening"))
                                            }) | flex,
                                        separator(),
                                        vbox({
                                            text("Night") | hcenter | flex,
                                            separator(),
                                            vbox(MakeInfo(json, day, "night"))
                                            }) | flex
                                })
                           );
        screen = Screen::Create(Dimension::Full(), Dimension::Fit(day_info));
        Render(screen, day_info);
        screen.Print();
    }
}

void WeatherView::DisplayRetry() {
    system("CLS");
    std::cout << "Connection failed, retrying..." << std::endl;
}

void WeatherView::DisplayNoCity(const std::string& city) {
    system("CLS");
    std::cout << "No city " << city << " found" << std::endl;
}

bool WeatherView::CheckInput() {
    return _kbhit();
}

Input WeatherView::ReadInput() {
    int key = _getch();
    if (key == 110)
        return Input::NEXT;
    if (key == 112)
        return Input::PREV;
    if (key == 61)
        return Input::PLUS;
    if (key == 45)
        return Input::MINUS;
    if (key == 27)
        return Input::ESC;
}
