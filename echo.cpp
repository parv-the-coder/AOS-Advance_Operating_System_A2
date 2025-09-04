#include "header.h"

// Function to mimic shell echo command with space normalization
void echo(char input[4096])
{
    string s(input);

    // Step 1: Trim leading spaces manually
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t'))
    {
        start++;
    }

    // Step 2: Trim trailing spaces manually
    size_t end = s.size();
    while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t'))
    {
        end--;
    }

    // Extract trimmed string
    s = s.substr(start, end - start);

    // Step 3: Remove "echo" prefix if present
    if (s.rfind("echo", 0) == 0)
    {
        s.erase(0, 4); // remove "echo"

        // Trim again after removing "echo"
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

    // Step 4: Remove surrounding double quotes if present
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
    {
        s = s.substr(1, s.size() - 2);
    }

    // Step 5: Collapse multiple spaces between words
    stringstream ss(s);
    string word;
    vector<string> words;

    while (ss >> word)
    {
        words.push_back(word);
    }

    // Reconstruct the string with a single space
    string result;
    for (size_t i = 0; i < words.size(); i++)
    {
        result += words[i];
        if (i != words.size() - 1)
        {
            result += " ";
        }
    }

    // Step 6: Print final output
    cout << result << "\n";
}
