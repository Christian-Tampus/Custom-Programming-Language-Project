/* UPDATE VERSION [30] */

#ifndef H_INTERPRETER
#define H_INTERPRETER

/*
==================================================
Dependencies
==================================================
*/
#include <iostream>
#include <map>
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
    std::string variableType;
    std::string variableName;
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
        std::map<int, VariableStruct*> variableStructMap;
        std::map<std::string, int> variablesMap;
        VariableStruct* createNewVariableStruct(int integer, double decimal, char character, bool boolean, std::string string, bool isConstant, int variableMemoryAddress, std::string variableType, std::string variableName);
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
    this->variableStructMap.clear();
    this->variablesMap.clear();
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
                if (currentASTNode->outputUsesVariable)
                {
                    if (this->variablesMap.find(currentASTNode->outputVariable) != this->variablesMap.end())
                    {
                        VariableStruct* currentVariableStruct = this->variableStructMap[this->variablesMap[currentASTNode->outputVariable]];
                        if (currentVariableStruct->variableType == "INTEGER")
                        {
                            this->terminalOutputVec.push_back(std::to_string(currentVariableStruct->integer));
                        }
                        else if (currentVariableStruct->variableType == "DECIMAL")
                        {
                            this->terminalOutputVec.push_back(std::to_string(currentVariableStruct->decimal));
                        }
                        else if (currentVariableStruct->variableType == "CHARACTER")
                        {
                            this->terminalOutputVec.push_back((currentVariableStruct->character + ""));
                        }
                        else if (currentVariableStruct->variableType == "BOOLEAN")
                        {
                            this->terminalOutputVec.push_back((currentVariableStruct->boolean == true ? "TRUE" : "FALSE"));
                        }
                        else if (currentVariableStruct->variableType == "STRING")
                        {
                            this->terminalOutputVec.push_back(currentVariableStruct->string);
                        };

                        /*
                        TEST THIS CODE!

                        INTEGER _integer1;
                        INTEGER integer2;

                        INTEGER _integer3 = 123;
                        INTEGER integer4 = -456;

                        DECIMAL _decimal1;
                        DECIMAL decimal2;

                        DECIMAL _decimal3 = 789.123;
                        DECIMAL decimal4 = -101112.4567890;

                        CHARACTER _character1;
                        CHARACTER character2;

                        CHARACTER _character3 = 'A';
                        CHARACTER character4 = ' ';

                        BOOLEAN _boolean1;
                        BOOLEAN boolean2;

                        BOOLEAN _boolean3 = TRUE;
                        BOOLEAN boolean4 = FALSE;

                        STRING _string1;
                        STRING string2;

                        STRING _string3 = "SAMPLE STRING 1";
                        STRING string4 = " SAMPLE STRING 2 ";

                        output(string4);

                        */






                    }
                    else
                    {
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    };
                }
                else
                {
                    this->terminalOutputVec.push_back(currentASTNode->output);
                };
            }
            else if (currentASTNode->command == C_INTEGER || currentASTNode->command == C_DECIMAL || currentASTNode->command == C_CHARACTER || currentASTNode->command == C_BOOLEAN || currentASTNode->command == C_STRING)
            {
                if (this->variableStructMap.find(currentASTNode->variableMemoryAddress) != this->variableStructMap.end())
                {
                    VariableStruct* newVariableStruct = this->createNewVariableStruct(currentASTNode->integer, currentASTNode->decimal, currentASTNode->character, currentASTNode->boolean, currentASTNode->string, currentASTNode->isConstant, currentASTNode->variableMemoryAddress, currentASTNode->variableType, currentASTNode->variableName);
                    this->variableStructMap[newVariableStruct->variableMemoryAddress] = newVariableStruct;
                    if (this->variablesMap.find(newVariableStruct->variableName) != this->variablesMap.end())
                    {
                        this->variablesMap[newVariableStruct->variableName] = newVariableStruct->variableMemoryAddress;
                    }
                    else
                    {
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    };
                }
                else
                {
                    newInterpretationSuccess = false;
                    currentASTNode = nullptr;
                    break;
                };
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
VariableStruct* Interpreter::createNewVariableStruct(int integer, double decimal, char character, bool boolean, std::string string, bool isConstant, int variableMemoryAddress, std::string variableType, std::string variableName)
{
    VariableStruct* newVariableStruct = new VariableStruct;
    newVariableStruct->integer = integer;
    newVariableStruct->decimal = decimal;
    newVariableStruct->character = character;
    newVariableStruct->boolean = boolean;
    newVariableStruct->string = string;
    newVariableStruct->isConstant = isConstant;
    newVariableStruct->variableMemoryAddress = variableMemoryAddress;
    newVariableStruct->variableType = variableType;
    newVariableStruct->variableName = variableName;
    std::cout << "[INTERPRETER] ==================================================" << std::endl;
    std::cout << "[INTERPRETER] newVariableStruct->integer:" << newVariableStruct->integer << std::endl;
    std::cout << "[INTERPRETER] newVariableStruct->decimal:" << newVariableStruct->decimal << std::endl;
    std::cout << "[INTERPRETER] newVariableStruct->character:" << newVariableStruct->character << std::endl;
    std::cout << "[INTERPRETER] newVariableStruct->boolean:" << newVariableStruct->boolean << std::endl;
    std::cout << "[INTERPRETER] newVariableStruct->string:" << newVariableStruct->string << std::endl;
    std::cout << "[INTERPRETER] newVariableStruct->isConstant:" << newVariableStruct->isConstant << std::endl;
    std::cout << "[INTERPRETER] newVariableStruct->variableMemoryAddress:" << newVariableStruct->variableMemoryAddress << std::endl;
    std::cout << "[INTERPRETER] newVariableStruct->variableType:" << newVariableStruct->variableType << std::endl;
    std::cout << "[INTERPRETER] newVariableStruct->variableName:" << newVariableStruct->variableName << std::endl;
    std::cout << "[INTERPRETER] ==================================================" << std::endl;
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