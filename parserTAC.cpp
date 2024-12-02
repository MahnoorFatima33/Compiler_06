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
// Symbol Table

// Symbol Table
class SymbolTable {
public:
    void declareVariable(const string &name, const string &type) {
        if (symbolTable.find(name) != symbolTable.end()) {
            throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
        }
        symbolTable[name] = type;
    }

    string getVariableType(const string &name) {
        if (symbolTable.find(name) == symbolTable.end()) {
            throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
        }
        return symbolTable[name];
    }

    void printSymbolTable() {
        cout << "Symbol Table:\n";
        for (const auto &entry : symbolTable) {
            cout << entry.first << " : " << entry.second << endl;
        }
    }

private:
    map<string, string> symbolTable;
};

// Intermediate Code Generator
class IntermediateCodeGenerator {
public:
    vector<string> instructions;
    int tempCount = 0;

    string newTemp() { return "t" + to_string(tempCount++); }

    void addInstruction(const string &instr) { instructions.push_back(instr); }

    void printInstructions() {
        cout << "Intermediate Code:\n";
        for (const auto &instr : instructions) {
            cout << instr << endl;
        }
    }
};

// Assembly Code Generator
class AssemblyCodeGenerator {
public:
    vector<string> assemblyCode;

    void generateAssembly(const vector<string> &intermediateCode) {
        for (const auto &instr : intermediateCode) {
            if (instr.find("=") != string::npos) {
                size_t eqPos = instr.find("=");
                string lhs = instr.substr(0, eqPos - 1);
                string rhs = instr.substr(eqPos + 2);

                assemblyCode.push_back("MOV AX, " + rhs);
                assemblyCode.push_back("MOV " + lhs + ", AX");
            } else if (instr.find("+") != string::npos) {
                size_t plusPos = instr.find("+");
                string lhs = instr.substr(0, plusPos - 1);
                string rhs1 = instr.substr(plusPos + 2);

                assemblyCode.push_back("ADD AX, " + rhs1);
                assemblyCode.push_back("MOV " + lhs + ", AX");
            }
        }
    }

    void printAssemblyCode() {
        cout << "Assembly Code:\n";
        for (const auto &line : assemblyCode) {
            cout << line << endl;
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
            cout << "Syntax error: expected token type " << type
                 << " but found '" << tokens[pos].value
                 << "' (type " << tokens[pos].type << ") at line " << tokens[pos].line << endl;
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
        symTable.declareVariable(idToken.value, varType);
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
        expect(T_ASSIGN);
        string value = parseExpression();
        icg.addInstruction(idToken.value + " = " + value);
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
        parseStatement();
        parseExpression();
        parseExpression();
        expect(T_SEMICOLON);
        expect(T_RPAREN);
        parseStatement();
    }
    void parseWhileLoop()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
    }
    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    string parseExpression()
    {
        string term = parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS)
        {
            Token op = tokens[pos];
            pos++;
            string nextTerm = parseTerm();
            string tempVar = icg.newTemp();
            icg.addInstruction(tempVar + " = " + term + " " + op.value + " " + nextTerm);
            term = tempVar;
        }
        return term;
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
        if (tokens[pos].type == T_NUM)
        {
            string value = tokens[pos].value;
            pos++;
            return value;
        }
        else if (tokens[pos].type == T_ID)
        {
            string id = tokens[pos].value;
            if (!symTable.declareVariable(id))
            {
                throw runtime_error("Semantic error: Variable '" + id + "' is not declared.");
            }
            pos++;
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

int main() {
    string code = "int a = 5; a = a + 10;";  // Example input

    // Tokenization
    Lexer lexer(code);
    vector<Token> tokens = lexer.tokenize();

    cout << "Tokens:\n";
    for (const auto &token : tokens) {
        cout << token.value << "\t(line: " << token.line << ")\n";
    }

    // Symbol Table
    SymbolTable symTable;
    symTable.declareVariable("a", "int");

    // Intermediate Code
    IntermediateCodeGenerator icg;
    icg.addInstruction("a = 5");
    icg.addInstruction("a = a + 10");

    // Assembly Code
    AssemblyCodeGenerator acg;
    acg.generateAssembly(icg.instructions);

    // Outputs
    cout << "\n";
    symTable.printSymbolTable();
    cout << "\n";
    icg.printInstructions();
    cout << "\n";
    acg.printAssemblyCode();

    return 0;
}
