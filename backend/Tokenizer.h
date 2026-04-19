/* UPDATE VERSION [22] */

#ifndef H_TOKENIZER
#define H_TOKENIZER

/*
==================================================
Dependencies
==================================================
*/
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

/*
==================================================
Class Declaration
==================================================
*/
class Tokenizer
{
    private:
        std::string code;
        std::vector<std::string> tokensVec;
    public:
        Tokenizer();
        Tokenizer(std::string code);
        ~Tokenizer();
        void tokenize();
        void insertCode(std::string code);
        std::vector<std::string> getTokens();
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
Tokenizer::Tokenizer()
{
    this->code = "";
};

/*
==================================================
Tokenizer(std::string code) Constructor
==================================================
*/
Tokenizer::Tokenizer(std::string code)
{
    this->code = code;
    std::cout << "[TOKENIZER] code:\n" << code << std::endl;
};

/*
==================================================
Destructor
==================================================
*/
Tokenizer::~Tokenizer()
{
    this->code = "";
    this->tokensVec.clear();
};

/*
==================================================
Splits Code Into Line Tokens
==================================================
*/
void Tokenizer::tokenize()
{
    if (this->code.length() > 0)
    {
        this->tokensVec.clear();
        std::string tempCode = this->code;
        std::stringstream ss(tempCode);
        std::string line;
        while (std::getline(ss, line))
        {
            this->tokensVec.push_back(line);
        };
        std::cout << "[TOKENIZER] tokensVec Tokens:" << std::endl;
        for (int index = 0; index < this->tokensVec.size(); index++)
        {
            std::cout << "[TOKENIZER] " << this->tokensVec[index] << std::endl;
        };
        std::cout << "[TOKENIZER] tokensVec Size: " << this->tokensVec.size() << std::endl;
    }
    else
    {
        std::cout << "[TOKENIZER] No Code To Tokenize!" << std::endl;
    };
    return;
};

/*
==================================================
Inserts Code Into The Class
==================================================
*/
void Tokenizer::insertCode(std::string code)
{
    this->code = code;
    return;
};

/*
==================================================
Returns A Vector Of String Tokens
==================================================
*/
std::vector<std::string> Tokenizer::getTokens()
{
    return this->tokensVec;
};

#endif