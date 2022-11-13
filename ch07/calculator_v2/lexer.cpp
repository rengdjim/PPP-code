#include "lexer.h"

// make a Token_stream that reads from istream
Token_stream::Token_stream(std::istream& is)
        :is(is), full(false), buffer{0} {
}

// get a Token
Token Token_stream::get() {
    if (full) {     // do we already have a Token ready?
        // remove Token from buffer
        full = false;
        return buffer;
    }

    char ch;
    is >> ch;   // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch) {
        case print:
        case quit:
        case '(': case ')': case '+': case '-': case '*': case '/': case '%': case '=':
            return Token(ch);   // let each character represent itself
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            is.putback(ch);         // put digit back into the input stream
            double val;
            is >> val;              // read a floating-point number
            return Token(number, val);
        }
        default:
            if (std::isalpha(ch)) {
                std::string s;
                s += ch;
                while (is.get(ch) && std::isalnum(ch))
                    s += ch;
                is.putback(ch);
                if (s == declkey)
                    return Token(let);
                return Token(name, s);
            }
            throw Lexer_error("Bad token");
    }
}

// put a Token back
void Token_stream::putback(Token t) {
    if (full)
        throw Lexer_error("putback() into a full buffer");
    buffer = t;     // copy t to buffer
    full = true;    // buffer is now full
}

// discard Token in buffer
void Token_stream::ignore() {
    full = false;
}

// discard characters up to and including c
void Token_stream::ignore(char c) {
    // first look in buffer
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;

    // now search input
    char ch;
    while (is >> ch && ch != c)
        ;
}
