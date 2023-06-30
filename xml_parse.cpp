#include <iostream>
//#include "pugixml.hpp"
#include "../../pugixml-1.13/src/pugixml.hpp"
#include <vector>
#include <string>

using namespace std;
using namespace pugi;


//Funkcija koja pretvara string u decimalni broj
double Broj(std::string x) { return std::stod(x); }


// Funkcija koja trazi koeficijente dif. jed. iz stringa
std::vector<double> PronadjiKoeficijente(std::string s) {
    std::vector<double> koeficijenti;
    for (int i = 0; i < s.size(); i++) {
        int pocetak, kraj;
        bool predznak = false;
        std::string temp;
        if (s.at(i) >= '0' && s.at(i) <= '9') {
            pocetak = i;
            if (s.at(i - 1) == '-' || s.at(i - 2) == '-') predznak = true;
            i++;
            while ((s.at(i) >= 0 && s.at(i) <= 9) || s.at(i) == '.') i++;
            kraj = i;
            while (pocetak != (kraj + 1)) {
                temp.push_back(s.at(pocetak));
                pocetak++;
            }
            koeficijenti.push_back(Broj(temp));
            if (predznak) {
                koeficijenti.at(koeficijenti.size() - 1) = -koeficijenti.at(koeficijenti.size() - 1);
                predznak = false;
            }
        }
    }
    return koeficijenti;
}


// Funkcija za citanje xml
int main()
{
    cout << "\nParsiranje parametara za Runge Kutta metod: \n\n";

    xml_document doc;

    // load the XML file
    if (!doc.load_file("sample.xml")) return -1;

    xml_node tools = doc.child("RungeKutta").child("Parametri");
    std::vector<double> pocetni_uslovi;

    //Za citanje pocetnih uslova
    for (xml_node_iterator it = tools.begin(); it != tools.end(); ++it) {
        cout << "Parametri: ";

        for (xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait) {
            pocetni_uslovi.push_back(std::stod(ait->value()));
            cout << " " << ait->name() << "=" << ait->value();
        }

        cout << endl;
    }

    xml_node tools_1 = doc.child("RungeKutta").child("Funkcija");
    std::string diferencijalna;
    std::vector<double> koeficijenti;

    //Za citanje diferencijalne jednacine
    for (xml_node_iterator it = tools_1.begin(); it != tools_1.end(); ++it) {
        cout << "Funkcija: ";

        for (xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait) {
            cout << " " << ait->name() << "=" << ait->value();
            diferencijalna = ait->value();
        }

        cout << endl;
    }
    koeficijenti = PronadjiKoeficijente(diferencijalna);

    xml_node tools_2 = doc.child("RungeKutta").child("Tacka_za_racunanje");
    double x_max;

    //Za citanje diferencijalne jednacine
    for (xml_node_iterator it = tools_2.begin(); it != tools_2.end(); ++it) {
        cout << "x_max: ";

        for (xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait) {
            cout << " " << ait->name() << "=" << ait->value();
            x_max = std::stod(ait->value());
        }

        cout << endl;
    }

    cout << endl;

    // Ispisivanje dobivenih vrijednosti 
    std::cout << "Pocetni uslovi su: (x_0, y_0) = (" << pocetni_uslovi.at(0) << ", " << pocetni_uslovi.at(1) << ")" << std::endl;
    std::cout << "Koeficijenti diferencijalne jednacine su (prvo uz x, zatim uz y): ";
    for (int i = 0; i < koeficijenti.size(); i++) {
        if (i == koeficijenti.size() - 1) std::cout << koeficijenti.at(i);
        else std::cout << koeficijenti.at(i) << ", ";
    }
    std::cout << std::endl;

    std::cout << "Tacka u kojoj se racuna funkcija je: x_max = " << x_max << std::endl;
    return 0;
}