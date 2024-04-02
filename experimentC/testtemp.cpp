#include <bits/stdc++.h>
using namespace std;

std::map<std::string, std::string> pl0Symbols = {
    // 基本字
    {"begin", "beginsym"},{"end", "endsym"},{"if", "ifsym"},
    {"then", "thensym"},{"else", "elsesym"},{"const", "constsym"},
    {"var", "varsym"},{"procedure", "procsym"},{"while", "whilesym"},
    {"do", "dosym"},{"call", "callsym"},{"read", "readsym"},
    {"write", "writesym"},{"repeat", "repeatsym"},{"until", "untilsym"},
    {"odd", "oddsym"},{"return", "returnsym"},
    // 运算符
    {"+", "plus"},{"-", "minus"},{"*", "times"},
    {"/", "slash"},{"=", "eql"},{"!=", "neq"},
    {"<", "lss"},{"<=", "leq"},{">", "gtr"},
    {">=", "geq"},
    // 界符
    {",", "comma"},{".", "period"},{";", "semicolon"},
    {":=", "becomes"},{"(", "lparen"},{")", "rparen"}
};

int main()
{
    return 0;
}