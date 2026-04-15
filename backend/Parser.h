/* UPDATE VERSION [17] */

#ifndef H_PARSER
#define H_PARSER

/*
==================================================
Dependencies
==================================================
*/
#include <iostream>
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
    public:
        Parser();
        Parser(std::vector<std::string> tokensVec);
        ~Parser();
        void insertTokensVec(std::vector<std::string> tokensVec);
        void parse();
        void buildAST(std::string codeLine, Command commandType, ASTNode* currentASTNode);
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
};

/*
==================================================
Default Constructor
==================================================
*/
Parser::~Parser()
{
    delete this->root;
    this->root = nullptr;
    delete this->currentASTNode;
    this->currentASTNode = nullptr;
    this->tokensVec.clear();
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
    for (int index = 0; index < this->tokensVec.size(); index++)
    {
        std::cout << "[PARSER] Line[" << index + 1 << "]: " << this->tokensVec[index] << std::endl;
        std::string currentCodeLine = this->tokensVec[index];
        if (currentCodeLine.length() > 0)
        {
            Command commandType = C_NONE;
            if (currentCodeLine[0] == '!')
            {
                std::cout << "[PARSER] Command: COMMENT" << std::endl;
                commandType = C_COMMENT;
                this->buildAST(currentCodeLine, commandType, this->currentASTNode);
            }
            else if (false) //IMPLEMENT output();
            {
                //IMPLEMENT output();
            }
            else
            {
                std::cout << "[PARSER] Invalid Syntax At Line[" << index + 1 << "]!" << std::endl;
                break;
            };
        }
        else
        {
            std::cout << "[PARSER] Line[" << index + 1 << "] Is A Blank Line!" << std::endl;
        };
    };
    return;
};

/*
==================================================
Builds The Abstract Syntax Tree (AST)
==================================================
*/
void Parser::buildAST(std::string codeLine, Command commandType, ASTNode* currentASTNode)
{
    std::cout << "[PARSER] Code Has Valid Syntax, Building AST Node..." << std::endl;
    ASTNode* newASTNode = new ASTNode;
    newASTNode->command = commandType;
    if (commandType == C_COMMENT)
    {
        newASTNode->comment = codeLine;
    };
    if (currentASTNode == nullptr && this->root == nullptr)
    {
        this->root = newASTNode;
        this->currentASTNode = newASTNode;
    }
    else if (currentASTNode != nullptr)
    {
        this->currentASTNode->childASTNodesVec.push_back(newASTNode);
        this->currentASTNode = newASTNode;
    };
    std::cout << "[PARSER] Built AST Node Successfully!" << std::endl;
    return;
};

#endif