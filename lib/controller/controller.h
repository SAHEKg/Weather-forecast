#pragma once

#include "../model/model.h"
#include "../view/view.h"
#include <cpr/cpr.h>

class WeatherController {
public:
    WeatherController(const std::string& config);
    WeatherController(WeatherView* view, WeatherModel* model);

    ~WeatherController();

    void DisplayWeather();
    std::pair<std::string, std::string> GetCityCoords(const std::string& name);
    nlohmann::json ConvertJson(const nlohmann::json& old_json);
    bool RequestWeather();
    void ReloadWeather();

    void NextCity();
    void PrevCity();

    void MoreForecastDays();
    void LessForecastDays();

    bool ParseInput(Input input);

    void Run();

private:
    WeatherModel* model_;
    WeatherView* view_;
};
