/* UPDATE VERSION [51] */

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
    std::vector<int> integerArray;
    std::vector<double> decimalArray;
    std::vector<char> characterArray;
    std::vector<bool> booleanArray;
    std::vector<std::string> stringArray;
    std::string arrayVariableNameToAssignSize = "";
    bool successfullyCreatedVariableStruct = true;
    int scope = 0;
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
        int variableMemoryAddressCounter = -1;
        int currentScope = 0;
        const int STANDARD_PRECISION = 10;
        int inputBufferIndex = 0;
        bool interpretationSuccess;
        std::vector<std::string> terminalOutputVec;
        std::map<int, VariableStruct*> variableStructMap;
        std::map<std::string, int> variablesMap;
        std::vector<std::string> inputBufferVec;
        std::vector<ASTNode*> functionsVec;
        bool performArithmetic(ArithmeticStruct*& arithmeticStruct, std::vector<std::string> arithmeticVec, std::string variableType);
        VariableStruct* createNewVariableStruct(ASTNode* currentASTNode);
        bool extractAndAssignDataFromArray(std::string arrayVariableString, VariableStruct* variableStructToUpdate);
        void clearOutOfScopeVariables();
        void assignVariableMemoryAddress(ASTNode* currentASTNode);
    public:
        Interpreter();
        ~Interpreter();
        bool interpret(ASTNode* root, std::string standardInput);
        std::vector<std::string> getTerminalOutputVec();
        void setFunctionsVec(std::vector<ASTNode*> newFunctionsVec);
        void setVariableMemoryAddressCounter(int newVariableMemoryAddressCounter);
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
        bool takeControlFlowBranch = false;
        bool calledAFunction = false;
        bool returningFromAFunction = false;
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
                        else if (currentVariableStruct->variableType == "ARRAY INTEGER" || currentVariableStruct->variableType == "ARRAY DECIMAL" || currentVariableStruct->variableType == "ARRAY CHARACTER" || currentVariableStruct->variableType == "ARRAY BOOLEAN" || currentVariableStruct->variableType == "ARRAY STRING")
                        {
                            std::string variableType = currentVariableStruct->variableType;
                            std::string arrayString = "[";
                            if (variableType == "ARRAY INTEGER" && currentVariableStruct->integerArray.size() > 0)
                            {
                                for (int index = 0; index < currentVariableStruct->integerArray.size() - 1; index++)
                                {
                                    arrayString += std::to_string(currentVariableStruct->integerArray[index]) + ",";
                                };
                                arrayString += std::to_string(currentVariableStruct->integerArray[currentVariableStruct->integerArray.size() - 1]);
                            }
                            else if (variableType == "ARRAY DECIMAL" && currentVariableStruct->decimalArray.size() > 0)
                            {
                                for (int index = 0; index < currentVariableStruct->decimalArray.size() - 1; index++)
                                {
                                    arrayString += std::to_string(currentVariableStruct->decimalArray[index]) + ",";
                                };
                                arrayString += std::to_string(currentVariableStruct->decimalArray[currentVariableStruct->decimalArray.size() - 1]);
                            }
                            else if (variableType == "ARRAY CHARACTER" && currentVariableStruct->characterArray.size() > 0)
                            {
                                for (int index = 0; index < currentVariableStruct->characterArray.size() - 1; index++)
                                {
                                    arrayString += std::string("'") + currentVariableStruct->characterArray[index] + "',";
                                };
                                arrayString += std::string("'") + currentVariableStruct->characterArray[currentVariableStruct->characterArray.size() - 1] + "'";
                            }
                            else if (variableType == "ARRAY BOOLEAN" && currentVariableStruct->booleanArray.size() > 0)
                            {
                                for (int index = 0; index < currentVariableStruct->booleanArray.size() - 1; index++)
                                {
                                    arrayString += (currentVariableStruct->booleanArray[index] == true ? "TRUE," : "FALSE,");
                                };
                                arrayString += (currentVariableStruct->booleanArray[currentVariableStruct->booleanArray.size() - 1] == true ? "TRUE" : "FALSE");
                            }
                            else if (variableType == "ARRAY STRING" && currentVariableStruct->stringArray.size() > 0)
                            {
                                for (int index = 0; index < currentVariableStruct->stringArray.size() - 1; index++)
                                {
                                    arrayString += "\"" + currentVariableStruct->stringArray[index] + "\",";
                                };
                                arrayString += "\"" + currentVariableStruct->stringArray[currentVariableStruct->stringArray.size() - 1] + "\"";
                            };
                            arrayString += "]";
                            this->terminalOutputVec.push_back(arrayString);
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
            else if (currentASTNode->command == C_INTEGER || currentASTNode->command == C_DECIMAL || currentASTNode->command == C_CHARACTER || currentASTNode->command == C_BOOLEAN || currentASTNode->command == C_STRING || currentASTNode->command == C_CONSTANT_INTEGER || currentASTNode->command == C_CONSTANT_DECIMAL || currentASTNode->command == C_CONSTANT_CHARACTER || currentASTNode->command == C_CONSTANT_BOOLEAN || currentASTNode->command == C_CONSTANT_STRING || currentASTNode->command == C_ARRAY_INTEGER || currentASTNode->command == C_ARRAY_DECIMAL || currentASTNode->command == C_ARRAY_BOOLEAN || currentASTNode->command == C_ARRAY_CHARACTER || currentASTNode->command == C_ARRAY_STRING)
            {
                if (this->variableStructMap.find(currentASTNode->variableMemoryAddress) == this->variableStructMap.end())
                {
                    VariableStruct* newVariableStruct = this->createNewVariableStruct(currentASTNode);
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
                std::cout << "ASSIGNMENT OPERATOR!" << std::endl;
                if (currentASTNode->functionASTNodeType.length() > 0 && currentASTNode->functionASTNodeType == "ASSIGNMENT FUNCTION CALL")
                {
                    std::cout << "C_ASSIGNMENT_OPERATOR FUNCTION CALL AST NODE!" << std::endl;
                    bool functionCallSuccess = false;
                    for (int index1 = 0; index1 < this->functionsVec.size(); index1++)
                    {
                        if (this->functionsVec[index1]->functionName == currentASTNode->functionToCall)
                        {
                            functionCallSuccess = true;
                            std::cout << "CALL FUNCTION: " << currentASTNode->functionToCall << std::endl;
                            if (this->functionsVec[index1]->functionVariablesVec.size() == currentASTNode->functionInputVariableVec.size())
                            {
                                for (int index2 = 0; index2 < this->functionsVec[index1]->functionVariablesVec.size(); index2++)
                                {
                                    if (currentASTNode->functionInputVariableVec[index2].variableDataType == "VARIABLE")
                                    {
                                        if (this->variablesMap.find(currentASTNode->functionInputVariableVec[index2].variableName) != this->variablesMap.end())
                                        {
                                            VariableStruct* variableStruct = this->variableStructMap[this->variablesMap[currentASTNode->functionInputVariableVec[index2].variableName]];
                                            currentASTNode->functionInputVariableVec[index2].variableDataType = variableStruct->variableType;
                                        }
                                        else
                                        {
                                            newInterpretationSuccess = false;
                                            currentASTNode = nullptr;
                                            break;
                                        };
                                    };
                                    if (this->functionsVec[index1]->functionVariablesVec[index2].variableDataType == currentASTNode->functionInputVariableVec[index2].variableDataType)
                                    {
                                        this->functionsVec[index1]->functionInputVariableVec.push_back(currentASTNode->functionInputVariableVec[index2]);
                                    }
                                    else
                                    {
                                        newInterpretationSuccess = false;
                                        currentASTNode = nullptr;
                                        break;
                                    };
                                };
                                if (this->functionsVec[index1]->functionInputVariableVec.size() == currentASTNode->functionInputVariableVec.size())
                                {
                                    this->functionsVec[index1]->functionCallReturnASTNode = currentASTNode;
                                    currentASTNode = this->functionsVec[index1];
                                    functionCallSuccess = true;
                                    break;
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
                    };
                    if (functionCallSuccess == false)
                    {
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    }
                    else
                    {
                        calledAFunction = true;
                    };
                }
                else
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
                        if (this->extractAndAssignDataFromArray(currentASTNode->assignmentOperatorValue, variableStructToUpdate) == false)
                        {
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
                                if (firstOperandStruct->variableType == "INTEGER" || firstOperandStruct->variableType == "CONSTANT INTEGER")
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
                                if (firstOperandStruct->variableType == "DECIMAL" || firstOperandStruct->variableType == "CONSTANT DECIMAL")
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
                                if (firstOperandStruct->variableType == "STRING" || firstOperandStruct->variableType == "CONSTANT STRING")
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
                            if (variableStructToUpdateWith->variableType == "INTEGER" || variableStructToUpdateWith->variableType == "CONSTANT INTEGER")
                            {
                                variableStructToUpdateWith->integer = arithmeticStruct->integerArithmeticResult;
                            }
                            else if (variableStructToUpdateWith->variableType == "DECIMAL" || variableStructToUpdateWith->variableType == "CONSTANT DECIMAL")
                            {
                                variableStructToUpdateWith->decimal = arithmeticStruct->decimalArithmeticResult;
                            }
                            else if (variableStructToUpdateWith->variableType == "STRING" || variableStructToUpdateWith->variableType == "CONSTANT STRING")
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
            else if (currentASTNode->command == C_CONTROL_FLOW)
            {
                if (currentASTNode->controlFlowType == IF || currentASTNode->controlFlowType == ELSE_IF)
                {
                    std::vector<bool> comparisonResultsVec;
                    for (int index = 0; index < currentASTNode->comparisonStructVec.size(); index++)
                    {
                        ComparisonStruct currentComparisonStruct = currentASTNode->comparisonStructVec[index];
                        VariableStruct* operand1VariableStruct = nullptr;
                        VariableStruct* operand2VariableStruct = nullptr;
                        if (currentComparisonStruct.operand1IsAVariable == true)
                        {
                            if (this->variablesMap.find(currentComparisonStruct.operand1VariableName) != this->variablesMap.end())
                            {
                                operand1VariableStruct = this->variableStructMap[this->variablesMap[currentComparisonStruct.operand1VariableName]];
                            }
                            else
                            {
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        };
                        if (currentComparisonStruct.operand2IsAVariable == true)
                        {
                            if (this->variablesMap.find(currentComparisonStruct.operand2VariableName) != this->variablesMap.end())
                            {
                                operand2VariableStruct = this->variableStructMap[this->variablesMap[currentComparisonStruct.operand2VariableName]];
                            }
                            else
                            {
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        };
                        if (currentComparisonStruct.dataType == D_INTEGER)
                        {
                            int operand1Value = 0;
                            int operand2Value = 0;
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "INTEGER" || operand1VariableStruct->variableType == "CONSTANT INTEGER")
                                {
                                    operand1Value = operand1VariableStruct->integer;
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
                                operand1Value = currentComparisonStruct.integerOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "INTEGER" || operand2VariableStruct->variableType == "CONSTANT INTEGER")
                                {
                                    operand2Value = operand2VariableStruct->integer;
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
                                operand2Value = currentComparisonStruct.integerOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResultsVec.push_back((operand1Value > operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResultsVec.push_back((operand1Value < operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResultsVec.push_back((operand1Value >= operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResultsVec.push_back((operand1Value <= operand2Value));
                            };
                        }
                        else if (currentComparisonStruct.dataType == D_DECIMAL)
                        {
                            double operand1Value = 0.0;
                            double operand2Value = 0.0;
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "DECIMAL" || operand1VariableStruct->variableType == "CONSTANT DECIMAL")
                                {
                                    operand1Value = operand1VariableStruct->decimal;
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
                                operand1Value = currentComparisonStruct.decimalOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "DECIMAL" || operand2VariableStruct->variableType == "CONSTANT DECIMAL")
                                {
                                    operand2Value = operand2VariableStruct->decimal;
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
                                operand2Value = currentComparisonStruct.decimalOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResultsVec.push_back((operand1Value > operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResultsVec.push_back((operand1Value < operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResultsVec.push_back((operand1Value >= operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResultsVec.push_back((operand1Value <= operand2Value));
                            };
                        }
                        else if (currentComparisonStruct.dataType == D_CHARACTER)
                        {
                            char operand1Value = ' ';
                            char operand2Value = ' ';
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "CHARACTER" || operand1VariableStruct->variableType == "CONSTANT CHARACTER")
                                {
                                    operand1Value = operand1VariableStruct->character;
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
                                operand1Value = currentComparisonStruct.characterOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "CHARACTER" || operand2VariableStruct->variableType == "CONSTANT CHARACTER")
                                {
                                    operand2Value = operand2VariableStruct->character;
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
                                operand2Value = currentComparisonStruct.characterOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResultsVec.push_back((operand1Value > operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResultsVec.push_back((operand1Value < operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResultsVec.push_back((operand1Value >= operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResultsVec.push_back((operand1Value <= operand2Value));
                            };
                        }
                        else if (currentComparisonStruct.dataType == D_BOOLEAN)
                        {
                            bool operand1Value = false;
                            bool operand2Value = false;
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "BOOLEAN" || operand1VariableStruct->variableType == "CONSTANT BOOLEAN")
                                {
                                    operand1Value = operand1VariableStruct->boolean;
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
                                operand1Value = currentComparisonStruct.booleanOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "BOOLEAN" || operand2VariableStruct->variableType == "CONSTANT BOOLEAN")
                                {
                                    operand2Value = operand2VariableStruct->boolean;
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
                                operand2Value = currentComparisonStruct.booleanOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            };
                        }
                        else if (currentComparisonStruct.dataType == D_STRING)
                        {
                            std::string operand1Value = "";
                            std::string operand2Value = "";
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "STRING" || operand1VariableStruct->variableType == "CONSTANT STRING")
                                {
                                    operand1Value = operand1VariableStruct->string;
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
                                operand1Value = currentComparisonStruct.stringOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "STRING" || operand2VariableStruct->variableType == "CONSTANT STRING")
                                {
                                    operand2Value = operand2VariableStruct->string;
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
                                operand2Value = currentComparisonStruct.stringOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResultsVec.push_back((operand1Value > operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResultsVec.push_back((operand1Value < operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResultsVec.push_back((operand1Value >= operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResultsVec.push_back((operand1Value <= operand2Value));
                            };
                        };
                    };
                    if (currentASTNode->comparisonStructVec.size() != comparisonResultsVec.size() || comparisonResultsVec.size() != currentASTNode->logicalOperatorVec.size() + 1)
                    {
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    }
                    else if (comparisonResultsVec.size() == 1 && currentASTNode->logicalOperatorVec.size() == 0)
                    {
                        if (comparisonResultsVec[0] == false)
                        {
                            takeControlFlowBranch = true;
                        };
                    }
                    else
                    {
                        int comparisonResultsVecIndex = 0;
                        bool breakEarly = false;
                        for (int index = 0; index < currentASTNode->logicalOperatorVec.size(); index++)
                        {
                            if (currentASTNode->logicalOperatorVec[index] == OR)
                            {
                                bool continueChecks = false;
                                while (comparisonResultsVecIndex <= index)
                                {
                                    if (comparisonResultsVec[comparisonResultsVecIndex] == false)
                                    {
                                        continueChecks = true;
                                        break;
                                    };
                                    comparisonResultsVecIndex++;
                                };
                                comparisonResultsVecIndex = index + 1;
                                if (continueChecks == false)
                                {
                                    breakEarly = true;
                                    break;
                                };
                            };
                        };
                        if (breakEarly == false)
                        {
                            while (comparisonResultsVecIndex < comparisonResultsVec.size())
                            {
                                if (comparisonResultsVec[comparisonResultsVecIndex] == false)
                                {
                                    takeControlFlowBranch = true;
                                    break;
                                };
                                comparisonResultsVecIndex++;
                            };
                        };
                    };
                }
                else if (currentASTNode->controlFlowType == ELSE)
                {
                    std::cout << "[INTERPRETER] ELSE BRANCH!" << std::endl;
                }
                else if (currentASTNode->controlFlowType == END)
                {
                    std::cout << "[INTERPRETER] END CONTROL FLOW!" << std::endl;
                }
                else
                {
                    newInterpretationSuccess = false;
                    currentASTNode = nullptr;
                    break;
                };
            }
            else if (currentASTNode->command == C_FOR_LOOP)
            {
                if (currentASTNode->controlFlowType == FOR)
                {
                    std::cout << "[INTERPRETER] FOR LOOP!" << std::endl;
                    if (currentASTNode->comparisonStructVec.size() == 1)
                    {
                        bool comparisonResult = false;
                        ComparisonStruct forLoopConditionComparisonStruct = currentASTNode->comparisonStructVec[0];
                        VariableStruct* operand1VariableStruct = nullptr;
                        VariableStruct* operand2VariableStruct = nullptr;
                        if (forLoopConditionComparisonStruct.operand1IsAVariable == true)
                        {
                            if (this->variablesMap.find(forLoopConditionComparisonStruct.operand1VariableName) != this->variablesMap.end())
                            {
                                operand1VariableStruct = this->variableStructMap[this->variablesMap[forLoopConditionComparisonStruct.operand1VariableName]];
                            }
                            else
                            {
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        };
                        if (forLoopConditionComparisonStruct.operand2IsAVariable == true)
                        {
                            if (this->variablesMap.find(forLoopConditionComparisonStruct.operand2VariableName) != this->variablesMap.end())
                            {
                                operand2VariableStruct = this->variableStructMap[this->variablesMap[forLoopConditionComparisonStruct.operand2VariableName]];
                            }
                            else
                            {
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        };
                        if (forLoopConditionComparisonStruct.dataType == D_INTEGER)
                        {
                            int operand1Value = 0;
                            int operand2Value = 0;
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "INTEGER" || operand1VariableStruct->variableType == "CONSTANT INTEGER")
                                {
                                    operand1Value = operand1VariableStruct->integer;
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
                                operand1Value = forLoopConditionComparisonStruct.integerOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "INTEGER" || operand2VariableStruct->variableType == "CONSTANT INTEGER")
                                {
                                    operand2Value = operand2VariableStruct->integer;
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
                                operand2Value = forLoopConditionComparisonStruct.integerOperand2;
                            };
                            if (forLoopConditionComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResult = (operand1Value == operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResult = (operand1Value != operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResult = (operand1Value > operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResult = (operand1Value < operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResult = (operand1Value >= operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResult = (operand1Value <= operand2Value);
                            };
                        }
                        else if (forLoopConditionComparisonStruct.dataType == D_DECIMAL)
                        {
                            double operand1Value = 0.0;
                            double operand2Value = 0.0;
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "DECIMAL" || operand1VariableStruct->variableType == "CONSTANT DECIMAL")
                                {
                                    operand1Value = operand1VariableStruct->decimal;
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
                                operand1Value = forLoopConditionComparisonStruct.decimalOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "DECIMAL" || operand2VariableStruct->variableType == "CONSTANT DECIMAL")
                                {
                                    operand2Value = operand2VariableStruct->decimal;
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
                                operand2Value = forLoopConditionComparisonStruct.decimalOperand2;
                            };
                            if (forLoopConditionComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResult = (operand1Value == operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResult = (operand1Value != operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResult = (operand1Value > operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResult = (operand1Value < operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResult = (operand1Value >= operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResult = (operand1Value <= operand2Value);
                            };
                        }
                        else if (forLoopConditionComparisonStruct.dataType == D_CHARACTER)
                        {
                            char operand1Value = ' ';
                            char operand2Value = ' ';
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "CHARACTER" || operand1VariableStruct->variableType == "CONSTANT CHARACTER")
                                {
                                    operand1Value = operand1VariableStruct->character;
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
                                operand1Value = forLoopConditionComparisonStruct.characterOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "CHARACTER" || operand2VariableStruct->variableType == "CONSTANT CHARACTER")
                                {
                                    operand2Value = operand2VariableStruct->character;
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
                                operand2Value = forLoopConditionComparisonStruct.characterOperand2;
                            };
                            if (forLoopConditionComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResult = (operand1Value == operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResult = (operand1Value != operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResult = (operand1Value > operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResult = (operand1Value < operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResult = (operand1Value >= operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResult = (operand1Value <= operand2Value);
                            };
                        }
                        else if (forLoopConditionComparisonStruct.dataType == D_BOOLEAN)
                        {
                            bool operand1Value = false;
                            bool operand2Value = false;
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "BOOLEAN" || operand1VariableStruct->variableType == "CONSTANT BOOLEAN")
                                {
                                    operand1Value = operand1VariableStruct->boolean;
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
                                operand1Value = forLoopConditionComparisonStruct.booleanOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "BOOLEAN" || operand2VariableStruct->variableType == "CONSTANT BOOLEAN")
                                {
                                    operand2Value = operand2VariableStruct->boolean;
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
                                operand2Value = forLoopConditionComparisonStruct.booleanOperand2;
                            };
                            if (forLoopConditionComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResult = (operand1Value == operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResult = (operand1Value != operand2Value);
                            };
                        }
                        else if (forLoopConditionComparisonStruct.dataType == D_STRING)
                        {
                            std::string operand1Value = "";
                            std::string operand2Value = "";
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "STRING" || operand1VariableStruct->variableType == "CONSTANT STRING")
                                {
                                    operand1Value = operand1VariableStruct->string;
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
                                operand1Value = forLoopConditionComparisonStruct.stringOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "STRING" || operand2VariableStruct->variableType == "CONSTANT STRING")
                                {
                                    operand2Value = operand2VariableStruct->string;
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
                                operand2Value = forLoopConditionComparisonStruct.stringOperand2;
                            };
                            if (forLoopConditionComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResult = (operand1Value == operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResult = (operand1Value != operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResult = (operand1Value > operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResult = (operand1Value < operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResult = (operand1Value >= operand2Value);
                            }
                            else if (forLoopConditionComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResult = (operand1Value <= operand2Value);
                            };
                        };
                        currentASTNode->forLoopCompleted = (comparisonResult == true ? false : true);
                        if (currentASTNode->forLoopCompleted == true)
                        {
                            takeControlFlowBranch = true;
                        };
                    }
                    else
                    {
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    };
                }
                else if (currentASTNode->controlFlowType == END)
                {
                    std::cout << "[INTERPRETER] FOR END!" << std::endl;
                    ASTNode* previousASTNode = currentASTNode;
                    currentASTNode = currentASTNode->controlFlowASTNode;
                    if (currentASTNode->forLoopCompleted == false)
                    {
                        if (currentASTNode->forLoopIncrementVariableName.size() > 0)
                        {
                            if (this->variablesMap.find(currentASTNode->forLoopIncrementVariableName) != this->variablesMap.end())
                            {
                                VariableStruct* variableToIncrementOrDecrement = this->variableStructMap[this->variablesMap[currentASTNode->forLoopIncrementVariableName]];
                                if (variableToIncrementOrDecrement->variableType == "INTEGER")
                                {
                                    if (currentASTNode->forLoopIncrementOrDecrementVariableCommand == C_FOR_LOOP_INCREMENT)
                                    {
                                        variableToIncrementOrDecrement->integer += 1;
                                        currentASTNode = previousASTNode;
                                    }
                                    else if (currentASTNode->forLoopIncrementOrDecrementVariableCommand == C_FOR_LOOP_DECREMENT)
                                    {
                                        variableToIncrementOrDecrement->integer -= 1;
                                        currentASTNode = previousASTNode;
                                    }
                                    else
                                    {
                                        newInterpretationSuccess = false;
                                        currentASTNode = nullptr;
                                        break;
                                    };
                                    std::cout << "variableToIncrementOrDecrement->integer:" << variableToIncrementOrDecrement->integer << std::endl;
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
                        takeControlFlowBranch = true;
                    }
                    else
                    {
                        currentASTNode = previousASTNode;
                    };
                }
                else
                {
                    newInterpretationSuccess = false;
                    currentASTNode = nullptr;
                    break;
                };
            }
            else if (currentASTNode->command == C_WHILE_LOOP)
            {
                if (currentASTNode->controlFlowType == WHILE)
                {
                    std::cout << "[INTERPRETER] WHILE LOOP!" << std::endl;
                    std::vector<bool> comparisonResultsVec;
                    for (int index = 0; index < currentASTNode->comparisonStructVec.size(); index++)
                    {
                        ComparisonStruct currentComparisonStruct = currentASTNode->comparisonStructVec[index];
                        VariableStruct* operand1VariableStruct = nullptr;
                        VariableStruct* operand2VariableStruct = nullptr;
                        if (currentComparisonStruct.operand1IsAVariable == true)
                        {
                            if (this->variablesMap.find(currentComparisonStruct.operand1VariableName) != this->variablesMap.end())
                            {
                                operand1VariableStruct = this->variableStructMap[this->variablesMap[currentComparisonStruct.operand1VariableName]];
                            }
                            else
                            {
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        };
                        if (currentComparisonStruct.operand2IsAVariable == true)
                        {
                            if (this->variablesMap.find(currentComparisonStruct.operand2VariableName) != this->variablesMap.end())
                            {
                                operand2VariableStruct = this->variableStructMap[this->variablesMap[currentComparisonStruct.operand2VariableName]];
                            }
                            else
                            {
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        };
                        if (currentComparisonStruct.dataType == D_INTEGER)
                        {
                            int operand1Value = 0;
                            int operand2Value = 0;
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "INTEGER" || operand1VariableStruct->variableType == "CONSTANT INTEGER")
                                {
                                    operand1Value = operand1VariableStruct->integer;
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
                                operand1Value = currentComparisonStruct.integerOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "INTEGER" || operand2VariableStruct->variableType == "CONSTANT INTEGER")
                                {
                                    operand2Value = operand2VariableStruct->integer;
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
                                operand2Value = currentComparisonStruct.integerOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResultsVec.push_back((operand1Value > operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResultsVec.push_back((operand1Value < operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResultsVec.push_back((operand1Value >= operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResultsVec.push_back((operand1Value <= operand2Value));
                            };
                        }
                        else if (currentComparisonStruct.dataType == D_DECIMAL)
                        {
                            double operand1Value = 0.0;
                            double operand2Value = 0.0;
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "DECIMAL" || operand1VariableStruct->variableType == "CONSTANT DECIMAL")
                                {
                                    operand1Value = operand1VariableStruct->decimal;
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
                                operand1Value = currentComparisonStruct.decimalOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "DECIMAL" || operand2VariableStruct->variableType == "CONSTANT DECIMAL")
                                {
                                    operand2Value = operand2VariableStruct->decimal;
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
                                operand2Value = currentComparisonStruct.decimalOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResultsVec.push_back((operand1Value > operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResultsVec.push_back((operand1Value < operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResultsVec.push_back((operand1Value >= operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResultsVec.push_back((operand1Value <= operand2Value));
                            };
                        }
                        else if (currentComparisonStruct.dataType == D_CHARACTER)
                        {
                            char operand1Value = ' ';
                            char operand2Value = ' ';
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "CHARACTER" || operand1VariableStruct->variableType == "CONSTANT CHARACTER")
                                {
                                    operand1Value = operand1VariableStruct->character;
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
                                operand1Value = currentComparisonStruct.characterOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "CHARACTER" || operand2VariableStruct->variableType == "CONSTANT CHARACTER")
                                {
                                    operand2Value = operand2VariableStruct->character;
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
                                operand2Value = currentComparisonStruct.characterOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResultsVec.push_back((operand1Value > operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResultsVec.push_back((operand1Value < operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResultsVec.push_back((operand1Value >= operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResultsVec.push_back((operand1Value <= operand2Value));
                            };
                        }
                        else if (currentComparisonStruct.dataType == D_BOOLEAN)
                        {
                            bool operand1Value = false;
                            bool operand2Value = false;
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "BOOLEAN" || operand1VariableStruct->variableType == "CONSTANT BOOLEAN")
                                {
                                    operand1Value = operand1VariableStruct->boolean;
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
                                operand1Value = currentComparisonStruct.booleanOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "BOOLEAN" || operand2VariableStruct->variableType == "CONSTANT BOOLEAN")
                                {
                                    operand2Value = operand2VariableStruct->boolean;
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
                                operand2Value = currentComparisonStruct.booleanOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            };
                        }
                        else if (currentComparisonStruct.dataType == D_STRING)
                        {
                            std::string operand1Value = "";
                            std::string operand2Value = "";
                            if (operand1VariableStruct != nullptr)
                            {
                                if (operand1VariableStruct->variableType == "STRING" || operand1VariableStruct->variableType == "CONSTANT STRING")
                                {
                                    operand1Value = operand1VariableStruct->string;
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
                                operand1Value = currentComparisonStruct.stringOperand1;
                            };
                            if (operand2VariableStruct != nullptr)
                            {
                                if (operand2VariableStruct->variableType == "STRING" || operand2VariableStruct->variableType == "CONSTANT STRING")
                                {
                                    operand2Value = operand2VariableStruct->string;
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
                                operand2Value = currentComparisonStruct.stringOperand2;
                            };
                            if (currentComparisonStruct.comparisonOperator == EQ)
                            {
                                comparisonResultsVec.push_back((operand1Value == operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == NE)
                            {
                                comparisonResultsVec.push_back((operand1Value != operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GT)
                            {
                                comparisonResultsVec.push_back((operand1Value > operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LT)
                            {
                                comparisonResultsVec.push_back((operand1Value < operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == GE)
                            {
                                comparisonResultsVec.push_back((operand1Value >= operand2Value));
                            }
                            else if (currentComparisonStruct.comparisonOperator == LE)
                            {
                                comparisonResultsVec.push_back((operand1Value <= operand2Value));
                            };
                        };
                    };
                    if (currentASTNode->comparisonStructVec.size() != comparisonResultsVec.size() || comparisonResultsVec.size() != currentASTNode->logicalOperatorVec.size() + 1)
                    {
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    }
                    else if (comparisonResultsVec.size() == 1 && currentASTNode->logicalOperatorVec.size() == 0)
                    {
                        if (comparisonResultsVec[0] == false)
                        {
                            takeControlFlowBranch = true;
                            currentASTNode->whileLoopCompleted = true;
                        };
                    }
                    else
                    {
                        int comparisonResultsVecIndex = 0;
                        bool breakEarly = false;
                        for (int index = 0; index < currentASTNode->logicalOperatorVec.size(); index++)
                        {
                            if (currentASTNode->logicalOperatorVec[index] == OR)
                            {
                                bool continueChecks = false;
                                while (comparisonResultsVecIndex <= index)
                                {
                                    if (comparisonResultsVec[comparisonResultsVecIndex] == false)
                                    {
                                        continueChecks = true;
                                        break;
                                    };
                                    comparisonResultsVecIndex++;
                                };
                                comparisonResultsVecIndex = index + 1;
                                if (continueChecks == false)
                                {
                                    breakEarly = true;
                                    break;
                                };
                            };
                        };
                        if (breakEarly == false)
                        {
                            while (comparisonResultsVecIndex < comparisonResultsVec.size())
                            {
                                if (comparisonResultsVec[comparisonResultsVecIndex] == false)
                                {
                                    takeControlFlowBranch = true;
                                    currentASTNode->whileLoopCompleted = true;
                                    break;
                                };
                                comparisonResultsVecIndex++;
                            };
                        };
                    };
                }
                else if (currentASTNode->controlFlowType == END)
                {
                    std::cout << "[INTERPRETER] WHILE END!" << std::endl;
                    ASTNode* previousASTNode = currentASTNode;
                    currentASTNode = currentASTNode->controlFlowASTNode;
                    if (currentASTNode->whileLoopCompleted == false)
                    {
                        takeControlFlowBranch = true;
                    };
                    currentASTNode = previousASTNode;
                }
                else
                {
                    newInterpretationSuccess = false;
                    currentASTNode = nullptr;
                    break;
                };
            }
            else if (currentASTNode->command == C_FUNCTION)
            {
                std::cout << "FUNCTION!!!!!" << std::endl;
                std::cout << "currentASTNode->functionASTNodeType:" << currentASTNode->functionASTNodeType << std::endl;
                if (currentASTNode->functionASTNodeType == "FUNCTION START")
                {
                    std::cout << "FUNCTION START AST NODE!" << std::endl;
                    if (currentASTNode->functionVariablesVec.size() > 0 && currentASTNode->functionVariablesVec.size() == currentASTNode->functionInputVariableVec.size())
                    {
                        for (int index = 0; index < currentASTNode->functionVariablesVec.size(); index++)
                        {
                            ASTNode* newASTNodeInputVariable = new ASTNode;
                            newASTNodeInputVariable->variableName = currentASTNode->functionVariablesVec[index].variableName;
                            newASTNodeInputVariable->variableType = currentASTNode->functionVariablesVec[index].variableDataType;
                            newASTNodeInputVariable->integer = currentASTNode->functionInputVariableVec[index].integer;
                            newASTNodeInputVariable->decimal = currentASTNode->functionInputVariableVec[index].decimal;
                            newASTNodeInputVariable->character = currentASTNode->functionInputVariableVec[index].character;
                            newASTNodeInputVariable->boolean = currentASTNode->functionInputVariableVec[index].boolean;
                            newASTNodeInputVariable->string = currentASTNode->functionInputVariableVec[index].string;
                            this->assignVariableMemoryAddress(newASTNodeInputVariable);
                            if (currentASTNode->functionInputVariableVec[index].variableName.length() > 0)
                            {
                                if (this->variablesMap.find(currentASTNode->functionInputVariableVec[index].variableName) != this->variablesMap.end())
                                {
                                    std::cout << "INPUT VARIABLE IS A DECLARED VARIABLE!:" << currentASTNode->functionVariablesVec[index].variableName << std::endl;
                                    VariableStruct* inputVariableStruct = this->variableStructMap[this->variablesMap[currentASTNode->functionInputVariableVec[index].variableName]];
                                    newASTNodeInputVariable->integer = inputVariableStruct->integer;
                                    newASTNodeInputVariable->decimal = inputVariableStruct->decimal;
                                    newASTNodeInputVariable->character = inputVariableStruct->character;
                                    newASTNodeInputVariable->boolean = inputVariableStruct->boolean;
                                    newASTNodeInputVariable->string = inputVariableStruct->string;
                                    if (inputVariableStruct->variableType == currentASTNode->functionVariablesVec[index].variableDataType)
                                    {
                                        if (this->variablesMap.find(currentASTNode->functionVariablesVec[index].variableName) == this->variablesMap.end())
                                        {
                                            VariableStruct* newVariableStruct = this->createNewVariableStruct(newASTNodeInputVariable);
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
                                            std::cout << "CREATED INPUT VARIABLE FOR A FUNCTION! INPUT VARIABLE NAME:" << newVariableStruct->variableName << std::endl;
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
                                if (this->variablesMap.find(currentASTNode->functionVariablesVec[index].variableName) == this->variablesMap.end())
                                {
                                    VariableStruct* newVariableStruct = this->createNewVariableStruct(newASTNodeInputVariable);
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
                                    std::cout << "CREATED INPUT VARIABLE FOR A FUNCTION! INPUT VARIABLE NAME:" << currentASTNode->functionVariablesVec[index].variableName << " IS A LITERAL!" << std::endl;
                                }
                                else
                                {
                                    newInterpretationSuccess = false;
                                    currentASTNode = nullptr;
                                    break;
                                };
                            };
                        };
                    }
                    else if (currentASTNode->functionVariablesVec.size() == 0 && currentASTNode->functionInputVariableVec.size() == 0)
                    {
                        std::cout << "FUNCTION HAS NO INPUT VARIABLES" << std::endl;
                    }
                    else
                    {
                        std::cout << "FUNCTION DOES NOT HAVE ENOUGH OR TOO MANY INPUT VARIABLES!" << std::endl;
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    };
                }
                else if (currentASTNode->functionASTNodeType == "FUNCTION CALL")
                {
                    std::cout << "FUNCTION CALL AST NODE!" << std::endl;
                    bool functionCallSuccess = false;
                    for (int index1 = 0; index1 < this->functionsVec.size(); index1++)
                    {
                        if (this->functionsVec[index1]->functionName == currentASTNode->functionToCall)
                        {
                            functionCallSuccess = true;
                            std::cout << "CALL FUNCTION: " << currentASTNode->functionToCall << std::endl;
                            if (this->functionsVec[index1]->functionVariablesVec.size() == currentASTNode->functionInputVariableVec.size())
                            {
                                for (int index2 = 0; index2 < this->functionsVec[index1]->functionVariablesVec.size(); index2++)
                                {
                                    if (currentASTNode->functionInputVariableVec[index2].variableDataType == "VARIABLE")
                                    {
                                        if (this->variablesMap.find(currentASTNode->functionInputVariableVec[index2].variableName) != this->variablesMap.end())
                                        {
                                            VariableStruct* variableStruct = this->variableStructMap[this->variablesMap[currentASTNode->functionInputVariableVec[index2].variableName]];
                                            currentASTNode->functionInputVariableVec[index2].variableDataType = variableStruct->variableType;
                                        }
                                        else
                                        {
                                            newInterpretationSuccess = false;
                                            currentASTNode = nullptr;
                                            break;
                                        };
                                    };
                                    if (this->functionsVec[index1]->functionVariablesVec[index2].variableDataType == currentASTNode->functionInputVariableVec[index2].variableDataType)
                                    {
                                        this->functionsVec[index1]->functionInputVariableVec.push_back(currentASTNode->functionInputVariableVec[index2]);
                                    }
                                    else
                                    {
                                        newInterpretationSuccess = false;
                                        currentASTNode = nullptr;
                                        break;
                                    };
                                };
                                if (this->functionsVec[index1]->functionInputVariableVec.size() == currentASTNode->functionInputVariableVec.size())
                                {
                                    this->functionsVec[index1]->functionCallReturnASTNode = currentASTNode;
                                    currentASTNode = this->functionsVec[index1];
                                    functionCallSuccess = true;
                                    break;
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
                    };
                    if (functionCallSuccess == false)
                    {
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    }
                    else
                    {
                        calledAFunction = true;
                    };
                }
                else if (currentASTNode->functionASTNodeType == "FUNCTION RETURN")
                {
                    std::cout << "FUNCTION RETURN!" << std::endl;
                    std::cout << "currentASTNode->functionReturnType:" << currentASTNode->functionReturnType << std::endl;
                    std::cout << "currentASTNode->functionReturnsVoid:" << currentASTNode->functionReturnsVoid << std::endl;
                    std::cout << "currentASTNode->functionReturnVariableName.length():" << currentASTNode->functionReturnVariableName.length() << std::endl;
                    if (currentASTNode->functionReturnType == R_VOID && currentASTNode->functionReturnsVoid == true && currentASTNode->functionReturnVariableName.length() == 0)
                    {
                        std::cout << "FUNCTION RETURNS VOID!" << std::endl;
                        ASTNode* functionASTNode = nullptr;
                        bool returnFunctionFound = false;
                        for (int index = 0; index < this->functionsVec.size(); index++)
                        {
                            if (this->functionsVec[index]->functionName == currentASTNode->functionName)
                            {
                                returnFunctionFound = true;
                                functionASTNode = this->functionsVec[index];
                                break;
                            };
                        };
                        if (returnFunctionFound == false)
                        {
                            newInterpretationSuccess = false;
                            currentASTNode = nullptr;
                            break;
                        }
                        else if (returnFunctionFound == true && functionASTNode != nullptr)
                        {
                            if (functionASTNode->command == C_FUNCTION && functionASTNode->functionCallReturnASTNode->functionASTNodeType == "FUNCTION CALL")
                            {
                                currentASTNode = functionASTNode->functionCallReturnASTNode->sequentialASTNode;
                                returningFromAFunction = true;
                            }
                            else
                            {
                                std::cout << "THIS IS AN ERROR, RETURNS VOID TO A STANDARD VARIABLE, THIS WAS NOT A STANDARD FUNCTION CALL!" << std::endl;
                                newInterpretationSuccess = false;
                                currentASTNode = nullptr;
                                break;
                            };
                        };
                    }
                    else if (currentASTNode->functionReturnType == R_INTEGER || currentASTNode->functionReturnType == R_DECIMAL || currentASTNode->functionReturnType == R_CHARACTER || currentASTNode->functionReturnType == R_BOOLEAN | currentASTNode->functionReturnType == R_STRING)
                    {
                        if (currentASTNode->functionReturnsVoid == false)
                        {
                            ASTNode* functionASTNode = nullptr;
                            bool returnFunctionFound = false;
                            for (int index = 0; index < this->functionsVec.size(); index++)
                            {
                                if (this->functionsVec[index]->functionName == currentASTNode->functionName)
                                {
                                    returnFunctionFound = true;
                                    functionASTNode = this->functionsVec[index];
                                    break;
                                };
                            };
                            if (returnFunctionFound == true && functionASTNode != nullptr)
                            {
                                if (currentASTNode->functionReturnVariableName.length() == 0)
                                {
                                    if (currentASTNode->functionReturnType == R_INTEGER)
                                    {
                                        functionASTNode->integer = currentASTNode->functionReturnInteger;
                                    }
                                    else if (currentASTNode->functionReturnType == R_DECIMAL)
                                    {
                                        functionASTNode->decimal = currentASTNode->functionReturnDecimal;
                                    }
                                    else if (currentASTNode->functionReturnType == R_CHARACTER)
                                    {
                                        functionASTNode->character = currentASTNode->functionReturnCharacter;
                                    }
                                    else if (currentASTNode->functionReturnType == R_BOOLEAN)
                                    {
                                        functionASTNode->boolean = currentASTNode->functionReturnBoolean;
                                    }
                                    else if (currentASTNode->functionReturnType == R_STRING)
                                    {
                                        functionASTNode->string = currentASTNode->functionReturnString;
                                    };
                                }
                                else
                                {
                                    if (this->variablesMap.find(currentASTNode->functionReturnVariableName) != this->variablesMap.end())
                                    {
                                        VariableStruct* variableStruct = this->variableStructMap[this->variablesMap[currentASTNode->functionReturnVariableName]];
                                        if (currentASTNode->functionReturnType == R_INTEGER)
                                        {
                                            functionASTNode->integer = variableStruct->integer;
                                        }
                                        else if (currentASTNode->functionReturnType == R_DECIMAL)
                                        {
                                            functionASTNode->decimal = variableStruct->decimal;
                                        }
                                        else if (currentASTNode->functionReturnType == R_CHARACTER)
                                        {
                                            functionASTNode->character = variableStruct->character;
                                        }
                                        else if (currentASTNode->functionReturnType == R_BOOLEAN)
                                        {
                                            functionASTNode->boolean = variableStruct->boolean;
                                        }
                                        else if (currentASTNode->functionReturnType == R_STRING)
                                        {
                                            functionASTNode->string = variableStruct->string;
                                        };
                                    }
                                    else
                                    {
                                        newInterpretationSuccess = false;
                                        currentASTNode = nullptr;
                                        break;
                                    };
                                };
                                if (functionASTNode->functionCallReturnASTNode->command == C_FUNCTION)
                                {
                                    currentASTNode = functionASTNode->functionCallReturnASTNode;
                                }
                                else if (functionASTNode->functionCallReturnASTNode->command = C_ASSIGNMENT_OPERATOR)
                                {
                                    std::cout << "22222. C_ASSIGNMENT_OPERATOR RETURN!" << std::endl;
                                    currentASTNode = functionASTNode->functionCallReturnASTNode;
                                    functionASTNode->functionCallReturnASTNode->integer = functionASTNode->integer;
                                    functionASTNode->functionCallReturnASTNode->decimal = functionASTNode->decimal;
                                    functionASTNode->functionCallReturnASTNode->character = functionASTNode->character;
                                    functionASTNode->functionCallReturnASTNode->boolean = functionASTNode->boolean;
                                    functionASTNode->functionCallReturnASTNode->string = functionASTNode->string;
                                    if (this->variablesMap.find(functionASTNode->functionCallReturnASTNode->variableName) != this->variablesMap.end())
                                    {
                                        VariableStruct* variableStructToUpdate = this->variableStructMap[this->variablesMap[functionASTNode->functionCallReturnASTNode->variableName]];
                                        if (variableStructToUpdate->variableType == "INTEGER")
                                        {
                                            variableStructToUpdate->integer = functionASTNode->functionCallReturnASTNode->integer;
                                        }
                                        else if (variableStructToUpdate->variableType == "DECIMAL")
                                        {
                                            variableStructToUpdate->decimal = functionASTNode->functionCallReturnASTNode->decimal;
                                        }
                                        else if (variableStructToUpdate->variableType == "CHARACTER")
                                        {
                                            variableStructToUpdate->character = functionASTNode->functionCallReturnASTNode->character;
                                        }
                                        else if (variableStructToUpdate->variableType == "BOOLEAN")
                                        {
                                            variableStructToUpdate->boolean = functionASTNode->functionCallReturnASTNode->boolean;
                                        }
                                        else if (variableStructToUpdate->variableType == "STRING")
                                        {
                                            variableStructToUpdate->string = functionASTNode->functionCallReturnASTNode->string;
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
                else if (currentASTNode->functionASTNodeType == "FUNCTION END")
                {
                    std::cout << "THIS IS FUNCTION END, THIS SHOULD NEVER BE CALLED!" << std::endl;
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
        if (returningFromAFunction == false)
        {
            if (calledAFunction == false)
            {
                if (takeControlFlowBranch == false)
                {
                    std::cout << "[INTERPRETER] SEQUENTIAL AST NODE!" << std::endl;
                    if (currentASTNode->sequentialASTNode != nullptr)
                    {
                        if (currentASTNode->command == C_CONTROL_FLOW || currentASTNode->command == C_FOR_LOOP || currentASTNode->command == C_WHILE_LOOP)
                        {
                            if (currentASTNode->controlFlowType == IF || currentASTNode->controlFlowType == ELSE_IF || currentASTNode->controlFlowType == ELSE || currentASTNode->controlFlowType == FOR || currentASTNode->controlFlowType == WHILE)
                            {
                                this->currentScope++;
                            }
                            else if (currentASTNode->controlFlowType == END)
                            {
                                this->currentScope--;
                                this->clearOutOfScopeVariables();
                            };
                        };
                        currentASTNode = currentASTNode->sequentialASTNode;
                        std::cout << "[INTERPRETER] currentScope:" << this->currentScope << std::endl;
                    }
                    else
                    {
                        currentASTNode = nullptr;
                        break;
                    };
                }
                else if (takeControlFlowBranch == true)
                {
                    std::cout << "[INTERPRETER] CONTROL FLOW AST NODE!" << std::endl;
                    if (currentASTNode->controlFlowASTNode != nullptr)
                    {
                        if (currentASTNode->command == C_FOR_LOOP || currentASTNode->command == C_WHILE_LOOP)
                        {
                            if (currentASTNode->controlFlowType == FOR || currentASTNode->controlFlowType == WHILE)
                            {
                                this->currentScope++;
                            }
                            else if (currentASTNode->controlFlowType == END)
                            {
                                this->currentScope--;
                                this->clearOutOfScopeVariables();
                            };
                        };
                        currentASTNode = currentASTNode->controlFlowASTNode;
                        std::cout << "[INTERPRETER] currentScope:" << this->currentScope << std::endl;
                    }
                    else
                    {
                        std::cout << "[INTERPRETER] ERROR, NEEDED TO TAKE CONTROL FLOW BRANCH BUT currentASTNode->controlFlowASTNode IS NULLPTR!" << std::endl;
                        newInterpretationSuccess = false;
                        currentASTNode = nullptr;
                        break;
                    };
                };
            }
            else
            {
                this->currentScope++;
                std::cout << "CALLED A FUNCTION, INCREMENT SCOPE!" << std::endl;
            };
        }
        else
        {
            this->currentScope--;
            this->clearOutOfScopeVariables();
            std::cout << "RETURNING FROM A FUNCTION CALL, DECREMENT SCOPE!" << std::endl;
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
            if (secondOperandStruct->variableType == "INTEGER" || secondOperandStruct->variableType == "CONSTANT INTEGER")
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
            if (secondOperandStruct->variableType == "DECIMAL" || secondOperandStruct->variableType == "CONSTANT DECIMAL")
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
        std::cout << "STRING ARITHMETIC!" << std::endl;
        std::string secondOperandString = arithmeticVec[arithmeticStruct->index + 1];
        std::cout << "secondOperandString:" << secondOperandString << std::endl;
        if (this->variablesMap.find(secondOperandString) != this->variablesMap.end())
        {
            VariableStruct* secondOperandStruct = this->variableStructMap[this->variablesMap[secondOperandString]];
            if (secondOperandStruct->variableType == "STRING" || secondOperandStruct->variableType == "CONSTANT STRING")
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
            std::cout << "2. arithmeticStruct->stringArithmeticResult:" << arithmeticStruct->stringArithmeticResult << std::endl;
            arithmeticStruct->index += 2;
            return this->performArithmetic(arithmeticStruct, arithmeticVec, variableType);
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
VariableStruct* Interpreter::createNewVariableStruct(ASTNode* currentASTNode)
{
    VariableStruct* newVariableStruct = new VariableStruct;
    newVariableStruct->scope = this->currentScope;
    newVariableStruct->integer = currentASTNode->integer;
    newVariableStruct->decimal = currentASTNode->decimal;
    newVariableStruct->character = currentASTNode->character;
    newVariableStruct->boolean = currentASTNode->boolean;
    newVariableStruct->string = currentASTNode->string;
    newVariableStruct->isConstant = currentASTNode->isConstant;
    newVariableStruct->variableMemoryAddress = currentASTNode->variableMemoryAddress;
    newVariableStruct->variableType = currentASTNode->variableType;
    newVariableStruct->variableName = currentASTNode->variableName;
    newVariableStruct->assignmentVariableName = currentASTNode->assignmentVariableName;
    newVariableStruct->integerArray = currentASTNode->integerArray;
    newVariableStruct->decimalArray = currentASTNode->decimalArray;
    newVariableStruct->characterArray = currentASTNode->characterArray;
    newVariableStruct->booleanArray = currentASTNode->booleanArray;
    newVariableStruct->stringArray = currentASTNode->stringArray;
    newVariableStruct->arrayVariableNameToAssignSize = currentASTNode->arrayVariableNameToAssignSize;
    if (currentASTNode->arrayVariableNameToExtractDataFrom.size() > 0)
    {
        if (this->variablesMap.find(currentASTNode->arrayVariableNameToExtractDataFrom) != this->variablesMap.end())
        {
            VariableStruct* arrayVariableStruct = this->variableStructMap[this->variablesMap[currentASTNode->arrayVariableNameToExtractDataFrom]];
            if (currentASTNode->indexToExtractFromArray >= 0)
            {
                if (arrayVariableStruct->variableType == "ARRAY INTEGER")
                {
                    if (newVariableStruct->variableType == "INTEGER" || newVariableStruct->variableType == "CONSTANT INTEGER")
                    {
                        if (currentASTNode->indexToExtractFromArray <= arrayVariableStruct->integerArray.size() - 1)
                        {
                            newVariableStruct->integer = arrayVariableStruct->integerArray[currentASTNode->indexToExtractFromArray];
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
                }
                else if (arrayVariableStruct->variableType == "ARRAY DECIMAL")
                {
                    if (newVariableStruct->variableType == "DECIMAL" || newVariableStruct->variableType == "CONSTANT DECIMAL")
                    {
                        if (currentASTNode->indexToExtractFromArray <= arrayVariableStruct->decimalArray.size() - 1)
                        {
                            newVariableStruct->decimal = arrayVariableStruct->decimalArray[currentASTNode->indexToExtractFromArray];
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
                }
                else if (arrayVariableStruct->variableType == "ARRAY CHARACTER")
                {
                    if (newVariableStruct->variableType == "CHARACTER" || newVariableStruct->variableType == "CONSTANT CHARACTER")
                    {
                        if (currentASTNode->indexToExtractFromArray <= arrayVariableStruct->characterArray.size() - 1)
                        {
                            newVariableStruct->character = arrayVariableStruct->characterArray[currentASTNode->indexToExtractFromArray];
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
                }
                else if (arrayVariableStruct->variableType == "ARRAY BOOLEAN")
                {
                    if (newVariableStruct->variableType == "BOOLEAN" || newVariableStruct->variableType == "CONSTANT BOOLEAN")
                    {
                        if (currentASTNode->indexToExtractFromArray <= arrayVariableStruct->booleanArray.size() - 1)
                        {
                            newVariableStruct->boolean = arrayVariableStruct->booleanArray[currentASTNode->indexToExtractFromArray];
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
                }
                else if (arrayVariableStruct->variableType == "ARRAY STRING")
                {
                    if (newVariableStruct->variableType == "STRING" || newVariableStruct->variableType == "CONSTANT STRING")
                    {
                        if (currentASTNode->indexToExtractFromArray <= arrayVariableStruct->stringArray.size() - 1)
                        {
                            newVariableStruct->string = arrayVariableStruct->stringArray[currentASTNode->indexToExtractFromArray];
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
                }
                else
                {
                    newVariableStruct->successfullyCreatedVariableStruct = false;
                };
            }
            else if (currentASTNode->variableNameToGetIntegerIndex.size() > 0 && this->variablesMap.find(currentASTNode->variableNameToGetIntegerIndex) != this->variablesMap.end())
            {
                VariableStruct* variableNameToGetIntegerIndexStruct = this->variableStructMap[this->variablesMap[currentASTNode->variableNameToGetIntegerIndex]];
                if (variableNameToGetIntegerIndexStruct->variableType == "INTEGER" || variableNameToGetIntegerIndexStruct->variableType == "CONSTANT INTEGER")
                {
                    if (arrayVariableStruct->variableType == "ARRAY INTEGER")
                    {
                        if (newVariableStruct->variableType == "INTEGER" || newVariableStruct->variableType == "CONSTANT INTEGER")
                        {
                            if (variableNameToGetIntegerIndexStruct->integer <= arrayVariableStruct->integerArray.size() - 1)
                            {
                                newVariableStruct->integer = arrayVariableStruct->integerArray[variableNameToGetIntegerIndexStruct->integer];
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
                    }
                    else if (arrayVariableStruct->variableType == "ARRAY DECIMAL")
                    {
                        if (newVariableStruct->variableType == "DECIMAL" || newVariableStruct->variableType == "CONSTANT DECIMAL")
                        {
                            if (variableNameToGetIntegerIndexStruct->integer <= arrayVariableStruct->decimalArray.size() - 1)
                            {
                                newVariableStruct->decimal = arrayVariableStruct->decimalArray[variableNameToGetIntegerIndexStruct->integer];
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
                    }
                    else if (arrayVariableStruct->variableType == "ARRAY CHARACTER")
                    {
                        if (newVariableStruct->variableType == "CHARACTER" || newVariableStruct->variableType == "CONSTANT CHARACTER")
                        {
                            if (variableNameToGetIntegerIndexStruct->integer <= arrayVariableStruct->characterArray.size() - 1)
                            {
                                newVariableStruct->character = arrayVariableStruct->characterArray[variableNameToGetIntegerIndexStruct->integer];
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
                    }
                    else if (arrayVariableStruct->variableType == "ARRAY BOOLEAN")
                    {
                        if (newVariableStruct->variableType == "BOOLEAN" || newVariableStruct->variableType == "CONSTANT BOOLEAN")
                        {
                            if (variableNameToGetIntegerIndexStruct->integer <= arrayVariableStruct->booleanArray.size() - 1)
                            {
                                newVariableStruct->boolean = arrayVariableStruct->booleanArray[variableNameToGetIntegerIndexStruct->integer];
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
                    }
                    else if (arrayVariableStruct->variableType == "ARRAY STRING")
                    {
                        if (newVariableStruct->variableType == "STRING" || newVariableStruct->variableType == "CONSTANT STRING")
                        {
                            if (variableNameToGetIntegerIndexStruct->integer <= arrayVariableStruct->stringArray.size() - 1)
                            {
                                newVariableStruct->string = arrayVariableStruct->stringArray[variableNameToGetIntegerIndexStruct->integer];
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
                };
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
    if (newVariableStruct->arrayVariableNameToAssignSize.length() > 0)
    {
        if (this->variablesMap.find(newVariableStruct->arrayVariableNameToAssignSize) != this->variablesMap.end())
        {
            VariableStruct* arrayVariableStruct = this->variableStructMap[this->variablesMap[newVariableStruct->arrayVariableNameToAssignSize]];
            if (arrayVariableStruct->variableType == "ARRAY INTEGER")
            {
                newVariableStruct->integer = arrayVariableStruct->integerArray.size();
            }
            else if (arrayVariableStruct->variableType == "ARRAY DECIMAL")
            {
                newVariableStruct->integer = arrayVariableStruct->decimalArray.size();
            }
            else if (arrayVariableStruct->variableType == "ARRAY CHARACTER")
            {
                newVariableStruct->integer = arrayVariableStruct->characterArray.size();
            }
            else if (arrayVariableStruct->variableType == "ARRAY BOOLEAN")
            {
                newVariableStruct->integer = arrayVariableStruct->booleanArray.size();
            }
            else if (arrayVariableStruct->variableType == "ARRAY STRING")
            {
                newVariableStruct->integer = arrayVariableStruct->stringArray.size();
            };
        };
    };
    if (newVariableStruct->assignmentVariableName.length() > 0 && this->variablesMap.find(newVariableStruct->assignmentVariableName) != this->variablesMap.end())
    {
        VariableStruct* otherVariableStruct = this->variableStructMap[this->variablesMap[newVariableStruct->assignmentVariableName]];
        std::string leftOperandDataTypeString = otherVariableStruct->variableType;
        std::string rightOperandDataTypeString = newVariableStruct->variableType;
        if (leftOperandDataTypeString.rfind("CONSTANT ", 0) == 0)
        {
            leftOperandDataTypeString.erase(0, 9);
        };
        if (rightOperandDataTypeString.rfind("CONSTANT ", 0) == 0)
        {
            rightOperandDataTypeString.erase(0, 9);
        };
        if (leftOperandDataTypeString == rightOperandDataTypeString)
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
    std::cout << "[INTERPRETER] newVariableStruct->scope:" << newVariableStruct->scope << std::endl;
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
    std::cout << "[INTERPRETER] newVariableStruct->integerArray.size():" << newVariableStruct->integerArray.size() << std::endl;
    for (int index = 0; index < newVariableStruct->integerArray.size(); index++)
    {
        std::cout << "[INTERPRETER] newVariableStruct->integerArray[" << index << "]:" << newVariableStruct->integerArray[index] << std::endl;
    };
    std::cout << "[INTERPRETER] newVariableStruct->decimalArray.size():" << newVariableStruct->decimalArray.size() << std::endl;
    for (int index = 0; index < newVariableStruct->decimalArray.size(); index++)
    {
        std::cout << "[INTERPRETER] newVariableStruct->decimalArray[" << index << "]:" << newVariableStruct->decimalArray[index] << std::endl;
    };
    std::cout << "[INTERPRETER] newVariableStruct->characterArray.size():" << newVariableStruct->characterArray.size() << std::endl;
    for (int index = 0; index < newVariableStruct->characterArray.size(); index++)
    {
        std::cout << "[INTERPRETER] newVariableStruct->characterArray[" << index << "]:" << newVariableStruct->characterArray[index] << std::endl;
    };
    std::cout << "[INTERPRETER] newVariableStruct->booleanArray.size():" << newVariableStruct->booleanArray.size() << std::endl;
    for (int index = 0; index < newVariableStruct->booleanArray.size(); index++)
    {
        std::cout << "[INTERPRETER] newVariableStruct->booleanArray[" << index << "]:" << newVariableStruct->booleanArray[index] << std::endl;
    };
    std::cout << "[INTERPRETER] newVariableStruct->stringArray.size():" << newVariableStruct->stringArray.size() << std::endl;
    for (int index = 0; index < newVariableStruct->stringArray.size(); index++)
    {
        std::cout << "[INTERPRETER] newVariableStruct->stringArray[" << index << "]:" << newVariableStruct->stringArray[index] << std::endl;
    };
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

/*
==================================================
Extracts Data From Array And Assigns It To Variable
==================================================
*/
bool Interpreter::extractAndAssignDataFromArray(std::string arrayVariableString, VariableStruct* variableStructToUpdate)
{
    std::regex re(R"([\[\]])");
    std::sregex_token_iterator it(arrayVariableString.begin(), arrayVariableString.end(), re, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> arrayVariableStringVec(it, end);
    if (arrayVariableStringVec.size() == 3 && arrayVariableStringVec[2] == ";")
    {
        std::string arrayVariableName = arrayVariableStringVec[0];
        if (this->variablesMap.find(arrayVariableName) != this->variablesMap.end())
        {
            VariableStruct* arrayVariableStruct = this->variableStructMap[this->variablesMap[arrayVariableName]];
            std::string arrayIndexString = arrayVariableStringVec[1];
            int index = -1;
            VariableStruct* indexVariableStruct;
            try
            {
                index = std::stoi(arrayIndexString);
            }
            catch(...)
            {
                if (this->variablesMap.find(arrayIndexString) != this->variablesMap.end())
                {
                    indexVariableStruct = this->variableStructMap[this->variablesMap[arrayIndexString]];
                    if (indexVariableStruct->variableType == "INTEGER" || indexVariableStruct->variableType == "CONSTANT INTEGER")
                    {
                        index = indexVariableStruct->integer;
                    }
                    else
                    {
                        return false;
                    };
                }
                else
                {
                    return false;
                };
            };
            if (index >= 0 && index <= arrayVariableStruct->integerArray.size() - 1)
            {
                if (arrayVariableStruct->variableType == "ARRAY INTEGER" && variableStructToUpdate->variableType == "INTEGER")
                {
                    variableStructToUpdate->integer = arrayVariableStruct->integerArray[index];
                }
                else if (arrayVariableStruct->variableType == "ARRAY DECIMAL" && variableStructToUpdate->variableType == "DECIMAL")
                {
                    variableStructToUpdate->decimal = arrayVariableStruct->decimalArray[index];
                }
                else if (arrayVariableStruct->variableType == "ARRAY CHARACTER" && variableStructToUpdate->variableType == "CHARACTER")
                {
                    variableStructToUpdate->character = arrayVariableStruct->characterArray[index];
                }
                else if (arrayVariableStruct->variableType == "ARRAY BOOLEAN" && variableStructToUpdate->variableType == "BOOLEAN")
                {
                    variableStructToUpdate->boolean = arrayVariableStruct->booleanArray[index];
                }
                else if (arrayVariableStruct->variableType == "ARRAY STRING" && variableStructToUpdate->variableType == "STRING")
                {
                    variableStructToUpdate->string = arrayVariableStruct->stringArray[index];
                }
                else
                {
                    return false;
                };
            }
            else
            {
                return false;
            };
        };
    };
    return true;
};

/*
==================================================
Clears Out Of Scope Variables
==================================================
*/
void Interpreter::clearOutOfScopeVariables()
{
    std::vector<int> variableIdsVec;
    for (auto iterator = this->variableStructMap.begin(); iterator != this->variableStructMap.end();)
    {
        VariableStruct* variableStruct = iterator->second;
        if (variableStruct->scope > this->currentScope)
        {
            std::cout << "[INTERPRETER] Remove Variable From this->variableStructMap:" << variableStruct->variableName << " Variable Is Out Of Scope!" << std::endl;
            variableIdsVec.push_back(iterator->first);
            delete variableStruct;
            iterator = this->variableStructMap.erase(iterator);
        }
        else
        {
            ++iterator;
        };
    };
    for (auto iterator = this->variablesMap.begin(); iterator != this->variablesMap.end();)
    {
        int variableId = iterator->second;
        bool removeVariable = false;
        for (int index = 0; index < variableIdsVec.size(); index++)
        {
            if (variableId == variableIdsVec[index])
            {
                removeVariable = true;
                break;
            };
        };
        if (removeVariable)
        {
            std::cout << "[INTERPRETER] Remove Variable From this->variablesMap:" << iterator->first << " Variable Is Out Of Scope!" << std::endl;
            iterator = this->variablesMap.erase(iterator);
        }
        else
        {
            ++iterator;
        }
    };
    return;
};

/*
==================================================
Sets The Functions Vector
==================================================
*/
void Interpreter::setFunctionsVec(std::vector<ASTNode*> newFunctionsVec)
{
    this->functionsVec = newFunctionsVec;
    return;
}

/*
==================================================
Sets The Variable Memory Address Counter
==================================================
*/
void Interpreter::assignVariableMemoryAddress(ASTNode* currentASTNode)
{
    currentASTNode->variableMemoryAddress = this->variableMemoryAddressCounter;
    this->variableMemoryAddressCounter++;
    return;
};

/*
==================================================
Sets The Variable Memory Address Counter
==================================================
*/
void Interpreter::setVariableMemoryAddressCounter(int newVariableMemoryAddressCounter)
{
    this->variableMemoryAddressCounter = newVariableMemoryAddressCounter;
    return;
};

#endif