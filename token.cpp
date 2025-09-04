#include "header.h"

char* trim(char* str) 
{
    
    char* end;

    // trim  space leading
    while (isspace((unsigned char)*str)) 
    {
        str++;
    }
    if (*str == 0)
        {
            return str;
        }

    // trim  space trailing
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) 
    {
        end--;
    }

    *(end + 1) = 0;

    return str;
    
}

// tokenizer
vector<char*> token(char* str, const char* delimiter) 
{
    vector<char*> tokens;        // store tokens
    bool in_quotes = false;      // flag to track if quotes
    string current;              // temp string 

    // go through each char input string
    for (int i = 0; str[i] != '\0'; i++)
    {
        char c = str[i];

        //  double quote
        if (c == '"')
        {
            in_quotes = !in_quotes;
        } 

        // char is whitespace
        else if (!in_quotes && isspace(c))
        {
            
            //token is non-empty
            if (!current.empty())   
            {
                char* tok = strdup(current.c_str());
                tokens.push_back(tok);               // add token
                current.clear();                     
            }
        } 

        else
        {
            // append char to token
            current.push_back(c);
        }
    }

    // add other remaining 
    if (!current.empty())
    {
        char* tok = strdup(current.c_str());
        tokens.push_back(tok);
    }

    tokens.push_back(nullptr);
    return tokens;
}
