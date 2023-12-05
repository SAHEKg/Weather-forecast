#include "model.h"
#include <fstream>

WeatherModel::WeatherModel(const std::string& config) {
    std::ifstream file(config);
    nlohmann::json config_json = nlohmann::json::parse(file);
    cities_ = config_json["cities"];
    city_number_ = 0;
    forecast_days_ = config_json["forecast_days"].get<int>();
    frequency_seconds_ = config_json["frequency_seconds"].get<int>();
}

const std::string& WeatherModel::City() {
    return cities_[city_number_];
}

int WeatherModel::city_number() {
    return city_number_;
}

void WeatherModel::IncCityNumber() {
    city_number_ = (city_number_ + 1) % cities_.size();
}

void WeatherModel::DecCityNumber() {
    if (city_number_ == 0)
        city_number_ = cities_.size() - 1;
    else
        city_number_ -= 1;
}

const nlohmann::json& WeatherModel::GetWeather() {
    return weather_json_;
}

void WeatherModel::SetWeather(const nlohmann::json& weather) {
    weather_json_ = weather;
}

bool WeatherModel::city_found() {
    return city_found_;
}

void WeatherModel::SetCityFound(bool value) {
    city_found_ = value;
}

int WeatherModel::forecast_days() {
    return forecast_days_;
}

void WeatherModel::IncForecastDays() {
    forecast_days_ += 1;
}

void WeatherModel::DecForecastDays() {
    if (forecast_days_ != 1)
        forecast_days_ -= 1;
}

int WeatherModel::frequency_seconds() {
    return frequency_seconds_;
}
