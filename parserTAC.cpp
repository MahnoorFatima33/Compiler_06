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
    T_STRING_LITERAL,
    T_GT,
    T_EOF,
    T_TRUE,
    T_LT,
    T_FALSE,
    T_LBRACKET,
    T_RBRACKET,
    T_FUNC,
    T_COMMA,
};
struct Token
{
    TokenType type;
    string value;
    int line;
};
string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case T_INT:
        return "INT";
    case T_FLOAT:
        return "FLOAT";
    case T_DOUBLE:
        return "DOUBLE";
    case T_CHAR:
        return "CHAR";
    case T_STRING:
        return "STRING";
    case T_BOOL:
        return "BOOL";
    case T_ID:
        return "ID";
    case T_NUM:
        return "number";
    case T_IF:
        return "IF";
    case T_ELSE:
        return "ELSE";
    case T_GTE:
        return "GTE";
    case T_LTE:
        return "LTE";
    case T_AND:
        return "AND";
    case T_OR:
        return "OR";
    case T_EQ:
        return "EQ";
    case T_NEQ:
        return "NEQ";
    case T_NOT:
        return "NOT";
    case T_FOR:
        return "FOR";
    case T_WHILE:
        return "WHILE";
    case T_RETURN:
        return "RETURN";
    case T_ASSIGN:
        return "ASSIGN";
    case T_PLUS:
        return "PLUS";
    case T_MINUS:
        return "MINUS";
    case T_MUL:
        return "T_MUL";
    case T_DIV:
        return "DIV";
    case T_LPAREN:
        return "LPAREN";
    case T_RPAREN:
        return "RPAREN";
    case T_LBRACE:
        return "LBRACE";
    case T_RBRACE:
        return "RBRACE";
    case T_SEMICOLON:
        return "SEMICOLON";
    case T_STRING_LITERAL:
        return "STRING_LITERAL";
    case T_GT:
        return "GT";
    case T_EOF:
        return "EOF";
    case T_TRUE:
        return "TRUE";
    case T_LT:
        return "LT";
    case T_FALSE:
        return "FALSE";
    case T_LBRACKET:
        return "LBRACKET";
    case T_RBRACKET:
        return "RBRACKET";
    case T_FUNC:
        return "FUNC";
    case T_COMMA:
        return "COMMA";
    default:
        return "UNKNOWN";
    }
}
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
                    cout << "Suggested fix: Ensure that the string literal is properly closed with double quotes." << endl;
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
                else if (word == "func")
                    tokens.push_back(Token{T_FUNC, word, line});
                else if (word == "print")
                    tokens.push_back(Token{T_ID, word, line});
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
                    cout << "Suggested fix: Check if this character should be part of '&&'." << endl;
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
                    cout << "Suggested fix: Check if this character should be part of '||'." << endl;
                    exit(1);
                }
                break;
            case '[':
                tokens.push_back(Token{T_LBRACKET, "[", line});
                break;
            case ']':
                tokens.push_back(Token{T_RBRACKET, "]", line});
                break;
            case ',':
                tokens.push_back(Token{T_COMMA, ",", line});
                break;
            default:
                cout << "Unexpected character: " << current << " at line " << line << endl;
                cout << "Suggested fix: Check for typos or incorrect syntax near this character." << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});
        return tokens;
    }
};
class SymbolTable
{
public:
    void declareVariable(const string &name, const string &type)
    {
        if (symbolTable.find(name) != symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
        }
        symbolTable[name] = type;
    }
    string getVariableType(const string &name)
    {
        if (symbolTable.find(name) == symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
        }
        return symbolTable[name];
    }
    bool isDeclared(const string &name) const
    {
        return symbolTable.find(name) != symbolTable.end();
    }

private:
    map<string, string> symbolTable;
};
class IntermediateCodeGenerator
{
public:
    vector<string> instructions;
    int tempCount = 0;
    string newTemp() { return "t" + to_string(tempCount++); }
    void addInstruction(const string &instr) { instructions.push_back(instr); }
    void printInstructions()
    {
        for (const auto &instr : instructions)
        {
            cout << instr << endl;
        }
    }
    void generateAssembly()
    {
        for (const auto &instr : instructions)
        {
            cout << translateToAssembly(instr) << endl;
        }
    }

private:
    string translateToAssembly(const string &instr)
    {
        if (instr.find("=") != string::npos)
        {
            size_t pos = instr.find("=");
            string lhs = instr.substr(0, pos - 1);
            string rhs = instr.substr(pos + 2);
            if (rhs.find("+") != string::npos)
            {
                size_t addPos = rhs.find("+");
                string operand1 = rhs.substr(0, addPos - 1);
                string operand2 = rhs.substr(addPos + 2);
                return "ADD " + lhs + ", " + operand2;
            }
            else if (rhs.find("-") != string::npos)
            {
                size_t subPos = rhs.find("-");
                string operand1 = rhs.substr(0, subPos - 1);
                string operand2 = rhs.substr(subPos + 2);
                return "SUB " + lhs + ", " + operand2;
            }
            else if (rhs.find("*") != string::npos)
            {
                size_t mulPos = rhs.find("*");
                string operand1 = rhs.substr(0, mulPos - 1);
                string operand2 = rhs.substr(mulPos + 2);
                return "MUL " + lhs + ", " + operand2;
            }
            else if (rhs.find("/") != string::npos)
            {
                size_t divPos = rhs.find("/");
                string operand1 = rhs.substr(0, divPos - 1);
                string operand2 = rhs.substr(divPos + 2);
                return "DIV " + lhs + ", " + operand2;
            }
            else
            {
                return "MOV " + lhs + ", " + rhs;
            }
        }
        else if (instr.find("(") != string::npos)
        {
            size_t openParen = instr.find("(");
            size_t closeParen = instr.find(")");
            string innerExpr = instr.substr(openParen + 1, closeParen - openParen - 1);
            string outerExpr = instr.substr(0, openParen - 1);
            string innerAssembly = translateToAssembly(innerExpr);
            return innerAssembly + "\n" + "MOV " + outerExpr + ", " + innerExpr;
        }
        else
        {
            return instr;
        }
    }
};
class Parser
{
private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable &symTable;
    IntermediateCodeGenerator &icg;
    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            cout << "Syntax error: expected token type " << tokenTypeToString(type)
                 << " but found '" << tokens[pos].value
                 << "' (type " << tokenTypeToString(tokens[pos].type) << ") at line " << tokens[pos].line << endl;
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
            if (tokens[pos].value == "print")
            {
                parsePrintStatement(); 
            }
            else
            {
                parseAssignment();
            }
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
        else if (tokens[pos].type == T_FUNC)
        {
            parseFunctionDefinition();
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
    void parseDeclaration(TokenType type)
    {
        expect(type);
        Token idToken = tokens[pos];
        expect(T_ID);
        string varType;
        if (type == T_INT)
            varType = "int";
        else if (type == T_FLOAT)
            varType = "float";
        else if (type == T_STRING)
            varType = "string";
        else if (type == T_BOOL)
            varType = "bool";
        else if (type == T_CHAR)
            varType = "char";
        if (tokens[pos].type == T_LBRACKET)
        {
            expect(T_LBRACKET);
            if (tokens[pos].type == T_RBRACKET)
            {
                symTable.declareVariable(idToken.value, varType + "[]");
                expect(T_RBRACKET);
            }
            else
            {
                expect(T_NUM);
                string arraySize = tokens[pos].value;
                symTable.declareVariable(idToken.value, varType + "[" + arraySize + "]");
                expect(T_RBRACKET);
            }
        }
        else
        {
            symTable.declareVariable(idToken.value, varType);
        }
        if (tokens[pos].type == T_ASSIGN)
        {
            expect(T_ASSIGN);
            string value = parseExpression();
            string tempVar = icg.newTemp();
            icg.addInstruction(tempVar + " = " + value);
        }
        expect(T_SEMICOLON);
    }
    void parseAssignment()
    {
        Token idToken = tokens[pos];
        expect(T_ID);
        if (!symTable.isDeclared(idToken.value))
        {
            throw runtime_error("Semantic error: Variable '" + idToken.value + "' is not declared.");
        }
        if (tokens[pos].type == T_LBRACKET)
        {
            expect(T_LBRACKET);
            string index = parseExpression();
            expect(T_RBRACKET);
            expect(T_ASSIGN);
            string value = parseExpression();
            string tempVar = icg.newTemp();
            icg.addInstruction(tempVar + " = " + value);
            icg.addInstruction("store " + tempVar + " to " + idToken.value + "[" + index + "]");
        }
        else
        {
            expect(T_ASSIGN);
            string value = parseExpression();
            icg.addInstruction(idToken.value + " = " + value);
        }
        expect(T_SEMICOLON);
    }
    void parseForLoop()
    {
        expect(T_FOR);
        expect(T_LPAREN);
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT ||
            tokens[pos].type == T_DOUBLE || tokens[pos].type == T_STRING ||
            tokens[pos].type == T_BOOL || tokens[pos].type == T_CHAR)
        {
            parseDeclaration(tokens[pos].type);
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else
        {
            throw runtime_error("Syntax error: Unexpected token in for loop initialization.");
        }
        expect(T_SEMICOLON);
        string condition = parseExpression();
        expect(T_SEMICOLON);
        string increment = parseExpression();
        expect(T_RPAREN);
        icg.addInstruction("loop_start:");
        icg.addInstruction("if not " + condition + " goto loop_end");
        parseStatement();
        icg.addInstruction(increment);
        icg.addInstruction("goto loop_start");
        icg.addInstruction("loop_end:");
    }
    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);
        string condition = parseExpression();
        expect(T_RPAREN);
        icg.addInstruction("if not " + condition + " goto else");
        parseStatement();
        if (tokens[pos].type == T_ELSE)
        {
            expect(T_ELSE);
            icg.addInstruction("else ");
            parseStatement();
        }
    }
    void parseWhileLoop()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_DOUBLE ||
            tokens[pos].type == T_STRING || tokens[pos].type == T_BOOL || tokens[pos].type == T_CHAR)
        {
            parseDeclaration(tokens[pos].type);
            expect(T_ID);
        }
        string condition = parseExpression();
        expect(T_RPAREN);
        icg.addInstruction("while_start:");
        icg.addInstruction("if not " + condition + " goto while_end");
        parseStatement();
        icg.addInstruction("goto while_start");
        icg.addInstruction("while_end:");
    }
    void parseFunctionDefinition()
    {
        expect(T_FUNC);
        if (tokens[pos].type != T_ID)
        {
            cout << "Syntax error: Expected function name at line " << tokens[pos].line << endl;
            exit(1);
        }
        string funcName = tokens[pos].value;
        expect(T_ID);
        expect(T_LPAREN);
        if (tokens[pos].type != T_RPAREN)
        {
            cout << "Syntax error: Functions cannot have parameters at line " << tokens[pos].line << endl;
            exit(1);
        }
        expect(T_RPAREN);
        icg.addInstruction("FUNC_START " + funcName);
        symTable.declareVariable(funcName, "func");
        parseBlock();
        icg.addInstruction("FUNC_END " + funcName);
    }
    void parsePrintStatement()
    {
        expect(T_ID);
        expect(T_LPAREN);
        string expression = parseExpression();
        expect(T_RPAREN);
        expect(T_SEMICOLON);
        icg.addInstruction("PRINT " + expression);
    }
    void parseReturnStatement()
    {
        expect(T_RETURN);
        string expr = parseExpression();
        expect(T_SEMICOLON);

        icg.addInstruction("return " + expr);
    }
    string parseExpression()
    {
        string left = parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS || tokens[pos].type == T_EQ ||
               tokens[pos].type == T_GT || tokens[pos].type == T_GTE || tokens[pos].type == T_LT ||
               tokens[pos].type == T_LTE || tokens[pos].type == T_NEQ)
        {
            string op = tokens[pos].value;
            expect(tokens[pos].type);
            string right = parseTerm();
            string tempVar = icg.newTemp();
            icg.addInstruction(tempVar + " = " + left + " " + op + " " + right);
            left = tempVar;
        }
        return left;
    }

    string parseTerm()
    {
        string factor = parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            Token op = tokens[pos];
            pos++;
            string nextFactor = parseFactor();
            string tempVar = icg.newTemp();
            icg.addInstruction(tempVar + " = " + factor + " " + op.value + " " + nextFactor);
            factor = tempVar;
        }
        return factor;
    }

    string parseFactor()
    {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_STRING_LITERAL)
        {
            string value = tokens[pos].value;
            pos++;
            return value;
        }
        else if (tokens[pos].type == T_ID)
        {
            string id = tokens[pos].value;
            pos++;
            if (tokens[pos].type == T_LBRACKET)
            {
                expect(T_LBRACKET);
                string index = parseExpression();
                expect(T_RBRACKET);
                return id + "[" + index + "]";
            }
            if (!symTable.isDeclared(id))
            {
                throw runtime_error("Semantic error: Variable '" + id + "' is not declared.");
            }
            return id;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            string expr = parseExpression();
            expect(T_RPAREN);
            return expr;
        }
        else
        {
            throw runtime_error("Syntax error: Unexpected token at parseFactor.");
        }
    }
public:
    Parser(const vector<Token> &tokens, SymbolTable &symTable, IntermediateCodeGenerator &icg)
        : tokens(tokens), pos(0), symTable(symTable), icg(icg) {}
    void parse()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
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
    SymbolTable symTable;
    IntermediateCodeGenerator icg;
    Parser parser(tokens, symTable, icg);
    for (const auto &token : tokens)
    {
        cout << "Token: " << token.value << " (Type: " << tokenTypeToString(token.type) << ") at line " << token.line << endl;
    }
    try
    {
        parser.parse();
        cout << endl;
        cout << "Three Address Code" << endl
             << endl;
        icg.printInstructions();
        cout << endl;
        cout << "Assembly Code" << endl
             << endl;
        icg.generateAssembly();
        cout << endl;
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
    }
    return 0;
}