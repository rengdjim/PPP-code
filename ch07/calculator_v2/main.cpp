#include <iostream>

#include "lexer.h"
#include "parser.h"

/*
    Simple calculator

    This program implements a basic expression calculator.
    Input from cin; output to cout.

    The grammar for input is:

    Statement:
        Expression
        Print
        Quit
    Print:
        ";"
    Quit:
        "q"
    Expression:
        Term
        Expression "+" Term
        Expression "-" Term
    Term:
        Primary
        Term "*" Primary
        Term "/" Primary
        Term "%" Primary
    Primary:
        Number
        "(" Expression ")"
        "+" Primary
        "-" Primary
    Number:
        floating-point-literal

    Input comes from cin through the Token_stream called ts.
*/

using namespace std;

const string prompt = "> ";     // prompt user input
const string result = "= ";     // used to indicate that what follows is a result

Token_stream ts(cin);
Parser parser(ts);

void calculate();
void clean_up_mess();

int main() {
    calculate();
    return 0;
}

// expression calculation loop
void calculate() {
    while (cin) {
        try {
            cout << prompt;
            Token t = ts.get();
            while (t.kind == print)
                t = ts.get();
            if (t.kind == quit)
                break;
            ts.putback(t);
            cout << result << parser.expression() << endl;
        }
        catch (exception& e) {
            cerr << e.what() << endl;
            clean_up_mess();
        }
    }
}

void clean_up_mess() {
    ts.ignore(print);
}
