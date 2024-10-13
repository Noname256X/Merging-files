#include <iostream>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <Windows.h>
#include <iomanip>
#include <chrono>
#include <unordered_set>

using namespace std;

void Merging_files(string& directory_input, string& path_output) {
    namespace fs = boost::filesystem;
    fs::path path_input(directory_input);

    if (!fs::exists(directory_input)) {
        cerr << "Директория не существует!" << endl;
        exit(0);
    }
    else {
        string pass;
        vector<string> fileNames;
        vector<int> Number_of_Lines;
        int count = 0;

        for (fs::directory_iterator it(directory_input); it != fs::directory_iterator(); ++it) {
            fileNames.push_back(it->path().filename().string());
        }

        cout << "Пожалуйста, подождите... Инициализация файлов:" << endl;
        for (const auto& fileName : fileNames) {
            fs::path filePath = fs::path(directory_input) / fileName;

            ifstream file(filePath.string());
            if (!file.is_open()) {
                cerr << "Ошибка открытия файла: " << filePath.string() << endl;
            }
            else {
                count = 0;
                while (getline(file, pass)) {
                    count++;
                    if (count % 100000 == 0) {
                        cout << count << " <- Обрабатываемая строка" << "\r";
                    }
                }
                Number_of_Lines.push_back(count);
                file.close();
            }
            cout << setw(30) << left << fileName << setw(10) << right << Number_of_Lines.back() << " Строк в файле" << endl;
        }

        ofstream Myfile(path_output);
        if (!Myfile.is_open()) {
            cerr << "Ошибка открытия выходного файла!" << endl;
            return;
        }

        cout << "Создание файла: " << endl;
        unordered_set<string> passwordSet;
        auto start = chrono::high_resolution_clock::now();
        for (const auto& fileName : fileNames) {
            cout << "объединение файла: " << fileName << " файл:" << (&fileName - &fileNames[0] + 1) << " из " << fileNames.size() << endl;
            fs::path filePath = fs::path(directory_input) / fileName;

            ifstream file(filePath.string());
            if (!file.is_open()) {
                cerr << "Ошибка открытия файла: " << filePath.string() << endl;
                continue;
            }

            count = 0;
            while (getline(file, pass)) {
                count++;
                pass.erase(remove_if(pass.begin(), pass.end(), ::isspace), pass.end());
                if (pass.size() > 3 && passwordSet.find(pass) == passwordSet.end()) {
                    Myfile << pass << endl;
                    passwordSet.insert(pass);
                }
                if (count % 10000 == 0) {
                    cout << count << " <- Обрабатываемая строка" << "\r";
                }
            }
            file.close();
        }

        cout << endl << "В файл не добавлялись пароли менее 4 символов" << endl;
        Myfile.close();

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(end - start);
        cout << "Время выполнения объединения: " << duration.count() << " сек" << endl;
    }
}

int main() {
    SetConsoleCP(1251);
    setlocale(LC_ALL, "ru");
    SetConsoleOutputCP(1251);

    string directory_input = "the path to the file folder";
    string path_output = "the path to save the processed file";

    Merging_files(directory_input, path_output);
    return 0;
}
