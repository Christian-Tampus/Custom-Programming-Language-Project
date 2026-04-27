/* UPDATE VERSION [29] */

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
    int integer;
    double decimal;
    char character;
    bool boolean;
    std::string string;
    bool isConstant;
    int variableMemoryAddress;
    std::string variableType;
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
        std::vector<std::string> tokensVec;
        bool parsedSuccessfully;
        std::string errorString;
        std::string trimString(std::string inputString);
        bool buildAST(std::string codeLine, Command commandType, ASTNode* currentASTNode);
        void buildASTHelper(std::string currentCodeLine, Command commandType, ASTNode* currentASTNode, bool& hasInvalidSyntax, int& syntaxErrorLine, int index);
        bool isValidVariableName(std::string variableName, bool isBeingAssigned);
        void assignVariableMemoryAddress(ASTNode* astNode, std::string variableType);
        bool isValidVariableAssignment(std::string variableAssignment, std::string variableType);
        std::vector<std::string> splitCodeLine(std::string codeLine);
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
    if (this->currentASTNode != nullptr)
    {
        delete this->currentASTNode;
    };
    this->currentASTNode = nullptr;
    this->tokensVec.clear();
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
    newASTNode->command = commandType;
    newASTNode->branchIndex = 0;
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
    }
    else if (commandType == C_INTEGER)
    {
        std::vector<std::string> integerTokensVec = this->splitCodeLine(codeLine);
        for (int index = 0; index < integerTokensVec.size(); index++)
        {
            std::cout << "integerTokensVec: " << integerTokensVec[index] << std::endl;
        };
        if (integerTokensVec.size() == 2)
        {
            if (integerTokensVec[0] == "INTEGER" && this->isValidVariableName(integerTokensVec[1], false))
            {
                this->assignVariableMemoryAddress(newASTNode, "INTEGER");
            }
            else
            {
                return false;
            };
        }
        else if (integerTokensVec.size() == 4)
        {
            if (integerTokensVec[0] == "INTEGER" && this->isValidVariableName(integerTokensVec[1], true) && integerTokensVec[2] == "=" && this->isValidVariableAssignment(integerTokensVec[3], "INTEGER"))
            {
                this->assignVariableMemoryAddress(newASTNode, "INTEGER");
                std::string extractVariable = integerTokensVec[3].substr(0, integerTokensVec[3].length() - 1);
                newASTNode->integer = std::stoi(extractVariable);
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
            if (decimalTokensVec[0] == "DECIMAL" && this->isValidVariableName(decimalTokensVec[1], false))
            {
                this->assignVariableMemoryAddress(newASTNode, "DECIMAL");
            }
            else
            {
                return false;
            };
        }
        else if (decimalTokensVec.size() == 4)
        {
            if (decimalTokensVec[0] == "DECIMAL" && this->isValidVariableName(decimalTokensVec[1], true) && decimalTokensVec[2] == "=" && this->isValidVariableAssignment(decimalTokensVec[3], "DECIMAL"))
            {
                this->assignVariableMemoryAddress(newASTNode, "DECIMAL");
                std::string extractVariable = decimalTokensVec[3].substr(0, decimalTokensVec[3].length() - 1);
                newASTNode->decimal = std::stod(extractVariable);
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
            if (characterTokensVec[0] == "CHARACTER" && this->isValidVariableName(characterTokensVec[1], false))
            {
                this->assignVariableMemoryAddress(newASTNode, "CHARACTER");
            }
            else
            {
                return false;
            };
        }
        else if (characterTokensVec.size() == 4)
        {
            if (characterTokensVec[0] == "CHARACTER" && this->isValidVariableName(characterTokensVec[1], true) && characterTokensVec[2] == "=" && this->isValidVariableAssignment(characterTokensVec[3], "CHARACTER"))
            {
                this->assignVariableMemoryAddress(newASTNode, "CHARACTER");
                newASTNode->character = characterTokensVec[3][1];
            }
            else
            {
                return false;
            };
        }
        else if (characterTokensVec.size() == 5)
        {
            if (characterTokensVec[0] == "CHARACTER" && this->isValidVariableName(characterTokensVec[1], true) && characterTokensVec[2] == "=" && characterTokensVec[3] == "'" && characterTokensVec[4] == "';")
            {
                this->assignVariableMemoryAddress(newASTNode, "CHARACTER");
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
            if (booleanTokensVec[0] == "BOOLEAN" && this->isValidVariableName(booleanTokensVec[1], false))
            {
                this->assignVariableMemoryAddress(newASTNode, "BOOLEAN");
            }
            else
            {
                return false;
            };
        }
        else if (booleanTokensVec.size() == 4)
        {
            if (booleanTokensVec[0] == "BOOLEAN" && this->isValidVariableName(booleanTokensVec[1], true) && booleanTokensVec[2] == "=" && this->isValidVariableAssignment(booleanTokensVec[3],  "BOOLEAN"))
            {
                this->assignVariableMemoryAddress(newASTNode, "BOOLEAN");
                newASTNode->boolean = (booleanTokensVec[3] == "TRUE;" ? true : false);
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
            if (stringTokensVec[0] == "STRING" && this->isValidVariableName(stringTokensVec[1], false))
            {
                this->assignVariableMemoryAddress(newASTNode, "STRING");
            }
            else
            {
                return false;
            };
        }
        else if (stringTokensVec.size() >= 4)
        {
            if (stringTokensVec[0] == "STRING" && this->isValidVariableName(stringTokensVec[1], true) && stringTokensVec[2] == "=")
            {
                std::string fullString = stringTokensVec[3];
                for (int index = 4; index < stringTokensVec.size(); index++)
                {
                    fullString += " " + stringTokensVec[index];
                };
                if (fullString[0] == '"' && fullString[fullString.length() - 2] == '"' && fullString[fullString.length() - 1] == ';')
                {
                    this->assignVariableMemoryAddress(newASTNode, "STRING");
                    newASTNode->string = fullString.substr(1, fullString.size() - 3);
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
    if (variableName.length() == 0)
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
    if (variableType == "INTEGER")
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
    else if (variableType == "DECIMAL")
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
    else if (variableType == "CHARACTER")
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
    else if (variableType == "BOOLEAN")
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
    else if (variableType == "STRING")
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
#endif