#define ARDUINOJSON_ENABLE_STD_STRING 1
#include "ArduinoJson-v6.21.2.h"
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>
#include <conio.h>
#include <algorithm>
#include <vector>

using namespace std;

struct HighScoreEntry {
    std::string playerName; // Struktura przechowująca nazwę gracza i wynik
    int score;
};

bool compareByScore(const HighScoreEntry& a, const HighScoreEntry& b) {
    return a.score > b.score; // Funkcja porównująca wpisy wyników
}

void saveHighScore(const std::string& playerName, int score)
{
    std::ofstream file("highscores.json", std::ios::app); // Otwarcie pliku do zapisu
    if (file.good())
    {
        StaticJsonDocument<1024> doc;
        doc["PlayerName"] = playerName; // Tworzenie dokumentu JSON z nazwą gracza i wynikiem
        doc["Score"] = score;

        serializeJson(doc, file); // Serializacja dokumentu do pliku
        file << '\n';

        file.close();
    }
    else
    {
        std::cout << "Blad podczas zapisu do pliku highscores.json\n";
    }
}

void showHighScores()
{
    std::ifstream file("highscores.json");
    if (file.good())
    {
        std::vector<HighScoreEntry> highScores; // Wektor przechowujący wpisy wyników

        std::string line;
        while (std::getline(file, line))
        {
            StaticJsonDocument<1024> doc;
            DeserializationError error = deserializeJson(doc, line); // Deserializacja linii do dokumentu JSON
            if (!error)
            {
                std::string playerName = doc["PlayerName"].as<std::string>(); // Odczytanie nazwy gracza i wyniku
                int score = doc["Score"].as<int>();
                highScores.push_back({ playerName, score }); // Dodanie wpisu do wektora
            }
            else
            {
                std::cout << "Blad podczas odczytu danych z pliku highscores.json\n";
                break;
            }
        }

        file.close();

        // Sortowanie wyników od najwyższego do najmniejszego
        std::sort(highScores.begin(), highScores.end(), compareByScore);

        // Wyświetlanie posortowanych wyników
        for (const auto& entry : highScores) {
            std::cout << "Gracz: " << entry.playerName << ", Wynik: " << entry.score << '\n'; // Wyświetlenie posortowanych wpisów
        }
    }
    else
    {
        std::cout << "Blad podczas otwierania pliku highscores.json\n";
    }
}

void showOptions(StaticJsonDocument<1024>& doc)
{
    //Wyprintowanie zawartości dokumentu
    for (JsonPair p : doc.as<JsonObject>())
    {
        cout << p.key() << " " << p.value() << '\n'; // Wyświetlenie klucza i wartości w dokumencie
    }
}

void createDefaultConfig()
{
    std::ofstream file("settings.json"); // Otwarcie pliku do zapisu
    if (file.good())
    {
        StaticJsonDocument<1024> doc;
        doc["Resolution"] = "1920x1080";
        doc["Fullscreen"] = true;
        doc["RayTracing"] = false;

        serializeJson(doc, file); // Serializacja dokumentu do pliku
        file.close();
    }
    else
    {
        std::cout << "Blad podczas tworzenia pliku settings.json\n";
    }
}

void updateConfig(const std::string& key, const std::string& value)
{
    std::ifstream readFile("settings.json");
    if (readFile.good())
    {
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, readFile); // Deserializacja zawartości pliku do dokumentu JSON
        readFile.close();

        if (!error)
        {
            doc[key] = value;

            std::ofstream writeFile("settings.json"); // Otwarcie pliku do zapisu
            if (writeFile.good())
            {
                serializeJson(doc, writeFile); // Serializacja dokumentu do pliku
                writeFile.close();
                std::cout << "Pomyslnie zaktualizowano ustawienie " << key << '\n';
            }
            else
            {
                std::cout << "Blad podczas zapisu do pliku settings.json\n";
            }
        }
        else
        {
            std::cout << "Blad podczas odczytu danych z pliku settings.json\n";
        }
    }
    else
    {
        std::cout << "Blad podczas otwierania pliku settings.json\n";
    }
}


void graj();
void instrukcja();
void ustawienia();
void highScores();

int main()
{
    // Tworzenie domyślnego pliku konfiguracyjnego, jeśli nie istnieje
    std::ifstream configFile("settings.json");
    if (!configFile.good())
    {
        createDefaultConfig();
    }
    configFile.close();

    int wybor;

    cout << "\nMENU GLOWNE\n";
    cout << "1. NOWA GRA\n";
    cout << "2. INSTRUKCJA\n";
    cout << "3. USTAWIENIA\n";
    cout << "4. HIGH SCORES\n";
    cout << "5. WYJDZ\n\n";
    cout << "Wybierz opcje (wpisz numer): ";
    cin >> wybor;

    switch (wybor)
    {
    case 1:
        graj();
        break;
    case 2:
        instrukcja();
        break;
    case 3:
        ustawienia();
        break;
    case 4:
        highScores();
        break;
    case 5:
        return 0;
    default:
        cout << "Niepoprawny wybor. Sprobuj jeszcze raz.\n";
        break;
    }

    return 0;
}

void graj()
{
    const int czasGry = 10; // Czas gry w sekundach
    int wynik = 0;          // Wynik gracza (liczba kliknięć)

    cout << "\nNacisnij dowolny klawisz, aby rozpoczac...\n";
    _getch(); // Oczekiwanie na naciśnięcie dowolnego klawisza

    time_t czasStart = time(nullptr); // Pobranie aktualnego czasu
    while (difftime(time(nullptr), czasStart) < czasGry)
    {
        if (_kbhit())
        { // Sprawdzanie, czy został naciśnięty klawisz
            int klawisz = _getch();
            if (klawisz == 32)
            { // 32 to kod ASCII dla spacji
                ++wynik;
                switch (wynik % 3)
                { // Wybór różnych napisów w zależności od wyniku
                case 0:
                    cout << "Szybciej\n";
                    break;
                case 1:
                    cout << "Jeszcze szybciej\n";
                    break;
                case 2:
                    cout << "Nie poddawaj sie!!\n";
                    break;
                }
            }
        }
    }

    std::cout << "\nGRATULACJE! Twoj wynik to " << wynik << "!\n";

    std::string playerName;
    std::cout << "Wpisz swoje imie: ";
    std::cin >> playerName;

    saveHighScore(playerName, wynik);

    int wybor;
    do
    {
        std::cout << "\n1. Wroc do menu glownego\n";
        std::cout << "Wybierz opcje (wpisz numer): ";
        std::cin >> wybor;

        if (wybor != 1)
        {
            std::cout << "Niepoprawny wybor. Sprobuj jeszcze raz.\n";
        }
    } while (wybor != 1);

    main();
}

void instrukcja()
{
    cout << "\nINSTRUKCJA\n";
    cout << "Twoim zadaniem jest jak najszybsze klikanie spacji.\n";
    cout << "Masz 10 sekund na zdobycie jak najwiekszej liczby punktow.\n";
    cout << "Kazde klikniecie spacji zwieksza twoj wynik.\n";
    cout << "Powodzenia!\n";

    int wybor;
    do
    {
        cout << "1. Wroc do menu glownego\n";
        cout << "Wybierz opcje (wpisz numer): ";
        cin >> wybor;

        if (wybor != 1)
        {
            cout << "Niepoprawny wybor. Sprobuj jeszcze raz.\n";
        }
    } while (wybor != 1);

    main(); // Wywołanie ponownego menu głównego
}

void ustawienia()
{
    cout << "\nUSTAWIENIA\n";

    int wybor;
    do
    {
        cout << "1. Zmien rozdzielczosc\n";
        cout << "2. Zmien tryb pelnoekranowy\n";
        cout << "3. Zmien opcje Ray Tracing\n";
        cout << "4. Powrot do menu glownego\n";
        cout << "Wybierz opcje (wpisz numer): ";
        cin >> wybor;

        switch (wybor)
        {
        case 1:
        {
            std::string rozdzielczosc;
            cout << "Dostepne rozdzielczosci:\n";
            cout << "1. 1920x1080\n";
            cout << "2. 3840x2160\n";
            cout << "3. 2560x1440\n";
            cout << "Wybierz rozdzielczosc (wpisz numer): ";
            cin >> wybor;

            switch (wybor)
            {
            case 1:
                rozdzielczosc = "1920x1080";
                break;
            case 2:
                rozdzielczosc = "3840x2160";
                break;
            case 3:
                rozdzielczosc = "2560x1440";
                break;
            default:
                cout << "Niepoprawny wybor rozdzielczosci. Powrot do menu glownego.\n";
                main();
                return;
            }

            updateConfig("Resolution", rozdzielczosc);
            break;
        }
        case 2:
        {
            std::string trybPelnoekranowy;
            cout << "Pełny ekran:\n";
            cout << "1. Tak\n";
            cout << "2. Nie\n";
            cout << "Wybierz tryb (wpisz numer): ";
            cin >> wybor;

            switch (wybor)
            {
            case 1:
                trybPelnoekranowy = "true";
                break;
            case 2:
                trybPelnoekranowy = "false";
                break;
            default:
                cout << "Niepoprawny wybor trybu pelnoekranowego. Powrot do menu glownego.\n";
                main();
                return;
            }

            updateConfig("Fullscreen", trybPelnoekranowy);
            break;
        }
        case 3:
        {
            std::string rayTracing;
            cout << "Ray Tracing:\n";
            cout << "1. Wlaczony\n";
            cout << "2. Wylaczony\n";
            cout << "Wybierz opcje (wpisz numer): ";
            cin >> wybor;

            switch (wybor)
            {
            case 1:
                rayTracing = "true";
                break;
            case 2:
                rayTracing = "false";
                break;
            default:
                cout << "Niepoprawny wybor opcji Ray Tracing. Powrot do menu glownego.\n";
                main();
                return;
            }

            updateConfig("RayTracing", rayTracing);
            break;
        }
        case 4:
            main(); // Powrót do menu głównego
            return;
        default:
            cout << "Niepoprawny wybor. Sprobuj jeszcze raz.\n";
            break;
        }
    } while (true);
}

void highScores()
{
    cout << "\nHIGH SCORES\n";

    showHighScores();

    int wybor;
    do
    {
        cout << "\n1. Wroc do menu glownego\n";
        cout << "Wybierz opcje (wpisz numer): ";
        cin >> wybor;

        if (wybor != 1)
        {
            cout << "Niepoprawny wybor. Sprobuj jeszcze raz.\n";
        }
    } while (wybor != 1);

    main(); // Wywołanie ponownego menu głównego
}
