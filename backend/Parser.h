/* UPDATE VERSION [16] */

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
};

/*
==================================================
Class Declaration
==================================================
*/
class Parser
{
    private:
        bool validSyntax;
        ASTNode* root;
        std::vector<std::string> tokensVec;
    public:
        Parser();
        Parser(std::vector<std::string> tokensVec);
        void insertTokensVec(std::vector<std::string> tokensVec);
        void parse();
        void buildAST();
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
    this->validSyntax = false;
    this->root = nullptr;
};

/*
==================================================
Parser(std::vector<std::string> tokensVec) Constructor
==================================================
*/
Parser::Parser(std::vector<std::string> tokensVec)
{
    this->validSyntax = false;
    this->root = nullptr;
    this->tokensVec = tokensVec;
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
    bool isCodeValidSyntax = true;
    for (int index = 0; index < this->tokensVec.size(); index++)
    {
        std::cout << "[PARSER] Line[" << index + 1 << "]: " << this->tokensVec[index] << std::endl;
        std::string currentCodeLine = this->tokensVec[index];
        if (currentCodeLine.length() > 0)
        {
            if (currentCodeLine[0] == '!')
            {
                std::cout << "[PARSER] Command: COMMENT" << std::endl;
                /*
                ==================================================
                CREATE buildASTNode() FUNCTION TO BUILD NEW AST NODE!
                ==================================================
                */
            };
        };
    };
    this->validSyntax = isCodeValidSyntax;
    return;
};

/*
==================================================
Builds The Abstract Syntax Tree (AST)
==================================================
*/
void Parser::buildAST()
{
    if (this->validSyntax == false)
    {
        std::cout << "[PARSER] Code Has Invalid Syntax, Unable To Build AST!" << std::endl;
        return;
    };
    std::cout << "[PARSER] Code Has Valid Syntax, Building AST..." << std::endl;
    /*
    ==================================================
    ADD AST BUILD LOGIC HERE!!!
    ==================================================
    */
    std::cout << "[PARSER] AST Successfully Built!" << std::endl;
    return;
};

#endif