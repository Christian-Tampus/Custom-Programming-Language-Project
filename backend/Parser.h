/* UPDATE VERSION [22] */

#ifndef H_PARSER
#define H_PARSER

/*
==================================================
Dependencies
==================================================
*/
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

/*
==================================================
Command Enums
==================================================
*/
enum Command
{
    C_NONE,
    C_COMMENT,
    C_OUTPUT,
    C_INPUT,
    C_INTEGER,
    C_DECIMAL,
    C_CHARACTER,
    C_BOOLEAN,
    C_STRING,
    C_CONSTANT_INTEGER,
    C_CONSTANT_DECIMAL,
    C_CONSTANT_CHARACTER,
    C_CONSTANT_BOOLEAN,
    C_CONSTANT_STRING,
};

/*
==================================================
Abstract Syntax Tree (AST) Node Struct
==================================================
*/
struct ASTNode
{
    Command command;
    std::vector<ASTNode*> childASTNodesVec;
    std::string comment;
    std::string output;
    int branchIndex;
    int integer;
    double decimal;
    char character;
    bool boolean;
    std::string string;
    bool isConstant;
};

/*
==================================================
Class Declaration
==================================================
*/
class Parser
{
    private:
        ASTNode* root;
        ASTNode* currentASTNode;
        std::vector<std::string> tokensVec;
        bool parsedSuccessfully;
        std::string errorString;
    public:
        Parser();
        Parser(std::vector<std::string> tokensVec);
        ~Parser();
        void insertTokensVec(std::vector<std::string> tokensVec);
        std::string trimString(std::string inputString);
        std::string getErrorString();
        void parse();
        bool buildAST(std::string codeLine, Command commandType, ASTNode* currentASTNode);
        bool isParsedAndASTBuiltSuccessfully();
        ASTNode* getASTNodeRoot();
};

/*
==================================================
Class Definition
==================================================
*/

/*
==================================================
Default Constructor
==================================================
*/
Parser::Parser()
{
    this->currentASTNode = nullptr;
    this->root = nullptr;
    this->parsedSuccessfully = false;
    this->errorString = "";
};

/*
==================================================
Parser(std::vector<std::string> tokensVec) Constructor
==================================================
*/
Parser::Parser(std::vector<std::string> tokensVec)
{
    this->root = nullptr;
    this->currentASTNode = nullptr;
    this->tokensVec = tokensVec;
    this->parsedSuccessfully = false;
    this->errorString = "";
};

/*
==================================================
Destructor
==================================================
*/
Parser::~Parser()
{
    if (this->root != nullptr)
    {
        delete this->root;
    };
    this->root = nullptr;
    if (this->currentASTNode != nullptr)
    {
        delete this->currentASTNode;
    };
    this->currentASTNode = nullptr;
    this->tokensVec.clear();
    this->parsedSuccessfully = false;
    this->errorString = "";
};

/*
==================================================
Inserts tokensVec Into Class
==================================================
*/
void Parser::insertTokensVec(std::vector<std::string> tokensVec)
{
    this->tokensVec = tokensVec;
    return;
};

/*
==================================================
Trims String By Removing Leading & Trailing White Space
==================================================
*/
std::string Parser::trimString(std::string inputString)
{
    std::string whiteSpace = "\t\n\r\f\v ";
    auto startIndex = inputString.find_first_not_of(whiteSpace);
    if (startIndex == std::string::npos)
    {
        return "";
    };
    auto endIndex = inputString.find_last_not_of(whiteSpace);
    return inputString.substr(startIndex, endIndex - startIndex + 1);
};

/*
==================================================
Returns errorString
==================================================
*/
std::string Parser::getErrorString()
{
    return this->errorString;
};

/*
==================================================
Inserts tokensVec Into Class
==================================================
*/
void Parser::parse()
{
    if (this->tokensVec.size() == 0)
    {
        std::cout << "[PARSER] No Code To Parse!" << std::endl;
        return;
    };
    bool parsedSuccessfully = true;
    for (int index = 0; index < this->tokensVec.size(); index++)
    {
        std::cout << "[PARSER] Line[" << index + 1 << "]: " << this->tokensVec[index] << std::endl;
        std::string currentCodeLine = this->trimString(this->tokensVec[index]);
        bool hasInvalidSyntax = true;
        int syntaxErrorLine = 0;
        if (currentCodeLine.length() > 0)
        {
            Command commandType = C_NONE;
            if (currentCodeLine[0] == '!')
            {
                std::cout << "[PARSER] Command: COMMENT" << std::endl;
                commandType = C_COMMENT;
                bool buildASTSuccessfully = this->buildAST(currentCodeLine, commandType, this->currentASTNode);
                if (buildASTSuccessfully)
                {
                    hasInvalidSyntax = false;
                }
                else
                {
                    syntaxErrorLine = index + 1;
                };
            }
            else
            {
                bool commandFound = false;
                std::string commandString = "";
                for (int index2 = 0; index2 < currentCodeLine.length(); index2++)
                {
                    commandString += currentCodeLine[index2];
                    if (commandString == "output")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: OUTPUT" << std::endl;
                        commandType = C_OUTPUT;
                        bool buildASTSuccessfully = this->buildAST(currentCodeLine, commandType, this->currentASTNode);
                        if (buildASTSuccessfully)
                        {
                            hasInvalidSyntax = false;
                        }
                        else
                        {
                            syntaxErrorLine = index + 1;
                        };
                    }
                    else if (commandString == "INTEGER")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: INTEGER" << std::endl;
                        commandType = C_INTEGER;
                    }
                    else if (commandString == "DECIMAL")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: DECIMAL" << std::endl;
                        commandType = C_DECIMAL;
                    }
                    else if (commandString == "CHARACTER")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: CHARACTER" << std::endl;
                        commandType = C_CHARACTER;
                    }
                    else if (commandString == "BOOLEAN")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: BOOLEAN" << std::endl;
                        commandType = C_BOOLEAN;
                    }
                    else if (commandString == "STRING")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: STRING" << std::endl;
                        commandType = C_STRING;
                    }
                    else if (commandString == "CONSTANT INTEGER")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: CONSTANT INTEGER" << std::endl;
                        commandType = C_CONSTANT_INTEGER;
                    }
                    else if (commandString == "CONSTANT DECIMAL")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: CONSTANT DECIMAL" << std::endl;
                        commandType = C_CONSTANT_DECIMAL;
                    }
                    else if (commandString == "CONSTANT CHARACTER")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: CONSTANT CHARACTER" << std::endl;
                        commandType = C_CONSTANT_CHARACTER;
                    }
                    else if (commandString == "CONSTANT BOOLEAN")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: CONSTANT BOOLEAN" << std::endl;
                        commandType = C_CONSTANT_BOOLEAN;
                    }
                    else if (commandString == "CONSTANT STRING")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: CONSTANT STRING" << std::endl;
                        commandType = C_CONSTANT_STRING;
                    };
                };
                if (!commandFound)
                {
                    syntaxErrorLine = index + 1;
                };
            };
        }
        else
        {
            std::cout << "[PARSER] Line[" << index + 1 << "] Is A Blank Line!" << std::endl;
            hasInvalidSyntax = false;
        };
        if (hasInvalidSyntax)
        {
            this->errorString = "[SYSTEM ERROR] Syntax Error At Line[" + std::to_string(syntaxErrorLine) + "]! Code:" + this->tokensVec[index];
            parsedSuccessfully = false;
            break;
        };
    };
    this->parsedSuccessfully = parsedSuccessfully;
    std::cout << "[PARSER] parsedSuccessfully: " << (this->parsedSuccessfully == true ? "TRUE!" : "FALSE!") << std::endl;
    return;
};

/*
==================================================
Builds The Abstract Syntax Tree (AST)
==================================================
*/
bool Parser::buildAST(std::string codeLine, Command commandType, ASTNode* currentASTNode)
{
    std::cout << "[PARSER] Attempting To Building AST Node..." << std::endl;
    std::cout << "[PARSER] codeLine:" << codeLine << std::endl;
    bool buildASTSuccessfully = false;
    ASTNode* newASTNode = new ASTNode;
    newASTNode->command = commandType;
    newASTNode->branchIndex = 0;
    if (commandType == C_COMMENT)
    {
        newASTNode->comment = codeLine;
        std::cout << "[PARSER] Comment:" << newASTNode->comment << std::endl;
    }
    else if (commandType == C_OUTPUT)
    {
        //output(""); <- 11 Length()
        //output(X); <- 10 Length()
        if (codeLine.length() < 10)
        {
            return false;
        };
        std::string outputString;
        if (codeLine == "output(\"\")")
        {
            outputString = "";
        }
        else if (codeLine.substr(0, 8) == "output(\"" && codeLine.substr(codeLine.size() - 3) == "\");")
        {
            outputString = codeLine.substr(8, codeLine.size() - 3 - 8);
        }
        else
        {
            return false;
        };
        newASTNode->output = outputString;
        std::cout << "[PARSER] Output:" << newASTNode->output << std::endl;
    };
    if (currentASTNode == nullptr && this->root == nullptr)
    {
        this->root = newASTNode;
        this->currentASTNode = newASTNode;
        buildASTSuccessfully = true;
    }
    else if (currentASTNode != nullptr)
    {
        this->currentASTNode->childASTNodesVec.push_back(newASTNode);
        this->currentASTNode = newASTNode;
        buildASTSuccessfully = true;
    };
    std::cout << "[PARSER] Built AST Node Successfully!" << std::endl;
    return buildASTSuccessfully;
};

/*
==================================================
Returns If Parser Parsed The Code & Built The Abstract Syntax Tree (AST) Successfully
==================================================
*/
bool Parser::isParsedAndASTBuiltSuccessfully()
{
    return this->parsedSuccessfully;
};

/*
==================================================
Returns ASTNode* root
==================================================
*/
ASTNode* Parser::getASTNodeRoot()
{
    return this->root;
};

#endif