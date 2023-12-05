#include "../lib/controller/controller.h"

int main(int argc, char** argv) {
    WeatherController forecast("C:/Users/user/CLionProjects/labwork-10-SAHEKg/config.json");
    forecast.Run();

    return 0;
}