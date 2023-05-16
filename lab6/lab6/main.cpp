#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// ����������� ����� Observer ���������� ����� ����������, ������� ���������� ��� ��������� ��������.
class Observer {
public:
    virtual void update() = 0;
};

// ����������� ����� Subject ���������� ������ ��� ���������� � �������� ������������.
// ����� ���������� ����� �����������, ������� ���������� ��� ��������� ��������� ��������.
class Subject {
public:
    virtual void attach(Observer* observer) = 0;
    virtual void detach(Observer* observer) = 0;
    virtual void notify() = 0;
};

// ���������� ����� Subject, ������� ��������� ���������������� ���� ���� �� ���������.
class Config : public Subject {
private:
    std::string filename;
    std::vector<Observer*> observers;
    std::string lastConfig;

public:
    Config(std::string filename) : filename(filename) {
        lastConfig = readConfig();
    }

    // �����, ������� ��������� ���������������� ���� �� ��������� � ���������� ������������.
    void checkChanges() {
        std::string newConfig = readConfig();
        if (newConfig != lastConfig) {
            lastConfig = newConfig;
            notify();
        }
    }

    // �����, ������� ������ ���������������� ����.
    std::string readConfig() {
        std::ifstream file(filename);
        std::string config;
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                config += line;
            }
            file.close();
        }
        else {
            std::cout << "Error: cannot open file" << std::endl;
        }
        return config;
    }

    // ������ ��� ���������� � �������� ������������.
    void attach(Observer* observer) override {
        observers.push_back(observer);
    }

    void detach(Observer* observer) override {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // ����� ����������� ������������.
    void notify() override {
        for (Observer* observer : observers) {
            observer->update();
        }
    }
};

// ���������� ����� Observer, ������� ��������� �� ��������� ����������������� �����.
class Game : public Observer {
public:
    void update() override {
        std::cout << "Config file has been changed. Updating the game." << std::endl;
    }
};

int main() {
    Config config("C:\\Users\\ReWoodPC\\Desktop\\LABS\\PAPS_LABS\\lab6\\x64\\Debug\\game.cfg");
    Game game;

    config.attach(&game);

    while (true) {
        config.checkChanges();
        // ����� ����� ��������� ����� ������ ������ ����.
    }

    return 0;
}