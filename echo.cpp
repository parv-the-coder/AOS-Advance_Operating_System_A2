#include "header.h"

// echo comm
void echo(char input[4096])
{
    string s(input);

    // trim spaces leading
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t'))
    {
        start++;
    }

    // trim  spaces trailing
    size_t end = s.size();
    while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t'))
    {
        end--;
    }

    // extract trimmed string
    s = s.substr(start, end - start);

    // removing echo
    if (s.rfind("echo", 0) == 0)
    {
        s.erase(0, 4);

        // trimming again
        start = 0;
        while (start < s.size() && (s[start] == ' ' || s[start] == '\t'))
        {
            start++;
        }

        end = s.size();

        while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t'))
        {
            end--;
        }

        s = s.substr(start, end - start);
    }

    // removing double quotes if present
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
    {
        s = s.substr(1, s.size() - 2);
    }

    // removing multiple spaces between words
    stringstream ss(s);
    string word;
    vector<string> words;

    while (ss >> word)
    {
        words.push_back(word);
    }

    // making the string with a single space
    string result;
    for (size_t i = 0; i < words.size(); i++)
    {
        result += words[i];
        
        if (i != words.size() - 1)
        {
            result += " ";
        }
    }

    cout << result << "\n";
}
