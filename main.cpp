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
* A helper pop function to facilitate the use of a vector-based stack.
*/
float pop(vector<float> *vect)
{
    float tmp = vect->back();
    vect->pop_back();
    return tmp;
}

/**
* This function takes an expression provided by the user as a parameter.
* Then the numbers and operators that will remain are extracted from the expression
* used to perform calculations.
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
            result = pop(vect) + pop(vect);           /**< After encountering the operator, it takes two numbers, performs the operation, and pushes the result onto the stack. */
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

    /**< Checks whether any number has been loaded. If not, it returns an exception with code 3. */
    if(!number_read)
        throw 3;

    return pop(vect);   /**\return The result of the expression. */
}

/**
* A function that returns user-readable exception names
*/
string print_exception_description(int e)
{
    string desc;

    switch(e)
    {
    case 1:
        desc = "division by zero";
        break;

    case 2:
        desc = "extra comma in the number";
        break;

    case 3:
        desc = "no numbers loaded";
        break;

    case 4:
        desc = "incorrect number of brackets";
        break;

    case 5:
        desc = "letter detected";
        break;

    default:
        desc = "unknown exception";
    }

    return desc;
}
/**
* \brief Function responsible for converting an expression from infix to postfix form
*
* The converted expression is used to calculate the ONP.
*/
long double calculate(char wyr[])
{
    int bracket=0;         /*!\param bracket a variable used to count occurrences of opening and closing brackets*/
    stringstream oss;       
    stringstream iss;       
    strcat(wyr, " =");    ///we add "=" to the end of the expression with an equal sign

    char stos[256],element[256]; /*!\param stack on stack we store characters like + * etc.*/
    int wsk = 0;                /*!\param wsk the pointer with which we move around the array. If it is set to 0, it means that the stack is empty*/
    oss<<wyr;                    /**< oss<<wyr we write the contents of the array to the auxiliary stream */
    do
    {

        oss >> element;                 /**<We enter the contents of the stream into the table */

        if((element[0] >= 'a' && element[0] <= 'z') || (element[0]>='A' && element[0] <= 'Z'))
            throw 5;                        /**< When a letter is detected, it returns an exception */

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
            iss << element << " ";    /**<If the element is a number, we write it to the stream*/
        }

        else
            switch(element[0])
            {
            case '+':;                                                            /**<------------ If the '+' sign appears, we perform the same instructions as for the '-' sign ------------*/
            case '-':
                if((element[1]>='0'&&element[1]<='9') || element[1]== '.')
                {
                    bool comma = false;
                    iss<<"-";      /**< If there is a number after the '-' sign, write -number to the stream*/
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
                    while(wsk && stos[wsk - 1] != '(')          
                    {
                        wsk--;
                        iss << stos[wsk] << " ";            
                    }

                    stos[wsk++] = element[0];              
                }
                break;
            case '*':               
                ;
            case '/':
                while(wsk && stos[wsk - 1] != '(' && stos[wsk - 1] != '+' &&stos[wsk - 1] != '-')
                {
                    /**<If the top of the stack is different from '(' , '+' , '-' then the loop will execute and print the contents of the stack*/
                    wsk--;
                    iss<<stos[wsk]<<" " ;
                }                                                

                stos[wsk++] = element[0];
                break;
            case '^':              
                while(wsk && stos[wsk - 1] == '^')
                {
                    wsk--;
                    iss << stos[wsk] << " ";
                }

                stos[wsk++] = element[0];     
                break;
            case '(':
                bracket++;
                stos[wsk++] = '(';      
                break;
            case ')':
                bracket--;
                while(stos[wsk - 1] != '(')
                {
                    wsk--;
                    iss << stos[wsk] << " ";
                }

                wsk--;
                break;
            case '=':
                while(wsk)
                {
                    
                    wsk--;
                    iss << stos[wsk] << " ";
                }

                iss << " =\n\n";
                break;
            }

    }
    while(element[0] != '=');
    char conv[256]; 
    iss>>conv;      /**<sending characters from a stream to an array (conv)*/
    if(bracket!=0)
        throw 4;
    strncpy( conv, iss.str().c_str(), sizeof( conv ) );
    cout <<"Wyrazenie po konwersji: "<< conv  ; 
    return parse_and_calculate_rpn(conv);      
}

/**
* The function is used to handle expressions in ONP
*/
void calculate_rpn()
{
    char wyr[256];
    long double ans;
    bool error;
    do
    {
        clear();
        cout << "To exit type \"q\"" << endl << endl;
        error = false;

        cout << "Enter the expression in ONP: ";          
        scanf(" %[^\n]255", wyr);

        
        if(!strcmp(wyr, "q"))
            break;

        
        try
        {
            ans = parse_and_calculate_rpn(wyr);
        }

       
        catch(int e)
        {
            cout << "A problem occured: " << print_exception_description(e) << endl << endl;
            pause();
            error = true;
        }

       
        if(!error)
        {
            cout <<  "Result: " << ans << endl << endl;
            pause();
        }

        cout << endl;

    }
    while(strcmp(wyr, "q"));
}

/**
* The function is used to handle expressions in infix form
*/
void calculate_norm()
{
    char wyr[256];
    long double ans;
    bool error;

    do
    {
        clear();
        cout << "To exit type \"q\"" << endl << endl;
        error = false;

        cout << "Enter the expression : ";      
        scanf(" %[^\n]255", wyr);

       
        if(!strcmp(wyr, "q"))
            break;

        
        try
        {
            ans = calculate(wyr);
        }

        
        catch(int e)
        {
            cout << "A problem occured: " << print_exception_description(e) << endl;
            pause();
            error = true;
        }

        if(!error)
        {
            cout << "Result: " << ans << endl;
            pause();
        }

        cout << endl;

    }
    while(strcmp(wyr, "q"));
}

void menu()
{
    char seletion;
    do
    {
        clear();
        cout << "------------------------------------------------------";
        cout << "\n 1. ONP mode";
        cout << "\n 2. Standard expression mode";
        cout << "\n q. Exit";
        cout << "\n----------------------------------------------------" << endl;
        cin >> seletion;
        
        switch (seletion)
        {
        case '1':
            calculate_rpn();    
            break;
        case '2':
            calculate_norm();   
            break;
        case 'q':               
            break;
        default:
            printf("select options 1 or 2\n");  
            continue;
        }
    }
    while(seletion != 'q');
}

int main()
{
    menu();
    return 0;
}
