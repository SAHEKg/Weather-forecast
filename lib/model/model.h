#pragma once

#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>

class WeatherModel {
public:
    WeatherModel() = default;
    WeatherModel(const std::string& config);

    const std::string& City();

    int city_number();
    void IncCityNumber();
    void DecCityNumber();

    const nlohmann::json& GetWeather();
    void SetWeather(const nlohmann::json& weather);

    bool city_found();
    void SetCityFound(bool value);

    int forecast_days();
    void IncForecastDays();
    void DecForecastDays();

    int frequency_seconds();

private:
    std::vector<std::string> cities_;
    int city_number_;
    bool city_found_;
    int forecast_days_;
    int frequency_seconds_;
    nlohmann::json weather_json_;
};
