#include <iostream>
#include <memory>
#include <fstream>
#include <string>

using namespace std;
/** 
klasa Element ktora jest wykorzystywana w klasie Lista jako elementy tej listy
@param T wartosc 
@param shared_ptr<Element<T>> nastepny
@param weak_ptr<Element<T>> poprzedni
*/
template <typename T>
class Element
{
public:
    T wartosc;
    Element(const T& wartosc) : wartosc(wartosc) {}
    shared_ptr<Element<T>> nastepny;
    weak_ptr<Element<T>> poprzedni;

};
/**
klasa Lista bedaca lista dwukierunkowa 
@param int dlugosc
@param shared_ptr<Element<T>> glowa
@param shared_ptr<Element<T>> ogon
*/
template <typename T>
class Lista
{

    shared_ptr<Element<T>> glowa;
    shared_ptr<Element<T>> ogon;
    int dlugosc;

public:
    /**
metoda dodajNaKoncu dodajaca element na koncu listy
@param Element<T> element
*/
    void dodajNaKoncu(Element<T> element)
    {
        if (dlugosc == 0)
        {
            glowa = make_shared<Element<T>>(element);
            ogon = glowa;
        }
        else
        {
            shared_ptr<Element<T>> nowy = make_shared<Element<T>>(element);
            ogon->nastepny = nowy;
            nowy->poprzedni = ogon;
            ogon = ogon->nastepny;
        }
        dlugosc++;
    }
    /**
metoda dodajWSrodku dodajaca element w liscie jako nastepny po elemencie z podanym indexem
@param int index
@param Element<T> element
*/
    void dodajWSrodku(int index, Element<T>& element)
    {
        if (!(index >= 0 && index <= dlugosc))
        {
            throw std::invalid_argument("Niewlasciwy index");
        }

        shared_ptr<Element<T>> tymczasowy = glowa;
        for (int i = 0; i < index - 1; i++)
        {
            tymczasowy = tymczasowy->nastepny;
        }
        shared_ptr<Element<T>> nowy = make_shared<Element<T>>(element);
        shared_ptr<Element<T>> tymczasowy2 = tymczasowy->nastepny;
        tymczasowy->nastepny = nowy;
        nowy->poprzedni = tymczasowy;
        tymczasowy2->poprzedni = nowy;
        nowy->nastepny = tymczasowy2;
    }

    /**
metoda wyswietlajaca cala zawartosc listy
*/
    void wyswietl()
    {
        shared_ptr<Element<T>> tymczasowy = glowa;
        while (tymczasowy)
        {
            std::cout << tymczasowy->wartosc << endl;
            tymczasowy = tymczasowy->nastepny;
        }
    }
    /**
metoda wyswietlajaca cala zawartosc listy od konca
 */
    void wyswietlOdKonca()
    {
        shared_ptr<Element<T>> tymczasowy = ogon;
        while (tymczasowy)
        {
            std::cout << tymczasowy->wartosc << endl;
            tymczasowy = tymczasowy->poprzedni.lock();
        }
    }

    Lista()
    {
        glowa = nullptr;
        ogon = nullptr;
        dlugosc = 0;
    }
    /**
metoda usuwajaca element listy z podana wartoscia jednej z zmiennych elementu
@param int pesell
@param auto(*lambda)(T&, int&)->bool
*/
    void usunElement(int pesell, auto(*lambda)(T&, int&)->bool) {
        std::shared_ptr<Element<T>> current = glowa;
        while (current != nullptr) {
            if (lambda(current->wartosc, pesell)) {
                if (current == glowa) {
                    glowa = current->nastepny;
                }
                else {
                    auto prev = current->poprzedni.lock();
                    if (prev) {
                        prev->nastepny = current->nastepny;
                    }
                }
                if (current == ogon) {
                    ogon = current->poprzedni.lock();
                }
                else {
                    auto next = current->nastepny;
                    if (next) {
                        next->poprzedni = current->poprzedni;
                    }
                }
                break;
            }
            current = current->nastepny;
        }
    }
    /**
metoda sortujaca zawartosc listy
@param auto(*lambda)(T&, T&)->bool
*/
    void sortuj(auto(*lambda)(T&, T&)->bool) {
        for (std::shared_ptr<Element<T>> i = glowa; i != ogon; i = i->nastepny) {
            std::shared_ptr<Element<T>> max = i;
            for (std::shared_ptr<Element<T>> j = i->nastepny; j != nullptr; j = j->nastepny) {
                if (lambda(j->wartosc, max->wartosc))
                    max = j;
            }
            std::swap(i->wartosc, max->wartosc);
        }
    }
    /**
metoda znajdujaca element listy o danej wartosci jednego z parametrow elementu
@param int pesell
@param auto(*lambda)(T&, int&)->bool
*/
    T znajdz(int pesell, auto(*lambda)(T&, int&)->bool) {
        shared_ptr<Element<T>> current = glowa;
        while (current != nullptr) {
            if (lambda(current->wartosc, pesell))
                return current.get()->wartosc;
            current = current->nastepny;
        }
        T student;
        student.pesel = 0;
        return student;
    }
    T first() {
        return glowa.get()->wartosc;
    }
    /**
metoda zapisujaca liste do pliku tekstowego
@param string nazwa
*/
    void zapis(string nazwa) {
        ofstream plik(nazwa);
        if (plik) {
            shared_ptr<Element<T>> curr = glowa;
            while (curr != nullptr) {
                plik << curr->wartosc << endl;
                curr = curr->nastepny;
            }
        }
    }
};


/**
klasa studenci pomocnicza klasa ktora przechowuje rozny typ danych by sprawdzic dzialanie listy dwukierunkowej
*/
class studenci {
public:
    int pesel;
    int album;
    string imie;
    string nazwisko;
    string miasto;
    string ulica;
    string numer;
    studenci(int pesel, int album, string imie, string nazwisko, string miasto, string ulica, string numer) : pesel(pesel), album(album), imie(imie), nazwisko(nazwisko), miasto(miasto), ulica(ulica), numer(numer) {}
    friend std::ostream& operator<<(std::ostream& os, const studenci& obj);
    studenci() {};


};
/**
klasa oceny pomocnicza klasa ktora przechowuje rozny typ danych by sprawdzic dzialanie listy dwukierunkowej
*/
class oceny {
public:
    int pesel;
    int ocena;
    string opis;
    string zdane;
    oceny(int pesel, int ocena, string opis, string zdane) : pesel(pesel), ocena(ocena), opis(opis), zdane(zdane) {}
    friend std::ostream& operator<<(std::ostream& os, const oceny& obj);
    oceny() {};


};

std::ostream& operator<<(std::ostream& os, const studenci& obj)
{
    os << obj.pesel << '\t' << obj.album << '\t' << obj.imie << '\t' << obj.nazwisko << '\t' << obj.miasto << '\t' << obj.ulica << '\t' << obj.numer << '\t';
    return os;
}
std::ostream& operator<<(std::ostream& os, const oceny& obj)
{
    os << obj.pesel << '\t' << obj.ocena << '\t' << obj.opis << '\t' << obj.zdane << '\t';
    return os;
}
/**
funkcja wyswietlajaca menu 
*/
void menu() {
    
    std::cout << "Wybierz opcje" << std::endl << std::endl;
    std::cout << "1 - Wyswietl studentow" << std::endl;
    std::cout << "2 - Wyswietl oceny " << std::endl;
    std::cout << "3 - Posortuj studentow malejaco po peselu." << std::endl;
    std::cout << "4 - Posortuj studentow rosnaco po peselu" << std::endl;
    std::cout << "5 - Posortuj oceny rosnaco" << std::endl;
    std::cout << "6 - Posortuj oceny malejaco" << std::endl;
    std::cout << "7 - Usun studenta po peselu" << std::endl;
    std::cout << "8 - Dodaj Studenta" << std::endl;
    std::cout << "9 - Dodaj ocene " << std::endl;
    std::cout << "10 - Wyszukaj studenta" << std::endl;
    std::cout << "11 - Zamknij system" << std::endl;

}
int main()
{
    studenci found;
    auto lambda0 = [](studenci& value1, studenci& value2)->bool { return value1.pesel >= value2.pesel; };
    auto lambda1 = [](studenci& value1, studenci& value2)->bool { return value1.pesel <= value2.pesel; };
    auto lambda3 = [](oceny& value1, oceny& value2)->bool { return value1.ocena >= value2.ocena; };
    auto lambda4 = [](oceny& value1, oceny& value2)->bool { return value1.ocena <= value2.ocena; };
    auto lambda2 = [](studenci& value1, int& value2)->bool { return value1.pesel == value2; };
    auto lambda5 = [](oceny& value1, int& value2)->bool { return value1.ocena == value2; };

    ifstream plik("studenci.txt");
    int pesel_;
    int album_;
    string imie_;
    string nazwisko_;
    string miasto_;
    string ulica_;
    string numer_;
    studenci student;
    Lista<studenci> listaS;
    while (plik >> pesel_ >> album_ >> imie_ >> nazwisko_ >> miasto_ >> ulica_ >> numer_) {
        student.pesel = pesel_;
        student.album = album_;
        student.imie = imie_;
        student.nazwisko = nazwisko_;
        student.miasto = miasto_;
        student.ulica = ulica_;
        student.numer = numer_;
        listaS.dodajNaKoncu(student);
    };

    
    ifstream plik2("oceny.txt");
    int pesel__;
    int ocena_;
    string opis_;
    string zdane_;
    oceny ocena;
    Lista<oceny> listaOcen;
    while (plik2 >> pesel__ >> ocena_ >> opis_ >> zdane_) {
        ocena.pesel = pesel__;
        ocena.ocena = ocena_;
        ocena.opis = opis_;
        ocena.zdane = zdane_;

        listaOcen.dodajNaKoncu(ocena);
    };
    menu();
    int wybor;
    std::cin >> wybor;
    while (std::cin.fail()) {
        std::cout << "Wprowadziles bledne dane" << std::endl;

        std::cin.clear();
        std::cin.ignore(1000, '\n');

        std::cout << "Wprowadz dane ponownie:";
        std::cin >> wybor;
    }
    while (wybor != 11) {

        switch (wybor) {
        case 1: {

            std::cout << "Wyswietl studentow" << std::endl;
            listaS.wyswietl();
            menu();
            break;
        }
        case 2: {
            std::cout << "Wyswietl oceny" << std::endl;
            listaOcen.wyswietl();
            menu();
            break;
        }
        case 3: {
            std::cout << "sortowanie studentow rosnaco" << std::endl;
            listaS.sortuj(lambda1);
            menu();
            break;
        }
        case 4: {
            std::cout << "Sortowanie studentow malejaco" << std::endl;
            listaS.sortuj(lambda0);
            menu();
            break;
        }
        case 5: {
            std::cout << "Posortuj oceny malejaco" << std::endl;
            listaOcen.sortuj(lambda3);
            menu();

            break;
        }
        case 6: {
            std::cout << "Posortuj oceny rosnaco" << std::endl;
            listaOcen.sortuj(lambda4);
            menu();
            break;
        }
        case 7: {
            std::cout << "Usun studenta po peselu" << std::endl;
            std::cout << "podaj pesel osoby do usuniecia" << std::endl;
            int a;
            std::cin >> a;
            listaS.usunElement(a, lambda2);
            
            menu();
            break;
        }
        case 8: {
            std::cout << "Dodaj studenta" << std::endl;
             std::cout << "podaj po kolei pesel nr_albumu imie nazwisko miasto ulice i numer domu" << std::endl;
             int pesel__;
             int album__;
             string imie__;
             string nazwisko__;
             string miasto__;
             string ulica__;
             string numer__;

             cin >> pesel__;
             cin >> album__;
             cin >> imie__;
             cin >> nazwisko__;
             cin >> miasto__;
             cin >> ulica__;
             cin >> numer__;
             studenci student__(pesel__,album__,imie__,nazwisko__,miasto__,ulica__,numer__);

             listaS.dodajNaKoncu(student__);
             cout << "dodano studenta do listy" << endl;
             menu();
            break;
        }
        case 9: {
            std::cout << "Dodaj ocene" << std::endl;
            std::cout << "podaj po kolei pesel ocene opis oceny i informacje czy student zdal" << std::endl;
            int pesel___;
            int ocena___;
            string opis___;
            string zdal___;
          

            cin >> pesel___;
            cin >> ocena___;
            cin >> opis___;
            cin >> zdal___;
          
           oceny ocenka___(pesel___, ocena___, opis___, zdal___);

            listaOcen.dodajNaKoncu(ocenka___);
            cout << "dodano ocene do listy" << endl;
            menu();
            break;
        }
        case 10: {
            std::cout << "wyszukaj studenta" << std::endl;
            int abc;
            std::cout << "Podaj pesel" << std::endl;
            std::cin >> abc;
            found = listaS.znajdz(abc, lambda2);
            if (found.pesel != 0) {
                std::cout << "Znaleziono element o wartosci " << found << std::endl;
            }
            else {
                std::cout << "Nie znaleziono elementu o podanej wartosci" << std::endl;
            }
            menu();
            break;
        }
        default: {

            system("cls");
            menu();
            std::cout << "Zostala wprowadzona zla wartosc" << std::endl;


            break;
        }
        }
        std::cin >> wybor;
    }
 

    listaS.zapis("studenci.txt");
    listaOcen.zapis("oceny.txt");

}