%{
#include <cassert>
#include <functional>
#include <vector>

enum yytoken {
    NUM = 258,
    ADD,
    SUB,
    MUL,
    DIV,
    EOL
};

int yyval;
%}

%%

\n      { return EOL; }
"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
[\t ]   { }
[0-9]+  { yyval = atoi(yytext); return NUM; }
.       { printf("Unrecognized character %c\n", yytext[0]); }

%%

void calc(std::vector<int>& stack, std::function<int(int, int)> op)
{
    assert(stack.size() >= 2);
    const auto x = stack.back();
    stack.pop_back();
    const auto y = stack.back();
    stack.pop_back();
    stack.push_back(op(x, y));
}

int main()
{
    std::vector<int> stack;

    while (int tok = yylex()) {
        switch(tok) {
            case NUM:   stack.push_back(yyval); break;
            case ADD:   calc(stack, std::plus<int>()); break;
            case SUB:   calc(stack, std::minus<int>()); break;
            case MUL:   calc(stack, std::multiplies<int>()); break;
            case DIV:   calc(stack, std::divides<int>()); break;
            case EOL:   printf("%d\n", stack.back()); break;
        }
    }
    return 0;
}
