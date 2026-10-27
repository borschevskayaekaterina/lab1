#include <iostream>
#include <clocale>
#include <fstream>
#include <map>
#include <vector>
#include <chrono>
#include <ctime>
#include "Pipe.h"
#include "CompressorStation.h"
#include "Utils.h"
#include "Manager.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    time_t now = time(nullptr);
    char buf[80];
    strftime(buf, sizeof(buf), "log_%Y-%m-%d_%H-%M-%S.txt", localtime(&now));
    string logFilename = buf;

    ofstream logfile(logFilename, ios::app);

    redirect_output_wrapper cerr_out(cerr);
    if (logfile) {
        cerr_out.redirect(logfile);
    }

    cerr << "=== ЗАПУСК ПРОГРАММЫ ===\n";

    map<int, Pipe> pipe;
    map<int, CompressorStation> compressorStation;

    while (true) {
        cout << "\nМЕНЮ\n"
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
        case 1: {
            Pipe newPipe;
            newPipe.input();
            pipe.emplace(newPipe.getId(), newPipe);
            cout << "Труба добавлена (ID " << newPipe.getId() << ")\n";
            cerr << "Добавлена труба ID: " << newPipe.getId() << "\n";
            break;
        }
        case 2: {
            CompressorStation newCs;
            newCs.input();
            compressorStation.emplace(newCs.getId(), newCs);
            cout << "КС добавлена (ID " << newCs.getId() << ")\n";
            cerr << "Добавлена КС ID: " << newCs.getId() << "\n";
            break;
        }
        case 3: {
            if (pipe.empty() && compressorStation.empty()) {
                cout << "Пока ничего не введено\n";
            }
            else {
                for (const auto& pair : pipe) pair.second.print();
                for (const auto& pair : compressorStation) pair.second.print();
            }
            break;
        }
        case 4: {
            int type = readInt("С чем работаем? (1 - Трубы, 2 - КС): ");
            if (type == 1) {
                managePipes(pipe);
            }
            else if (type == 2) {
                manageStations(compressorStation);
            }
            else cout << "Неверный выбор.\n";
            break;
        }
        case 5: {
            controlSingleStation(compressorStation);
            break;
        }
        case 6: {
            cout << "Введите имя файла для сохранения: ";
            string filename;
            getline(cin, filename);
            ofstream file(filename);
            if (!file.is_open()) { cout << "Ошибка открытия файла\n"; break; }

            file << pipe.size() << "\n";
            for (auto& pair : pipe) pair.second.saveToFile(file);
            file << compressorStation.size() << "\n";
            for (auto& pair : compressorStation) pair.second.saveToFile(file);

            file.close();
            cout << "Данные сохранены\n";
            cerr << "Данные сохранены в файл: " << filename << "\n";
            break;
        }
        case 7: {
            cout << "Введите имя файла для загрузки: ";
            string filename;
            getline(cin, filename);
            ifstream file(filename);
            if (!file.is_open()) { cout << "Файл не найден\n"; break; }

            int pipeCount, csCount;
            if (!(file >> pipeCount)) { cout << "Ошибка: файл поврежден!\n"; break; }
            file.ignore(10000, '\n');

            pipe.clear();
            compressorStation.clear();
            bool isBroken = false;

            for (int i = 0; i < pipeCount; ++i) {
                Pipe p;
                if (!p.loadFromFile(file)) { isBroken = true; break; }
                pipe.emplace(p.getId(), p);
            }
            if (isBroken) { cout << "Ошибка чтения труб!\n"; pipe.clear(); break; }

            if (!(file >> csCount)) { cout << "Ошибка: файл поврежден!\n"; pipe.clear(); break; }
            file.ignore(10000, '\n');

            for (int i = 0; i < csCount; ++i) {
                CompressorStation cs;
                if (!cs.loadFromFile(file)) { isBroken = true; break; }
                compressorStation.emplace(cs.getId(), cs);
            }
            if (isBroken) {
                cout << "Ошибка чтения КС!\n";
                pipe.clear();
                compressorStation.clear();
                break;
            }

            file.close();
            cout << "Загружено труб: " << pipe.size() << ", КС: " << compressorStation.size() << "\n";
            cerr << "Загружены данные из файла: " << filename << "\n";
            break;
        }
        case 0:
            cout << "Выход\n";
            cerr << "=== ЗАВЕРШЕНИЕ РАБОТЫ ===\n";
            return 0;
        default:
            cout << "Неверный пункт меню!\n";
        }
    }
    return 0;
}