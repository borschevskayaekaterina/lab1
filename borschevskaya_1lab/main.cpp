#include <iostream>
#include <clocale>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <ctime>
#include <windows.h>
#include "Pipe.h"
#include "CompressorStation.h"
#include "Utils.h"
#include "Manager.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    time_t now = time(nullptr);
    char buf[80];
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(buf, sizeof(buf), "log_%Y-%m-%d_%H-%M-%S.txt", &timeinfo);
    string logFilename = buf;

    ofstream logfile(logFilename, ios::app);

    redirect_output_wrapper cerr_out(cerr);
    if (logfile) {
        cerr_out.redirect(logfile);
    }

    cerr << "=== Запуск программы ===\n";

    Manager manager;

    while (true) {
        cout << "\nМеню\n"
            << "1. Добавить трубу\n"
            << "2. Добавить КС\n"
            << "3. Просмотр всех объектов\n"
            << "4. Пакетный поиск и управление объектами\n"
            << "5. Запуск/остановка цеха на конкретной КС\n"
            << "6. Сохранить в файл\n"
            << "7. Загрузить из файла\n"
            << "0. Выход\n\n";

        int choice = readInt("Ваш выбор: ");
        cerr << "Выбран пункт меню: " << choice << "\n";

        switch (choice) {
        case 1: manager.addPipe(); break;
        case 2: manager.addStation(); break;
        case 3: manager.viewAll(); break;
        case 4: {
            int type = readInt("С чем работаем? (1 - Трубы, 2 - КС): ");
            if (type == 1) manager.managePipes();
            else if (type == 2) manager.manageStations();
            else cout << "Неверный выбор.\n";
            break;
        }
        case 5: manager.controlSingleStation(); break;
        case 6: {
            cout << "Введите имя файла для сохранения: ";
            string filename;
            getline(cin, filename);
            manager.saveToFile(filename);
            break;
        }
        case 7: {
            cout << "Введите имя файла для загрузки: ";
            string filename;
            getline(cin, filename);
            manager.loadFromFile(filename);
            break;
        }
        case 0:
            cout << "Выход\n";
            cerr << "=== Завершение работы! ===\n";
            return 0;
        default:
            cout << "Неверный пункт меню!\n";
        }
    }
    return 0;
}