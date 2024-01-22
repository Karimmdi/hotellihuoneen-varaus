#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

// Enumerointi huonetyypeille
enum HuoneTyyppi {
    YhdenHengenHuone,    // Arvo yhden hengen huoneelle
    KahdenHengenHuone    // Arvo kahden hengen huoneelle
};

// Funktio alustaa huoneet satunnaisella mrll yhden ja kahden hengen huoneita
void alustaHuoneet(std::vector<bool>& saatavuus) {
    // ts olisin voinut kytt c++ 11 random-device, satunnaislukugeneraattoria mutta kun kyseess ei niin tarkka satunnaista niin kytn tt.
    int HuonNum = 40 + (rand() % 131 ) * 2;  // varmistetaan generointia 40-300 parllisena luvina

    if (HuonNum % 2 != 0) {
        HuonNum++; // Jos luku on pariton, tee se parilliseksi limll yksi
    }
    saatavuus.resize(HuonNum, true);
}

// Funktio tulostaa vapaat huoneet huonetyypin perusteella
void tulostaVapaatHuoneet(const std::vector<bool>& saatavuus, HuoneTyyppi tyyppi) {
    std::cout << "Vapaat huoneet: ";
    size_t puoli = saatavuus.size() / 2;
    if (puoli % 2 != 0) {
        puoli++;  // Jos puoli on pariton, tee se parilliseksi lismll yksi
    }

    if (tyyppi == YhdenHengenHuone) {
        for (size_t i = 0; i < puoli; ++i) {
            if (saatavuus[i]) {
                std::cout << i + 1 << " ";
            }
        }
    }
    else if (tyyppi == KahdenHengenHuone) {
        for (size_t i = puoli; i < saatavuus.size(); ++i) {
            if (saatavuus[i]) {
                std::cout << i + 1 << " ";
            }
        }
    }

    std::cout << std::endl;
}

// Tulostaa valikon ohjelman toiminnoille
void tulostaValikko() {
    std::cout << "1. Varaa huone\n"
              << "2. Lopeta varaus\n"
              << "Valitse toiminto: ";
}

// Tarkistaa, onko haluttua huonetyyppi saatavilla
bool onkoHuonettaSaatavilla(const std::vector<bool>& saatavuus, HuoneTyyppi tyyppi) {
    size_t puoli = saatavuus.size() / 2;

    if (tyyppi == YhdenHengenHuone) {
        for (size_t i = 0; i < puoli; ++i) {
            if (saatavuus[i]) {
                return true;
            }
        }
    }
    else if (tyyppi == KahdenHengenHuone) {
        for (size_t i = puoli; i < saatavuus.size(); ++i) {
            if (saatavuus[i]) {
                return true;
            }
        }
    }

    return false;
}

// Funktio laskee varauksen hinnan huonetyypin ja pivien mrn perusteella
int laskeVarauksenHinta(HuoneTyyppi tyyppi, int paivat) {
    int hinta = (tyyppi == YhdenHengenHuone) ? 100 : 150;
    return hinta * paivat;
}

// Funktio laskee alennetun hinnan alkuperisest hinnasta ja annetusta alennusprosentista
double laskeAlennettuHinta(int alkuperainenHinta, int alennusProsentti) {
    return alkuperainenHinta - (alkuperainenHinta * alennusProsentti / 100.0);
}

// Funktio generoi satunnaisen alennusprosentin (0%, 10% tai 20%)
int generoiSatunnainenAlennus() {
    int alennukset[] = { 0, 10, 20 };
    return alennukset[rand() % 3];
}

// Funktio varaa huoneen, jos se on saatavilla, ja tulostaa varauksen tiedot
void varaaHuone(std::vector<bool>& saatavuus, HuoneTyyppi tyyppi, int valittuHuone) {
    size_t puoli = saatavuus.size() / 2;

    if ((tyyppi == YhdenHengenHuone && valittuHuone > 0 && valittuHuone <= static_cast<int>(puoli) && saatavuus[valittuHuone - 1]) ||
        (tyyppi == KahdenHengenHuone && valittuHuone > static_cast<int>(puoli) && valittuHuone <= static_cast<int>(saatavuus.size()) && saatavuus[valittuHuone - 1])) {

        saatavuus[valittuHuone - 1] = false;
        std::cout << "Huone " << valittuHuone << " varattu onnistuneesti.\n";

        int yo;
        std::cout << "Anna oiden maara: ";

        while (!(std::cin >> yo) || yo <= 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Virheellinen syote. Anna kelvollinen yomaara: ";
        }

        int varauksenHinta = laskeVarauksenHinta(tyyppi, yo);
        int alennusProsentti = generoiSatunnainenAlennus();
        double alennettuHinta = laskeAlennettuHinta(varauksenHinta, alennusProsentti);

        std::cout << "Varattu huone: " << valittuHuone << std::endl;
        std::cout << "Saat " << alennusProsentti << "% alennuksen varauksestasi, mika on " << (varauksenHinta * alennusProsentti / 100.0) << " euroa.\n";
        std::cout << "Varauksen loppusumma alennuksen jlkeen: " << alennettuHinta << " euroa\n";
    }
    else {
        std::cout << "Virheellinen huonevalinta.\n";
    }
}

// Valitsee vapaan huoneen, huonetyypin perusteella
int valitseVapaaHuone(const std::vector<bool>& saatavuus, HuoneTyyppi tyyppi) {
    size_t puoli = saatavuus.size() / 2;

    if (tyyppi == YhdenHengenHuone) {
        for (size_t i = 0; i < puoli; ++i) {
            if (saatavuus[i]) {
                return i + 1;
            }
        }
    }
    else if (tyyppi == KahdenHengenHuone) {
        for (size_t i = puoli; i < saatavuus.size(); ++i) {
            if (saatavuus[i]) {
                return i + 1;
            }
        }
    }

    return -1; // -1 on tss kytetty erityisen arvona, joka osoittaa "ei lytynyt".
}

int main() {
    srand(time(nullptr)); // Tm auttaa tekemn satunnaislukugeneraattorin tuottamat luvut erilaisiksi joka kerta, kun ohjelma suoritetaan.

    std::vector<bool> saatavuus;
    alustaHuoneet(saatavuus);

    int valinta;
    do {
        tulostaValikko();
        std::cin >> valinta;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Virheellinen syote. Anna kelvollinen syote (numero).\n";
            continue;
        }

        switch (valinta) {
            case 1: {
                int huoneTyyppi, valittuHuone;
                std::cout << "Valitse huonetyyppi (1: Yhden hengen huone, 2: Kahden hengen huone): ";

                while (!(std::cin >> huoneTyyppi) || (huoneTyyppi != 1 && huoneTyyppi != 2)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Virheellinen syote. Anna kelvollinen syote (numero): ";
                }

                HuoneTyyppi tyyppi = (huoneTyyppi == 1) ? YhdenHengenHuone : KahdenHengenHuone;

                if (onkoHuonettaSaatavilla(saatavuus, tyyppi)) {
                    tulostaVapaatHuoneet(saatavuus, tyyppi);

                    std::cout << "Haluatko valita huoneen itse vai valitaan puolestasi? (1: Itse, 2: Puolestasi): ";
                    while (!(std::cin >> valinta) || (valinta != 1 && valinta != 2)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Virheellinen syote. Anna kelvollinen syote (numero): ";
                    }

                    if (valinta == 1) {
                        std::cout << "Valitse vapaista huoneista  haluttu numero: ";
                        std::cin >> valittuHuone;
                    }
                    else if (valinta == 2) {
                        valittuHuone = valitseVapaaHuone(saatavuus, tyyppi);

                        if (valittuHuone == -1) {
                            std::cout << "Valitsemasi huonetyyppi ei ole saatavilla.\n";
                            continue;
                        }

                        std::cout << "Valitsimme huoneen puolestasi: " << valittuHuone << std::endl;
                    }
                    else {
                        std::cout << "Virheellinen valinta.\n";
                        continue;
                    }

                    varaaHuone(saatavuus, tyyppi, valittuHuone);
                }
                else {
                    std::cout << "Valitsemasi huonetyyppi ei ole saatavilla.\n";
                }
                break;
            }
            case 2:
                std::cout << "Kiitos varauksestasi!\n";
                break;
            default:
                std::cout << "Virheellinen valinta.\n";
        }
    } while (valinta != 2);

    return 0;
}
