/* UPDATE VERSION [33] */

#ifndef H_INTERPRETER
#define H_INTERPRETER

/*
==================================================
Dependencies
==================================================
*/
#include <iostream>
#include <map>
#include <iomanip>
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
        const int STANDARD_PRECISION = 10;
        int inputBufferIndex = 0;
        bool interpretationSuccess;
        std::vector<std::string> terminalOutputVec;
        std::map<int, VariableStruct*> variableStructMap;
        std::map<std::string, int> variablesMap;
        std::vector<std::string> inputBufferVec;
        VariableStruct* createNewVariableStruct(int integer, double decimal, char character, bool boolean, std::string string, bool isConstant, int variableMemoryAddress, std::string variableType, std::string variableName);
    public:
        Interpreter();
        ~Interpreter();
        bool interpret(ASTNode* root, std::string standardInput);
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
    this->inputBufferIndex = 0;
    this->terminalOutputVec.clear();
    this->variableStructMap.clear();
    this->variablesMap.clear();
    this->inputBufferVec.clear();
};

/*
==================================================
Interprets The Abstract Syntax Tree (AST)
==================================================
*/
bool Interpreter::interpret(ASTNode* root, std::string standardInput)
{
    std::string delimiter = " , ";
    std::string inputToken;
    int index = 0;
    while ((index = standardInput.find(delimiter)) != std::string::npos)
    {
        inputToken = standardInput.substr(0, index);
        this->inputBufferVec.push_back(inputToken);
        standardInput.erase(0, index + delimiter.length());
    };
    this->inputBufferVec.push_back(standardInput);
    for (int index = 0; index < this->inputBufferVec.size(); index++)
    {
        std::cout << "[INTERPRETER] Input Buffer Index[" << index << "] " << this->inputBufferVec[index] << std::endl;
    };
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
                        if (currentVariableStruct->variableType == "INTEGER" || currentVariableStruct->variableType == "CONSTANT INTEGER")
                        {
                            this->terminalOutputVec.push_back(std::to_string(currentVariableStruct->integer));
                        }
                        else if (currentVariableStruct->variableType == "DECIMAL" || currentVariableStruct->variableType == "CONSTANT DECIMAL")
                        {
                            this->terminalOutputVec.push_back(std::to_string(currentVariableStruct->decimal));
                        }
                        else if (currentVariableStruct->variableType == "CHARACTER" || currentVariableStruct->variableType == "CONSTANT CHARACTER")
                        {
                            std::string characterToString = "";
                            characterToString += currentVariableStruct->character;
                            this->terminalOutputVec.push_back(characterToString);
                        }
                        else if (currentVariableStruct->variableType == "BOOLEAN" || currentVariableStruct->variableType == "CONSTANT BOOLEAN")
                        {
                            this->terminalOutputVec.push_back((currentVariableStruct->boolean == true ? "TRUE" : "FALSE"));
                        }
                        else if (currentVariableStruct->variableType == "STRING" || currentVariableStruct->variableType == "CONSTANT STRING")
                        {
                            this->terminalOutputVec.push_back(currentVariableStruct->string);
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
                    this->terminalOutputVec.push_back(currentASTNode->output);
                };
            }
            else if (currentASTNode->command == C_INPUT)
            {
                std::cout << "[INTERPRETER] Input Variable Name:" << currentASTNode->inputVariableName << std::endl;
                std::string input = this->inputBufferVec[this->inputBufferIndex];
                this->terminalOutputVec.push_back(input);
                if (this->variablesMap.find(currentASTNode->inputVariableName) != this->variablesMap.end())
                {
                    VariableStruct* currentVariableStruct = this->variableStructMap[this->variablesMap[currentASTNode->inputVariableName]];
                    if (currentVariableStruct->isConstant == false)
                    {
                        if (this->inputBufferIndex + 1 > this->inputBufferVec.size())
                        {
                            newInterpretationSuccess = false;
                            currentASTNode = nullptr;
                            break;
                        };
                        std::string variableType = currentVariableStruct->variableType;
                        if (variableType == "INTEGER")
                        {
                            try
                            {
                                currentVariableStruct->integer = std::stoi(input);
                                this->inputBufferIndex++;
                            }
                            catch (...)
                            {
                                std::cout << "[INTERPRETER] String To Integer Conversion Error!" << std::endl;
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        }
                        else if (variableType == "DECIMAL")
                        {
                            try
                            {
                                currentVariableStruct->decimal = std::stod(input);
                                this->inputBufferIndex++;
                            }
                            catch (...)
                            {
                                std::cout << "[INTERPRETER] String To Double Conversion Error!" << std::endl;
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        }
                        else if (variableType == "CHARACTER")
                        {
                            if (input.size() > 1)
                            {
                                std::cout << "[INTERPRETER] String To Character Conversion Error!" << std::endl;
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break; 
                            };
                            currentVariableStruct->character = input[0];
                            this->inputBufferIndex++;
                        }
                        else if (variableType == "BOOLEAN")
                        {
                            if (input != "TRUE" && input != "FALSE")
                            {
                                std::cout << "[INTERPRETER] String To Boolean Conversion Error!" << std::endl;
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break; 
                            };
                            currentVariableStruct->boolean = (input == "TRUE" ? true : false);
                            this->inputBufferIndex++;
                        }
                        else if (variableType == "STRING")
                        {
                            currentVariableStruct->string = input;
                            this->inputBufferIndex++;
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
                        std::cout << "[INTERPRETER] Cannot Assign New Value To A Constant!" << std::endl;
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
            else if (currentASTNode->command == C_INTEGER || currentASTNode->command == C_DECIMAL || currentASTNode->command == C_CHARACTER || currentASTNode->command == C_BOOLEAN || currentASTNode->command == C_STRING || currentASTNode->command == C_CONSTANT_INTEGER || currentASTNode->command == C_CONSTANT_DECIMAL || currentASTNode->command == C_CONSTANT_CHARACTER || currentASTNode->command == C_CONSTANT_BOOLEAN || currentASTNode->command == C_CONSTANT_STRING)
            {
                if (this->variableStructMap.find(currentASTNode->variableMemoryAddress) == this->variableStructMap.end())
                {
                    VariableStruct* newVariableStruct = this->createNewVariableStruct(currentASTNode->integer, currentASTNode->decimal, currentASTNode->character, currentASTNode->boolean, currentASTNode->string, currentASTNode->isConstant, currentASTNode->variableMemoryAddress, currentASTNode->variableType, currentASTNode->variableName);
                    this->variableStructMap[newVariableStruct->variableMemoryAddress] = newVariableStruct;
                    if (this->variablesMap.find(newVariableStruct->variableName) == this->variablesMap.end())
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
    std::cout << "[INTERPRETER] newVariableStruct->decimal:" << std::setprecision(this->STANDARD_PRECISION) << newVariableStruct->decimal << std::endl;
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