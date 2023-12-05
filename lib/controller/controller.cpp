#include "controller.h"
#include <chrono>

WeatherController::WeatherController(const std::string &config) {
    view_ = new WeatherView;
    model_ = new WeatherModel(config);
}

WeatherController::WeatherController(WeatherView* view, WeatherModel* model)
    : view_(view)
    , model_(model)
{}

WeatherController::~WeatherController() {
    delete view_;
    delete model_;
}

void WeatherController::DisplayWeather() {
    if ((*model_).city_found())
        (*view_).DisplayForecast((*model_).GetWeather());
    else
        (*view_).DisplayNoCity((*model_).City());
}

std::pair<std::string, std::string> WeatherController::GetCityCoords(const std::string& name) {
    cpr::Response api_ninjas = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                                        cpr::Header{{"X-Api-Key", "Qh05nPXydaXi2dFusxa5+w==ppONmgtZLpRhPtu3"}},
                                        cpr::Parameters{{"name", name}});
    if (api_ninjas.text == "[]") {
        return std::make_pair("", "");
    }

    nlohmann::json info = nlohmann::json::parse(api_ninjas.text.substr(1, api_ninjas.text.size()-2));
    std::string latitude = std::to_string(info["latitude"].get<float>());
    std::string longitude = std::to_string(info["longitude"].get<float>());
    std::pair<std::string, std::string> result = std::make_pair(latitude, longitude);

    return result;
}

nlohmann::json WeatherController::ConvertJson(const nlohmann::json& old_json) {
    std::string city = (*model_).City();
    int forecast_days = (*model_).forecast_days();
    nlohmann::json weather_json;
    weather_json["city"] = city;

    int weather_type;
    int min_temp = 1000000;
    int max_temp = -1000000;
    int cur_temp;
    int wind_direction;
    int min_wind_speed = 1000000;
    int max_wind_speed = -1000000;
    int cur_wind_speed;
    float precipitation = 0;
    int precipitation_probability = 0;
    int visibility;
    std::vector<std::string> times_of_day = {"morning", "day", "evening", "night"};

    std::string current_date = old_json["current_weather"]["time"];
    int current_time = std::stoi(current_date.substr(11, 2));
    current_date = current_date.substr(5, 5);
    weather_json["current_weather"]["date"] = current_date;
    weather_json["current_weather"]["time"] = times_of_day[current_time / 6];

    const int kTimesOfDay = 4;
    const int kHoursInTimeOfDay = 6;
    const int kDayHours = kTimesOfDay * kHoursInTimeOfDay;
    const int kKilometer = 1000;
    for (int days = 0; days < forecast_days; ++days) {
        std::string date = old_json["hourly"]["time"][days * kDayHours];
        date = date.substr(5, 5);
        for (int time = 0; time < kTimesOfDay; ++time) {
            for (int hour = 0; hour < kHoursInTimeOfDay; ++hour) {
                int index = days * kDayHours + time * kHoursInTimeOfDay + hour;

                cur_temp = int(old_json["hourly"]["temperature_2m"][index].get<float>());
                cur_wind_speed = int(old_json["hourly"]["windspeed_10m"][index].get<float>());
                max_temp = std::max(max_temp, cur_temp);
                min_temp = std::min(min_temp, cur_temp);
                max_wind_speed = std::max(max_wind_speed, cur_wind_speed);
                min_wind_speed = std::min(min_wind_speed, cur_wind_speed);
                precipitation += old_json["hourly"]["precipitation"][index].get<float>();
                precipitation_probability += old_json["hourly"]["precipitation_probability"][index].get<int>();
                visibility += int(old_json["hourly"]["visibility"][index].get<float>());

                if (hour == 3) {
                    wind_direction = old_json["hourly"]["winddirection_10m"][index].get<int>();
                    weather_type = old_json["hourly"]["weathercode"][index].get<int>();
                }
            }
            precipitation_probability = precipitation_probability / kHoursInTimeOfDay;
            visibility = visibility / kHoursInTimeOfDay / kKilometer;
            weather_json[date][times_of_day[time]] = {
                    {"weathercode", weather_type},
                    {"min_temp", min_temp},
                    {"max_temp", max_temp},
                    {"wind_direction",wind_direction},
                    {"min_wind_speed", min_wind_speed},
                    {"max_wind_speed", max_wind_speed},
                    {"precipitation", precipitation},
                    {"precipitation_probability", precipitation_probability},
                    {"visibility", visibility}
            };
            min_temp = 1000000;
            max_temp = -1000000;
            min_wind_speed = 1000000;
            max_wind_speed = -1000000;
            precipitation = 0;
            precipitation_probability = 0;
        }
    }

    return weather_json;
}

bool WeatherController::RequestWeather() {
    std::string city = (*model_).City();
    std::pair<std::string, std::string> coords = GetCityCoords(city);
    if (coords.first.empty() && coords.second.empty()) {
        return false;
    }
    std::string latitude = coords.first;
    std::string longitude = coords.second;
    int forecast_days = (*model_).forecast_days();
    std::string parameters = "temperature_2m,weathercode,visibility,windspeed_10m,winddirection_10m,precipitation_probability,precipitation";
    cpr::Response api_weather = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                                         cpr::Parameters{{"latitude", latitude},
                                                         {"longitude", longitude},
                                                         {"forecast_days", std::to_string(forecast_days)},
                                                         {"hourly", parameters},
                                                         {"current_weather", "true"}});
    while (api_weather.status_code != 200) {
        (*view_).DisplayRetry();
        api_weather = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                               cpr::Parameters{{"latitude", latitude},
                                               {"longitude", longitude},
                                               {"forecast_days", std::to_string(forecast_days)},
                                               {"hourly", parameters},
                                               {"current_weather", "true"}});
    }
    nlohmann::json info_weather = nlohmann::json::parse(api_weather.text);

    nlohmann::json weather_json = ConvertJson(info_weather);

    (*model_).SetWeather(weather_json);
    return true;
}

void WeatherController::ReloadWeather() {
    if (!RequestWeather())
        (*model_).SetCityFound(false);
    else
        (*model_).SetCityFound(true);
}

void WeatherController::NextCity() {
    (*model_).IncCityNumber();
    RequestWeather();
}

void WeatherController::PrevCity() {
    (*model_).DecCityNumber();
    RequestWeather();
}

void WeatherController::MoreForecastDays() {
    (*model_).IncForecastDays();
    RequestWeather();
}

void WeatherController::LessForecastDays() {
    (*model_).DecForecastDays();
    RequestWeather();
}

bool WeatherController::ParseInput(Input input) {
    if (input == Input::NEXT)
        NextCity();
    if (input == Input::PREV)
        PrevCity();
    if (input == Input::PLUS)
        MoreForecastDays();
    if (input == Input::MINUS)
        LessForecastDays();
    if (input == Input::ESC)
        return true;
    return false;
}

void WeatherController::Run() {
    ReloadWeather();
    DisplayWeather();
    auto start = std::chrono::steady_clock::now();
    while (true) {
        while (!(*view_).CheckInput()) {
            if (std::chrono::steady_clock::now() - start > std::chrono::seconds((*model_).frequency_seconds())) {
                ReloadWeather();
                DisplayWeather();
                start = std::chrono::steady_clock::now();
            }
        }
        if (ParseInput((*view_).ReadInput()))
            break;
        ReloadWeather();
        DisplayWeather();
        start = std::chrono::steady_clock::now();
    }
}
