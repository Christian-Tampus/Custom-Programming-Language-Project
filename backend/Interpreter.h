/* UPDATE VERSION [37] */

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
    int integer = 0;
    double decimal = 0.0;
    char character = ' ';
    bool boolean = false;
    std::string string = "";
    bool isConstant = false;
    int variableMemoryAddress = -1;
    std::string variableType = "";
    std::string variableName = "";
    std::string assignmentVariableName = "";
    bool successfullyCreatedVariableStruct = true;
};

/*
==================================================
IntegerOperand Struct
==================================================
*/
struct Operand
{
    int integer = 0;
    double decimal = 0.0;
    std::string string = "";
    int row = 0;
    int col = 0;
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
        bool performArithmetic(Operand* leftOperand, std::string variableType, std::vector<std::vector<std::string>> arithmeticOperationsVector, int& integerArithmetic, double& decimalArithmetic, std::string& stringArithmetic);
        VariableStruct* createNewVariableStruct(int integer, double decimal, char character, bool boolean, std::string string, bool isConstant, int variableMemoryAddress, std::string variableType, std::string variableName, std::string assignmentVariableName);
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
                    VariableStruct* newVariableStruct = this->createNewVariableStruct(currentASTNode->integer, currentASTNode->decimal, currentASTNode->character, currentASTNode->boolean, currentASTNode->string, currentASTNode->isConstant, currentASTNode->variableMemoryAddress, currentASTNode->variableType, currentASTNode->variableName, currentASTNode->assignmentVariableName);
                    if (newVariableStruct->successfullyCreatedVariableStruct == false)
                    {
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    };
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
            else if (currentASTNode->command == C_ASSIGNMENT_OPERATOR)
            {
                if (this->variablesMap.find(currentASTNode->variableName) != this->variablesMap.end())
                {
                    VariableStruct* variableStructToUpdate = this->variableStructMap[this->variablesMap[currentASTNode->variableName]];
                    std::string targetString = "CONSTANT";
                    int position = currentASTNode->variableName.find(targetString);
                    if (position != std::string::npos && position == 0)
                    {
                        std::cout << "[INTERPRETER] Error, Cannot Assign A Value To A [" << variableStructToUpdate->variableType << "]!" << std::endl; 
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    };
                    if (this->variablesMap.find(currentASTNode->assignmentOperatorValue) != this->variablesMap.end())
                    {
                        VariableStruct* variableStructToUpdateWith = this->variableStructMap[this->variablesMap[currentASTNode->assignmentOperatorValue]];
                        if (variableStructToUpdate->variableType == variableStructToUpdateWith->variableType || "CONSTANT " + variableStructToUpdate->variableType == variableStructToUpdateWith->variableType)
                        {
                            if (variableStructToUpdate->variableType == "INTEGER")
                            {
                                variableStructToUpdate->integer = variableStructToUpdateWith->integer;
                            }
                            else if (variableStructToUpdate->variableType == "DECIMAL")
                            {
                                variableStructToUpdate->decimal = variableStructToUpdateWith->decimal;
                            }
                            else if (variableStructToUpdate->variableType == "CHARACTER")
                            {
                                variableStructToUpdate->character = variableStructToUpdateWith->character;
                            }
                            else if (variableStructToUpdate->variableType == "BOOLEAN")
                            {
                                variableStructToUpdate->boolean = variableStructToUpdateWith->boolean;
                            }
                            else if (variableStructToUpdate->variableType == "STRING")
                            {
                                variableStructToUpdate->string = variableStructToUpdateWith->string;
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
                        if (variableStructToUpdate->variableType == "INTEGER")
                        {
                            try
                            {
                                variableStructToUpdate->integer = std::stoi(currentASTNode->assignmentOperatorValue);
                            }
                            catch (...)
                            {
                                std::cout << "[INTERPRETER] Cannot Assign A Non Integer As A Integer For Variable: " << currentASTNode->variableName << "!" << std::endl;
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        }
                        else if (variableStructToUpdate->variableType == "DECIMAL")
                        {
                            try
                            {
                                variableStructToUpdate->decimal = std::stod(currentASTNode->assignmentOperatorValue);
                            }
                            catch (...)
                            {
                                std::cout << "[INTERPRETER] Cannot Assign A Non Decimal As A Decimal For Variable: " << currentASTNode->variableName << "!" << std::endl;
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        }
                        else if (variableStructToUpdate->variableType == "CHARACTER")
                        {
                            if (currentASTNode->assignmentOperatorValue.size() == 3 && currentASTNode->assignmentOperatorValue[0] == '\'' && currentASTNode->assignmentOperatorValue[2] == '\'')
                            {
                                variableStructToUpdate->character = currentASTNode->assignmentOperatorValue[1];
                            }
                            else
                            {
                                std::cout << "[INTERPRETER] Cannot Assign A Non Character As A Character For Variable: " << currentASTNode->variableName << "!" << std::endl;
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        }
                        else if (variableStructToUpdate->variableType == "BOOLEAN")
                        {
                            if (currentASTNode->assignmentOperatorValue == "TRUE" || currentASTNode->assignmentOperatorValue == "FALSE")
                            {
                                variableStructToUpdate->boolean = (currentASTNode->assignmentOperatorValue == "TRUE" ? true : false);
                            }
                            else
                            {
                                std::cout << "[INTERPRETER] Cannot Assign A Non Boolean As A Boolean For Variable: " << currentASTNode->variableName << "!" << std::endl;
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        }
                        else if (variableStructToUpdate->variableType == "STRING")
                        {
                            if (currentASTNode->assignmentOperatorValue[0] == '\"' && currentASTNode->assignmentOperatorValue[currentASTNode->assignmentOperatorValue.length() - 1] == '\"')
                            {
                                variableStructToUpdate->string = currentASTNode->assignmentOperatorValue.substr(1, currentASTNode->assignmentOperatorValue.length() - 2);
                            }
                            else
                            {
                                std::cout << "[INTERPRETER] Cannot Assign A Non String As A String For Variable: " << currentASTNode->variableName << "!" << std::endl;
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
                }
                else
                {
                    newInterpretationSuccess = false;
                    currentASTNode = nullptr;
                    break;
                };
            }
            else if (currentASTNode->command == C_ARITHMETIC)
            {
                //123 + varInt1 - ((varInt2 / (601 + (481)) * 456 + 1111) * varInt3) + varInt4 - 789;
                //[PARSER] [STACK] [4]:481 |
                //[PARSER] [STACK] [3]:601 | + |
                //[PARSER] [STACK] [2]:varInt2 | / | * | 456 | + | 1111 |
                //[PARSER] [STACK] [1]:* | varInt3 |
                //[PARSER] [STACK] [0]:123 | + | varInt1 | - | + | varInt4 | - | 789 |
                if (this->variablesMap.find(currentASTNode->variableName) != this->variablesMap.end())
                {
                    VariableStruct* variableStructToUpdate = this->variableStructMap[this->variablesMap[currentASTNode->variableName]];
                    if (variableStructToUpdate->variableType == "INTEGER" || variableStructToUpdate->variableType == "DECIMAL" || variableStructToUpdate->variableType == "STRING")
                    {
                        Operand* firstOperand = new Operand;
                        int integerArithmetic = 0;
                        double decimalArithmetic = 0;
                        std::string stringArithmetic = "";
                        if(this->performArithmetic(firstOperand, currentASTNode->variableType, currentASTNode->arithmeticOperationsVector, integerArithmetic, decimalArithmetic, stringArithmetic))
                        {
                            if (variableStructToUpdate->variableType == "INTEGER")
                            {
                                variableStructToUpdate->integer = integerArithmetic;
                            }
                            else if (variableStructToUpdate->variableType == "DECIMAL")
                            {
                                variableStructToUpdate->decimal = decimalArithmetic;
                            }
                            else if (variableStructToUpdate->variableType == "STRING")
                            {
                                variableStructToUpdate->string = stringArithmetic;
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
bool Interpreter::performArithmetic(Operand* leftOperand, std::string variableType, std::vector<std::vector<std::string>> arithmeticOperationsVector, int& integerArithmetic, double& decimalArithmetic, std::string& stringArithmetic)
{
    //123 + varInt1 - ((varInt2 / (601 + (481)) * 456 + 1111) * varInt3) + varInt4 - 789;
    //[PARSER] [STACK] [4]:481 |
    //[PARSER] [STACK] [3]:601 | + |
    //[PARSER] [STACK] [2]:varInt2 | / | * | 456 | + | 1111 |
    //[PARSER] [STACK] [1]:* | varInt3 |
    //[PARSER] [STACK] [0]:123 | + | varInt1 | - | + | varInt4 | - | 789 |
    if (leftOperand != nullptr)
    {
        if (variableType == "INTEGER")
        {
            bool leftOperandIsAVariable = false;
            std::string leftOperandString = arithmeticOperationsVector[leftOperand->row][leftOperand->col];
            int leftOperandInteger = 0;
            VariableStruct* leftOperandVariableStruct;
            if (this->variablesMap.find(leftOperandString) != this->variablesMap.end())
            {
                leftOperandVariableStruct = this->variableStructMap[this->variablesMap[leftOperandString]];
                if (leftOperandVariableStruct->variableType == "INTEGER")
                {
                    leftOperandIsAVariable = true;
                }
                else
                {
                    return false;
                };
            }
            else if (leftOperandString == "+" || leftOperandString == "-" || leftOperandString == "*" || leftOperandString == "/")
            {
                /*
                WE MUST HANDLE WHEN LEFT SIDE IS ALREADY A COMPLETED OPERATION
                */
            }
            else
            {
                try
                {
                    leftOperandInteger = std::stoi(leftOperandString);
                }
                catch (...)
                {
                    std::cout << "[INTERPRETER] Error! Cannot Perform Arithmetic On A Non-Integer For An Integer!" << std::endl;
                    return false;
                };
            };
            //123 + varInt1 - ((varInt2 / (601 + (481)) * 456 + 1111) * varInt3) + varInt4 - 789;
            //[PARSER] [STACK] [4]:481 |
            //[PARSER] [STACK] [3]:601 | + |
            //[PARSER] [STACK] [2]:varInt2 | / | * | 456 | + | 1111 |
            //[PARSER] [STACK] [1]:* | varInt3 |
            //[PARSER] [STACK] [0]:123 | + | varInt1 | - | + | varInt4 | - | 789 |
            if (leftOperand->col + 1 < arithmeticOperationsVector[leftOperand->row].size())
            {
                std::string operatorString = arithmeticOperationsVector[leftOperand->row][leftOperand->col + 1];
                if (operatorString == "+" || operatorString == "-" || operatorString == "*" || operatorString == "/")
                {
                    if (leftOperand->col + 2 < arithmeticOperationsVector[leftOperand->row].size())
                    {
                        std::string rightOperandString = arithmeticOperationsVector[leftOperand->row][leftOperand->col + 2];
                        if (this->variablesMap.find(rightOperandString) != this->variablesMap.end())
                        {
                            VariableStruct* rightOperandVariableStruct = this->variableStructMap[this->variablesMap[rightOperandString]];
                            if (rightOperandVariableStruct->variableType == "INTEGER")
                            {
                                if (leftOperandIsAVariable)
                                {
                                    if (operatorString == "+")
                                    {
                                        leftOperand->integer = leftOperandVariableStruct->integer + rightOperandVariableStruct->integer;
                                        integerArithmetic = leftOperand->integer;
                                    }
                                    else if (operatorString == "-")
                                    {
                                        leftOperand->integer = leftOperandVariableStruct->integer - rightOperandVariableStruct->integer;
                                        integerArithmetic = leftOperand->integer;
                                    }
                                    else if (operatorString == "*")
                                    {
                                        leftOperand->integer = leftOperandVariableStruct->integer * rightOperandVariableStruct->integer;
                                        integerArithmetic = leftOperand->integer;
                                    }
                                    else if (operatorString == "/" && rightOperandVariableStruct->integer != 0)
                                    {
                                        leftOperand->integer = leftOperandVariableStruct->integer / rightOperandVariableStruct->integer;
                                        integerArithmetic = leftOperand->integer;
                                    }
                                    else
                                    {
                                        return false;
                                    };
                                    leftOperand->col += 3;
                                    return this->performArithmetic(leftOperand, variableType, arithmeticOperationsVector, integerArithmetic, decimalArithmetic, stringArithmetic);
                                }
                                else
                                {
                                    if (operatorString == "+")
                                    {
                                        leftOperand->integer = leftOperandInteger + rightOperandVariableStruct->integer;
                                        integerArithmetic = leftOperand->integer;
                                    }
                                    else if (operatorString == "-")
                                    {
                                        leftOperand->integer = leftOperandInteger - rightOperandVariableStruct->integer;
                                        integerArithmetic = leftOperand->integer;
                                    }
                                    else if (operatorString == "*")
                                    {
                                        leftOperand->integer = leftOperandInteger * rightOperandVariableStruct->integer;
                                        integerArithmetic = leftOperand->integer;
                                    }
                                    else if (operatorString == "/" && rightOperandVariableStruct->integer != 0)
                                    {
                                        leftOperand->integer = leftOperandInteger / rightOperandVariableStruct->integer;
                                        integerArithmetic = leftOperand->integer;
                                    }
                                    else
                                    {
                                        return false;
                                    };
                                    leftOperand->col += 3;
                                    return this->performArithmetic(leftOperand, variableType, arithmeticOperationsVector, integerArithmetic, decimalArithmetic, stringArithmetic);
                                };
                            }
                            else
                            {
                                return false;
                            };
                        }
                        else
                        {
                            int rightOperandInteger = 0;
                            try
                            {
                                leftOperandInteger = std::stoi(rightOperandString);
                            }
                            catch (...)
                            {
                                std::cout << "[INTERPRETER] Error! Cannot Perform Arithmetic On A Non-Integer For An Integer!" << std::endl;
                                return false;
                            };
                            if (leftOperandIsAVariable)
                            {
                                if (operatorString == "+")
                                {
                                    leftOperand->integer = leftOperandVariableStruct->integer + rightOperandInteger;
                                    integerArithmetic = leftOperand->integer;
                                }
                                else if (operatorString == "-")
                                {
                                    leftOperand->integer = leftOperandVariableStruct->integer - rightOperandInteger;
                                    integerArithmetic = leftOperand->integer;
                                }
                                else if (operatorString == "*")
                                {
                                    leftOperand->integer = leftOperandVariableStruct->integer * rightOperandInteger;
                                    integerArithmetic = leftOperand->integer;
                                }
                                else if (operatorString == "/" && rightOperandInteger != 0)
                                {
                                    leftOperand->integer = leftOperandVariableStruct->integer / rightOperandInteger;
                                    integerArithmetic = leftOperand->integer;
                                }
                                else
                                {
                                    return false;
                                };
                            }
                            else
                            {
                                if (operatorString == "+")
                                {
                                    leftOperand->integer = leftOperandInteger + rightOperandInteger;
                                    integerArithmetic = leftOperand->integer;
                                }
                                else if (operatorString == "-")
                                {
                                    leftOperand->integer = leftOperandInteger - rightOperandInteger;
                                    integerArithmetic = leftOperand->integer;
                                }
                                else if (operatorString == "*")
                                {
                                    leftOperand->integer = leftOperandInteger * rightOperandInteger;
                                    integerArithmetic = leftOperand->integer;
                                }
                                else if (operatorString == "/" && rightOperandInteger != 0)
                                {
                                    leftOperand->integer = leftOperandInteger / rightOperandInteger;
                                    integerArithmetic = leftOperand->integer;
                                }
                                else
                                {
                                    return false;
                                };
                            };
                        };
                    };
                }
                else
                {
                    return false;
                };
            }
            else
            {
                try
                {
                    leftOperandInteger = std::stoi(leftOperandString);
                }
                catch (...)
                {
                    std::cout << "[INTERPRETER] Error! Cannot Perform Arithmetic On A Non-Integer For An Integer!" << std::endl;
                    return false;
                };

            };
        }
        else if (variableType == "DECIMAL")
        {

        }
        else if (variableType == "STRING")
        {

        }
        else
        {
            return false;
        };
    };
    return true;
};

/*
==================================================
Returns terminalOutputVec
==================================================
*/
VariableStruct* Interpreter::createNewVariableStruct(int integer, double decimal, char character, bool boolean, std::string string, bool isConstant, int variableMemoryAddress, std::string variableType, std::string variableName, std::string assignmentVariableName)
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
    newVariableStruct->assignmentVariableName = assignmentVariableName;
    if (newVariableStruct->assignmentVariableName.length() > 0 && this->variablesMap.find(newVariableStruct->assignmentVariableName) != this->variablesMap.end())
    {
        VariableStruct* otherVariableStruct = this->variableStructMap[this->variablesMap[newVariableStruct->assignmentVariableName]];
        if (otherVariableStruct->variableType == newVariableStruct->variableType)
        {
            if (newVariableStruct->variableType == "INTEGER")
            {
                newVariableStruct->integer = otherVariableStruct->integer;
            }
            else if (newVariableStruct->variableType == "DECIMAL")
            {
                newVariableStruct->decimal = otherVariableStruct->decimal;
            }
            else if (newVariableStruct->variableType == "CHARACTER")
            {
                newVariableStruct->character = otherVariableStruct->character;
            }
            else if (newVariableStruct->variableType == "BOOLEAN")
            {
                newVariableStruct->boolean = otherVariableStruct->boolean;
            }
            else if (newVariableStruct->variableType == "STRING")
            {
                newVariableStruct->string = otherVariableStruct->string;
            }
            else
            {
                newVariableStruct->successfullyCreatedVariableStruct = false;
            };
        }
        else
        {
            newVariableStruct->successfullyCreatedVariableStruct = false;
        };
    };
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
    std::cout << "[INTERPRETER] newVariableStruct->assignmentVariableName:" << newVariableStruct->assignmentVariableName << std::endl;
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