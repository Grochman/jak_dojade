#include <iostream>
#include "String.h"

using namespace std;

struct miasto;

struct droga {
    miasto* city;
    int dist;
    droga* next = nullptr;
    droga* prev = nullptr;
};
struct miasto {
    droga* polaczenia = new droga();
    String nazwa;
    int miejsce; //na mapie zahaszowane
};

struct shortestPath {
    String nazwa;
    int dist;
    String lastupdate;
    //shoortestPath* lastupdateprev;
};
struct trasa {
    String miasto;
    trasa* next = nullptr;
    trasa* prev = nullptr;
};
struct mapa {
    char* pola;
    int width;
    int height;
    int size;
    int l_miast = 0;
};


bool isLetter(char x) {
    if ((int)x > 64 && (int)x < 91) return true;
    return false;
}
String findName(int x, int y, mapa swiat) {
    String result;
    int start = -1;

    if (x + 1 < swiat.width && isLetter(swiat.pola[y * swiat.width + x + 1])) start = y * swiat.width + x + 1;//prawo
    else if (x > 0 && isLetter(swiat.pola[y * swiat.width + x - 1])) start = y * swiat.width + x - 1;//lewo
    else if (y > 0 && isLetter(swiat.pola[(y - 1) * swiat.width + x])) start = (y - 1) * swiat.width + x; //gora
    else if (y + 1 < swiat.height && isLetter(swiat.pola[(y + 1) * swiat.width + x]))start = (y + 1) * swiat.width + x;//dol

    else if (x > 0 && y > 0 && isLetter(swiat.pola[(y - 1) * swiat.width + x - 1]))
        start = (y - 1) * swiat.width + x - 1; //lewy gorny
    else if (x > 0 && y + 1 < swiat.height && isLetter(swiat.pola[(y + 1) * swiat.width + x - 1]))
        start = (y + 1) * swiat.width + x - 1;//lewy dolny
    else if (x + 1 < swiat.width && y > 0 && isLetter(swiat.pola[(y - 1) * swiat.width + x + 1]))
        start = (y - 1) * swiat.width + x + 1; //prawy gorny
    else if (x + 1 < swiat.width && y + 1 < swiat.height && isLetter(swiat.pola[(y + 1) * swiat.width + x + 1]))
        start = (y + 1) * swiat.width + x + 1;//prawy dolny

    if (start != -1) {
        while (start >= 0 && isLetter(swiat.pola[start]))
            start--;

        start++;
        while (start < swiat.width * swiat.height && isLetter(swiat.pola[start])) {
            result.AddChar(swiat.pola[start]);
            start++;
        }
    }

    return result;
}

bool moreThanOneRout(int x, int y, char temp[], mapa swiat) {
    int result = 0;
    
    int place = y * swiat.width + x + 1;
    if (x + 1 < swiat.width && temp[place] == '#') result++;
    place -= 2;
    if (x > 0 && temp[place] == '#') result++;
    place = (y - 1) * swiat.width + x;
    if (y > 0 && temp[place] == '#') result++;
    place = (y + 1) * swiat.width + x;
    if (y + 1 < swiat.height && temp[place] == '#') result++;

    if (result > 1) return true;
    else return false;
}
void conect(mapa swiat, int x, int y, int length, int licznik, miasto city[]) {
    if (y * swiat.width + x != licznik) {
        droga* temp = city[licznik].polaczenia;
        while (temp != nullptr) {
            if (temp->city == &city[y * swiat.width + x]) {
                if (length + 1 < temp->dist) {
                    temp->dist = length + 1;
                    cout << length << "update";
                }
                return;
            }
            temp = temp->prev;
        }

        city[licznik].polaczenia->dist = length + 1;
        city[licznik].polaczenia->city = &city[y * swiat.width + x];
        cout << city[licznik].nazwa << length << city[licznik].polaczenia->city->nazwa << endl;
        city[licznik].polaczenia->next = new droga();
        city[licznik].polaczenia->next->prev = city[licznik].polaczenia;
        city[licznik].polaczenia = city[licznik].polaczenia->next;
        city[licznik].polaczenia->dist = INT_MAX;
    }
}
void findRoads(miasto city[], int x, int y, int licznik, int length, mapa swiat, char temp[]) {
    temp[y * swiat.width + x] = '@';

    char* tempmap;
    if (moreThanOneRout(x, y, temp, swiat)) {
        tempmap = new char[swiat.size];

        for (int k = 0; k < swiat.size; k++)
            tempmap[k] = temp[k];
    }
    else tempmap = temp;

    int place = y * swiat.width + x + 1;
    if (x + 1 < swiat.width) {
        if (tempmap[place] == '#') {
            findRoads(city, x + 1, y, licznik, length + 1, swiat, tempmap);
        }
        else if (swiat.pola[place] == '*') {
            conect(swiat, x + 1, y, length, licznik, city);
        }
    }
    place -= 2;
    if (x > 0) {
        if (tempmap[place] == '#') {
            findRoads(city, x - 1, y, licznik, length + 1, swiat, tempmap);
        }
        else if (swiat.pola[place] == '*') {
            conect(swiat, x - 1, y, length, licznik, city);
        }
    }
    place = (y - 1) * swiat.width + x;
    if (y > 0) {
        if (tempmap[place] == '#') {
            findRoads(city, x, y - 1, licznik, length + 1, swiat, tempmap);
        }
        else if (swiat.pola[place] == '*') {
            conect(swiat, x, y - 1, length, licznik, city);
        }
    }
    place = (y + 1) * swiat.width + x;
    if (y + 1 < swiat.height) {
        if (tempmap[place] == '#') {
            findRoads(city, x, y + 1, licznik, length + 1, swiat, tempmap);
        }
        else if (swiat.pola[place] == '*') {
            conect(swiat, x, y + 1, length, licznik, city);
        }
    }

    //delete[] tempmap; //wypierdala bo kopiuje czasem a czasem nie
}

int getNearest(bool visited[], mapa swiat, shortestPath paths[]) {
    int mindist = INT_MAX;
    int result = -1;
    for (int i = 0; i < swiat.size; i++)
        if (visited[i] == false && paths[i].dist < mindist) {
            mindist = paths[i].dist;
            result = i;
        }
    visited[result] = true;
    return result;
}
void wypiszTrase(mapa swiat, shortestPath paths[], int koniec, int poczatek, miasto city[]) {
    String odwiedzone = paths[koniec].lastupdate;

    trasa* t1 = new trasa();

    for (int i = 0; i < swiat.size; i++) {
        if (odwiedzone == city[poczatek].nazwa) break;
        if (paths[i].nazwa == odwiedzone) {
            t1->miasto = odwiedzone;
            t1->prev = new trasa();
            t1->prev->next = t1;
            t1 = t1->prev;

            odwiedzone = paths[i].lastupdate;
            i = -1;
        }
    }

    t1 = t1->next;
    while (t1 != nullptr) {
        cout << ' ' << t1->miasto;
        t1 = t1->next;
    }
}
void dijkster(String& zrodlo, String& cel, int typ, miasto city[], mapa swiat, shortestPath paths[]) {
    bool* visited = new bool[swiat.size];

    int poczatek, koniec;
    int nearest;

    for (int i = 0; i < swiat.size; i++) {
        if (zrodlo == city[i].nazwa) poczatek = i;
        else if (cel == city[i].nazwa) koniec = i;
        visited[i] = false;
    }
    visited[poczatek] = true;

    droga* temp = city[poczatek].polaczenia->prev;
    while (temp != nullptr) {
        for (int i = 0; i < swiat.size; i++) {
            if (temp->city->nazwa == paths[i].nazwa && temp->dist < paths[i].dist) {
                paths[i].dist = temp->dist;
                paths[i].lastupdate = city[poczatek].nazwa;
            }
        }
        temp = temp->prev;
    }//wczytane odleglosci z noda startowego


    for (int i = 0; i < swiat.size - 1; i++) {
        nearest = getNearest(visited, swiat, paths);
        if (nearest == -1)break;
        else if (city[nearest].nazwa == cel) {
            cout << paths[koniec].dist;

            if (typ == 1)
                wypiszTrase(swiat, paths, koniec, poczatek, city);

            return;
        }

        temp = city[nearest].polaczenia->prev;
        while (temp != nullptr) {
            for (int j = 0; j < swiat.size; j++) {
                if (temp->city->nazwa == paths[j].nazwa && temp->dist + paths[nearest].dist < paths[j].dist) {
                    paths[j].dist = temp->dist + paths[nearest].dist;
                    paths[j].lastupdate = city[nearest].nazwa;
                }
            }
            temp = temp->prev;
        }
    }

    //delete[] visited; // rozpierdala sie z jakiegos powodu
    cout << paths[koniec].dist;
}

int main()
{
    int loty, trasy;
    char buf;
    mapa swiat;
    cin >> swiat.width >> swiat.height;
    swiat.size = swiat.width * swiat.height;
    swiat.pola = new char[swiat.size];
    miasto* city = new miasto[swiat.size];


    for (int i = 0; i < swiat.height; i++) {
        for (int j = 0; j < swiat.width; ) {
            buf = getchar();
            if (isspace(buf) == 0) {
                swiat.pola[i * swiat.width + j] = buf;
                j++;
            }
        }
    }

    shortestPath* paths = new shortestPath[swiat.size];

    int licznik = 0;
    for (int i = 0; i < swiat.height; i++) {
        for (int j = 0; j < swiat.width; j++) {
            licznik = i * swiat.width + j;
            if (swiat.pola[licznik] == '*') {
                
                city[licznik].nazwa = findName(j, i, swiat);
                cout << city[licznik].nazwa << endl;
                city[licznik].miejsce = i * swiat.width + j;
                city[licznik].polaczenia->dist = INT_MAX;
                paths[licznik].nazwa = city[licznik].nazwa;
            }
        }
    }


    licznik = 0;
    char* tempmap = new char[swiat.size];
    for (int i = 0; i < swiat.height; i++) {
        for (int j = 0; j < swiat.width; j++) {
            if (swiat.pola[i * swiat.width + j] == '*') {

                for (int k = 0; k < swiat.size; k++)
                    tempmap[k] = swiat.pola[k];

                findRoads(city, j, i,  i * swiat.width + j, 0, swiat, tempmap);
                licznik++;
            }
        }
    }
    delete[] tempmap;
    delete[] swiat.pola;

    cin >> loty;
    String zrodlo, cel;
    int czas;
    for (int x = 0; x < loty; x++) {
        cin >> zrodlo >> cel >> czas;

        for (int i = 0; i < swiat.height; i++) {
            for (int j = 0; j < swiat.width; j++) {
                if (city[i * swiat.width + j].nazwa == zrodlo) {
                    city[i * swiat.width + j].polaczenia->dist = czas;

                    for (int k = 0; k < swiat.height; k++) {
                        for (int l = 0; l < swiat.width; l++) {
                            if (city[k * swiat.width + l].nazwa == cel) {
                                city[i * swiat.width + j].polaczenia->city = &city[k * swiat.width + l];
                                break;
                            }
                        }
                    }

                    cout << city[i * swiat.width + j].nazwa;
                    cout << city[i * swiat.width + j].polaczenia->city->nazwa;
                    cout << city[i * swiat.width + j].polaczenia->dist << endl;
                    city[i * swiat.width + j].polaczenia->next = new droga();
                    city[i * swiat.width + j].polaczenia->next->prev = city[i * swiat.width + j].polaczenia;
                    city[i * swiat.width + j].polaczenia = city[i * swiat.width + j].polaczenia->next;

                    break;
                }
            }
        }

        
    }


    cin >> trasy;
    int typ;
    for (int i = 0; i < trasy; i++) {
        cin >> zrodlo >> cel >> typ;

        if (zrodlo == cel)
            cout << '0' << endl;
        else {
            for (int j = 0; j < swiat.size; j++)
                paths[j].dist = INT_MAX;

            dijkster(zrodlo, cel, typ, city, swiat, paths);
            cout << endl;
        }
    }
    //czyszczenie pamieci: wszystkie paths i wszystkie drogi

    return 0;
}