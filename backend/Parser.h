/* UPDATE VERSION [45] */

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
#include <regex>
#include <stack>
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
    C_ASSIGNMENT_OPERATOR,
    C_ARITHMETIC,
    C_CONTROL_FLOW,
    C_ARRAY_INTEGER,
    C_ARRAY_DECIMAL,
    C_ARRAY_CHARACTER,
    C_ARRAY_BOOLEAN,
    C_ARRAY_STRING,
    C_FOR_LOOP,
    C_WHILE_LOOP,
};

/*
==================================================
Comparison Operators Enums
==================================================
*/
enum ComparisonOperator
{
    EQ,
    NE,
    GT,
    LT,
    GE,
    LE,
};

/*
==================================================
Logical Operators Enums
==================================================
*/
enum LogicalOperator
{
    AND,
    OR,
    NOT,
};

/*
==================================================
Control Flow Enums
==================================================
*/
enum ControlFlow
{
    IF,
    ELSE_IF,
    ELSE,
    END,
};

/*
==================================================
Data Type Enums
==================================================
*/
enum DataType
{
    D_NULL,
    D_VARIABLE,
    D_INTEGER,
    D_DECIMAL,
    D_CHARACTER,
    D_BOOLEAN,
    D_STRING,
};

/*
==================================================
Comparison Struct
==================================================
*/
struct ComparisonStruct
{
    int integerOperand1 = 0;
    int integerOperand2 = 0;
    double decimalOperand1 = 0.0;
    double decimalOperand2 = 0.0;
    bool booleanOperand1 = false;
    bool booleanOperand2 = false;
    char characterOperand1 = ' ';
    char characterOperand2 = ' ';
    std::string stringOperand1 = "";
    std::string stringOperand2 = "";
    DataType dataType = D_NULL;
    bool operand1IsAVariable = false;
    bool operand2IsAVariable = false;
    std::string operand1VariableName = "";
    std::string operand2VariableName = "";
    ComparisonOperator comparisonOperator = EQ;
};

/*
==================================================
Abstract Syntax Tree (AST) Node Struct
==================================================
*/
struct ASTNode
{
    Command command = C_NONE;
    ControlFlow controlFlowType = IF;
    ASTNode* sequentialASTNode = nullptr;
    ASTNode* controlFlowASTNode = nullptr;
    std::string comment = "";
    std::string output = "";
    std::string outputVariable = "";
    std::string inputVariableName = "";
    bool outputUsesVariable = false;
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
    std::string assignmentOperatorValue = "";
    std::vector<std::string> arithmeticVec;
    std::vector<ComparisonStruct> comparisonStructVec;
    std::vector<LogicalOperator> logicalOperatorVec;
    std::string arrayVariableNameToAssignSize = "";
    std::vector<int> integerArray;
    std::vector<double> decimalArray;
    std::vector<char> characterArray;
    std::vector<bool> booleanArray;
    std::vector<std::string> stringArray;
    int indexToExtractFromArray = -1;
    std::string arrayVariableNameToExtractDataFrom = "";
    std::string variableNameToGetIntegerIndex = "";
    std::string forLoopIncrementVariableName = "";
    ~ASTNode() = default;
};

/*
==================================================
TempVariableStruct
==================================================
*/
struct TempVariableStruct
{
    std::string variableType = "";
    std::string variableName = "";
};

/*
==================================================
Class Declaration
==================================================
*/
class Parser
{
    private:
        int variableMemoryAddressCounter;
        ASTNode* root;
        ASTNode* currentASTNode;
        std::vector<ASTNode*> ASTNodesVec;
        std::vector<std::string> tokensVec;
        std::vector<std::string> variableNamesVec;
        std::vector<TempVariableStruct> tempVariableStructVec;
        std::stack<ASTNode*> controlFlowStack;
        bool parsedSuccessfully;
        std::string errorString;
        std::string trimString(std::string inputString);
        bool buildAST(std::string codeLine, Command commandType, ASTNode* currentASTNode);
        void buildASTHelper(std::string currentCodeLine, Command commandType, ASTNode* currentASTNode, bool& hasInvalidSyntax, int& syntaxErrorLine, int index);
        bool isValidVariableName(std::string variableName, bool isBeingAssigned);
        void assignVariableMemoryAddress(ASTNode* astNode, std::string variableType);
        bool isValidVariableAssignment(std::string variableAssignment, std::string variableType);
        std::vector<std::string> splitCodeLine(std::string codeLine);
        bool checkIfVariableNameExists(std::string variableName);
        bool checkIfArithmeticOperations(std::string inputString);
        DataType checkDataType(std::string data);
        void createNewTempVariableStruct(std::string variableName, std::string variableType);
        void connectDanglingBranches(ASTNode* currentASTNode, ASTNode* connectorASTNode);
        bool isValidArrayDataExtraction(std::string arrayVariableName, std::string variableType, ASTNode* currentASTNode);
    public:
        Parser();
        Parser(std::vector<std::string> tokensVec);
        ~Parser();
        void insertTokensVec(std::vector<std::string> tokensVec);
        void parse();
        std::string getErrorString();
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
    this->variableMemoryAddressCounter = 0;
    this->currentASTNode = nullptr;
    this->root = nullptr;
    for (int index = 0; index < this->ASTNodesVec.size(); index++)
    {
        delete this->ASTNodesVec[index];
    };
    this->ASTNodesVec.clear();
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
    this->variableMemoryAddressCounter = 0;
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
    this->variableMemoryAddressCounter = -1;
    if (this->root != nullptr)
    {
        delete this->root;
    };
    this->root = nullptr;
    this->currentASTNode = nullptr;
    this->tokensVec.clear();
    this->variableNamesVec.clear();
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
                this->buildASTHelper(currentCodeLine, commandType, this->currentASTNode, hasInvalidSyntax, syntaxErrorLine, index);
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
                    }
                    else if (commandString == "input")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: INPUT" << std::endl;
                        commandType = C_INPUT;
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
                    }
                    else if (commandString == "IF" || commandString == "ELSE IF" || commandString == "ELSE BRANCH;" || commandString == "END;")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: CONTROL FLOW" << std::endl;
                        commandType = C_CONTROL_FLOW;
                    }
                    else if (commandString == "FOR LOOP")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: FOR LOOP" << std::endl;
                        commandType = C_FOR_LOOP;
                    }
                    else if (commandString == "WHILE LOOP")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: WHILE LOOP" << std::endl;
                        commandType = C_WHILE_LOOP;
                    }
                    else if (commandString == "ARRAY INTEGER")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: ARRAY INTEGER" << std::endl;
                        commandType = C_ARRAY_INTEGER;
                    }
                    else if (commandString == "ARRAY DECIMAL")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: ARRAY DECIMAL" << std::endl;
                        commandType = C_ARRAY_DECIMAL;
                    }
                    else if (commandString == "ARRAY CHARACTER")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: ARRAY CHARACTER" << std::endl;
                        commandType = C_ARRAY_CHARACTER;
                    }
                    else if (commandString == "ARRAY BOOLEAN")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: ARRAY BOOLEAN" << std::endl;
                        commandType = C_ARRAY_BOOLEAN;
                    }
                    else if (commandString == "ARRAY STRING")
                    {
                        commandFound = true;
                        std::cout << "[PARSER] Command: ARRAY STRING" << std::endl;
                        commandType = C_ARRAY_STRING;
                    }
                    else if (currentCodeLine[index2] == ' ' && this->isValidVariableName(commandString.substr(0, commandString.length() - 1), true) && index2 + 1 < currentCodeLine.length() && currentCodeLine[index2 + 1] == '=' && index2 + 2 < currentCodeLine.length() && currentCodeLine[index2 + 2] == ' ')
                    {
                        std::string assignmentValue = currentCodeLine.substr(index2 + 3);
                        if (this->isValidArrayDataExtraction(assignmentValue, "", nullptr))
                        {
                            commandFound = true;
                            std::cout << "[PARSER] Command: ASSIGNMENT OPERATOR" << std::endl;
                            commandType = C_ASSIGNMENT_OPERATOR;
                        }
                        else
                        {
                            bool isArithmeticOperations = this->checkIfArithmeticOperations(assignmentValue);
                            if (assignmentValue[assignmentValue.length() - 1] == ';')
                            {
                                if (isArithmeticOperations)
                                {
                                    commandFound = true;
                                    std::cout << "[PARSER] Command: ARITHMETIC" << std::endl;
                                    commandType = C_ARITHMETIC;
                                }
                                else
                                {
                                    if (assignmentValue[0] == '\'' && assignmentValue[assignmentValue.length() - 2] == '\'' || assignmentValue[0] == '\"' && assignmentValue[assignmentValue.length() - 2] == '\"')
                                    {
                                        commandFound = true;
                                        std::cout << "[PARSER] Command: ASSIGNMENT OPERATOR" << std::endl;
                                        commandType = C_ASSIGNMENT_OPERATOR;
                                    }
                                    else
                                    {
                                        bool isAssignmentOperator = true;
                                        for (int index3 = 1; index3 < assignmentValue.length() - 2; index3++)
                                        {
                                            if (assignmentValue[index3] == ' ')
                                            {
                                                isAssignmentOperator = false;
                                                break;
                                            };
                                        };
                                        if (isAssignmentOperator)
                                        {
                                            commandFound = true;
                                            std::cout << "[PARSER] Command: ASSIGNMENT OPERATOR" << std::endl;
                                            commandType = C_ASSIGNMENT_OPERATOR;
                                        };
                                    };
                                };
                            };
                        };
                    };
                    if (commandFound)
                    {
                        this->buildASTHelper(currentCodeLine, commandType, this->currentASTNode, hasInvalidSyntax, syntaxErrorLine, index);
                        break;
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
    this->ASTNodesVec.push_back(newASTNode);
    std::string ASTNodeType = "SEQUENTIAL AST NODE";
    newASTNode->command = commandType;
    if (commandType == C_COMMENT)
    {
        newASTNode->comment = codeLine;
        std::cout << "[PARSER] Comment:" << newASTNode->comment << std::endl;
    }
    else if (commandType == C_OUTPUT)
    {
        if (codeLine.length() < 10)
        {
            return false;
        };
        std::string outputString;
        newASTNode->outputUsesVariable = false;
        if (codeLine == "output(\"\")")
        {
            outputString = "";
        }
        else if (codeLine.substr(0, 8) == "output(\"" && codeLine.substr(codeLine.size() - 3) == "\");")
        {
            outputString = codeLine.substr(8, codeLine.size() - 3 - 8);
        }
        else if (codeLine.substr(0, 7) == "output(" && codeLine.substr(codeLine.size() - 2) == ");")    
        {
            std::string variable = codeLine.substr(7, codeLine.size() - 9);
            newASTNode->outputVariable = variable;
            newASTNode->outputUsesVariable = true;
        }
        else
        {
            return false;
        };
        newASTNode->output = outputString;
        std::cout << "[PARSER] Output:" << newASTNode->output << std::endl;
    }
    else if (commandType == C_INPUT)
    {
        if (codeLine.substr(0,6) == "input(" && codeLine.substr(codeLine.size() - 2) == ");")
        {
            newASTNode->inputVariableName = codeLine.substr(6, codeLine.size() - 2 - 6);
        }
        else
        {
            return false;
        };
    }
    else if (commandType == C_INTEGER)
    {
        std::vector<std::string> integerTokensVec = this->splitCodeLine(codeLine);
        if (integerTokensVec.size() == 2)
        {
            if (integerTokensVec[0] == "INTEGER" && this->isValidVariableName(integerTokensVec[1], false) && !this->checkIfVariableNameExists(integerTokensVec[1].substr(0, integerTokensVec[1].length() - 1)))
            {
                this->assignVariableMemoryAddress(newASTNode, "INTEGER");
                newASTNode->variableName = integerTokensVec[1].substr(0, integerTokensVec[1].length() - 1);
                this->createNewTempVariableStruct(newASTNode->variableName, "INTEGER");
                this->variableNamesVec.push_back(newASTNode->variableName);
            }
            else
            {
                return false;
            };
        }
        else if (integerTokensVec.size() == 4)
        {
            if (integerTokensVec[0] == "INTEGER" && this->isValidVariableName(integerTokensVec[1], true) && !this->checkIfVariableNameExists(integerTokensVec[1]) && integerTokensVec[2] == "=")
            {
                if (this->isValidVariableAssignment(integerTokensVec[3], "INTEGER"))
                {
                    this->assignVariableMemoryAddress(newASTNode, "INTEGER");
                    newASTNode->variableName = integerTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "INTEGER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    std::string extractVariable = integerTokensVec[3].substr(0, integerTokensVec[3].length() - 1);
                    newASTNode->integer = std::stoi(extractVariable);
                }
                else if (this->isValidVariableName(integerTokensVec[3], false))
                {
                    this->assignVariableMemoryAddress(newASTNode, "INTEGER");
                    newASTNode->variableName = integerTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "INTEGER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    newASTNode->assignmentVariableName = integerTokensVec[3].substr(0, integerTokensVec[3].length() - 1);
                }
                else if (this->isValidArrayDataExtraction(integerTokensVec[3], "INTEGER", newASTNode))
                {
                    this->assignVariableMemoryAddress(newASTNode, "INTEGER");
                    newASTNode->variableName = integerTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "INTEGER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                }
                else if (integerTokensVec[3].substr(0, 5) == "size(" && integerTokensVec[3].substr(integerTokensVec[3].length() - 2) == ");" && this->isValidVariableName(integerTokensVec[3].substr(5, integerTokensVec[3].length() - 7), true))
                {
                    bool assignArraySize = false;
                    std::string arrayVariableName = integerTokensVec[3].substr(5, integerTokensVec[3].length() - 7);
                    for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                    {
                        if (this->tempVariableStructVec[index].variableName == arrayVariableName)
                        {
                            assignArraySize = true;
                            break;
                        };
                    };
                    if (assignArraySize == true)
                    {
                        this->assignVariableMemoryAddress(newASTNode, "INTEGER");
                        newASTNode->variableName = integerTokensVec[1];
                        this->createNewTempVariableStruct(newASTNode->variableName, "INTEGER");
                        this->variableNamesVec.push_back(newASTNode->variableName);
                        newASTNode->arrayVariableNameToAssignSize = arrayVariableName;
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
    }
    else if (commandType == C_DECIMAL)
    {
        std::vector<std::string> decimalTokensVec = this->splitCodeLine(codeLine);
        if (decimalTokensVec.size() == 2)
        {
            if (decimalTokensVec[0] == "DECIMAL" && this->isValidVariableName(decimalTokensVec[1], false) && !this->checkIfVariableNameExists(decimalTokensVec[1].substr(0, decimalTokensVec[1].length() - 1)))
            {
                this->assignVariableMemoryAddress(newASTNode, "DECIMAL");
                newASTNode->variableName = decimalTokensVec[1].substr(0, decimalTokensVec[1].length() - 1);
                this->createNewTempVariableStruct(newASTNode->variableName, "DECIMAL");
                this->variableNamesVec.push_back(newASTNode->variableName);
            }
            else
            {
                return false;
            };
        }
        else if (decimalTokensVec.size() == 4)
        {
            if (decimalTokensVec[0] == "DECIMAL" && this->isValidVariableName(decimalTokensVec[1], true) && !this->checkIfVariableNameExists(decimalTokensVec[1]) && decimalTokensVec[2] == "=")
            {
                if (this->isValidVariableAssignment(decimalTokensVec[3], "DECIMAL"))
                {
                    this->assignVariableMemoryAddress(newASTNode, "DECIMAL");
                    newASTNode->variableName = decimalTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "DECIMAL");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    std::string extractVariable = decimalTokensVec[3].substr(0, decimalTokensVec[3].length() - 1);
                    newASTNode->decimal = std::stod(extractVariable);
                }
                else if (this->isValidArrayDataExtraction(decimalTokensVec[3], "DECIMAL", newASTNode))
                {
                    this->assignVariableMemoryAddress(newASTNode, "DECIMAL");
                    newASTNode->variableName = decimalTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "DECIMAL");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                }
                else if (this->isValidVariableName(decimalTokensVec[3], false))
                {
                    this->assignVariableMemoryAddress(newASTNode, "DECIMAL");
                    newASTNode->variableName = decimalTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "DECIMAL");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    newASTNode->assignmentVariableName = decimalTokensVec[3].substr(0, decimalTokensVec[3].length() - 1);
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
        }
        else
        {
            return false;
        };
    }
    else if (commandType == C_CHARACTER)
    {
        std::vector<std::string> characterTokensVec = this->splitCodeLine(codeLine);
        if (characterTokensVec.size() == 2)
        {
            if (characterTokensVec[0] == "CHARACTER" && this->isValidVariableName(characterTokensVec[1], false) && !this->checkIfVariableNameExists(characterTokensVec[1].substr(0, characterTokensVec[1].length() - 1)))
            {
                this->assignVariableMemoryAddress(newASTNode, "CHARACTER");
                newASTNode->variableName = characterTokensVec[1].substr(0, characterTokensVec[1].length() - 1);
                this->createNewTempVariableStruct(newASTNode->variableName, "CHARACTER");
                this->variableNamesVec.push_back(newASTNode->variableName);
            }
            else
            {
                return false;
            };
        }
        else if (characterTokensVec.size() == 4)
        {
            if (characterTokensVec[0] == "CHARACTER" && this->isValidVariableName(characterTokensVec[1], true) && !this->checkIfVariableNameExists(characterTokensVec[1]) && characterTokensVec[2] == "=")
            {
                if (this->isValidVariableAssignment(characterTokensVec[3], "CHARACTER"))
                {
                    this->assignVariableMemoryAddress(newASTNode, "CHARACTER");
                    newASTNode->variableName = characterTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "CHARACTER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    newASTNode->character = characterTokensVec[3][1];
                }
                else if (this->isValidArrayDataExtraction(characterTokensVec[3], "CHARACTER", newASTNode))
                {
                    this->assignVariableMemoryAddress(newASTNode, "CHARACTER");
                    newASTNode->variableName = characterTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "CHARACTER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                }
                else if (this->isValidVariableName(characterTokensVec[3], false))
                {
                    this->assignVariableMemoryAddress(newASTNode, "CHARACTER");
                    newASTNode->variableName = characterTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "CHARACTER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    newASTNode->assignmentVariableName = characterTokensVec[3].substr(0, characterTokensVec[3].length() - 1);
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
        }
        else if (characterTokensVec.size() == 5)
        {
            if (characterTokensVec[0] == "CHARACTER" && this->isValidVariableName(characterTokensVec[1], true) && !this->checkIfVariableNameExists(characterTokensVec[1]) && characterTokensVec[2] == "=" && characterTokensVec[3] == "'" && characterTokensVec[4] == "';")
            {
                this->assignVariableMemoryAddress(newASTNode, "CHARACTER");
                newASTNode->variableName = characterTokensVec[1];
                this->createNewTempVariableStruct(newASTNode->variableName, "CHARACTER");
                this->variableNamesVec.push_back(newASTNode->variableName);
                newASTNode->character = ' ';
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
    }
    else if (commandType == C_BOOLEAN)
    {
        std::vector<std::string> booleanTokensVec = this->splitCodeLine(codeLine);
        if (booleanTokensVec.size() == 2)
        {
            if (booleanTokensVec[0] == "BOOLEAN" && this->isValidVariableName(booleanTokensVec[1], false) && !this->checkIfVariableNameExists(booleanTokensVec[1].substr(0, booleanTokensVec[1].length() - 1)))
            {
                this->assignVariableMemoryAddress(newASTNode, "BOOLEAN");
                newASTNode->variableName = booleanTokensVec[1].substr(0, booleanTokensVec[1].length() - 1);
                this->createNewTempVariableStruct(newASTNode->variableName, "BOOLEAN");
                this->variableNamesVec.push_back(newASTNode->variableName);
            }
            else
            {
                return false;
            };
        }
        else if (booleanTokensVec.size() == 4)
        {
            if (booleanTokensVec[0] == "BOOLEAN" && this->isValidVariableName(booleanTokensVec[1], true) && !this->checkIfVariableNameExists(booleanTokensVec[1]) && booleanTokensVec[2] == "=")
            {
                if (this->isValidVariableAssignment(booleanTokensVec[3],  "BOOLEAN"))
                {
                    this->assignVariableMemoryAddress(newASTNode, "BOOLEAN");
                    newASTNode->variableName = booleanTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "BOOLEAN");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    newASTNode->boolean = (booleanTokensVec[3] == "TRUE;" ? true : false);
                }
                else if (this->isValidArrayDataExtraction(booleanTokensVec[3], "BOOLEAN", newASTNode))
                {
                    this->assignVariableMemoryAddress(newASTNode, "BOOLEAN");
                    newASTNode->variableName = booleanTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "BOOLEAN");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                }
                else if (this->isValidVariableName(booleanTokensVec[3], false))
                {
                    this->assignVariableMemoryAddress(newASTNode, "BOOLEAN");
                    newASTNode->variableName = booleanTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "BOOLEAN");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    newASTNode->assignmentVariableName = booleanTokensVec[3].substr(0, booleanTokensVec[3].length() - 1);
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
        }
        else
        {
            return false;
        };
    }
    else if (commandType == C_STRING)
    {
        std::vector<std::string> stringTokensVec = this->splitCodeLine(codeLine);
        if (stringTokensVec.size() == 2)
        {
            if (stringTokensVec[0] == "STRING" && this->isValidVariableName(stringTokensVec[1], false) && !this->checkIfVariableNameExists(stringTokensVec[1].substr(0, stringTokensVec[1].length() - 1)))
            {
                this->assignVariableMemoryAddress(newASTNode, "STRING");
                newASTNode->variableName = stringTokensVec[1].substr(0, stringTokensVec[1].length() - 1);
                this->createNewTempVariableStruct(newASTNode->variableName, "STRING");
                this->variableNamesVec.push_back(newASTNode->variableName);
            }
            else
            {
                return false;
            };
        }
        else if (stringTokensVec.size() >= 4)
        {
            if (stringTokensVec[0] == "STRING" && this->isValidVariableName(stringTokensVec[1], true) && !this->checkIfVariableNameExists(stringTokensVec[1]) && stringTokensVec[2] == "=")
            {
                if (this->isValidArrayDataExtraction(stringTokensVec[3], "STRING", newASTNode))
                {
                    this->assignVariableMemoryAddress(newASTNode, "STRING");
                    newASTNode->variableName = stringTokensVec[1];
                    this->createNewTempVariableStruct(newASTNode->variableName, "STRING");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                }
                else
                {
                    std::string fullString = stringTokensVec[3];
                    for (int index = 4; index < stringTokensVec.size(); index++)
                    {
                        fullString += " " + stringTokensVec[index];
                    };
                    if (fullString[0] == '"' && fullString[fullString.length() - 2] == '"' && fullString[fullString.length() - 1] == ';')
                    {
                        this->assignVariableMemoryAddress(newASTNode, "STRING");
                        newASTNode->variableName = stringTokensVec[1];
                        this->createNewTempVariableStruct(newASTNode->variableName, "STRING");
                        this->variableNamesVec.push_back(newASTNode->variableName);
                        newASTNode->string = fullString.substr(1, fullString.size() - 3);
                    }
                    else if (this->isValidVariableName(fullString, false))
                    {
                        this->assignVariableMemoryAddress(newASTNode, "STRING");
                        newASTNode->variableName = stringTokensVec[1];
                        this->createNewTempVariableStruct(newASTNode->variableName, "STRING");
                        this->variableNamesVec.push_back(newASTNode->variableName);
                        newASTNode->assignmentVariableName = fullString.substr(0, fullString.size() - 1);
                    }
                    else
                    {
                        return false;
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
            return false;
        };
    }
    else if (commandType == C_CONSTANT_INTEGER)
    {
        std::vector<std::string> constantIntegerTokensVec = this->splitCodeLine(codeLine);
        if (constantIntegerTokensVec.size() == 5)
        {
            if (constantIntegerTokensVec[0] == "CONSTANT" && constantIntegerTokensVec[1] == "INTEGER" && this->isValidVariableName(constantIntegerTokensVec[2], true) && !this->checkIfVariableNameExists(constantIntegerTokensVec[2]) && constantIntegerTokensVec[3] == "=" && this->isValidVariableAssignment(constantIntegerTokensVec[4], "CONSTANT INTEGER"))
            {
                this->assignVariableMemoryAddress(newASTNode, "CONSTANT INTEGER");
                newASTNode->variableName = constantIntegerTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT INTEGER");
                this->variableNamesVec.push_back(newASTNode->variableName);
                std::string extractVariable = constantIntegerTokensVec[4].substr(0, constantIntegerTokensVec[4].length() - 1);
                newASTNode->integer = std::stoi(extractVariable);
                newASTNode->isConstant = true;
            }
            else if (constantIntegerTokensVec[0] == "CONSTANT" && constantIntegerTokensVec[1] == "INTEGER" && this->isValidVariableName(constantIntegerTokensVec[2], true) && !this->checkIfVariableNameExists(constantIntegerTokensVec[2]) && constantIntegerTokensVec[3] == "=" && this->isValidArrayDataExtraction(constantIntegerTokensVec[4], "CONSTANT INTEGER", newASTNode))
            {
                this->assignVariableMemoryAddress(newASTNode, "CONSTANT INTEGER");
                newASTNode->variableName = constantIntegerTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT INTEGER");
                this->variableNamesVec.push_back(newASTNode->variableName);
            }
            else if (constantIntegerTokensVec[0] == "CONSTANT" && constantIntegerTokensVec[1] == "INTEGER" && this->isValidVariableName(constantIntegerTokensVec[2], true) && !this->checkIfVariableNameExists(constantIntegerTokensVec[2]) && constantIntegerTokensVec[3] == "=" && constantIntegerTokensVec[4].substr(0, 5) == "size(" && constantIntegerTokensVec[4].substr(constantIntegerTokensVec[4].length() - 2) == ");" && this->isValidVariableName(constantIntegerTokensVec[4].substr(5, constantIntegerTokensVec[4].length() - 7), true))
            {
                bool assignArraySize = false;
                std::string arrayVariableName = constantIntegerTokensVec[4].substr(5, constantIntegerTokensVec[4].length() - 7);
                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                {
                    if (this->tempVariableStructVec[index].variableName == arrayVariableName)
                    {
                        std::string variableType = this->tempVariableStructVec[index].variableType;
                        if (variableType == "ARRAY INTEGER" || variableType == "ARRAY DECIMAL" || variableType == "ARRAY CHARACTER" || variableType == "ARRAY BOOLEAN" || variableType == "ARRAY STRING")
                        {
                            assignArraySize = true;
                            break;
                        };
                    };
                };
                if (assignArraySize == true)
                {
                    this->assignVariableMemoryAddress(newASTNode, "CONSTANT INTEGER");
                    newASTNode->variableName = constantIntegerTokensVec[2];
                    this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT INTEGER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    newASTNode->arrayVariableNameToAssignSize = arrayVariableName;
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
        }
        else
        {
            return false;
        };
    }
    else if (commandType == C_CONSTANT_DECIMAL)
    {
        std::vector<std::string> constantDecimalTokensVec = this->splitCodeLine(codeLine);
        if (constantDecimalTokensVec.size() == 5)
        {
            if (constantDecimalTokensVec[0] == "CONSTANT" && constantDecimalTokensVec[1] == "DECIMAL" && this->isValidVariableName(constantDecimalTokensVec[2], true) && !this->checkIfVariableNameExists(constantDecimalTokensVec[2]) && constantDecimalTokensVec[3] == "=" && this->isValidVariableAssignment(constantDecimalTokensVec[4], "CONSTANT DECIMAL"))
            {
                this->assignVariableMemoryAddress(newASTNode, "CONSTANT DECIMAL");
                newASTNode->variableName = constantDecimalTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT DECIMAL");
                this->variableNamesVec.push_back(newASTNode->variableName);
                std::string extractVariable = constantDecimalTokensVec[4].substr(0, constantDecimalTokensVec[4].length() - 1);
                newASTNode->decimal = std::stod(extractVariable);
                newASTNode->isConstant = true;
            }
            else if (constantDecimalTokensVec[0] == "CONSTANT" && constantDecimalTokensVec[1] == "DECIMAL" && this->isValidVariableName(constantDecimalTokensVec[2], true) && !this->checkIfVariableNameExists(constantDecimalTokensVec[2]) && constantDecimalTokensVec[3] == "=" && this->isValidArrayDataExtraction(constantDecimalTokensVec[4], "CONSTANT DECIMAL", newASTNode))
            {
                this->assignVariableMemoryAddress(newASTNode, "CONSTANT DECIMAL");
                newASTNode->variableName = constantDecimalTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT DECIMAL");
                this->variableNamesVec.push_back(newASTNode->variableName);
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
    }
    else if (commandType == C_CONSTANT_CHARACTER)
    {
        std::vector<std::string> constantCharacterTokensVec = this->splitCodeLine(codeLine);
        if (constantCharacterTokensVec.size() == 5)
        {
            if (constantCharacterTokensVec[0] == "CONSTANT" && constantCharacterTokensVec[1] == "CHARACTER" && this->isValidVariableName(constantCharacterTokensVec[2], true) && !this->checkIfVariableNameExists(constantCharacterTokensVec[2]) && constantCharacterTokensVec[3] == "=" && this->isValidVariableAssignment(constantCharacterTokensVec[4], "CONSTANT CHARACTER"))
            {
                this->assignVariableMemoryAddress(newASTNode, "CONSTANT CHARACTER");
                newASTNode->variableName = constantCharacterTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT CHARACTER");
                this->variableNamesVec.push_back(newASTNode->variableName);
                newASTNode->character = constantCharacterTokensVec[4][1];
                newASTNode->isConstant = true;
            }
            else if (constantCharacterTokensVec[0] == "CONSTANT" && constantCharacterTokensVec[1] == "CHARACTER" && this->isValidVariableName(constantCharacterTokensVec[2], true) && !this->checkIfVariableNameExists(constantCharacterTokensVec[2]) && constantCharacterTokensVec[3] == "=" && this->isValidArrayDataExtraction(constantCharacterTokensVec[4], "CONSTANT CHARACTER", newASTNode))
            {
                this->assignVariableMemoryAddress(newASTNode, "CONSTANT CHARACTER");
                newASTNode->variableName = constantCharacterTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT CHARACTER");
                this->variableNamesVec.push_back(newASTNode->variableName);
            }
            else
            {
                return false;
            };
        }
        else if (constantCharacterTokensVec.size() == 6)
        {
            if (constantCharacterTokensVec[0] == "CONSTANT" && constantCharacterTokensVec[1] == "CHARACTER" && this->isValidVariableName(constantCharacterTokensVec[2], true) && !this->checkIfVariableNameExists(constantCharacterTokensVec[2]) && constantCharacterTokensVec[3] == "=" && constantCharacterTokensVec[4] == "'" && constantCharacterTokensVec[5] == "';")
            {
                this->assignVariableMemoryAddress(newASTNode, "CONSTANT CHARACTER");
                newASTNode->variableName = constantCharacterTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT CHARACTER");
                this->variableNamesVec.push_back(newASTNode->variableName);
                newASTNode->character = ' ';
                newASTNode->isConstant = true;
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
    }
    else if (commandType == C_CONSTANT_BOOLEAN)
    {
        std::vector<std::string> constantBooleanTokensVec = this->splitCodeLine(codeLine);
        if (constantBooleanTokensVec.size() == 5)
        {
            if (constantBooleanTokensVec[0] == "CONSTANT" && constantBooleanTokensVec[1] == "BOOLEAN" && this->isValidVariableName(constantBooleanTokensVec[2], true) && !this->checkIfVariableNameExists(constantBooleanTokensVec[2]) && constantBooleanTokensVec[3] == "=" && this->isValidVariableAssignment(constantBooleanTokensVec[4], "CONSTANT BOOLEAN"))
            {
                this->assignVariableMemoryAddress(newASTNode, "CONSTANT BOOLEAN");
                newASTNode->variableName = constantBooleanTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT BOOLEAN");
                this->variableNamesVec.push_back(newASTNode->variableName);
                newASTNode->boolean = (constantBooleanTokensVec[4] == "TRUE;" ? true: false);
                newASTNode->isConstant = true;
            }
            else if (constantBooleanTokensVec[0] == "CONSTANT" && constantBooleanTokensVec[1] == "BOOLEAN" && this->isValidVariableName(constantBooleanTokensVec[2], true) && !this->checkIfVariableNameExists(constantBooleanTokensVec[2]) && constantBooleanTokensVec[3] == "=" && this->isValidArrayDataExtraction(constantBooleanTokensVec[4], "CONSTANT BOOLEAN", newASTNode))
            {
                this->assignVariableMemoryAddress(newASTNode, "CONSTANT BOOLEAN");
                newASTNode->variableName = constantBooleanTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT BOOLEAN");
                this->variableNamesVec.push_back(newASTNode->variableName);
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
    }
    else if (commandType == C_CONSTANT_STRING)
    {
        std::vector<std::string> constantStringTokensVector = this->splitCodeLine(codeLine);
        if (constantStringTokensVector.size() >= 5)
        {
            if (constantStringTokensVector[0] == "CONSTANT" && constantStringTokensVector[1] == "STRING" && this->isValidVariableName(constantStringTokensVector[2], true) && !this->checkIfVariableNameExists(constantStringTokensVector[2]) && constantStringTokensVector[3] == "=")
            {
                if (constantStringTokensVector[0] == "CONSTANT" && constantStringTokensVector[1] == "STRING" && this->isValidVariableName(constantStringTokensVector[2], true) && !this->checkIfVariableNameExists(constantStringTokensVector[2]) && constantStringTokensVector[3] == "=" && this->isValidArrayDataExtraction(constantStringTokensVector[4], "CONSTANT STRING", newASTNode))
                {
                    this->assignVariableMemoryAddress(newASTNode, "CONSTANT STRING");
                    newASTNode->variableName = constantStringTokensVector[2];
                    this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT STRING");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                }
                else
                {
                    std::string fullString = constantStringTokensVector[4];
                    for (int index = 5; index < constantStringTokensVector.size(); index++)
                    {
                        fullString += " " + constantStringTokensVector[index];
                    };
                    if (fullString[0] == '"' && fullString[fullString.length() - 2] == '"' && fullString[fullString.length() - 1] == ';')
                    {
                        this->assignVariableMemoryAddress(newASTNode, "CONSTANT STRING");
                        newASTNode->variableName = constantStringTokensVector[2];
                        this->createNewTempVariableStruct(newASTNode->variableName, "CONSTANT STRING");
                        this->variableNamesVec.push_back(newASTNode->variableName);
                        newASTNode->string = fullString.substr(1, fullString.size() - 3);
                    }
                    else
                    {
                        return false;
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
            return false;
        };
    }
    else if (commandType == C_ASSIGNMENT_OPERATOR)
    {
        std::vector<std::string> assignmentOperatorTokensVector = this->splitCodeLine(codeLine);
        if (assignmentOperatorTokensVector.size() == 3 && this->isValidVariableName(assignmentOperatorTokensVector[0], true) && assignmentOperatorTokensVector[1] == "=" && this->isValidArrayDataExtraction(assignmentOperatorTokensVector[2], "", nullptr))
        {
            newASTNode->variableName = assignmentOperatorTokensVector[0];
            newASTNode->assignmentOperatorValue = assignmentOperatorTokensVector[2];
        }
        else
        {
            if (assignmentOperatorTokensVector.size() >= 3 && this->isValidVariableName(assignmentOperatorTokensVector[0], true) && assignmentOperatorTokensVector[1] == "=")
            {
                newASTNode->variableName = assignmentOperatorTokensVector[0];
                std::string assignmentOperatorString = assignmentOperatorTokensVector[2];
                for (int index = 3; index< assignmentOperatorTokensVector.size(); index++)
                {
                    assignmentOperatorString += " " + assignmentOperatorTokensVector[index];
                };
                newASTNode->assignmentOperatorValue = assignmentOperatorString.substr(0, assignmentOperatorString.length() - 1);
            }
            else
            {
                return false;
            };
        };
    }
    else if (commandType == C_ARITHMETIC)
    {
        std::string newCodeLine = codeLine;
        newCodeLine.erase(std::remove(newCodeLine.begin(), newCodeLine.end(), ';'), newCodeLine.end());
        std::vector<std::string> arithmeticTokensVector;
        bool tokenIsAString = false;
        std::string currentToken = "";
        for (int index = 0; index < newCodeLine.length(); index++)
        {
            if (currentToken.length() == 0)
            {
                if (newCodeLine[index] == '"')
                {
                    tokenIsAString = true;
                    currentToken += newCodeLine[index];
                }
                else if (newCodeLine[index] != ' ')
                {
                    tokenIsAString = false;
                    currentToken += newCodeLine[index];
                };
            }
            else if (tokenIsAString)
            {
                if (newCodeLine[index] == '"')
                {
                    currentToken += newCodeLine[index];
                    arithmeticTokensVector.push_back(currentToken);
                    currentToken = "";
                }
                else
                {
                    currentToken += newCodeLine[index];
                };
            }
            else if (newCodeLine[index] != ' ')
            {
                currentToken += newCodeLine[index];
            }
            else
            {
                arithmeticTokensVector.push_back(currentToken);
                currentToken = "";
            };
        };
        if (currentToken.length() > 0)
        {
            arithmeticTokensVector.push_back(currentToken);
        };
        if (this->isValidVariableName(arithmeticTokensVector[0], true) && arithmeticTokensVector[1] == "=")
        {
            for (int index = 2; index < arithmeticTokensVector.size(); index++)
            {
                newASTNode->arithmeticVec.push_back(arithmeticTokensVector[index]);
            };
            newASTNode->variableName = arithmeticTokensVector[0];
        }
        else
        {
            return false;
        };
    }
    else if (commandType == C_CONTROL_FLOW)
    {
        std::regex re(R"([()])");
        std::sregex_token_iterator it(codeLine.begin(), codeLine.end(), re, {-1});
        std::sregex_token_iterator end;
        std::vector<std::string> parts(it, end);
        bool isAnIfStatement = true;
        for (int index = 0; index < parts.size(); index++)
        {
            parts[index] = this->trimString(parts[index]);
        };
        if (parts[0] == "IF" && parts[parts.size() - 1] == "BRANCH;" || parts[0] == "ELSE IF" && parts[parts.size() - 1] == "BRANCH;")
        {
            if (parts[0] == "ELSE IF" && parts[parts.size() - 1] == "BRANCH;")
            {
                isAnIfStatement = false;
                ASTNodeType = "CONTROL FLOW AST NODE";
                newASTNode->controlFlowType = ELSE_IF;
            }
            else
            {
                newASTNode->controlFlowType = IF;
            };
            for (int index = 2; index < parts.size(); index += 2)
            {
                if (index != parts.size() - 1)
                {
                    if (parts[index] != "AND" && parts[index] != "OR")
                    {
                        return false;
                    }
                    else if (parts[index] == "AND")
                    {
                        LogicalOperator logicalOperator = AND;
                        newASTNode->logicalOperatorVec.push_back(logicalOperator);
                    }
                    else if (parts[index] == "OR")
                    {
                        LogicalOperator logicalOperator = OR;
                        newASTNode->logicalOperatorVec.push_back(logicalOperator);
                    };
                };
            };
            for (int index = 1; index < parts.size(); index += 2)
            {
                std::regex re(R"( (EQ|NE|GT|LT|GE|LE) )");
                std::sregex_token_iterator begin(parts[index].begin(), parts[index].end(), re, -1);
                std::sregex_token_iterator end;
                std::vector<std::string> operandsVec(begin, end);
                if (operandsVec.size() == 2)
                {
                    DataType leftOperandDataType = this->checkDataType(operandsVec[0]);
                    DataType rightOperandDataType = this->checkDataType(operandsVec[1]);
                    if (leftOperandDataType != D_NULL && rightOperandDataType != D_NULL)
                    {
                        if (leftOperandDataType != D_VARIABLE && rightOperandDataType != D_VARIABLE && leftOperandDataType == rightOperandDataType)
                        {
                            ComparisonStruct newComparisonStruct;
                            if (leftOperandDataType == D_INTEGER)
                            {
                                newComparisonStruct.dataType = D_INTEGER;
                                newComparisonStruct.integerOperand1 = std::stoi(operandsVec[0]);
                                newComparisonStruct.integerOperand2 = std::stoi(operandsVec[1]);
                            }
                            else if (leftOperandDataType == D_DECIMAL)
                            {
                                newComparisonStruct.dataType = D_DECIMAL;
                                newComparisonStruct.decimalOperand1 = std::stod(operandsVec[0]);
                                newComparisonStruct.decimalOperand2 = std::stod(operandsVec[1]);
                            }
                            else if (leftOperandDataType == D_CHARACTER)
                            {
                                newComparisonStruct.dataType = D_CHARACTER;
                                newComparisonStruct.characterOperand1 = operandsVec[0][1];
                                newComparisonStruct.characterOperand2 = operandsVec[1][1];
                            }
                            else if (leftOperandDataType == D_BOOLEAN)
                            {
                                newComparisonStruct.dataType = D_BOOLEAN;
                                newComparisonStruct.booleanOperand1 = (operandsVec[0] == "TRUE" ? true : false);
                                newComparisonStruct.booleanOperand2 = (operandsVec[1] == "TRUE" ? true : false);
                            }
                            else if (leftOperandDataType == D_STRING)
                            {
                                newComparisonStruct.dataType = D_STRING;
                                newComparisonStruct.stringOperand1 = operandsVec[0].substr(1, operandsVec[0].size() - 2);
                                newComparisonStruct.stringOperand2 = operandsVec[1].substr(1, operandsVec[1].size() - 2);
                            };
                            std::string comparisonOperatorString = this->trimString(parts[index].substr(operandsVec[0].size(), 4));
                            if (newComparisonStruct.dataType == D_BOOLEAN)
                            {
                                if (comparisonOperatorString == "EQ")
                                {
                                    newComparisonStruct.comparisonOperator = EQ;
                                }
                                else if (comparisonOperatorString == "NE")
                                {
                                    newComparisonStruct.comparisonOperator = NE;
                                }
                                else
                                {
                                    return false;
                                };
                            }
                            else
                            {
                                if (comparisonOperatorString == "EQ")
                                {
                                    newComparisonStruct.comparisonOperator = EQ;
                                }
                                else if (comparisonOperatorString == "NE")
                                {
                                    newComparisonStruct.comparisonOperator = NE;
                                }
                                else if (comparisonOperatorString == "GT")
                                {
                                    newComparisonStruct.comparisonOperator = GT;
                                }
                                else if (comparisonOperatorString == "LT")
                                {
                                    newComparisonStruct.comparisonOperator = LT;
                                }
                                else if (comparisonOperatorString == "GE")
                                {
                                    newComparisonStruct.comparisonOperator = GE;
                                }
                                else if (comparisonOperatorString == "LE")
                                {
                                    newComparisonStruct.comparisonOperator = LE;
                                };
                            };
                            newASTNode->comparisonStructVec.push_back(newComparisonStruct);
                        }
                        else if (leftOperandDataType != D_VARIABLE && rightOperandDataType != D_VARIABLE && leftOperandDataType != rightOperandDataType)
                        {
                            return false;
                        }
                        else
                        {
                            std::string leftOperandDataTypeString = "";
                            std::string rightOperandDataTypeString = "";
                            if (leftOperandDataType == D_VARIABLE)
                            {
                                bool foundLeftOperandVariableName = false;
                                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                                {
                                    if (this->tempVariableStructVec[index].variableName == operandsVec[0])
                                    {
                                        leftOperandDataTypeString = this->tempVariableStructVec[index].variableType;
                                        foundLeftOperandVariableName = true;
                                        break;
                                    };
                                };
                                if (foundLeftOperandVariableName == false)
                                {
                                    return false;
                                };
                            }
                            else
                            {
                                if (leftOperandDataType == D_INTEGER)
                                {
                                    leftOperandDataTypeString = "INTEGER";
                                }
                                else if (leftOperandDataType == D_DECIMAL)
                                {
                                    leftOperandDataTypeString = "DECIMAL";
                                }
                                else if (leftOperandDataType == D_CHARACTER)
                                {
                                    leftOperandDataTypeString = "CHARACTER";
                                }
                                else if (leftOperandDataType == D_BOOLEAN)
                                {
                                    leftOperandDataTypeString = "BOOLEAN";
                                }
                                else if (leftOperandDataType == D_STRING)
                                {
                                    leftOperandDataTypeString = "STRING";
                                }
                                else
                                {
                                    return false;
                                };
                            };
                            if (rightOperandDataType == D_VARIABLE)
                            {
                                bool foundRightOperandVariableName = false;
                                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                                {
                                    if (this->tempVariableStructVec[index].variableName == operandsVec[1])
                                    {
                                        rightOperandDataTypeString = this->tempVariableStructVec[index].variableType;
                                        foundRightOperandVariableName = true;
                                        break;
                                    };
                                };
                                if (foundRightOperandVariableName == false)
                                {
                                    return false;
                                };
                            }
                            else
                            {
                                if (rightOperandDataType == D_INTEGER)
                                {
                                    rightOperandDataTypeString = "INTEGER";
                                }
                                else if (rightOperandDataType == D_DECIMAL)
                                {
                                    rightOperandDataTypeString = "DECIMAL";
                                }
                                else if (rightOperandDataType == D_CHARACTER)
                                {
                                    rightOperandDataTypeString = "CHARACTER";
                                }
                                else if (rightOperandDataType == D_BOOLEAN)
                                {
                                    rightOperandDataTypeString = "BOOLEAN";
                                }
                                else if (rightOperandDataType == D_STRING)
                                {
                                    rightOperandDataTypeString = "STRING";
                                }
                                else
                                {
                                    return false;
                                };
                            };
                            if (leftOperandDataTypeString == rightOperandDataTypeString)
                            {
                                ComparisonStruct newComparisonStruct;
                                if (leftOperandDataType == D_INTEGER)
                                {
                                    newComparisonStruct.dataType = D_INTEGER;
                                    newComparisonStruct.integerOperand1 = std::stoi(operandsVec[0]);
                                }
                                else if (leftOperandDataType == D_DECIMAL)
                                {
                                    newComparisonStruct.dataType = D_DECIMAL;
                                    newComparisonStruct.decimalOperand1 = std::stod(operandsVec[0]);
                                }
                                else if (leftOperandDataType == D_CHARACTER)
                                {
                                    newComparisonStruct.dataType = D_CHARACTER;
                                    newComparisonStruct.characterOperand1 = operandsVec[0][1];
                                }
                                else if (leftOperandDataType == D_BOOLEAN)
                                {
                                    newComparisonStruct.dataType = D_BOOLEAN;
                                    newComparisonStruct.booleanOperand1 = (operandsVec[0] == "TRUE" ? true : false);
                                }
                                else if (leftOperandDataType == D_STRING)
                                {
                                    newComparisonStruct.dataType = D_STRING;
                                    newComparisonStruct.stringOperand1 = operandsVec[0].substr(1, operandsVec[0].size() - 2);
                                }
                                else if (leftOperandDataType == D_VARIABLE)
                                {
                                    if (leftOperandDataTypeString == "INTEGER")
                                    {
                                        newComparisonStruct.dataType = D_INTEGER;
                                    }
                                    else if (leftOperandDataTypeString == "DECIMAL")
                                    {
                                        newComparisonStruct.dataType = D_DECIMAL;
                                    }
                                    else if (leftOperandDataTypeString == "CHARACTER")
                                    {
                                        newComparisonStruct.dataType = D_CHARACTER;
                                    }
                                    else if (leftOperandDataTypeString == "BOOLEAN")
                                    {
                                        newComparisonStruct.dataType = D_BOOLEAN;
                                    }
                                    else if (leftOperandDataTypeString == "STRING")
                                    {
                                        newComparisonStruct.dataType = D_STRING;
                                    };
                                    newComparisonStruct.operand1VariableName = operandsVec[0];
                                    newComparisonStruct.operand1IsAVariable = true;
                                };
                                if (rightOperandDataType == D_INTEGER)
                                {
                                    newComparisonStruct.integerOperand2 = std::stoi(operandsVec[1]);
                                }
                                else if (rightOperandDataType == D_DECIMAL)
                                {
                                    newComparisonStruct.decimalOperand2 = std::stod(operandsVec[1]);
                                }
                                else if (rightOperandDataType == D_CHARACTER)
                                {
                                    newComparisonStruct.characterOperand2 = operandsVec[1][1];
                                }
                                else if (rightOperandDataType == D_BOOLEAN)
                                {
                                    newComparisonStruct.booleanOperand2 = (operandsVec[1] == "TRUE" ? true : false);
                                }
                                else if (rightOperandDataType == D_STRING)
                                {
                                    newComparisonStruct.stringOperand2 = operandsVec[1].substr(1, operandsVec[1].size() - 2);
                                }
                                else if (rightOperandDataType == D_VARIABLE)
                                {
                                    newComparisonStruct.operand2VariableName = operandsVec[1];
                                    newComparisonStruct.operand2IsAVariable = true;
                                };
                                std::string comparisonOperatorString = this->trimString(parts[index].substr(operandsVec[0].size(), 4));
                                if (newComparisonStruct.dataType == D_BOOLEAN)
                                {
                                    if (comparisonOperatorString == "EQ")
                                    {
                                        newComparisonStruct.comparisonOperator = EQ;
                                    }
                                    else if (comparisonOperatorString == "NE")
                                    {
                                        newComparisonStruct.comparisonOperator = NE;
                                    }
                                    else
                                    {
                                        return false;
                                    };
                                }
                                else
                                {
                                    if (comparisonOperatorString == "EQ")
                                    {
                                        newComparisonStruct.comparisonOperator = EQ;
                                    }
                                    else if (comparisonOperatorString == "NE")
                                    {
                                        newComparisonStruct.comparisonOperator = NE;
                                    }
                                    else if (comparisonOperatorString == "GT")
                                    {
                                        newComparisonStruct.comparisonOperator = GT;
                                    }
                                    else if (comparisonOperatorString == "LT")
                                    {
                                        newComparisonStruct.comparisonOperator = LT;
                                    }
                                    else if (comparisonOperatorString == "GE")
                                    {
                                        newComparisonStruct.comparisonOperator = GE;
                                    }
                                    else if (comparisonOperatorString == "LE")
                                    {
                                        newComparisonStruct.comparisonOperator = LE;
                                    };
                                };
                                newASTNode->comparisonStructVec.push_back(newComparisonStruct);
                            }
                            else
                            {
                                return false;
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
                    return false;
                };
            };
            if (isAnIfStatement)
            {
                this->controlFlowStack.push(newASTNode);
            };
        }
        else if (codeLine == "ELSE BRANCH;")
        {
            ASTNodeType = "CONTROL FLOW AST NODE";
            newASTNode->controlFlowType = ELSE;
        }
        else if (codeLine == "END;")
        {
            ASTNodeType = "CONTROL FLOW AST NODE END";
            newASTNode->controlFlowType = END;
        }
        else
        {
            return false;
        };
    }
    else if (commandType == C_ARRAY_INTEGER)
    {
        std::vector<std::string> arrayIntegerTokensVec = this->splitCodeLine(codeLine);
        if (arrayIntegerTokensVec.size() == 5)
        {
            if (arrayIntegerTokensVec[0] == "ARRAY" && arrayIntegerTokensVec[1] == "INTEGER" && this->isValidVariableName(arrayIntegerTokensVec[2], true) && arrayIntegerTokensVec[3] == "=" && arrayIntegerTokensVec[4][0] == '[' && arrayIntegerTokensVec[4][arrayIntegerTokensVec[4].length() - 2] == ']' && arrayIntegerTokensVec[4][arrayIntegerTokensVec[4].length() - 1] == ';')
            {
                if (arrayIntegerTokensVec[4] != "[];")
                {
                    std::string integerArrayString = arrayIntegerTokensVec[4].substr(1, arrayIntegerTokensVec[4].length() - 3);
                    std::regex re(",");
                    std::sregex_token_iterator first(integerArrayString.begin(), integerArrayString.end(), re, -1);
                    std::sregex_token_iterator last;
                    std::vector<std::string> arrayIntegerItemsVec(first, last);
                    for (int index = 0; index < arrayIntegerItemsVec.size(); index++)
                    {
                        if (this->isValidVariableAssignment(arrayIntegerItemsVec[index] + ";", "INTEGER"))
                        {
                            newASTNode->integerArray.push_back(std::stoi(arrayIntegerItemsVec[index]));
                        };
                    };
                    this->assignVariableMemoryAddress(newASTNode, "ARRAY INTEGER");
                    newASTNode->variableName = arrayIntegerTokensVec[2];
                    this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY INTEGER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                }
                else
                {
                    this->assignVariableMemoryAddress(newASTNode, "ARRAY INTEGER");
                    newASTNode->variableName = arrayIntegerTokensVec[2];
                    this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY INTEGER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                };
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
    }
    else if (commandType == C_ARRAY_DECIMAL)
    {
        std::vector<std::string> arrayDecimalTokensVec = this->splitCodeLine(codeLine);
        if (arrayDecimalTokensVec.size() == 5)
        {
            if (arrayDecimalTokensVec[0] == "ARRAY" && arrayDecimalTokensVec[1] == "DECIMAL" && this->isValidVariableName(arrayDecimalTokensVec[2], true) && arrayDecimalTokensVec[3] == "=" && arrayDecimalTokensVec[4][0] == '[' && arrayDecimalTokensVec[4][arrayDecimalTokensVec[4].length() - 2] == ']' && arrayDecimalTokensVec[4][arrayDecimalTokensVec[4].length() - 1] == ';')
            {
                if (arrayDecimalTokensVec[4] != "[];")
                {
                    std::string decimalArrayString = arrayDecimalTokensVec[4].substr(1, arrayDecimalTokensVec[4].length() - 3);
                    std::regex re(",");
                    std::sregex_token_iterator first(decimalArrayString.begin(), decimalArrayString.end(), re, -1);
                    std::sregex_token_iterator last;
                    std::vector<std::string> arrayDecimalItemsVec(first, last);
                    for (int index = 0; index < arrayDecimalItemsVec.size(); index++)
                    {
                        if (this->isValidVariableAssignment(arrayDecimalItemsVec[index] + ";", "DECIMAL"))
                        {
                            newASTNode->decimalArray.push_back(std::stod(arrayDecimalItemsVec[index]));
                        };
                    };
                    this->assignVariableMemoryAddress(newASTNode, "ARRAY DECIMAL");
                    newASTNode->variableName = arrayDecimalTokensVec[2];
                    this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY DECIMAL");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                }
                else
                {
                    this->assignVariableMemoryAddress(newASTNode, "ARRAY DECIMAL");
                    newASTNode->variableName = arrayDecimalTokensVec[2];
                    this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY DECIMAL");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                };
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
    }
    else if (commandType == C_ARRAY_CHARACTER)
    {
        std::vector<std::string> arrayCharacterTokensVec = this->splitCodeLine(codeLine);
        if (arrayCharacterTokensVec.size() == 5)
        {
            if (arrayCharacterTokensVec[0] == "ARRAY" && arrayCharacterTokensVec[1] == "CHARACTER" && this->isValidVariableName(arrayCharacterTokensVec[2], true) && arrayCharacterTokensVec[3] == "=" && arrayCharacterTokensVec[4][0] == '[' && arrayCharacterTokensVec[4][arrayCharacterTokensVec[4].length() - 2] == ']' && arrayCharacterTokensVec[4][arrayCharacterTokensVec[4].length() - 1] == ';')
            {
                if (arrayCharacterTokensVec[4] != "[];")
                {
                    bool incorrectFormat = false;
                    for (int index = 1; index < arrayCharacterTokensVec[4].length() - 2; index+=4)
                    {
                        if (arrayCharacterTokensVec[4][index] == '\'' && arrayCharacterTokensVec[4][index + 2] == '\'')
                        {
                            newASTNode->characterArray.push_back(arrayCharacterTokensVec[4][index + 1]);
                        }
                        else
                        {
                            incorrectFormat = true;  
                        };
                    };
                    if (incorrectFormat == false)
                    {
                        this->assignVariableMemoryAddress(newASTNode, "ARRAY CHARACTER");
                        newASTNode->variableName = arrayCharacterTokensVec[2];
                        this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY CHARACTER");
                        this->variableNamesVec.push_back(newASTNode->variableName);
                    }
                    else
                    {
                        return false;
                    };
                }
                else
                {
                    this->assignVariableMemoryAddress(newASTNode, "ARRAY CHARACTER");
                    newASTNode->variableName = arrayCharacterTokensVec[2];
                    this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY CHARACTER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                };
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
    }
    else if (commandType == C_ARRAY_BOOLEAN)
    {
        std::vector<std::string> arrayBooleanTokensVec = this->splitCodeLine(codeLine);
        if (arrayBooleanTokensVec.size() == 5)
        {
            if (arrayBooleanTokensVec[0] == "ARRAY" && arrayBooleanTokensVec[1] == "BOOLEAN" && this->isValidVariableName(arrayBooleanTokensVec[2], true) && arrayBooleanTokensVec[3] == "=" && arrayBooleanTokensVec[4][0] == '[' && arrayBooleanTokensVec[4][arrayBooleanTokensVec[4].length() - 2] == ']' && arrayBooleanTokensVec[4][arrayBooleanTokensVec[4].length() - 1] == ';')
            {
                if (arrayBooleanTokensVec[4] != "[];")
                {
                    std::string booleanArrayString = arrayBooleanTokensVec[4].substr(1, arrayBooleanTokensVec[4].length() - 3);
                    std::regex re(",");
                    std::sregex_token_iterator first(booleanArrayString.begin(), booleanArrayString.end(), re, -1);
                    std::sregex_token_iterator last;
                    std::vector<std::string> arrayBooleanItemsVec(first, last);
                    for (int index = 0; index < arrayBooleanItemsVec.size(); index++)
                    {
                        if (this->isValidVariableAssignment(arrayBooleanItemsVec[index] + ";", "BOOLEAN"))
                        {
                            newASTNode->booleanArray.push_back((arrayBooleanItemsVec[index] == "TRUE" ? true : false));
                        };
                    };
                    this->assignVariableMemoryAddress(newASTNode, "ARRAY BOOLEAN");
                    newASTNode->variableName = arrayBooleanTokensVec[2];
                    this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY BOOLEAN");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                }
                else
                {
                    this->assignVariableMemoryAddress(newASTNode, "ARRAY BOOLEAN");
                    newASTNode->variableName = arrayBooleanTokensVec[2];
                    this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY BOOLEAN");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                };
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
    }
    else if (commandType == C_ARRAY_STRING)
    {
        std::vector<std::string> arrayStringTokensVec = this->splitCodeLine(codeLine);
        if (arrayStringTokensVec.size() == 5 && arrayStringTokensVec[0] == "ARRAY" && arrayStringTokensVec[1] == "STRING" && this->isValidVariableName(arrayStringTokensVec[2], true) && arrayStringTokensVec[3] == "=" && arrayStringTokensVec[4] == "[];")
        {
            this->assignVariableMemoryAddress(newASTNode, "ARRAY STRING");
            newASTNode->variableName = arrayStringTokensVec[2];
            this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY STRING");
            this->variableNamesVec.push_back(newASTNode->variableName);
        }
        else if (arrayStringTokensVec.size() >= 5)
        {
            if (arrayStringTokensVec[0] == "ARRAY" && arrayStringTokensVec[1] == "STRING" && this->isValidVariableName(arrayStringTokensVec[2], true) && arrayStringTokensVec[3] == "=")
            {
                std::string stringArray = "";
                for (int index = 4; index < arrayStringTokensVec.size(); index++)
                {
                    stringArray += arrayStringTokensVec[index] + " ";
                };
                std::string stringArrayString = this->trimString(stringArray);
                std::regex re(R"(",")");
                std::sregex_token_iterator first(stringArrayString.begin(), stringArrayString.end(), re, -1);
                std::sregex_token_iterator last;
                std::vector<std::string> arrayStringItemsVec(first, last);
                for (int index = 0; index < arrayStringItemsVec.size(); index++)
                {
                    if (index == 0)
                    {
                        newASTNode->stringArray.push_back(arrayStringItemsVec[index].substr(2));
                    }
                    else if (index + 1 == arrayStringItemsVec.size())
                    {
                        newASTNode->stringArray.push_back(arrayStringItemsVec[index].substr(0, arrayStringItemsVec[index].size() - 3));
                    }
                    else
                    {
                        newASTNode->stringArray.push_back(arrayStringItemsVec[index]);
                    };
                };
                this->assignVariableMemoryAddress(newASTNode, "ARRAY STRING");
                newASTNode->variableName = arrayStringTokensVec[2];
                this->createNewTempVariableStruct(newASTNode->variableName, "ARRAY STRING");
                this->variableNamesVec.push_back(newASTNode->variableName);
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
    }
    else if (commandType == C_FOR_LOOP)
    {
        std::regex re(R"([()])");
        std::sregex_token_iterator split1Start(codeLine.begin(), codeLine.end(), re, {-1});
        std::sregex_token_iterator split1End;
        std::vector<std::string> forLoopTokensVec(split1Start, split1End);
        for (int index = 0; index < forLoopTokensVec.size(); index++)
        {
            std::cout << "forLoopTokensVec:" << forLoopTokensVec[index] << std::endl;
        };
        if (forLoopTokensVec.size() == 3 && forLoopTokensVec[0] == "FOR LOOP" && forLoopTokensVec[2] == "BRANCH;")
        {
            std::regex re(R"([;])");
            std::sregex_token_iterator split2Start(codeLine.begin(), codeLine.end(), re, {-1});
            std::sregex_token_iterator split2End;
            std::vector<std::string> conditionTokensVec(split1Start, split2End);
            for (int index = 0; index < conditionTokensVec.size(); index++)
            {
                conditionTokensVec[index] = this->trimString(conditionTokensVec[index]);
            };
            if (conditionTokensVec.size() == 3)
            {
                std::string initializationString = conditionTokensVec[0];
                std::string conditionString = conditionTokensVec[1];
                std::string incrementString = conditionTokensVec[2];
                std::vector<std::string> initializationVec = this->splitCodeLine(initializationString);

                if (initializationVec.size() == 4 && initializationVec[0] == "INTEGER" && this->isValidVariableName(initializationVec[1], true) && !this->checkIfVariableNameExists(initializationVec[1]) && initializationVec[2] == "=" && this->isValidVariableAssignment(initializationVec[3] + ";", "INTEGER"))
                {
                    newASTNode->forLoopIncrementVariableName = initializationVec[1];
                    ASTNode* incrementVariableASTNode = new ASTNode;
                    incrementVariableASTNode->command = C_INTEGER;
                    this->assignVariableMemoryAddress(incrementVariableASTNode, "INTEGER");
                    incrementVariableASTNode->variableName = initializationVec[1];
                    this->createNewTempVariableStruct(incrementVariableASTNode->variableName, "INTEGER");
                    this->variableNamesVec.push_back(newASTNode->variableName);
                    incrementVariableASTNode->integer = std::stoi(initializationVec[3]);
                    std::regex re(R"( (EQ|NE|GT|LT|GE|LE) )");
                    std::sregex_token_iterator split3Start(conditionString.begin(), conditionString.end(), re, -1);
                    std::sregex_token_iterator split3End;
                    std::vector<std::string> operandsVec(split3Start, split3End);
                    if (operandsVec.size() == 2)
                    {
                        DataType leftOperandDataType = this->checkDataType(operandsVec[0]);
                        DataType rightOperandDataType = this->checkDataType(operandsVec[1]);








                        
                    };
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
        }
        else
        {
            return false;
        }
    }
    else if (commandType == C_WHILE_LOOP)
    {

    }
    else
    {
        return false;
    };
    if (ASTNodeType == "SEQUENTIAL AST NODE")
    {
        if (currentASTNode == nullptr && this->root == nullptr)
        {
            this->root = newASTNode;
            this->currentASTNode = newASTNode;
            buildASTSuccessfully = true;
        }
        else if (currentASTNode != nullptr)
        {
            this->currentASTNode->sequentialASTNode = newASTNode;
            this->currentASTNode = newASTNode;
            buildASTSuccessfully = true;
        };
    }
    else if (ASTNodeType == "CONTROL FLOW AST NODE")
    {
        ASTNode* controlFlowStackASTNode = this->controlFlowStack.top();
        while (controlFlowStackASTNode != nullptr && controlFlowStackASTNode->controlFlowASTNode != nullptr)
        {
            controlFlowStackASTNode = controlFlowStackASTNode->controlFlowASTNode;
        };
        if (controlFlowStackASTNode->controlFlowASTNode == nullptr)
        {
            controlFlowStackASTNode->controlFlowASTNode = newASTNode;
            this->currentASTNode = newASTNode;
            buildASTSuccessfully = true;
        }
        else
        {
            return false;
        };
    }
    else if (ASTNodeType == "CONTROL FLOW AST NODE END")
    {
        ASTNode* controlFlowStackASTNode = this->controlFlowStack.top();
        this->connectDanglingBranches(controlFlowStackASTNode, newASTNode);
        this->controlFlowStack.pop();
        this->currentASTNode = newASTNode;
        buildASTSuccessfully = true;
    }
    else
    {
        return false;
    }
    std::cout << "[PARSER] Built AST Node Successfully!" << std::endl;
    return buildASTSuccessfully;
};

/*
==================================================
buildASTHelper function
==================================================
*/
void Parser::buildASTHelper(std::string currentCodeLine, Command commandType, ASTNode* currentASTNode, bool& hasInvalidSyntax, int& syntaxErrorLine, int index)
{
    if (commandType != C_NONE)
    {
        bool buildASTSuccessfully = this->buildAST(currentCodeLine, commandType, this->currentASTNode);
        if (buildASTSuccessfully)
        {
            hasInvalidSyntax = false;
        }
        else
        {
            syntaxErrorLine = index + 1;
        };
    };
    return;
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
Returns if the variableName input is in valid form
==================================================
*/
bool Parser::isValidVariableName(std::string variableName, bool isBeingAssigned)
{
    if (variableName.length() == 0 || variableName == "TRUE;" || variableName == "FALSE;" || variableName == "TRUE" || variableName == "FALSE")
    {
        return false;
    };
    if (variableName[0] != '_' && !std::isalpha(variableName[0]))
    {
        return false;
    };
    if (!isBeingAssigned)
    {
        if (variableName.length() <= 1 || variableName[variableName.length() - 1] != ';')
        {
            return false;
        };
    };
    for (int index = 1; index < variableName.length() - 1; index++)
    {
        if (variableName[index] != '_' && !std::isalnum(variableName[index]))
        {
            return false;
        };
    };
    return true;
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

/*
==================================================
Assigns a variable memory address to an ASTNode
==================================================
*/
void Parser::assignVariableMemoryAddress(ASTNode* astNode, std::string variableType)
{
    astNode->variableType = variableType;
    astNode->variableMemoryAddress = this->variableMemoryAddressCounter;
    this->variableMemoryAddressCounter++;
    return;
};

/*
==================================================
Validates if a variable assignment is valid
==================================================
*/
bool Parser::isValidVariableAssignment(std::string variableAssignment, std::string variableType)
{
    if (variableAssignment.length() <= 1)
    {
        return false;
    };
    if (variableAssignment[variableAssignment.length() - 1] != ';')
    {
        return false;
    };
    if (variableType == "INTEGER" || variableType == "CONSTANT INTEGER")
    {
        for (int index = 0; index < variableAssignment.length() - 1; index++)
        {
            if (variableAssignment[index] == '-' && index != 0)
            {
                return false;
            }
            else if (variableAssignment[index] == '-' && index == 0)
            {
                continue;
            }
            else if (!std::isdigit(variableAssignment[index]))
            {
                return false;
            };
        };
    }
    else if (variableType == "DECIMAL" || variableType == "CONSTANT DECIMAL")
    {
        int decimalCount = 0;
        for (int index = 0; index < variableAssignment.length() - 1; index++)
        {
            if (variableAssignment[index] == '-' && index != 0)
            {
                return false;
            }
            else if (variableAssignment[index] == '-' && index == 0)
            {
                continue;
            }
            else if (variableAssignment[index] == '.')
            {
                decimalCount++;
                if (index + 1 < variableAssignment.length() - 1 && !std::isdigit(variableAssignment[index + 1]))
                {
                    return false;
                }
                else if (index + 1 >= variableAssignment.length() - 1)
                {
                    return false;
                };
            }
            else if (!std::isdigit(variableAssignment[index]))
            {
                return false;
            };
        };
        if (decimalCount != 1)
        {
            return false;
        };
    }
    else if (variableType == "CHARACTER" || variableType == "CONSTANT CHARACTER")
    {
        if (variableAssignment[0] == '\'' && variableAssignment[variableAssignment.length() - 2] == '\'' && variableAssignment.length() == 4)
        {
            return true;   
        }
        else
        {
            return false;
        };
    }
    else if (variableType == "BOOLEAN" || variableType == "CONSTANT BOOLEAN")
    {
        if (variableAssignment == "TRUE;" || variableAssignment == "FALSE;")
        {
            return true;
        }
        else
        {
            return false;
        };
    }
    else if (variableType == "STRING" || variableType == "CONSTANT STRING")
    {
        if (variableAssignment[0] != '"' || variableAssignment[variableAssignment.length() - 2] != '"')
        {
            return false;
        };
    }
    else
    {
        return false;
    };
    return true;
};

/*
==================================================
Split codeLine by blank spaces
==================================================
*/
std::vector<std::string> Parser::splitCodeLine(std::string codeLine)
{
    std::istringstream ss(codeLine);
    std::vector<std::string> tokensVec;
    std::string token;
    while (ss >> token)
    {
        tokensVec.push_back(token);
    };
    return tokensVec;
};

/*
==================================================
Checks if variableName already exists
==================================================
*/
bool Parser::checkIfVariableNameExists(std::string variableName)
{
    for (int index = 0; index < this->variableNamesVec.size(); index++)
    {
        if (this->variableNamesVec[index] == variableName)
        {
            return true;
        };
    };
    return false;
};

/*
==================================================
Checks if the code are arithmetic operations
==================================================
*/
bool Parser::checkIfArithmeticOperations(std::string inputString)
{
    std::cout << "checkIfArithmeticOperations:" << inputString << std::endl;
    std::regex re(R"( \+ | \- | \* | \/ )");
    std::sregex_token_iterator it(inputString.begin(), inputString.end(), re, {-1});
    std::sregex_token_iterator end;
    std::vector<std::string> parts(it, end);
    if (parts.size() <= 1)
    {
        return false;
    };
    bool isArithmeticOperation = true;
    for (int index = 0; index < parts.size(); index++)
    {
        std::string variableOrLiteral = parts[index];
        variableOrLiteral.erase(std::remove(variableOrLiteral.begin(), variableOrLiteral.end(), '('), variableOrLiteral.end());
        variableOrLiteral.erase(std::remove(variableOrLiteral.begin(), variableOrLiteral.end(), ')'), variableOrLiteral.end());
        variableOrLiteral.erase(std::remove(variableOrLiteral.begin(), variableOrLiteral.end(), ';'), variableOrLiteral.end());
        variableOrLiteral += ";";
        if (!this->isValidVariableName(variableOrLiteral, true) && !this->isValidVariableAssignment(variableOrLiteral, "INTEGER") && !this->isValidVariableAssignment(variableOrLiteral,"DECIMAL") && !this->isValidVariableAssignment(variableOrLiteral,"STRING"))
        {
            isArithmeticOperation = false;
        };
    };
    return isArithmeticOperation;
};

/*
==================================================
Checks & Returns The Data Type
==================================================
*/
DataType Parser::checkDataType(std::string data)
{
    data += ";";
    DataType dataType = D_NULL;
    if (this->isValidVariableName(data, true))
    {
        dataType = D_VARIABLE;
    }
    else if (this->isValidVariableAssignment(data, "INTEGER"))
    {
        dataType = D_INTEGER;
    }
    else if (this->isValidVariableAssignment(data, "DECIMAL"))
    {
        dataType = D_DECIMAL;
    }
    else if (this->isValidVariableAssignment(data, "CHARACTER"))
    {
        dataType = D_CHARACTER;
    }
    else if (this->isValidVariableAssignment(data, "BOOLEAN"))
    {
        dataType = D_BOOLEAN;
    }
    else if (this->isValidVariableAssignment(data, "STRING"))
    {
        dataType = D_STRING;
    };
    return dataType;
};

/*
==================================================
Creates A New TempVariableStruct
==================================================
*/
void Parser::createNewTempVariableStruct(std::string variableName, std::string variableType)
{
    TempVariableStruct newTempVariableStruct;
    newTempVariableStruct.variableName = variableName;
    newTempVariableStruct.variableType = variableType;
    this->tempVariableStructVec.push_back(newTempVariableStruct);
    return;
};

/*
==================================================
Connects All Dangling ASTNodes
==================================================
*/
void Parser::connectDanglingBranches(ASTNode* currentASTNode, ASTNode* connectorASTNode)
{
    if (currentASTNode == nullptr)
    {
        return;
    };
    if (currentASTNode->sequentialASTNode == nullptr)
    {
        if (currentASTNode != connectorASTNode)
        {
            currentASTNode->sequentialASTNode = connectorASTNode;
        };
    }
    else
    {
        this->connectDanglingBranches(currentASTNode->sequentialASTNode, connectorASTNode);
    };
    if (currentASTNode->controlFlowASTNode == nullptr)
    {
        if (currentASTNode != connectorASTNode)
        {
            currentASTNode->controlFlowASTNode = connectorASTNode;
        };
    }
    else
    {
        this->connectDanglingBranches(currentASTNode->controlFlowASTNode, connectorASTNode);
    };
    return;
};

/*
==================================================
Checks If arrayString Is A Valid Data Extraction For An Array
==================================================
*/
bool Parser::isValidArrayDataExtraction(std::string arrayVariableName, std::string variableType, ASTNode* currentASTNode)
{
    std::regex re(R"([\[\]])");
    std::sregex_token_iterator it(arrayVariableName.begin(), arrayVariableName.end(), re, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> arrayVariableNameVec(it, end);
    if (currentASTNode != nullptr && variableType != "")
    {
        if (arrayVariableNameVec.size() == 3 && arrayVariableNameVec[2] == ";" && this->isValidVariableName(arrayVariableNameVec[0], true))
        {
            bool isAVariable = true;
            bool isValidVariable = false;
            if (this->isValidVariableAssignment(arrayVariableNameVec[1] + ";", "INTEGER"))
            {
                isAVariable = false;
            }
            else if (this->isValidVariableName(arrayVariableNameVec[1], true))
            {
                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                {
                    if (this->tempVariableStructVec[index].variableName == arrayVariableNameVec[1])
                    {
                        if (this->tempVariableStructVec[index].variableType == "INTEGER" || this->tempVariableStructVec[index].variableType == "CONSTANT INTEGER")
                        {
                            isValidVariable = true;
                            break;
                        }
                        else
                        {
                            return false;
                        };
                    };
                };
            };
            if (isAVariable == true && isValidVariable == false)
            {
                return false;
            };
            if (variableType == "INTEGER" || variableType == "CONSTANT INTEGER")
            {
                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                {
                    if (this->tempVariableStructVec[index].variableName == arrayVariableNameVec[0] && this->tempVariableStructVec[index].variableType == "ARRAY INTEGER")
                    {
                        if (isAVariable == false)
                        {
                            currentASTNode->indexToExtractFromArray = std::stoi(arrayVariableNameVec[1]);
                        }
                        else
                        {
                            currentASTNode->variableNameToGetIntegerIndex = arrayVariableNameVec[1];
                        };
                        currentASTNode->arrayVariableNameToExtractDataFrom = arrayVariableNameVec[0];
                        return true;
                    };
                };
            }
            else if (variableType == "DECIMAL" || variableType == "CONSTANT DECIMAL")
            {
                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                {
                    if (this->tempVariableStructVec[index].variableName == arrayVariableNameVec[0] && this->tempVariableStructVec[index].variableType == "ARRAY DECIMAL")
                    {
                        if (isAVariable == false)
                        {
                            currentASTNode->indexToExtractFromArray = std::stoi(arrayVariableNameVec[1]);
                        }
                        else
                        {
                            currentASTNode->variableNameToGetIntegerIndex = arrayVariableNameVec[1];
                        };
                        currentASTNode->arrayVariableNameToExtractDataFrom = arrayVariableNameVec[0];
                        return true;
                    };
                };
            }
            else if (variableType == "CHARACTER" || variableType == "CONSTANT CHARACTER")
            {
                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                {
                    if (this->tempVariableStructVec[index].variableName == arrayVariableNameVec[0] && this->tempVariableStructVec[index].variableType == "ARRAY CHARACTER")
                    {
                        if (isAVariable == false)
                        {
                            currentASTNode->indexToExtractFromArray = std::stoi(arrayVariableNameVec[1]);
                        }
                        else
                        {
                            currentASTNode->variableNameToGetIntegerIndex = arrayVariableNameVec[1];
                        };
                        currentASTNode->arrayVariableNameToExtractDataFrom = arrayVariableNameVec[0];
                        return true;
                    };
                };
            }
            else if (variableType == "BOOLEAN" || variableType == "CONSTANT BOOLEAN")
            {
                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                {
                    if (this->tempVariableStructVec[index].variableName == arrayVariableNameVec[0] && this->tempVariableStructVec[index].variableType == "ARRAY BOOLEAN")
                    {
                        if (isAVariable == false)
                        {
                            currentASTNode->indexToExtractFromArray = std::stoi(arrayVariableNameVec[1]);
                        }
                        else
                        {
                            currentASTNode->variableNameToGetIntegerIndex = arrayVariableNameVec[1];
                        };
                        currentASTNode->arrayVariableNameToExtractDataFrom = arrayVariableNameVec[0];
                        return true;
                    };
                };
            }
            else if (variableType == "STRING" || variableType == "CONSTANT STRING")
            {
                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                {
                    if (this->tempVariableStructVec[index].variableName == arrayVariableNameVec[0] && this->tempVariableStructVec[index].variableType == "ARRAY STRING")
                    {
                        if (isAVariable == false)
                        {
                            currentASTNode->indexToExtractFromArray = std::stoi(arrayVariableNameVec[1]);
                        }
                        else
                        {
                            currentASTNode->variableNameToGetIntegerIndex = arrayVariableNameVec[1];
                        };
                        currentASTNode->arrayVariableNameToExtractDataFrom = arrayVariableNameVec[0];
                        return true;
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
            return false;
        };
    }
    else
    {
        if (arrayVariableNameVec.size() == 3 && arrayVariableNameVec[2] == ";" && this->isValidVariableName(arrayVariableNameVec[0], true))
        {
            bool isAVariable = true;
            bool isValidVariable = false;
            if (this->isValidVariableAssignment(arrayVariableNameVec[1] + ";", "INTEGER"))
            {
                isAVariable = false;
            }
            else if (this->isValidVariableName(arrayVariableNameVec[1], true))
            {
                for (int index = 0; index < this->tempVariableStructVec.size(); index++)
                {
                    if (this->tempVariableStructVec[index].variableName == arrayVariableNameVec[1])
                    {
                        if (this->tempVariableStructVec[index].variableType == "INTEGER" || this->tempVariableStructVec[index].variableType == "CONSTANT INTEGER")
                        {
                            isValidVariable = true;
                            break;
                        }
                        else
                        {
                            return false;
                        };
                    };
                };
            };
            if (isAVariable == true && isValidVariable == false)
            {
                return false;
            };
            return true;
        };
    };
    return false;
};

#endif