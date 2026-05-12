/* UPDATE VERSION [38] */

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
Arithmetic Struct
==================================================
*/
struct ArithmeticStruct
{
    int integerArithmeticResult = 0;
    double decimalArithmeticResult = 0.0;
    std::string stringArithmeticResult = "";
    int index = 1;
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
        bool performArithmetic(ArithmeticStruct*& arithmeticStruct, std::vector<std::string> arithmeticVec, std::string variableType);
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
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] String To Integer Conversion Error!");
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
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] String To Double Conversion Error!");
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
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] String To Character Conversion Error!");
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
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] String To Boolean Conversion Error!");
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
                        this->terminalOutputVec.push_back("[SYSTEM ERROR] Cannot Assign New Value To A Constant!");
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
                        this->terminalOutputVec.push_back("[SYSTEM ERROR] Error, Cannot Assign A Value To A [" + variableStructToUpdate->variableType + "]!");
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
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] Cannot Assign A Non Integer As A Integer For Variable: " + currentASTNode->variableName + "]!");
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
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] Cannot Assign A Non Decimal As A Decimal For Variable: " + currentASTNode->variableName + "]!");
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
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] Cannot Assign A Non Character As A Character For Variable: " + currentASTNode->variableName + "]!");
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
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] Cannot Assign A Non Boolean As A Boolean For Variable: " + currentASTNode->variableName + "]!");
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
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] Cannot Assign A Non String As A String For Variable: " + currentASTNode->variableName + "]!");
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
                if (currentASTNode->arithmeticVec.size() < 3)
                {
                    newInterpretationSuccess = false;
                    currentASTNode = nullptr;
                    break;
                };
                if (this->variablesMap.find(currentASTNode->variableName) != this->variablesMap.end())
                {
                    VariableStruct* variableStructToUpdateWith = this->variableStructMap[this->variablesMap[currentASTNode->variableName]];
                    if (variableStructToUpdateWith->variableType == "INTEGER" || variableStructToUpdateWith->variableType == "DECIMAL" || variableStructToUpdateWith->variableType == "STRING")
                    {
                        ArithmeticStruct* arithmeticStruct = new ArithmeticStruct;
                        std::string firstOperand = currentASTNode->arithmeticVec[0];
                        if (variableStructToUpdateWith->variableType == "INTEGER")
                        {
                            if (this->variablesMap.find(firstOperand) != this->variablesMap.end())
                            {
                                VariableStruct* firstOperandStruct = this->variableStructMap[this->variablesMap[currentASTNode->arithmeticVec[0]]];
                                if (firstOperandStruct->variableType == "INTEGER")
                                {
                                    arithmeticStruct->integerArithmeticResult = firstOperandStruct->integer;
                                }
                                else
                                {
                                    std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non Integer To A Integer!" << std::endl;
                                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non Integer To A Integer!");
                                    newInterpretationSuccess = false;
                                    currentASTNode = nullptr;
                                    break;
                                };
                            }
                            else
                            {
                                int firstOperandInteger = 0;
                                try
                                {
                                    firstOperandInteger = std::stoi(firstOperand);
                                    arithmeticStruct->integerArithmeticResult = firstOperandInteger;
                                }
                                catch(...)
                                {
                                    std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non Integer To A Integer!" << std::endl;
                                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non Integer To A Integer!");
                                    newInterpretationSuccess = false;
                                    currentASTNode = nullptr;
                                    break;
                                };
                            };
                        }
                        else if (variableStructToUpdateWith->variableType == "DECIMAL")
                        {
                            if (this->variablesMap.find(firstOperand) != this->variablesMap.end())
                            {
                                VariableStruct* firstOperandStruct = this->variableStructMap[this->variablesMap[currentASTNode->arithmeticVec[0]]];
                                if (firstOperandStruct->variableType == "DECIMAL")
                                {
                                    arithmeticStruct->decimalArithmeticResult = firstOperandStruct->decimal;
                                }
                                else
                                {
                                    std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non Decimal To A Decimal!" << std::endl;
                                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non Decimal To A Decimal!");
                                    newInterpretationSuccess = false;
                                    currentASTNode = nullptr;
                                    break;
                                };
                            }
                            else
                            {
                                int firstOperandDecimal = 0;
                                try
                                {
                                    firstOperandDecimal = std::stoi(firstOperand);
                                    arithmeticStruct->decimalArithmeticResult = firstOperandDecimal;
                                }
                                catch(...)
                                {
                                    std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non Decimal To A Decimal!" << std::endl;
                                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non Decimal To A Decimal!");
                                    newInterpretationSuccess = false;
                                    currentASTNode = nullptr;
                                    break;
                                };
                            };
                        }
                        else if (variableStructToUpdateWith->variableType == "STRING")
                        {
                            if (this->variablesMap.find(firstOperand) != this->variablesMap.end())
                            {
                                VariableStruct* firstOperandStruct = this->variableStructMap[this->variablesMap[currentASTNode->arithmeticVec[0]]];
                                if (firstOperandStruct->variableType == "STRING")
                                {
                                    arithmeticStruct->stringArithmeticResult = firstOperandStruct->string;
                                }
                                else
                                {
                                    std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non String To A String!" << std::endl;
                                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non String To A String!");
                                    newInterpretationSuccess = false;
                                    currentASTNode = nullptr;
                                    break;
                                };
                            }
                            else if (currentASTNode->arithmeticVec[0][0] == '"' && currentASTNode->arithmeticVec[0][currentASTNode->arithmeticVec[0].length() - 1] == '"')
                            {
                                arithmeticStruct->stringArithmeticResult = currentASTNode->arithmeticVec[0].substr(1, currentASTNode->arithmeticVec[0].size() - 2);
                            }
                            else
                            {
                                std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non String To A String!" << std::endl;
                                this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non String To A String!");
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
                        if (this->performArithmetic(arithmeticStruct, currentASTNode->arithmeticVec, variableStructToUpdateWith->variableType))
                        {
                            if (variableStructToUpdateWith->variableType == "INTEGER")
                            {
                                variableStructToUpdateWith->integer = arithmeticStruct->integerArithmeticResult;
                            }
                            else if (variableStructToUpdateWith->variableType == "DECIMAL")
                            {
                                variableStructToUpdateWith->decimal = arithmeticStruct->decimalArithmeticResult;
                            }
                            else if (variableStructToUpdateWith->variableType == "STRING")
                            {
                                variableStructToUpdateWith->string = arithmeticStruct->stringArithmeticResult;
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
Recursively Performs Arithmetic Operations On Integers, Decimals Or Strings Only
==================================================
*/
bool Interpreter::performArithmetic(ArithmeticStruct*& arithmeticStruct, std::vector<std::string> arithmeticVec, std::string variableType)
{
    //123 [+] varInt1 - varInt2 * 456 / varInt3 + varInt4 - 789;
    std::cout << "arithmeticVec SIZE: " << arithmeticVec.size() << std::endl;
    for (int index = 0; index < arithmeticVec.size(); index++)
    {
        std::cout << "arithmeticVec[" << index << "]:" << arithmeticVec[index] << std::endl;
    };
    std::cout << "CURRENT INDEX: " << arithmeticStruct->index << std::endl;
    if (arithmeticStruct->index >= arithmeticVec.size())
    {
        return true;
    };
    std::string operatorString = arithmeticVec[arithmeticStruct->index];
    if (arithmeticStruct->index + 1 >= arithmeticVec.size() || arithmeticVec[arithmeticStruct->index].size() != 1 || operatorString != "+" && operatorString != "-" && operatorString != "*" && operatorString != "/")
    {
        return false;
    };
    if (variableType == "INTEGER")
    {
        std::string secondOperandString = arithmeticVec[arithmeticStruct->index + 1];
        if (this->variablesMap.find(secondOperandString) != this->variablesMap.end())
        {
            VariableStruct* secondOperandStruct = this->variableStructMap[this->variablesMap[secondOperandString]];
            if (secondOperandStruct->variableType == "INTEGER")
            {
                if (operatorString == "+")
                {
                    arithmeticStruct->integerArithmeticResult += secondOperandStruct->integer;
                }
                else if (operatorString == "-")
                {
                    arithmeticStruct->integerArithmeticResult -= secondOperandStruct->integer;
                }
                else if (operatorString == "*")
                {
                    arithmeticStruct->integerArithmeticResult *= secondOperandStruct->integer;
                }
                else if (operatorString == "/" && secondOperandStruct->integer != 0)
                {
                    arithmeticStruct->integerArithmeticResult /= secondOperandStruct->integer;
                }
                else
                {
                    std::cout << "[INTERPRETER] Error! Attempted To Perform Division By 0!" << std::endl;
                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempted To Perform Division By 0!");
                    return false;
                };
                arithmeticStruct->index += 2;
                return this->performArithmetic(arithmeticStruct, arithmeticVec, variableType);
            }
            else
            {
                std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non Integer To A Integer!" << std::endl;
                this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non Integer To A Integer!");
                return false;
            };
        }
        else
        {
            int secondOperandInteger = 0;
            try
            {
                secondOperandInteger = std::stoi(secondOperandString);
                if (operatorString == "+")
                {
                    arithmeticStruct->integerArithmeticResult += secondOperandInteger;
                }
                else if (operatorString == "-")
                {
                    arithmeticStruct->integerArithmeticResult -= secondOperandInteger;
                }
                else if (operatorString == "*")
                {
                    arithmeticStruct->integerArithmeticResult *= secondOperandInteger;
                }
                else if (operatorString == "/" && secondOperandInteger != 0)
                {
                    arithmeticStruct->integerArithmeticResult /= secondOperandInteger;
                }
                else
                {
                    std::cout << "[INTERPRETER] Error! Attempted To Perform Division By 0!" << std::endl;
                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempted To Perform Division By 0!");
                    return false;
                };
                arithmeticStruct->index += 2;
                return this->performArithmetic(arithmeticStruct, arithmeticVec, variableType);
            }
            catch(...)
            {
                std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non Integer To A Integer!" << std::endl;
                this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non Integer To A Integer!");
                return false;
            };
        };
    }
    else if (variableType == "DECIMAL")
    {
        std::string secondOperandString = arithmeticVec[arithmeticStruct->index + 1];
        if (this->variablesMap.find(secondOperandString) != this->variablesMap.end())
        {
            VariableStruct* secondOperandStruct = this->variableStructMap[this->variablesMap[secondOperandString]];
            if (secondOperandStruct->variableType == "DECIMAL")
            {
                if (operatorString == "+")
                {
                    arithmeticStruct->decimalArithmeticResult += secondOperandStruct->decimal;
                }
                else if (operatorString == "-")
                {
                    arithmeticStruct->decimalArithmeticResult -= secondOperandStruct->decimal;
                }
                else if (operatorString == "*")
                {
                    arithmeticStruct->decimalArithmeticResult *= secondOperandStruct->decimal;
                }
                else if (operatorString == "/" && secondOperandStruct->decimal != 0)
                {
                    arithmeticStruct->decimalArithmeticResult /= secondOperandStruct->decimal;
                }
                else
                {
                    std::cout << "[INTERPRETER] Error! Attempted To Perform Division By 0!" << std::endl;
                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempted To Perform Division By 0!");
                    return false;
                };
                arithmeticStruct->index += 2;
                return this->performArithmetic(arithmeticStruct, arithmeticVec, variableType);
            }
            else
            {
                std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non Decimal To A Decimal!" << std::endl;
                this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non Decimal To A Decimal!");
                return false;
            };
        }
        else
        {
            int secondOperandInteger = 0;
            try
            {
                secondOperandInteger = std::stoi(secondOperandString);
                if (operatorString == "+")
                {
                    arithmeticStruct->decimalArithmeticResult += secondOperandInteger;
                }
                else if (operatorString == "-")
                {
                    arithmeticStruct->decimalArithmeticResult -= secondOperandInteger;
                }
                else if (operatorString == "*")
                {
                    arithmeticStruct->decimalArithmeticResult *= secondOperandInteger;
                }
                else if (operatorString == "/" && secondOperandInteger != 0)
                {
                    arithmeticStruct->decimalArithmeticResult /= secondOperandInteger;
                }
                else
                {
                    std::cout << "[INTERPRETER] Error! Attempted To Perform Division By 0!" << std::endl;
                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempted To Perform Division By 0!");
                    return false;
                };
                arithmeticStruct->index += 2;
                return this->performArithmetic(arithmeticStruct, arithmeticVec, variableType);
            }
            catch(...)
            {
                std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non Decimal To A Decimal!" << std::endl;
                this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non Decimal To A Decimal!");
                return false;
            };
        };
    }
    else if (variableType == "STRING")
    {
        std::string secondOperandString = arithmeticVec[arithmeticStruct->index + 1];
        if (this->variablesMap.find(secondOperandString) != this->variablesMap.end())
        {
            VariableStruct* secondOperandStruct = this->variableStructMap[this->variablesMap[secondOperandString]];
            if (secondOperandStruct->variableType == "STRING")
            {
                if (operatorString == "+")
                {
                    arithmeticStruct->stringArithmeticResult += secondOperandStruct->string;
                }
                else
                {
                    std::cout << "[INTERPRETER] Error! Attempted To Perform Non Addition Operation On A String!" << std::endl;
                    this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempted To Perform Non Addition Operation On A String!");
                    return false;
                };
                arithmeticStruct->index += 2;
                return this->performArithmetic(arithmeticStruct, arithmeticVec, variableType);
            }
            else
            {
                std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non String To A String!" << std::endl;
                this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non String To A String!");
                return false;
            };
        }
        else if (secondOperandString[0] == '"' && secondOperandString[secondOperandString.size() - 1] == '"')
        {
            arithmeticStruct->stringArithmeticResult += secondOperandString.substr(1, secondOperandString.size() - 2);
            /*
            FIX THIS:
            STRING string1 = "HELLO WORLD!";
            output(string1);
            string1 = string1 + " " + string1;
            output(string1);
            
            STRING string1 = "HELLO WORLD!";
            output(string1);
            string1 = string1 + "a" + string1;
            output(string1);
            */
        }
        else
        {
            std::cout << "[INTERPRETER] Error! Attempting To Perform Arithmetic On Non String To A String!" << std::endl;
            this->terminalOutputVec.push_back("[SYSTEM ERROR] Error! Attempting To Perform Arithmetic On Non String To A String!");
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