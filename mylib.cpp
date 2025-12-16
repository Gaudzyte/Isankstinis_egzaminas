#include "mylib.h"

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <regex>

using namespace std;

bool arSkyrybosZenklas(char c)
{
    return ispunct(static_cast<unsigned char>(c));
}

string valytiZodi(const string &zodis)
{
    string normalizuotas = zodis; 

    for (char &c : normalizuotas)
    {
        c = tolower(static_cast<unsigned char>(c));   // Paverčia į mažąsias
    }

    // šalinam skyrybos ženklus
    string valytas;
    for (char c : normalizuotas)
    {
        if (!arSkyrybosZenklas(c)) 
        {
            valytas += c;
        }
    }

    // tikrinam ar yra bent viena raidė
    bool yraRaide = any_of(valytas.begin(), valytas.end(),
                           [](char c)
                           {
                               return isalpha(static_cast<unsigned char>(c)) || (unsigned char)c >= 128;
                           });

    return yraRaide ? valytas : ""; // Jei nėra raidės grąžina tuščią stringą
}

void apdorotiTeksta(
    ifstream &input,
    map<string, int> &zodziuSkaicius,
    map<string, set<int>> &zodziuEilutes,
    set<string> &rastiURL)
{
    regex urlRegex(
        R"((https?:\/\/)?(www\.)?([a-zA-Z0-9-]+\.)+[a-zA-Z]{2,}(\/[\w\d\?\.\=\&\-]*)*)",
        regex::icase);

    string eilute;
    int eiluciuNumeris = 1;

    while (getline(input, eilute)) //skaito failą eilutėmis
    {
        auto urls_begin = sregex_iterator(eilute.begin(), eilute.end(), urlRegex); 
        auto urls_end = sregex_iterator(); 
        for (auto it = urls_begin; it != urls_end; ++it) 
        {
            rastiURL.insert(it->str()); 
        }

        // Žodžių analizė
        istringstream iss(eilute);
        string zodis;
        while (iss >> zodis) 
        {
            string isvalytas = valytiZodi(zodis); //Kviečiama funkcija, kuri:pašalina skyrybos ženklus, paverčia mažosiomis; patikrina, ar liko bent viena raidė
            if (!isvalytas.empty()) 
            {
                zodziuSkaicius[isvalytas]++; 
                zodziuEilutes[isvalytas].insert(eiluciuNumeris); 
            }
        }
        eiluciuNumeris++; 
    }
}

void isvestiZodziuRezultatus(
    ofstream &output,
    const map<string, int> &zodziuSkaicius,
    const map<string, set<int>> &zodziuEilutes)
{
    const int zodzioPlotis = 20;
    const int skaiciausPlotis = 25;
    const int eilutesPlotis = 40;

    output << left
           << setw(zodzioPlotis) << "Zodis"
           << setw(skaiciausPlotis) << "Pasikartojimai"
           << "Eilutes"
           << endl;

    output << string(70, '-') << endl;

    for (const auto &poros : zodziuSkaicius)
    {
        if (poros.second > 1) // Tik pasikartojantys žodžiai
        {
            output << left
                   << setw(zodzioPlotis) << poros.first
                   << setw(skaiciausPlotis) << poros.second;

            for (int ln : zodziuEilutes.at(poros.first)) 
            {
                output << ln << " ";
            }
            output << endl;
        }
    }
}

void isvestiURLRezultatus(
    ofstream &output,
    const set<string> &rastiURL)
{
    output << endl
           << endl;
    output << "URL ADRESAI " << endl;
    output << string(50, '-') << endl;

    for (const auto &url : rastiURL)
    {
        output << url << endl;
    }
}

void analizuotiTeksta(const string &inputPath, const string &zodziuOutputPath,const string &urlOutputPath)
{
    ifstream input(inputPath);
    ofstream zodziuOutput(zodziuOutputPath);
    ofstream urlOutput(urlOutputPath);

    if (!input.is_open())
    {
        cerr << "Nepavyko atidaryti ivesties failo: " << inputPath << "\n";
        return;
    }
    if (!zodziuOutput.is_open()) {
        cerr << "Nepavyko atidaryti zodziu failo: " << zodziuOutputPath << "\n";
        return;
    }
    if (!urlOutput.is_open()) {
        cerr << "Nepavyko atidaryti URL failo: " << urlOutputPath << "\n";
        return;
    }

    map<string, int> zodziuSkaicius;
    map<string, set<int>> zodziuEilutes;
    set<string> rastiURL;

    apdorotiTeksta(input, zodziuSkaicius, zodziuEilutes, rastiURL); 
    isvestiZodziuRezultatus(zodziuOutput, zodziuSkaicius, zodziuEilutes);
    isvestiURLRezultatus(urlOutput, rastiURL); 
}