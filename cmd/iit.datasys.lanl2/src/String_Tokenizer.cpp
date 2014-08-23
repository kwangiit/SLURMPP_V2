#include "String_Tokenizer.h"
using std::string;

/** Position start and end so that start is the index of the start
 of the next token and end is the end.
 */
void String_Tokenizer::find_next() {
	// Find the first character that is not a delimiter.
	start = the_source.find_first_not_of(the_delim, end);
	// Find the next delimiter.
	end = the_source.find_first_of(the_delim, start);
}

/** Determine if there are more tokens.
 @return true if there are more tokens
 */
bool String_Tokenizer::has_more_tokens() {
	return start != string::npos;
}

/** Retrieve the next token. 
 @return The next token. If there are no more
 tokens, an empty string is returned
 */
string String_Tokenizer::next_token() {
	// Make sure there is a next token
	if (!has_more_tokens())
		return "";
	// Save the next token.
	string token = the_source.substr(start, end - start);
	// Find the following token.
	find_next();
	// Return the next token.
	return token;
}
