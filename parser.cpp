#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <map>

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

    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            cout << "Syntax error: expected token type " << type
                 << " but found '" << tokens[pos].value << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseStatement()
    {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_DOUBLE ||
            tokens[pos].type == T_STRING || tokens[pos].type == T_BOOL || tokens[pos].type == T_CHAR)
        {
            parseDeclaration(tokens[pos].type);
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForLoop();
        }
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileLoop();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else
        {
            cout << "Syntax error: unexpected token '" << tokens[pos].value
                 << "' at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseBlock()
    {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    void parseDeclaration(TokenType T)
    {
        expect(T);
        expect(T_ID);
        //  expect(T_ASSIGN);
        // expect(T_NUM);
        expect(T_SEMICOLON);
    }

    void parseAssignment()
    {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE)
        {
            expect(T_ELSE);
            parseStatement();
        }
    }
    void parseForLoop()
    {
        expect(T_FOR);
        expect(T_LPAREN);
        parseStatement(); // Initialization
        expect(T_SEMICOLON);
        parseExpression(); // Condition
        expect(T_SEMICOLON);
        parseStatement(); // Update
        expect(T_RPAREN);
        parseStatement(); // Body
    }

    void parseWhileLoop()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        parseExpression(); // Condition
        expect(T_RPAREN);
        parseStatement(); // Body
    }
    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression()
    {
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS)
        {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT)
        {
            pos++;
            parseExpression();
        }
    }

    void parseTerm()
    {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            pos++;
            parseFactor();
        }
    }

    void parseFactor()
    {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID || tokens[pos].type == T_TRUE || tokens[pos].type == T_FALSE)
        {
            pos++;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        }
        else
        {
            cout << "Syntax error: unexpected token " << tokens[pos].value << endl;
            exit(1);
        }
    }

public:
    Parser(const vector<Token> &tokens) : tokens(tokens), pos(0) {}

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        cout << "Parsing completed successfully! No syntax error." << endl;
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
        cout << "Token: " << token.value << " (Type: " << token.type << ") at line " << token.line << endl;

    Parser parser(tokens);
    parser.parseProgram();
    return 0;
}
