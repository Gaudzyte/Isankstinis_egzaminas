#ifndef MYLIB_H
#define MYLIB_H

#include <map>
#include <set>
#include <string>

bool arSkyrybosZenklas(char c);

std::string valytiZodi(const std::string &word); // paverčia žodį mažosiomis; pašalina skyrybos ženklus; patikrina, ar liko bent viena raidė

// URL paieška ir žodžių analize
void apdorotiTeksta(
    std::ifstream &input,
    std::map<std::string, int> &zodziuSkaicius,
    std::map<std::string, std::set<int>> &zodziuEilutes,
    std::set<std::string> &rastiURL);

void isvestiZodziuRezultatus(
    std::ofstream &output,
    const std::map<std::string, int> &zodziuSkaicius,
    const std::map<std::string, std::set<int>> &zodziuEilutes);

void isvestiURLRezultatus(
    std::ofstream &output,
    const std::set<std::string> &rastiURL);

void analizuotiTeksta(
    const std::string &inputPath,
    const std::string &zodziuOutputPath,
    const std::string &urlOutputPath);

#endif