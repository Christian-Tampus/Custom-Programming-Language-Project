/* UPDATE VERSION [20] */

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
    C_INPUT
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
    public:
        Parser();
        Parser(std::vector<std::string> tokensVec);
        ~Parser();
        void insertTokensVec(std::vector<std::string> tokensVec);
        std::string trimString(std::string inputString);
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
                };
            }
            else
            {
                std::string commandString = "";
                for (int index2 = 0; index2 < currentCodeLine.length(); index2++)
                {
                    commandString += currentCodeLine[index2];
                    if (commandString == "output")
                    {
                        std::cout << "[PARSER] Command: OUTPUT" << std::endl;
                        commandType = C_OUTPUT;
                        bool buildASTSuccessfully = this->buildAST(currentCodeLine, commandType, this->currentASTNode);
                        if (buildASTSuccessfully)
                        {
                            hasInvalidSyntax = false;
                        };
                    };
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