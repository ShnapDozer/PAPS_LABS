//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//
//// Абстрактный класс Observer определяет метод обновления, который вызывается при изменении субъекта.
//class Observer {
//public:
//    virtual void update() = 0;
//};
//
//// Абстрактный класс Subject определяет методы для добавления и удаления наблюдателей.
//// Также определяет метод уведомления, который вызывается при изменении состояния субъекта.
//class Subject {
//public:
//    virtual void attach(Observer* observer) = 0;
//    virtual void detach(Observer* observer) = 0;
//    virtual void notify() = 0;
//};
//
//// Конкретный класс Subject, который проверяет конфигурационный файл игры на изменения.
//class Config : public Subject {
//private:
//    std::string filename;
//    std::vector<Observer*> observers;
//    std::string lastConfig;
//
//public:
//    Config(std::string filename) : filename(filename) {
//        lastConfig = readConfig();
//    }
//
//    // Метод, который проверяет конфигурационный файл на изменения и уведомляет наблюдателей.
//    void checkChanges() {
//        std::string newConfig = readConfig();
//        if (newConfig != lastConfig) {
//            lastConfig = newConfig;
//            notify();
//        }
//    }
//
//    // Метод, который читает конфигурационный файл.
//    std::string readConfig() {
//        std::ifstream file(filename);
//        std::string config;
//        if (file.is_open()) {
//            std::string line;
//            while (std::getline(file, line)) {
//                config += line;
//            }
//            file.close();
//        }
//        else {
//            std::cout << "Error: cannot open file" << std::endl;
//        }
//        return config;
//    }
//
//    // Методы для добавления и удаления наблюдателей.
//    void attach(Observer* observer) override {
//        observers.push_back(observer);
//    }
//
//    void detach(Observer* observer) override {
//        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
//    }
//
//    // Метод уведомления наблюдателей.
//    void notify() override {
//        for (Observer* observer : observers) {
//            observer->update();
//        }
//    }
//};
//
//// Конкретный класс Observer, который реагирует на изменения конфигурационного файла.
//class Game : public Observer {
//public:
//    void update() override {
//        std::cout << "Config file has been changed. Updating the game." << std::endl;
//    }
//};
//
//int main() {
//    Config config("C:\\Users\\HellNout\\Desktop\\LABS\\PAPS_LABS\\lab6\\x64\\Debug\\game.cfg");
//    Game game;
//
//    config.attach(&game);
//
//    while (true) {
//        config.checkChanges();
//        // Здесь можно поместить любую другую логику игры.
//    }
//
//    return 0;
//}

#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>

void print_help(const std::string program_name)
{
    using namespace std;
    cout << "Usage: " << program_name << " [input file] [output file]\n\n";
    cout << "Convert CYBERcobra program file into $readmemh acceptable file.\n";
    cout << "CYBERcobra program file may contain only comments (starting with \"//\"),\n";
    cout << "whitespaces and binary digits '0' or '1'.\n";
    cout << "This program will erase this parts from every line and then convert\n";
    cout << "32-bits binary strings into 4 little endian 8-bit strings in hex-format.\n\n";
    cout << "If output file omitted, the <input_file_base>_converted.<input_file_ext>\n";
    cout << "will be produced.\n\n";
    cout << "If input file omitted, program.txt will be used.\n\n";
    cout << "Example:\n\n";
    cout << program_name << "  open \"program.txt\" and produce \"program_converted.txt\"\n";
    cout << program_name << " test.txt  open \"test.txt\" and produce \"test_converted.txt\"\n";
    cout << program_name << " test.txt myname.dat  open \"test.txt\" and produce \"myname.dat\"\n";

}

int main(int argc, char** argv)
{
    using namespace std;
    /*
      Parse argument list and print help message if needed
    */
    string ifname;
    string ofname;
    string start;
    string end;
    string filename = argv[0];
    size_t dot_pos;
    filename = filename.substr(filename.find_last_of("/\\") + 1);
    switch (argc)
    {
    case 1:
        ifname = "program.txt";
        ofname = "program_converted.txt";
        break;
    case 2:
        if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))
        {
            print_help(filename);
            return 0;
        }
        ifname = argv[1];
        dot_pos = ifname.find(".");
        if (dot_pos != string::npos)
        {
            start = ifname.substr(0, dot_pos);
            end = ifname.substr(dot_pos, ifname.size() - dot_pos);
            ofname = start + "_converted" + end;
        }
        else
        {
            ofname = ifname + "_converted";
        }
        break;
    case 3:
        ifname = argv[1];
        ofname = argv[2];
        break;
    default:
        print_help(filename);
        return 0;
    }


    /*
      Program body
    */
    // Open input and output files
    ifstream ifs(ifname);
    if (!ifs)
    {
        cerr << "Unable to open file: \"" << ifname << "\"" << endl;
        return -1;
    }
    ofstream ofs(ofname);

    if (!ofs.is_open())
    {
        cerr << "Unable to open file: \"" << ofname << "\"" << endl;
        return -1;
    }
    string str;
    size_t line_counter = 0;
    while (getline(ifs, str))
    {
        line_counter++;
        // trim line from comments and whitespaces, skip empty lines after trimming
        auto comment_pos = str.find("//");
        if (comment_pos != string::npos)
        {
            str.erase(comment_pos);
        }
        str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
        if (!str.size())
        {
            continue;
        }
        if (str.size() != 32)
        {
            cerr << "line " << line_counter << " length is not equal 32 after trimming comments and whitespaces" << endl;
            return -2;
        }
        // split 32-bits binary line into 4 little-endian hex lines and write them into file
        for (size_t i = 0; i < 4; i++)
        {
            // For every 8-bit part of 32-bit line get int representation.
            // If illegal character found, throw error.
            size_t valid_char_num;
            string byte_substr = str.substr(8 * (3 - i), 8);
            int cur_byte = std::stoi(byte_substr, &valid_char_num, 2);
            if (valid_char_num != 8)
            {
                cerr << "Illegal character '" << byte_substr.at(valid_char_num) <<
                    "' found in line " << line_counter << ": \"" << str << "\"\n";
                cerr << "Should be only '0' or '1'." << endl;
                return -3;
            }
            char hex_byte_str[3];
            // convert int representation into hex string
            snprintf(hex_byte_str, 3, "%02x", cur_byte);
            ofs << hex_byte_str << "\n";
        }
    }
    ifs.close();
    ofs.close();

    return 0;
}