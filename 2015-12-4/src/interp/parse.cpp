#include "parse.hpp"

namespace parse
{

  list<string> tokenize(const string& input)
  {
    list<string> tokens;
    string_size i = 0;
    while (i != input.size()) {				

      // skip whitespace
      while (i != input.size() && isspace(input[i])) 
	i++;
      
      // if it's a paren, add it as a token
      if (isparen(input[i])) {
	tokens.push_back(input.substr(i,1));
	i++;
	continue;
      }

      // otherwise go until it's a paren or whitespace
      string_size j = i;
      while (j != input.size() && !isspace(input[j]) && !isparen(input[j]))
	j++;
      
      if (i != j) {
	tokens.push_back(input.substr(i, j - i));
	i = j;
      }
    }
    return tokens;
  }

  bool isparen(const char& c)
  {
    return c == '(' || c == ')';
  }


}
