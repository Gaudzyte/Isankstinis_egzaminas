#include <iostream>
#include "mylib.h"

int main()
{
    analizuotiTeksta("Tekstas.txt", "rezultatai.txt", "urlRezultatai.txt");

    std::cout << "Zodziu rezultatai isvestas i 'rezultatai.txt' faila.\n"
              << "Url rezultatai isvestas i 'urlRezultatai.txt' faila.\n"
              << std::endl;
    return 0;
}
