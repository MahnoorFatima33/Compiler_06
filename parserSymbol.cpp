#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <stack>

using namespace std;

enum TokenType
{
    T_INT,
    T_FLOAT,
    T_DOUBLE,
    T_CHAR,
    T_STRING,
    T_BOOL,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_GTE,
    T_LTE,
    T_AND,
    T_OR,
    T_EQ,
    T_NEQ,
    T_NOT,
    T_FOR,
    T_WHILE,
    T_RETURN,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_SEMICOLON,
    T_STRING_LITERAL,
    T_GT,
    T_EOF,
    T_TRUE,
    T_LT,
    T_FALSE
};

struct Token
{
    TokenType type;
    string value;
    int line;
};

struct Symbol
{
    string name;
    TokenType type;
    string value;
};
struct ThreeAddressCode
{
    string temp;
    string op;
    string arg1;
    string arg2;
    string toString() const
    {
        return temp + " = " + arg1 + " " + op + " " + arg2;
    }
};

class TACGenerator
{
private:
    vector<ThreeAddressCode> tacList;
    stack<string> tempStack;
    int count = 0;
    string generateTempVar()
    {
        return "t" + to_string(count++);
    }

public:
    void generateTAC(const vector<Token> &tokens)
    {
        stack<string> operatorStack;
        stack<string> operandStack;
        for (const auto &token : tokens)
        {
            if (token.type == T_NUM || token.type == T_ID)
            {
                operandStack.push(token.value);
            }
            else if (token.type == T_PLUS || token.type == T_MINUS || token.type == T_MUL || token.type == T_DIV)
            {
                operatorStack.push(token.value);
            }
            else if (token.type == T_LPAREN)
            {
                operatorStack.push(token.value);
            }
            else if (token.type == T_RPAREN)
            {
                while (!operatorStack.empty() && operatorStack.top() != "(")
                {
                    string op = operatorStack.top();
                    operatorStack.pop();
                    string arg2 = operandStack.top();
                    operandStack.pop();
                    string arg1 = operandStack.top();
                    operandStack.pop();
                    string temp = generateTempVar();
                    tacList.push_back({temp, op, arg1, arg2});
                    operandStack.push(temp);
                    cout << "Operator: " << op << ", Arg1: " << arg1 << ", Arg2: " << arg2 << endl;
                }
                if (!operatorStack.empty())
                    operatorStack.pop();
                    
            }
        }
        while (!operatorStack.empty())
        {
            string op = operatorStack.top();
            operatorStack.pop();
            string arg2 = operandStack.top();
            operandStack.pop();
            string arg1 = operandStack.top();
            operandStack.pop();
            string temp = generateTempVar();
            tacList.push_back({temp, op, arg1, arg2});
            operandStack.push(temp);
            cout << "Operator: " << op << ", Arg1: " << arg1 << ", Arg2: " << arg2 << endl;
        }
        
    }
    void printTAC() const
    {
        cout << "\nThree Address Code is:" << endl;
        for (const auto &tac : tacList)
        {
            cout << tac.toString() << endl;
        }
    }
};

class SymbolTable
{
private:
    vector<Symbol> symbols;

public:
    void addSymbol(const string &name, TokenType type, const string &value = "")
    {
        symbols.push_back(Symbol{name, type, value});
    }
    bool lookup(const string &name)
    {
        for (const auto &symbol : symbols)
        {
            if (symbol.name == name)
                return true;
        }
        return false;
    }
    void printTable()
    {
        cout << "\nSymbol Table:" << endl;
        for (const auto &symbol : symbols)
        {
            cout << "Name: " << symbol.name << ", Type: " << symbol.type << ", Value: " << symbol.value << endl;
        }
    }
};

class Lexer
{
private:
    string src;
    size_t pos;
    int line;

public:
    Lexer(const string &src) : src(src), pos(0), line(1) {}
    string consumeNumber()
    {
        size_t start = pos;
        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.'))
            pos++;
        return src.substr(start, pos - start);
    }
    string consumeWord()
    {
        size_t start = pos;
        while (pos < src.size() && (isalnum(src[pos]) || src[pos] == '_'))
            pos++;
        return src.substr(start, pos - start);
    }
    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];
            if (current == '\n')
            {
                line++;
                pos++;
                continue;
            }
            if (isspace(current))
            {
                pos++;
                continue;
            }
            if (current == '/' && pos + 1 < src.size() && src[pos + 1] == '/')
            {
                while (pos < src.size() && src[pos] != '\n')
                {
                    pos++;
                }
                continue;
            }
            if (current == '"')
            {
                pos++;
                string strLiteral;
                while (pos < src.size() && src[pos] != '"')
                {
                    strLiteral += src[pos];
                    pos++;
                }
                if (pos < src.size() && src[pos] == '"')
                {
                    pos++;
                    tokens.push_back(Token{T_STRING_LITERAL, strLiteral, line});
                }
                else
                {
                    cout << "Error: Unterminated string literal at line " << line << endl;
                    exit(1);
                }
                continue;
            }
            if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            }
            if (isalpha(current))
            {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{T_INT, word, line});
                else if (word == "float")
                    tokens.push_back(Token{T_FLOAT, word, line});
                else if (word == "double")
                    tokens.push_back(Token{T_DOUBLE, word, line});
                else if (word == "string")
                    tokens.push_back(Token{T_STRING, word, line});
                else if (word == "bool")
                    tokens.push_back(Token{T_BOOL, word, line});
                else if (word == "char")
                    tokens.push_back(Token{T_CHAR, word, line});
                else if (word == "if")
                    tokens.push_back(Token{T_IF, word, line});
                else if (word == "else")
                    tokens.push_back(Token{T_ELSE, word, line});
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word, line});
                else if (word == "true")
                    tokens.push_back(Token{T_TRUE, word, line});
                else if (word == "false")
                    tokens.push_back(Token{T_FALSE, word, line});
                else if (word == "for")
                    tokens.push_back(Token{T_FOR, word, line});
                else if (word == "while")
                    tokens.push_back(Token{T_WHILE, word, line});
                else
                    tokens.push_back(Token{T_ID, word, line});
                continue;
            }
            switch (current)
            {
            case '=':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_EQ, "==", line});
                    pos++;
                }
                else
                    tokens.push_back(Token{T_ASSIGN, "=", line});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", line});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", line});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", line});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", line});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", line});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", line});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", line});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", line});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", line});
                break;
            case '>':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_GTE, ">=", line});
                    pos++;
                }
                else
                    tokens.push_back(Token{T_GT, ">", line});
                break;
            case '<':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_LTE, "<=", line});
                    pos++;
                }
                else
                    tokens.push_back(Token{T_LT, "<", line});
                break;
            case '!':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_NEQ, "!=", line});
                    pos++;
                }
                else
                    tokens.push_back(Token{T_NOT, "!", line});
                break;
            case '&':
                if (pos + 1 < src.size() && src[pos + 1] == '&')
                {
                    tokens.push_back(Token{T_AND, "&&", line});
                    pos++;
                }
                else
                {
                    cout << "Unexpected character '&' at line " << line << endl;
                    exit(1);
                }
                break;
            case '|':
                if (pos + 1 < src.size() && src[pos + 1] == '|')
                {
                    tokens.push_back(Token{T_OR, "||", line});
                    pos++;
                }
                else
                {
                    cout << "Unexpected character '|' at line " << line << endl;
                    exit(1);
                }
                break;
            default:
                cout << "Unexpected character: " << current << " at line " << line << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});
        return tokens;
    }
};
class Parser
{
private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable symbolTable;
    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            cout << "Syntax error: expected token type " << type
                 << " but found '" << tokens[pos].value
                 << "' (type " << tokens[pos].type << ") at line " << tokens[pos].line << endl;
            exit(1);
        }
    }
    void parseDeclaration(TokenType type)
    {
        expect(type);
        string varName = tokens[pos].value;
        expect(T_ID);

        // Add to symbol table
        symbolTable.addSymbol(varName, type);

        if (tokens[pos].type == T_ASSIGN)
        {
            expect(T_ASSIGN);
            string value = tokens[pos].value;
            expect(T_NUM);
            symbolTable.addSymbol(varName, type, value);
        }

        expect(T_SEMICOLON);
    }

public:
    Parser(const vector<Token> &tokens) : tokens(tokens), pos(0) {}
    void parse()
    {
        while (tokens[pos].type != T_EOF)
        {
            if (tokens[pos].type == T_INT)
            {
                parseDeclaration(T_INT);
            }
            else if (tokens[pos].type == T_FLOAT)
            {
                parseDeclaration(T_FLOAT);
            }
            else if (tokens[pos].type == T_DOUBLE)
            {
                parseDeclaration(T_DOUBLE);
            }
            else if (tokens[pos].type == T_CHAR)
            {
                parseDeclaration(T_CHAR);
            }
            else if (tokens[pos].type == T_STRING)
            {
                parseDeclaration(T_STRING);
            }
            else if (tokens[pos].type == T_BOOL)
            {
                parseDeclaration(T_BOOL);
            }
            else
            {
                pos++;
            }
        }
        symbolTable.printTable();
    }
};
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Provide a file name" << endl;
        return 1;
    }
    ifstream file(argv[1]);
    if (!file.is_open())
    {
        cout << "Failed to open file." << endl;
        return 1;
    }
    string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();
    for (const auto &token : tokens)
    {
        cout << "Token: " << token.value << ", Type: " << token.type << ", Line: " << token.line << endl;
    }
    SymbolTable symbolTable;
    symbolTable.printTable();
    TACGenerator tacGenerator;
    tacGenerator.generateTAC(tokens);
    tacGenerator.printTAC();
    return 0;
}