#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <cstring>
#include <math.h>
#include <vector>

#ifdef _WIN32
#define clear() system("cls")
#define pause() system("pause")
#else
#define clear() system("clear")
#define pause() while ('\n' != getchar()); printf ("Press enter to continue..."); getchar()
#endif // _WIN32

using namespace std;

/**
* Pomocnicza funkcja pop ułatwiająca użycie stosu na bazie vector.
*/
float pop(vector<float> *vect)
{
    float tmp = vect->back();
    vect->pop_back();
    return tmp;
}

/**
* Funkcja ta jako parametr przyjmuje wyrażenie podane przez użytkownika.
* Następnie z wyrażenia wyłuskiwane są liczby i operatory, które zostaną
* wykorzystane do wykonania obliczeń.
*/
long double  parse_and_calculate_rpn(char *wyr)
{
    vector<float> *vect = new vector<float>;
    bool number_read = false;
    int dot = 0;

    while(*wyr != '\0')
    {
        char* num_end;
        long double  first_argument=0, second_argument=0, result=0;

        double tmp = strtod(wyr, &num_end);
        if(num_end != wyr)
        {
            vect->push_back(tmp);
            wyr = num_end;
            number_read = true;
        }

        if(*wyr == '\0')
            break;

        if(*wyr == '.')
            dot++;

        if(*wyr == ' ')
            dot--;

        if(dot>=1)
            throw 2;

        if((*wyr >= 'a' && *wyr <= 'z') || (*wyr>='A' && *wyr <= 'Z'))
            throw 5;

        switch(*wyr)
        {
        case '+':
            result = pop(vect) + pop(vect);           /**< Po napotkaniu operatora pobiera dwie liczby wykonuje działanie, a wynik wrzuca na stos. */
            vect->push_back(result);
            break;
        case '-':
            first_argument = pop(vect);
            second_argument = pop(vect);
            vect->push_back(second_argument - first_argument);
            break;
        case '*':
            result = pop(vect)*pop(vect);
            vect->push_back(result);
            break;
        case '/':
            first_argument = pop(vect);
            second_argument = pop(vect);
            vect->push_back(second_argument / first_argument);
            break;
        case '^':
            first_argument = pop(vect);
            second_argument = pop(vect);
            result = pow(second_argument,first_argument);

            vect->push_back(result);
            break;
        }

        wyr++;
    }

    /**< Sprawdza czy wczytano jakąkolwiek liczbę. Jeżeli nie, zwraca wyjątek o kodzie 3. */
    if(!number_read)
        throw 3;

    return pop(vect);   /**\return Wynik wyrażenia. */
}

/**
* Funkcja zwracająca czytelne dla użytkownika nazwy wyjątków
*/
string print_exception_description(int e)
{
    string desc;

    switch(e)
    {
    case 1:
        desc = "dzielenie przez zero";
        break;

    case 2:
        desc = "dodatkowy przecinek w liczbie";
        break;

    case 3:
        desc = "nie wczytano zadnych liczb";
        break;

    case 4:
        desc = "niepoprawna ilosc nawiasow";
        break;

    case 5:
        desc = "wykryto litere";
        break;

    default:
        desc = "nieznany wyjatek";
    }

    return desc;
}
/**
* \brief Funkcja odpowiedzialna za konwersje wyrażenia z postaci infiksowej na postfiksową
*
* Wyrażenie po konwersji jest używane do policzenia ONP.
*/
long double calculate(char wyr[])
{
    int bracket=0;         /*!\param bracket zmienna służąca do liczenia wystapień nawiasów otwierających i zamykających*/
    stringstream oss;       /*!\param oss strumień pomocniczy*/
    stringstream iss;       /*!\param iss strumień pomocniczy*/
    strcat(wyr, " =");    ///dopisujemy " = " na koniec wyrażenia znak równosci

    char stos[256],element[256]; /*!\param stos na stosie przechowujemy znaki typu + * itp.*/
    int wsk = 0;                /*!\param wsk wskaznik za pomocą którego poruszamy się po tablicy jeśli jest ustawiony na 0 to znaczy ze stos jest pusty*/
    oss<<wyr;                    /**< oss<<wyr zawartość tablicy wpisujemy do strumienia pomocniczego */
    do
    {

        oss >> element;                 /**<Zawartość strumienia wpisujemy do tablicy */

        if((element[0] >= 'a' && element[0] <= 'z') || (element[0]>='A' && element[0] <= 'Z'))
            throw 5;                        /**< Po wykryciu litery zwraca wyjątek */

        if(element[0] >= '0' && element[0] <= '9')
        {

            bool comma = false;
            int i=0;
            do
            {
                if(element[i]=='.' && comma)
                {
                    throw 2;
                }

                if(element[i]=='.')
                {
                    comma = true;
                }
                i++;
            }
            while((element[i]>='0' && element[i]<='9') || element[i]=='.');
            iss << element << " ";    /**<Jeśli element jest liczbą to wpisujemy go do strumienia*/
        }

        else
            switch(element[0])
            {
            case '+':;                                                            /**<------------ W przypadku pojawienia się znaku '+' wykonujemy tą samą instrukcje co przy znaku '-' ------------*/
            case '-':
                if((element[1]>='0'&&element[1]<='9') || element[1]== '.')
                {
                    bool comma = false;
                    iss<<"-";      /**< Jesli po znaku '-' znajduje się liczba wpisz do strumienia -liczba*/
                    int i=1;
                    do
                    {
                        if(element[i]=='.' && comma)
                        {
                            throw 2;
                        }

                        if(element[i]=='.')
                        {
                            comma = true;
                        }

                        iss<<element[i++];
                    }
                    while((element[i]>='0'&&element[i]<='9') || element[i]=='.');
                    iss<<" ";
                }
                else
                {
                    while(wsk && stos[wsk - 1] != '(')          /**<Pętla wykona się jeśli stos nie jest pusty i szczyt nie jest nawiasem*/
                    {
                        wsk--;
                        iss << stos[wsk] << " ";            /**<Do strumienia zostanie wpisana zawartość stosu*/
                    }

                    stos[wsk++] = element[0];               /**<na szczyt stosu zostanie wpisany operator*/
                }
                break;
            case '*':               /**<------------ Po napotkaniu znaku '*' lub '/'... ------------*/
                ;
            case '/':
                while(wsk && stos[wsk - 1] != '(' && stos[wsk - 1] != '+' &&stos[wsk - 1] != '-')
                {
                    /**<Jeśli szczyt stosu jest różny od '(' , '+' , '-' to pętla sie wykona i wypisze zawartość stosu*/
                    wsk--;
                    iss<<stos[wsk]<<" " ;
                }                                                /**<Do strumienia zostanie wpisana zawartość stosu*/

                stos[wsk++] = element[0];
                break;/**<na szczyt stosu zostanie wpisany operator*/
            case '^':               /**<------------ Po napotkaniu znaku '^'...------------ */
                while(wsk && stos[wsk - 1] == '^')
                {
                    /**<jeśli stos nie jest pusty i na szczycie stosu leży '^' to pętla się wykona
                    * i zostanie wpisane do strumienia zawartość stosu
                    */
                    wsk--;
                    iss << stos[wsk] << " ";
                }

                stos[wsk++] = element[0];       /**<Wrzucenie na stos operatora*/
                break;
            case '(':
                bracket++;
                stos[wsk++] = '(';      /**<po napotkaniu nawiasu otwierającego wrzucamy go na stos*/
                break;
            case ')':
                bracket--;
                while(stos[wsk - 1] != '(')
                {
                    wsk--;
                    iss << stos[wsk] << " ";/**<Po napotkaniu nawiasu zmykającego wpisujemy zawartość stosu do napotkania nawiasu otwierajacego*/
                }

                wsk--;
                break;
            case '=':
                while(wsk)
                {
                    /**<Po napotkaniu operatora rownosci wypisujemy zawartosc stosu*/
                    wsk--;
                    iss << stos[wsk] << " ";
                }

                iss << " =\n\n";
                break;
            }

    }
    while(element[0] != '=');
    char conv[256]; /*!\param conv tablica na przekształcone wyrażenie*/
    iss>>conv;      /**<przesłanie znakow z strumienia do tablicy (conv)*/
    if(bracket!=0)
        throw 4;
    strncpy( conv, iss.str().c_str(), sizeof( conv ) );
    cout <<"Wyrazenie po konwersji: "<< conv  ; /**< wyświetlenie wyrażenia po konwersji */
    return parse_and_calculate_rpn(conv);       /**\return zwracana wartość to funkcja licząca onp przyjmująca wyrażenie po konwersji jako parametr  */
}

/**
* Funkcja służy do obsługi wyrażeń w ONP
*/
void calculate_rpn()
{
    char wyr[256];
    long double ans;
    bool error;
    do
    {
        clear();
        cout << "Aby wyjsc wpisz \"q\"" << endl << endl;
        error = false;

        cout << "Podaj wyrazenie w ONP: ";          /**< Program przyjmuje od użytkownika wyrażenie  */
        scanf(" %[^\n]255", wyr);

        /**< Przerwnij działanie jeśli napotkasz "q" */
        if(!strcmp(wyr, "q"))
            break;

        /**< Spróbuj wykonać funkcje parse_and_calculate_rpn() */
        try
        {
            ans = parse_and_calculate_rpn(wyr);
        }

        /**< Złap wyjątek i wyśwyetl komunikat */
        catch(int e)
        {
            cout << "Wystapil problem: " << print_exception_description(e) << endl << endl;
            pause();
            error = true;
        }

        /**< Jeżeli nie ma wyjątku, wyświetl wynik */
        if(!error)
        {
            cout <<  "Wynik: " << ans << endl << endl;
            pause();
        }

        cout << endl;

    }
    while(strcmp(wyr, "q"));
}

/**
* Funkcja służy do obsługi wyrażeń w postaci infiksowej
*/
void calculate_norm()
{
    char wyr[256];
    long double ans;
    bool error;

    do
    {
        clear();
        cout << "Aby wyjsc wpisz \"q\"" << endl << endl;
        error = false;

        cout << "Podaj wyrazenie : ";       /**< Program przyjmuje od użytkownika wyrażenie  */
        scanf(" %[^\n]255", wyr);

        /**< Przerwnij działanie jeśli napotkasz "q" */
        if(!strcmp(wyr, "q"))
            break;

        /**< Spróbuj wykonać funckje calculate() */
        try
        {
            ans = calculate(wyr);
        }

        /**< Złap wyjątek i wyśwyetl komunikat */
        catch(int e)
        {
            cout << "Wystapil problem: " << print_exception_description(e) << endl;
            pause();
            error = true;
        }

        /**<  Jeżeli nie ma wyjątku, wyświetl wynik*/
        if(!error)
        {
            cout << "Wynik: " << ans << endl;
            pause();
        }

        cout << endl;

    }
    while(strcmp(wyr, "q"));
}
/**
* Funkcja wyświetla i obsługuje menu.
*/
void menu()
{
    char wybor;
    do
    {
        clear();
        cout << "------------------------------------------------------";
        cout << "\n 1. Tryb ONP";
        cout << "\n 2. Tryb standardowego wyrazenia";
        cout << "\n q. Wyjscie";
        cout << "\n----------------------------------------------------" << endl;
        cin >> wybor;
        /**< Wyświetlenie dostepnych opcji i oczekiwanie na wybór użytkownika. */
        switch (wybor)
        {
        case '1':
            calculate_rpn();    /**< Po wyborze "1" uruchamia się funkcja obsługująca ONP */
            break;
        case '2':
            calculate_norm();   /**< Po wyborze "2" uruchamia się funkcja obsługująca postać infiksową */
            break;
        case 'q':               /**< Po wyborze "q" program wyłącza się */
            break;
        default:
            printf("wybierz opcje 1 lub 2\n");  /** Po wyborze opcji spoza listy wyświetla się
                                                    *komunikat o dostepnych opcjach i ponownie oczekuje na wybór
                                                    */
            continue;
        }
    }
    while(wybor != 'q');
}

int main()
{
    menu();
    return 0;
}
