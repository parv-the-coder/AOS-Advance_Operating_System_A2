#include "header.h"

char* trim(char* str) 
{
    
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) 
    {
        str++;
    }
    if (*str == 0)  // All spaces?
        {
            return str;
        }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) 
    {
        end--;
    }

    // Write new null terminator
    *(end + 1) = 0;

    return str;
    
}


vector<char*> token(char* str, const char* delimiter) {
    vector<char*> tokens;
    bool in_quotes = false;
    string current;

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c == '"') {
            in_quotes = !in_quotes; // toggle state
        } 
        else if (!in_quotes && isspace(c)) {
            if (!current.empty()) {
                char* tok = strdup(current.c_str());
                tokens.push_back(tok);
                current.clear();
            }
        } 
        else {
            current.push_back(c);
        }
    }

    if (!current.empty()) {
        char* tok = strdup(current.c_str());
        tokens.push_back(tok);
    }

    tokens.push_back(nullptr);
    return tokens;
}
