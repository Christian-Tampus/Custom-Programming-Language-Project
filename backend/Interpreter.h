/* UPDATE VERSION [27] */

#ifndef H_INTERPRETER
#define H_INTERPRETER

/*
==================================================
Dependencies
==================================================
*/
#include <iostream>
#include "Parser.h"
using namespace std;

/*
==================================================
Variable Struct
==================================================
*/
struct VariableStruct
{
    int integer;
    double decimal;
    char character;
    bool boolean;
    std::string string;
    bool isConstant;
    int variableMemoryAddress;
};

/*
==================================================
Class Declaration
==================================================
*/
class Interpreter
{
    private:
        bool interpretationSuccess;
        std::vector<std::string> terminalOutputVec;
        std::vector<VariableStruct> variableStructVec;
        VariableStruct* createNewVariableStruct(int integer, double decimal, char character, bool boolean, std::string string, bool isConstant, int variableMemoryAddress);
    public:
        Interpreter();
        ~Interpreter();
        bool interpret(ASTNode* root);
        std::vector<std::string> getTerminalOutputVec();
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
Interpreter::Interpreter()
{
    this->interpretationSuccess = false;
};

/*
==================================================
Destructor
==================================================
*/
Interpreter::~Interpreter()
{
    this->interpretationSuccess = false;
    this->terminalOutputVec.clear();
    this->variableStructVec.clear();
};

/*
==================================================
Interprets The Abstract Syntax Tree (AST)
==================================================
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
    int variableMemoryAddress;
    std::string variableType;
};
*/
bool Interpreter::interpret(ASTNode* root)
{
    bool newInterpretationSuccess = true;
    ASTNode* currentASTNode = root;
    while (currentASTNode != nullptr)
    {
        if (currentASTNode->command != C_COMMENT && currentASTNode->command != C_NONE)
        {
            if (currentASTNode->command == C_OUTPUT)
            {
                this->terminalOutputVec.push_back(currentASTNode->output);
            }
            else if (currentASTNode->command == C_INTEGER || currentASTNode->command == C_DECIMAL || currentASTNode->command == C_CHARACTER || currentASTNode->command == C_BOOLEAN || currentASTNode->command == C_STRING)
            {
                VariableStruct* newVariableStruct = this->createNewVariableStruct(newVariableStruct->integer, newVariableStruct->decimal, newVariableStruct->character, newVariableStruct->boolean, newVariableStruct->string, newVariableStruct->isConstant, newVariableStruct->variableMemoryAddress);
            }
            else
            {
                newInterpretationSuccess = false;
                currentASTNode = nullptr;
                break;
            };
        };
        if (currentASTNode->childASTNodesVec.size() > 0)
        {
            currentASTNode = currentASTNode->childASTNodesVec[currentASTNode->branchIndex];
        }
        else
        {
            currentASTNode = nullptr;
            break;
        };
    };
    this->interpretationSuccess = newInterpretationSuccess;
    return this->interpretationSuccess;
};

/*
==================================================
Returns terminalOutputVec
==================================================
*/
VariableStruct* createNewVariableStruct(int integer, double decimal, char character, bool boolean, std::string string, bool isConstant, int variableMemoryAddress)
{
    VariableStruct* newVariableStruct = new VariableStruct;
    newVariableStruct->integer = integer;
    newVariableStruct->decimal = decimal;
    newVariableStruct->character = character;
    newVariableStruct->boolean = boolean;
    newVariableStruct->string = string;
    newVariableStruct->isConstant = isConstant;
    newVariableStruct->variableMemoryAddress = variableMemoryAddress;
    return newVariableStruct;
};

/*
==================================================
Returns terminalOutputVec
==================================================
*/
std::vector<std::string> Interpreter::getTerminalOutputVec()
{
    return this->terminalOutputVec;
};
#endif