#include <iostream>
#include <memory>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <stdexcept>


class TemperatureData {
public:
    float temperature;
    TemperatureData(float t ) : temperature(t) {}
};


class LMSensor {
private:
    int fd;   // simulated sensor fd

public:
    explicit LMSensor(const std::string& filePath) {
        fd = open(filePath.c_str(), O_RDONLY);
        if (fd < 0) {
            std::cerr << "Failed to open sensor file" << std::endl;
        }
    }

    ~LMSensor() {
        if (fd >= 0) {
            close(fd);
        }
    }

    float readHumidity() {
        char buf[32] = {0};
        while (true) {
            int n = read(fd, buf, sizeof(buf) - 1);
            if (n > 0) {
                return std::stof(buf);
            }
            lseek(fd, 0, SEEK_SET);
        }
    }
};


class Display {
public:
    void displayTemperature(std::shared_ptr<TemperatureData> data) {
        std::cout << "Current temperature: " << data->temperature << std::endl;
    }
};


class Logger {
private:
    int fd;

public:
    Logger() {
        fd = open("temperature.log",O_WRONLY | O_CREAT | O_APPEND,0644);
        if (fd < 0) {
            std::cerr << "Failed to open log file" << std::endl;
        }
    }

    ~Logger() {
        if (fd >= 0) {
            close(fd);
        }
    }

    void logTemperature(const std::shared_ptr<TemperatureData>& data) {
        std::string line = std::to_string(data->temperature) + "\n";
        write(fd, line.c_str(), line.size());
    }
};

class MainProgram {
private:
    std::unique_ptr<LMSensor> sensor;
    std::shared_ptr<TemperatureData> temperatureData;
    Display display;
    Logger logger;

public:
    explicit MainProgram(const std::string& sensorFile) {
        sensor = std::make_unique<LMSensor>(sensorFile);
        temperatureData = std::make_shared<TemperatureData>(0.0f);
    }

    void run() {
        while (true) {
            temperatureData->temperature = sensor->readHumidity();

            display.displayTemperature(temperatureData);
            logger.logTemperature(temperatureData);

            sleep(60); 
        }
    }
};


int main() {

    MainProgram app("sensor.txt");
    app.run();

    return 0;
}