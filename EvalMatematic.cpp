#include <iostream>
#include <cstring>
#include <winbgim.h>
#include <graphics.h>
#include <fstream>
#include <math.h>
#include <limits.h>
#include <stack>
#include <queue>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define infinite FLT_MAX
#define epsilon 0.0001
const float pi = 3.14159;

using namespace std;

// BACKEND PENTRU EVALUAREA SI CALCULUL EXPRESIEI

int error_cnt = 0;
string errors[15];
double errCode = -9999999.999999;

bool checkUsualFunction(char a, char b, char c){
    if(a == 'e' && b == 'x' && c == 'p')
        return true;
    else if(a == 'a' && b == 'b' && c == 's')
        return true;
    else if(a == 's' && b == 'q' && c == 'r')
        return true;
    else if(a == 's' && b == 'i' && c == 'n')
        return true;
    else if(a == 'c' && b == 'o' && c == 's')
        return true;
    else return false;
}

bool checkLogarithm(char a, char b){
    if(a == 'l' && b == 'n')
        return true;
    else return false;
}

void addSpaces(char str[], int position){
    char aux[256];

    strcpy(aux, str + position);
    strcpy(str + position + 1, aux);
    str[position] = ' ';
}

void addSpacesToExpr(char str[]){
    for(int i = 0; str[i]; i++){
        if(strchr("+-/*^<>=#()", str[i])){
            addSpaces(str, i);
            i++;
        }

        if(checkUsualFunction(str[i], str[i + 1], str[i + 2]) == true){
            addSpaces(str, i);
            i += 2;
        }

        if(checkLogarithm(str[i], str[i + 1]) == true){
            addSpaces(str, i);
            i++;
        }
    }

    for(int i = 0; str[i]; i++){
        if(strchr("+-*/^<>=#()", str[i]) && str[i + 1] != ' ')
            addSpaces(str, i + 1);

        if(checkUsualFunction(str[i], str[i + 1], str[i + 2]) == true && str[i + 3] != ' ')
            addSpaces(str, i + 3);

        if(checkLogarithm(str[i], str[i + 1]) == true && str[i + 2] != ' ')
            addSpaces(str, i + 2);
    }
}

int priority(char a){
    if(a == '(' || a == ')') return 0;
    if(a == '+' || a == '-') return 1;
    if(a == '*' || a == '/') return 2;
    if(a == '=' || a == '#' || a == '<' || a == '>') return 4;
    if(a == 'e' || a == 'a' || a == 's' || a == 'c' || a == 'l' || a == 'q') return 5;
}

bool checkInfValue(float x){
    if(fabs(infinite - fabs(x)) > infinite / 2.0)
        return 1;
    else return 0;
}

float logarithm(float x){
    if(x > epsilon && checkInfValue(x))
        return log(x);
    else if(x <= 0){
        cout << "Atentie! Pentru valoarea oferita, expresia va efectua logaritmarea unei valori mai mici sau egale cu 0!";
        error_cnt++;
        errors[error_cnt] = "Atentie! Pentru valoarea oferita, expresia va efectua logaritmarea unei valori mai mici sau egale cu 0!";
    }
}

float multiplication(float a, float b){
    if(checkInfValue(a) && checkInfValue(b) && checkInfValue(a * b))
        return a * b;
    else return infinite;
}

float division(float a, float b){
    if(b != 0) return a / b;
    else if(b == 0){
        cout << "Atentie! Pentru valoarea oferita, expresia va efectua o impartire la 0!";
        error_cnt++;
        errors[error_cnt] = "Atentie! Pentru valoarea oferita, expresia va efectua o impartire la 0!";
    }
}

float squareRoot(float x){
    if(checkInfValue(x) && x >= 0)
        return sqrt(x);
    else if(x < 0){
        cout << "Atentie! Pentru valoarea oferita, expresia va efectua un radical pentru o valoare negativa";
        error_cnt++;
        errors[error_cnt] = "Atentie! Pentru valoarea oferita, expresia va efectua un radical pentru o valoare negativa";
    }
}

float eToPower(float x){
    if(checkInfValue(x))
        return exp(x);
    else return infinite;
}

float power(float a, float b){
    if(a == 0) return 0;
    else if(b == 0) return 1;
    else if(a == infinite || b == infinite) return infinite;
    else return pow(a, b);
}

float addition(float a, float b){
    if(checkInfValue(a) && checkInfValue(b))
        return a + b;
    else return infinite;
}

float subtraction(float a, float b){
    if(checkInfValue(a) && checkInfValue(b))
        return a - b;
    else return infinite;
}

bool equality(float a, float b){
    if(a == b) return 1;
    else return 0;
}

bool inequality(float a, float b){
    return !(equality(a, b));
}

bool greaterThan(float a, float b){
    if(a > b) return 1;
    else return 0;
}

bool smallerThan(float a, float b){
    return !greaterThan(a, b);
}

float sinus(float x){
    if(checkInfValue(x))
        return sin(x);
    else return infinite;
}

float cosinus(float x){
    if(checkInfValue(x))
        return cos(x);
    else return infinite;
}

float absoluteValue(float x){
    if(checkInfValue(x))
        if(x >= 0) return x;
        else return -x;
    else return infinite;
}

void string2char(string strn, char aux[256]){
    string::iterator i;
    int cnt = 0;

    for(i = strn.begin(); i != strn.end(); i++){
        aux[cnt] = *i;
        cnt++;
    }

    aux[cnt] = '\0';
}

bool checkIfNum(char num[256]){
    return (atof(num) != 0.0 && strchr("0123456789", num[0]));
}

double valueOfX;

void cloneExpression(char exp[256], char aux[256]){
    strcpy(aux, exp);
    if(aux[strlen(aux) - 1] != ' ') strcat(aux, " ");
    strcat(aux, ")");
}

void readExp(char exp[256]){
    cout << "Introduceti expresia dorita, cu mentiunea ca nu trebuie sa folositi caracterul spatiu: "<<endl;

    cin.get(exp, 100);

    for(int i = 0; exp[i]; i++)
        if(exp[i] == ' '){
            cout << "Eroare. Ati introdus caracterul spatiu!";
            exit(1);
        }
}

void readX_value(){
    double inpt;

    //cout << endl;
    cout << "Introduceti x pentru care doriti sa calculati valoarea expresiei: ";
    cin >> inpt;

    valueOfX = inpt;
}

void checkExpFunctions(char exp[256]){
    for(int i = 0; i < exp[i]; i++)
    {
        if(exp[i] == 's' && exp[i + 1] == 'i' && exp[i + 2] == 'n')
            if(exp[i + 3] != '(')
            {
                cout << "Ai vrut sa scrii sin()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii sin()?";
            }
        if(exp[i] != 's' && exp[i + 1] == 'i' && exp[i + 2] == 'n')
        {
                cout << "Ai vrut sa scrii sin()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii sin()?";
        }
        else if(exp[i] == 's' && exp[i + 1] != 'i' && exp[i + 2] == 'n')
        {
                cout << "Ai vrut sa scrii sin()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii sin()?";
        }
        else if(exp[i] == 's' && exp[i + 1] == 'i' && exp[i + 2] != 'n')
        {
                cout << "Ai vrut sa scrii sin()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii sin()?";
        }

        if(exp[i] == 'c' && exp[i + 1] == 'o' && exp[i + 2] == 's')
            if(exp[i + 3] != '(')
            {
                cout << "Ai vrut sa scrii cos()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii cos()?";
            }
        if(exp[i] == 'c' && exp[i + 1] != 'o' && exp[i + 2] == 's')
        {
                cout << "Ai vrut sa scrii cos()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii cos()?";
        }
        else if(exp[i] == 'c' && exp[i + 1] == 'o' && exp[i + 2] != 's')
        {
                cout << "Ai vrut sa scrii cos()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii cos()?";
        }
        else if(exp[i] != 'c' && exp[i + 1] == 'o' && exp[i + 2] == 's')
        {
                cout << "Ai vrut sa scrii cos()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii cos()?";
        }

        if(exp[i] == 'e' && exp[i + 1] == 'x' && exp[i + 2] == 'p')
            if(exp[i + 3] != '(')
            {
                cout << "Ai vrut sa scrii exp()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii exp()?";
            }
        if(exp[i] != 'e' && exp[i + 1] == 'x' && exp[i + 2] == 'p')
        {
                cout << "Ai vrut sa scrii exp()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii exp()?";
        }
        else if(exp[i] == 'e' && exp[i + 1] != 'x' && exp[i + 2] == 'p')
        {
                cout << "Ai vrut sa scrii exp()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii exp()?";
        }
        else if(exp[i] == 'e' && exp[i + 1] == 'x' && exp[i + 2] != 'p')
        {
                cout << "Ai vrut sa scrii exp()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii exp()?";
        }

        if(exp[i] == 'a' && exp[i + 1] == 'b' && exp[i + 2] == 's')
            if(exp[i + 3] != '(')
            {
                cout << "Ai vrut sa scrii abs()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii abs()?";
            }
        if(exp[i] != 'a' && exp[i + 1] == 'b' && exp[i + 2] == 's')
        {
                cout << "Ai vrut sa scrii abs()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii abs()?";
        }
        else if(exp[i] == 'a' && exp[i + 1] != 'b' && exp[i + 2] == 's')
        {
                cout << "Ai vrut sa scrii abs()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii abs()?";
        }
        else if(exp[i] == 'a' && exp[i + 1] == 'b' && exp[i + 2] != 's')
        {
                cout << "Ai vrut sa scrii abs()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii abs()?";
        }

        if(exp[i] == 'l' && exp[i + 1] == 'n')
            if(exp[i + 2] != '(')
            {
                cout << "Ai vrut sa scrii ln()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii ln()?";
            }
        if(exp[i + 1] != 'l' && exp[i + 2] == 'n' && exp[i] != 's'){
                cout << "Ai vrut sa scrii ln()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii ln()?";
        }
        else if(exp[i] == 'l' && exp[i + 1] != 'n'){
                cout << "Ai vrut sa scrii ln()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii ln()?";
        }

        if(exp[i] == 's' && exp[i + 1] == 'q' && exp[i + 2] == 'r')
            if(exp[i + 3] != '(')
            {
                cout << "Ai vrut sa scrii sqr()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii sqr()?";
            }
         if(exp[i] != 's' && exp[i + 1] == 'q' && exp[i + 2] == 'r')
         {
                cout << "Ai vrut sa scrii sqr()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii sqr()?";
         }
         else if(exp[i] == 's' && exp[i + 1] != 'q' && exp[i + 2] == 'r')
         {
                cout << "Ai vrut sa scrii sqr()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii sqr()?";
         }
         else if(exp[i] == 's' && exp[i + 1] != 'q' && exp[i + 2] == 'r')
         {
                cout << "Ai vrut sa scrii sqr()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii sqr()?";
         }
         else if(exp[i] == 's' && exp[i + 1] == 'q' && exp[i + 2] != 'r')
         {
                cout << "Ai vrut sa scrii sqr()?" << endl;
                error_cnt++;
                errors[error_cnt] = "Ai vrut sa scrii sqr()?";
         }
    }
}

void checkExpParanthesis(char exp[256]){

    stack<char> Stack;
    int ok = 0;
    char brace,topBrace;
    for(int i = 0; i < strlen(exp); i++)
    {
        brace = exp[i];
        if(brace == '(')
        {
            Stack.push(brace);
        }
        else if(brace == ')')
        {
            if(Stack.empty())
            {
                    cout << "Eroare! Grija la paranteze!" << endl;
                    error_cnt++;
                    errors[error_cnt] = "Eroare! Grija la paranteze!";
            }
            Stack.pop();
        }
    }

    if(!Stack.empty()){
        cout << "Eroare! Grija la paranteze!" << endl;
        error_cnt++;
        errors[error_cnt] = "Eroare! Grija la paranteze!";
        //exit(1);
    }
}

void checkExpOperators(char exp[256]){

    int i = 0, n = 0;

    char faux[256];
    strcpy(faux, exp);
    addSpacesToExpr(faux);

    char * p;
    p = strtok(faux," ");
    while(p != NULL)
    {
        n = strlen(p);
        if(!strchr("+*-/0123456789()", p[0]) && !checkUsualFunction(p[0], p[1], p[2]))
        {
            if(p[0] == p[1]){
                cout << "Lipseste un operator! Nu pot exista decat variabile simple." << endl;
                error_cnt++;
                errors[error_cnt] = "Lipseste un operator! Nu pot exista decat variabile simple.";
            }
        }
        p = strtok(NULL," ");
    }
}

void checkIfOperatorExists(char exp[256]){
    char aux[256];

    cloneExpression(exp, aux);
    addSpacesToExpr(aux);

    char * p;
    p = strtok(aux, " ");
    int k = 0;

    while(p != NULL){
        if(strlen(p) > 1){
            for(int i = 0; p[i]; i++)
                if(p[i] == 'x' && !(p[i + 1] == 'p' || p[i - 1] == 'e')){
                    cout << "Atentie! Lipseste cel putin un operator pentru operandul x de pe pozitia " << i + 1;
                    error_cnt++;
                    errors[error_cnt] = "Atentie! Lipseste cel putin un operator!";
                }
        }
        p = strtok(NULL, " ");
    }
}

void checkExpVariablesX(char exp[256]){
    bool ok = 1;
    char *p;
    char aux[256];
    cloneExpression(exp, aux);
    addSpacesToExpr(aux);
    p = strtok(aux, " ");
    while(p != NULL)
    {
        if(strlen(p) == 1 && !strchr("+*-^=#/0123456789()xp<>" ,p[0]) && p[0]!='x')
        {
            cout << p << " nu este recunoscuta ca fiind o variabila. ";
            cout << "Numele variabilelor trebuie sa fie x!" << endl;
            error_cnt++;
            errors[error_cnt] = "Numele variabilelor trebuie sa fie x!";
        }
        p = strtok(NULL, " ");
    }
}

void checkIfExpReturnsValue(char exp[256]){
    bool ok = 1;
    bool checkNum = 0;
    char aux[256];

    for(int i = 0; exp[i]; i++)
        aux[i] = exp[i];

    for(int i = 0; aux[i]; i++)
        if(strchr("0123456789xp", aux[i]))
            checkNum = 1;

    for(int i = 0; aux[i]; i++)
        if(strchr("=#+*/^", aux[0]))
            checkNum = 0;

    if(checkNum == 0){
        cout << "Eroare! Ati introdus o expresie gresita!";
        error_cnt++;
        errors[error_cnt] = "Eroare! Ati introdus o expresie gresita!";
        //exit(1);
    }
}

void checkIfOperatorsAreOk(char exp[256]){
    char aux[256];
    strcpy(aux, exp);

    for(int i = 0; aux[i]; i++){
        if(strchr("+-=#<>*/^", aux[i]) != 0 && strchr("+-=#<>*/^)", aux[i + 1]) != 0){
            cout << "Eroare! Ati introdus o expresie gresita!";
            error_cnt++;
            errors[error_cnt] = "Eroare! Ati introdus o expresie gresita!";
        }
    }

    if(strchr("+-=#<>*/^", aux[strlen(aux) - 1]) != 0){
         cout << "Eroare! Ati introdus o expresie gresita!";
            error_cnt++;
            errors[error_cnt] = "Eroare! Ati introdus o expresie gresita!";
    }
}

void checkDivBy0(char exp[256]){
    bool ok = 1;
    for(int i = 0; exp[i]; i++){
        if(exp[i] == '/' && exp[i + 1] == '0')
            ok = 0;
    }

    if(ok == 0){
        cout << "Atentie! Nu se poate efectua o impartire la 0!";
        error_cnt++;
        errors[error_cnt] = "Atentie! Nu se poate efectua o impartire la 0!";
    }
}

void checkExpLength(char exp[256]){
    if(strlen(exp) > 30){
        error_cnt++;
        errors[error_cnt] = "Atentie! Ati depasit lungimea maxima a expresiei!";
        cout << "Atentie! Ati depasit lungimea maxima a expresiei!";
    }
}

void check4Mistakes(char exp[256]){
    checkExpVariablesX(exp);
    checkIfExpReturnsValue(exp);
    checkExpOperators(exp);
    checkExpParanthesis(exp);
    checkIfOperatorExists(exp);
    checkIfOperatorsAreOk(exp);
    checkExpFunctions(exp);
    checkDivBy0(exp);
    checkExpLength(exp);
}

void solveMinus(char exp[256]){
    char aux[256];
    for(int i = 0; exp[i]; i++){
        if(exp[i] == '(' && exp[i + 1] == '-' && strchr("0123456789xp", exp[i + 2])){
            strcpy(aux, exp + i + 1);
            strcpy(exp + i + 4, aux);
            exp[i + 1] = '1';
            exp[i + 2] = '-';
            exp[i + 3] = '1';
        }
    }
}

void pleaseWork(char exp[256]){
    char aux[256];
    for(int i = 0; exp[i]; i++){
        if(exp[i] == '0' && strchr("123456789", exp[i - 1]) == 0 && exp[i + 1] != '.'){
            strcpy(aux, exp + i);
            strcpy(exp + i + 4, aux);
            exp[i] = '(';
            exp[i + 1] = '1';
            exp[i + 2] = '-';
            exp[i + 3] = '1';
            exp[i + 4] = ')';
        }
    }
}

void createInfixQueue(queue<string> &qu, char exp[256]){
    char aux[256];
    cloneExpression(exp, aux);

    while(!qu.empty())
        qu.pop();

   char * p;
   p = strtok(aux, " ");
   while(p != NULL){
        if(strlen(p) == 1) qu.push(p);
        else{
            if(!strcmp("sin", p)) qu.push("s");
            else if(!strcmp("cos", p)) qu.push("c");
            else if(!strcmp("sqr", p)) qu.push("q");
            else if(!strcmp("abs", p)) qu.push("a");
            else if(!strcmp("exp", p)) qu.push("e");
            else if(!strcmp("ln", p)) qu.push("l");
            else qu.push(p);
        }

        p = strtok(NULL, " ");
   }
}

double opd[100]; //stiva operanzilor
int topd;   // varful stivei operatorilor
char op[100];    // stiva operatorilor
int topp;   // varful stivei operanzilor

double expValue(char exp[256], double xval){
    int i;
    double value;
    double a, b;

    for(i = 1; i <= 100; i++){
        opd[i] = 0;
        op[i] = 'N';
    }

    topd = 0;
    topp = 1;

    op[topp] = '(';

    queue<string> myqueue;
    createInfixQueue(myqueue, exp);

    char auxx[256];
    while(!myqueue.empty() && topp > 0){
        string2char(myqueue.front(), auxx);
        myqueue.pop();

        if(checkIfNum(auxx)){
            topd++;
            value = atof(auxx);
            opd[topd] = value;
        }
        else
            switch(auxx[0]){
                case 'p' : topd++; opd[topd] = pi; break;
                case 'x' : topd++; opd[topd] = xval; break;
                case '(' : topp++; op[topp] = '('; break;
                default :
                    while(topp > 0 && !strchr("()", op[topp]) && priority(op[topp]) >= priority(auxx[0])){
                        if(topd > 1)
                            a = opd[topd - 1];
                        b = opd[topd];
                        switch(op[topp]){
                            case '=' : value = equality(a, b); break;
                            case '#' : value = inequality(a, b); break;
                            case '<' : value = smallerThan(a, b); break;
                            case '>' : value = greaterThan(a, b); break;
                            case '+' : value = addition(a, b); break;
                            case '-' : value = subtraction(a, b); break;
                            case '*' : value = multiplication(a, b); break;
                            case '/' : value = division(a, b); break;
                            case '^' : value = power(a, b); break;
                            case 'q' : value = squareRoot(b); break;
                            case 'e' : value = eToPower(b); break;
                            case 'l' : value = logarithm(b); break;
                            case 'a' : value = absoluteValue(b); break;
                            case 's' : value = sinus(b); break;
                            case 'c' : value = cosinus(b); break;
                        }
                        if(strchr("+-*/^<>=#", op[topp]))
                            topd--;
                        if(strchr("+-*/^<>=#qelasc", op[topp]))
                            opd[topd] = value;
                        topp--;
                    }
                    if(topp > 0)
                        if(op[topp] != '(' || strcmp(auxx, ")")){
                                topp++;
                                op[topp] = auxx[0];
                        }
                        else topp--;
            }
    }
    if(topp == 0)
        return opd[1];
    else{
        cout << "Eroare! Verificati expresia!";
        exit(0);
    }
}

// BACKEND PENTRU CONSTRUCTIA ARBORELUI

int prio4Postfix(char a){
    if(a == '(' || a == ')') return 5;
    if(a == 'e' || a == 'a' || a == 's' || a == 'c' || a == 'l' || a == 'q' || a == '^') return 4;
    if(a == '*' || a == '/') return 3;
    if(a == '+' || a == '-' || a == '=' || a == '#' || a == '>' || a == '<') return 2;
}

void createInfixQueueForTree(queue<string> &qu, char exp[256]){
    char aux[256];
    strcpy(aux, exp);

    while(!qu.empty())
        qu.pop();

    char * p;
    p = strtok(aux, " ");
    while(p != NULL){
        if(strlen(p) == 1) qu.push(p);
        else{
            if(!strcmp("sin", p)) qu.push("s");
            else if(!strcmp("cos", p)) qu.push("c");
            else if(!strcmp("sqr", p)) qu.push("q");
            else if(!strcmp("abs", p)) qu.push("a");
            else if(!strcmp("exp", p)) qu.push("e");
            else if(!strcmp("ln", p)) qu.push("l");
            else qu.push(p);
        }

        p = strtok(NULL, " ");
    }
}

void infix2postfix(queue<string> &infix, queue<string> &postfix, char exp[256]){
    while(!postfix.empty())
        postfix.pop();

    stack<string> stk;
    while(!stk.empty())
        stk.pop();

    createInfixQueueForTree(infix, exp);
    string strn;

    while(!infix.empty()){
        strn = infix.front();
        infix.pop();
        if(strchr("scqael+-/*^<>=#()", *strn.begin()) == 0)
            postfix.push(strn);
        else
            if(*strn.begin() == ')'){
                while(*stk.top().begin() != '('){
                    postfix.push(stk.top());
                    stk.pop();
                }
                stk.pop();
            }
            else{
                while(!stk.empty() && *stk.top().begin() != '(' && prio4Postfix(*stk.top().begin()) >= prio4Postfix(*strn.begin())){
                    postfix.push(stk.top());
                    stk.pop();
                }
                stk.push(strn);
            }
    }

    while(!stk.empty()){
        postfix.push(stk.top());
        stk.pop();
    }
}

void deleteParanthesis(char exp[256]){
    char aux[256];
    for(int i = 0; exp[i]; i++){
        if(exp[i] == '(' || exp[i] == ')'){
            strcpy(aux, exp + i + 1);
            strcpy(exp + i, aux);
        }
    }
}

void infixWithoutParanthesis(queue<string> &infix, char exp[256]){
    char aux[256];
    strcpy(aux, exp);
    deleteParanthesis(aux);

    while(!infix.empty())
        infix.pop();

    createInfixQueueForTree(infix, aux);
}

void traversals(char exp[256], queue<string> &infix, queue<string> &postfix, string IO[25], string PO[25], int &lengthOfIO, int &lengthOfPO){
    char aux[256];
    strcpy(aux, exp);
    createInfixQueueForTree(infix, aux);
    infix2postfix(infix, postfix, exp);

    strcpy(aux, exp);
    infixWithoutParanthesis(infix, aux);

    lengthOfIO = 0;
    lengthOfPO = 0;
    while(!infix.empty()){
        IO[lengthOfIO] = infix.front();
        infix.pop();
        lengthOfIO++;
    }
    while(!postfix.empty()){
        PO[lengthOfPO] = postfix.front();
        postfix.pop();
        lengthOfPO++;
    }
}

struct TreeNode{
    string val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string data){
        val = data;
        left = NULL;
        right = NULL;
    }
};

TreeNode* construct_tree(string inorder[25], string postorder[25], char exp[256]){
    stack<TreeNode*> st;

    queue<string> infix;
    queue<string> postfix;

    int sIO = 0, sPO = 0;
    traversals(exp, infix, postfix, inorder, postorder, sIO, sPO);

    int postIndex = sPO - 1;
    int inIndex = sIO - 1;
    int flag = 0;

    TreeNode* root = new TreeNode(postorder[postIndex]); // root ia valoarea ultimului element din postorder
    --postIndex;
    st.push(root);
    TreeNode *prev = root;

    while (postIndex >= 0){
        if (!st.empty() && inorder[inIndex] == st.top()->val){
            prev = st.top();
            st.pop();
            --inIndex;
            flag = 1;
        }
        else{
            TreeNode* node = new TreeNode(postorder[postIndex]);
            if (flag == 0){
                prev->right = node;
                prev = prev->right;
            }
            else{
                prev->left = node;
                prev = prev->left;
                flag = 0;
            }
            st.push(node);
            --postIndex;
        }
    }
    return root;
}

void restoreNodeValue(TreeNode *root){
    char aux[256];
    strcpy(aux, root->val.c_str());
    if(aux[0] == 's') root->val = "sin";
    else if(aux[0] == 'c') root->val = "cos";
    else if(aux[0] == 'a') root->val = "abs";
    else if(aux[0] == 'e') root->val = "exp";
    else if(aux[0] == 'q') root->val = "sqr";
    else if(aux[0] == 'l') root->val = "ln";

    if(root->left != NULL) restoreNodeValue(root->left);
    if(root->right != NULL) restoreNodeValue(root->right);
}

void inorder_traversal(TreeNode *node){
    if (node == NULL)
        return;
    inorder_traversal(node->left);
    cout << node->val;
    inorder_traversal(node->right);
}

void postorder_traversal(TreeNode *node){
    if (node == NULL)
        return;
    postorder_traversal(node->left);
    postorder_traversal(node->right);
    cout << node->val;
}

void BFS_traversal(TreeNode* root){
    if (root == NULL)
        return;

    queue<TreeNode*> q;

    q.push(root);

    while (q.empty() == false) {
        TreeNode* node = q.front();
        cout << node->val;
        q.pop();

        if (node->left != NULL)
            q.push(node->left);

        if (node->right != NULL)
            q.push(node->right);

    }
}

// FRONTEND PENTRU CONSTRUCTIA ARBORELUI

int treeHeight(TreeNode* root){
    if(root == NULL)
        return 0;
    else{
        int lt = treeHeight(root->left);
        int rt = treeHeight(root->right);
        if(lt > rt) return (lt + 1);
        else return (rt + 1);
    }
}

int treeWidth(TreeNode* root){
    if(root == NULL)
        return 0;
    else{
        int lt = treeWidth(root->left);
        int rt = treeWidth(root->right);
        return 1 + lt + rt;
    }
}

void drawNode(string strn, int xc, int yc, int width, int height){
    char arr[50], aux[50];
    string2char(strn, arr);

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(xc, yc, textwidth(arr) + 2, textheight("M")/2 + 6);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setbkcolor(WHITE);
    setcolor(BLACK);
    settextjustify(1, 1);
    outtextxy(xc, yc + 4, arr);
}

void drawTree(TreeNode* root, int lev, int lt, int width, int height){
    if(root != NULL){
        int n1 = treeWidth(root->left);
        int xc = lt + width * n1 + width / 2;
        int yc = lev * height - height / 2;

        if(root->left != NULL){
            int xcl = lt + width * treeWidth(root->left->left) + width / 2;
            setcolor(WHITE);
            line(xc, yc, xcl, yc + height);
        }
        if(root->right != NULL){
            int xcr = lt + width * (n1 + 1) + width * treeWidth(root->right->left)+ width / 2;
            setcolor(WHITE);
            line(xc, yc, xcr, yc + height);
        }
        drawNode(root->val, xc, yc, width, height);
        drawTree(root->left, lev + 1, lt, width, height);
        drawTree(root->right, lev + 1, lt + width * (n1 + 1), width, height);
    }
}

// FRONTEND - UI

string strn;
string valueX;
int cx, cy;

void string2int(char aux[256]){
    strcpy(aux, valueX.c_str());
    valueOfX = atof(aux);
}

char auxStr[256], auxVal[256], auxShow[256];
void showValue(){
    char arr[256], err[256];
    string2int(auxVal);
    strcpy(auxStr, strn.c_str());
    strcpy(arr, auxStr);
    strcpy(err, arr);
    check4Mistakes(err);
    solveMinus(arr);
    pleaseWork(arr);
    addSpacesToExpr(arr);

    double solution = 0;
    if(error_cnt != 0) solution = errCode;
    else solution = expValue(arr, valueOfX);

    sprintf(auxShow, "%f", solution);
    setcolor(BLACK);
}

char treeStr[256];
int nrApp = 0;
int winApp;
int winTree;
void showTree(){
    char arr[256], aux[256];

    strcpy(treeStr, strn.c_str());
    strcpy(arr, treeStr);

    addSpacesToExpr(arr);
    cloneExpression(arr, aux);

    queue<string> infix;
    queue<string> postfix;
    string PO[25], IO[25];

    createInfixQueueForTree(infix, arr);
    infix2postfix(infix, postfix, arr);
    infixWithoutParanthesis(infix, arr);

    int sPO, sIO;
    traversals(arr, infix, postfix, IO, PO, sIO, sPO);

    TreeNode* root = construct_tree(IO, PO, arr);
    restoreNodeValue(root);

    int heightConsole = 800, widthConsole = 800;
    if(nrApp == 0){
        initwindow(800, 800, "ARBORE");
        nrApp++;
        winTree = getcurrentwindow();
    }
    setcurrentwindow(winTree);
    setcolor(WHITE);
    setbkcolor(BLACK);
    cleardevice();
    int width, height;
    width = widthConsole / treeWidth(root);
    height = heightConsole / treeHeight(root);
    drawTree(root, 1, 0, width, height);
    //getch();
    //closegraph();
}

bool k = false;
void redoExpression(){
    k = false;
    strn.clear();
    valueX.clear();
    auxShow[0] = '\0';
    for(int i = 0; i < error_cnt; i++){
        errors[i].clear();
    }
    error_cnt = 0;
}

bool opn = true;
bool exitApp(){
    opn = false;
}

void expFromFile(){
    redoExpression();
    char aux[256];
    FILE * fl;
    fl = fopen("expresii.txt", "r");
    char exp[256];
    fscanf(fl, "%s", &aux);
    strn += aux;
    fclose(fl);
}

void giveInput(){
    if(kbhit() && k == false){
        char key = getch();
        if(key != 0){
            if(key == '\b' && strn.length() != 0){
                strn.erase(strn.end() - 1);
            }
            else if(key != '\b' && key != '\r' && strn.length() < 30 && key != ' ')
                strn += key;
            else if(key == '\r')
                k = true;
        }
    }

    if(kbhit() && k == true){
        char key = getch();
        if(key != 0){
            if(key == '\b' && valueX.length() != 0){
                valueX.erase(valueX.end() - 1);
            }
            else if(key != '\b' && strchr("0123456789.-", key))
                valueX += key;
        }
    }

    bool ok = 0;
    if(ismouseclick(WM_LBUTTONDOWN)){
        ok = 1;
        getmouseclick(WM_LBUTTONDOWN, cx, cy);
    }
    else ok = 0;

    if(ok == 1){
        if(cx >= 100 && cx <= 250 && cy >= 300 && cy <= 400){
            showValue();
        }
        if(cx >= 251 && cx <= 400 && cy >= 300 && cy <= 400){
            if(strn.length() != 0 && error_cnt == 0)
                showTree();
        }
        if(cx >= 1 && cx <= 99 && cy >= 301 && cy <= 400){
            redoExpression();
        }
        if(cx >= 401 && cx <= 500 && cy >= 300 && cy <= 400){
            exitApp();
        }

        if(cx >= 1 && cx <= 99 && cy >= 250 && cy <= 300){
            expFromFile();
        }
    }
}

int page = 0;
void actions(){
    if(page == 0) page = 1;
    else page = 0;
}

void output(){
    setcurrentwindow(winApp);
    setvisualpage(page);
    setactivepage(!page);
    setcolor(WHITE);
    cleardevice();
    setcolor(RED);
    char aux[256];
    strcpy(aux, strn.c_str());
    outtextxy(120, 268, aux);

    char aux_val[256];
    strcpy(aux_val, valueX.c_str());
    setcolor(BLACK);
    outtextxy(410, 267, aux_val);

    setcolor(RED);
    rectangle(100, 250, 400, 300);
    if(strn.length() == 0)
        outtextxy(105, 267, "Scrieti expresia aici");

    setcolor(BLACK);
    rectangle(401, 250, 499, 300);
    if(valueX.length() == 0)
        outtextxy(410, 267, "x = ");

    setcolor(MAGENTA);
    rectangle(100, 301, 250, 400);
    outtextxy(110, 346, "Valoarea Expresiei");

    setcolor(BLUE);
    rectangle(251, 301, 400, 400);
    outtextxy(270, 346, "Arborele Expresiei");

    outtextxy(25, 460, "Proiect Realizat De: ");
    setcolor(GREEN);
    outtextxy(25, 475, "Magda Tautu");
    setcolor(RED);
    outtextxy(150, 475, "Valentin Istrate");

    setcolor(GREEN);
    rectangle(100, 201, 400, 249);
    setcolor(GREEN);
    outtextxy(105, 219, "Rezultatul este: ");
    outtextxy(210, 219, auxShow);

    if(error_cnt != 0){
        int cnt;
        int x = 10, y = 10;
        for(cnt = 1; cnt <= error_cnt; cnt++){
            char auxi[256];
            strcpy(auxi, errors[cnt].c_str());
            outtextxy(x, y, auxi);
            y += 20;
        }
    }

    setcolor(BROWN);
    rectangle(1, 301, 99, 400);
    outtextxy(30, 345, "Redo");

    setcolor(DARKGRAY);
    rectangle(401, 301, 499, 400);
    outtextxy(440, 345, "Exit");

    setcolor(BLACK);
    rectangle(1, 250, 99, 300);
    outtextxy(10, 267, "expresii.txt");
}

void solveExpression(){
    char str[256], aux[256];
    readExp(str);
    check4Mistakes(str);
    addSpacesToExpr(str);
    cloneExpression(str, aux);

    queue<string> myqueue;
    createInfixQueue(myqueue, str);

    readX_value();
    double solution;

    solution = expValue(str, valueOfX);
    cout << solution << endl;

    queue<string> infix;
    queue<string> postfix;
    string PO[25], IO[25];

    createInfixQueueForTree(infix, str);
    infix2postfix(infix, postfix, str);
    infixWithoutParanthesis(infix, str);

    int sPO, sIO;
    traversals(str, infix, postfix, IO, PO, sIO, sPO);

    TreeNode* root = construct_tree(IO, PO, str);
    inorder_traversal(root);
    cout << endl;
    postorder_traversal(root);
    restoreNodeValue(root->left);
    cout << endl;
    cout << treeHeight(root->left);
    cout << endl;
    cout << treeWidth(root->left);
    cout << endl;
    BFS_traversal(root->left);

    int heightConsole = 800, widthConsole = 800;
    initwindow(heightConsole, widthConsole);
    setcolor(WHITE);
    setbkcolor(BLACK);
    cleardevice();
    int width, height;
    width = widthConsole / treeWidth(root);
    height = heightConsole / treeHeight(root);
    drawTree(root->left, 1, 0, width, height);
    getch();
    closegraph();
}

int main(){
    initwindow(500, 500, "Evaluator Matematic");
    winApp = getcurrentwindow();
    setbkcolor(WHITE);
    setcolor(BLUE);
    cleardevice();

    while(opn){
        giveInput();
        actions();
        output();
    }
    closegraph();

    return 0;
}
