//============================================================================
//
//% Student Name 1: student1
//% Student 1 #: 123456781
//% Student 1 userid (email): stu1 (stu1@sfu.ca)
//
//% Student Name 2: student2
//% Student 2 #: 123456782
//% Student 2 userid (email): stu2 (stu2@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put 'None' if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put 'None')__
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources:  ___________
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in tokenClasses.cpp  too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P5_<userid1>_<userid2>" (eg. P5_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit files to courses.cs.sfu.ca
//
// Name        : tokenClassifier.cpp
// Description : Course Project Part 2 (Token Classifier)
// Original portions Copyright (c) 2017 School of Engineering Science, Simon Fraser University
//============================================================================

#include <vector>
#include <sstream>
#include <algorithm>
#include "tokenClasses.hpp"

using namespace std;
//using namespace ensc251;


#define WHITE "\r\t\v\f "
string delim3[] =  {">>=", "<<="};
string delim2[] =  {"+=","-=", "/=", "%=", "<<", ">>", "++", "--"};
string delim1andWhite =  WHITE "/()*%:;=+-~?";
string white = WHITE;

bool processDigits(string line, string::size_type &index)
{
	bool DigiFound=false;
	while (index < line.length() && isdigit(line.at(index)))
		{
			index++;
			DigiFound=true;
		}
	return DigiFound;
}

// Construct token objects categorized into appropriate types
// Input: a stream
// Output: a vector containing the list of Token objects properly categorized
std::vector<ensc251::Token*> tokenClassifier(istream& code)
{
        std::vector<ensc251::Token*> tokenObjectPs;
        string token;
		string line;
		int lineNumber = -1;
		bool intCheck = true;
		bool floatCheck = false;
		bool pointerCheck = false;
		while (getline (code, line)) {
			lineNumber++;
			if (line.at(0) == '#')
				continue;

			string::size_type index2 = 0;
			auto length = line.length();

			while(index2 < length ) {
				auto index1 = line.find_first_not_of(white, index2);
				if (index1 == string::npos)
					break;

				/// Dealing with int and float consts
				if (isdigit(line.at(index1)) || line.at(index1) == '.')
				{
					index2 = index1;

					// integer consts
					bool preDigitFound = processDigits(line, index2);

					// floats
					if (index2 < length && line.at(index2) == '.')
					{
						index2++;
						bool postDigitFound = processDigits(line, index2);

						if(!preDigitFound && !postDigitFound) // detecting the single "." as an invalid token!
						{
							token = line.substr(index1, index2 - index1);
							cout << "Error -- invalid token in input: " << token << endl;
							tokenObjectPs.push_back(new ensc251::incorrect(token, lineNumber, index1));
							continue;
						}
					}

					// exponentials
					if (index2 < length && (line.at(index2) == 'e' || line.at(index2) == 'E'))
					{
						if(line.at(index2+1) == '+' || line.at(index2+1) == '-')
							index2+=2;
						else
							index2++;

						if(!processDigits(line, index2)) // invalid float -> roll back ...
						{
							if(line.at(index2-1) == 'e' || line.at(index2-1) == 'E')
								index2--;
							else
								index2-=2;
						}

					}

					// suffix f and F
					if (index2 < length && (line.at(index2) == 'f' || line.at(index2) == 'F'))
						index2++;

					auto subs = line.substr(index1, index2 - index1);
					if(subs.find_first_of(".eE") != string::npos)
						tokenObjectPs.push_back(new ensc251::numeric_const<float>(stof(subs), lineNumber, index1));
					else
						tokenObjectPs.push_back(new ensc251::numeric_int_const<int>(stoi(subs), lineNumber, index1));
					continue;
				}

				for (auto delim : delim3) {
					if (line.substr(index1, 3) == delim) {
						tokenObjectPs.push_back(new ensc251::int_assignment_operator(delim, lineNumber,index1));
						index2 = index1 + 3;
						break;
					}
				}
				if (index2 > index1) continue;

				// the below follows the pattern from the lines above.
				for (auto delim : delim2) {
					if (line.substr(index1, 2) == delim) {
						if(delim == "%=")
							tokenObjectPs.push_back(new ensc251::int_assignment_operator(delim, lineNumber, index1));
						else if(delim == ">>" || delim == "<<" )
							tokenObjectPs.push_back(new ensc251::shift_operator(delim, lineNumber, index1));
						else if(delim == "/=" || delim == "+=" || delim =="-=")
							tokenObjectPs.push_back(new ensc251::gen_assignment_operator(delim, lineNumber, index1));
						else if(delim == "++" || delim == "--")
							tokenObjectPs.push_back(new ensc251::postfix_operator(delim, lineNumber, index1));
						index2 = index1 + 2;
						break;
					}
				}
				if (index2 > index1) continue;

				if (line[index1] == '"') {
					index2 = index1 + 1;
					while (index2 < length) {
						if (line[index2] == '\\') {
							index2++; // skip over escape character
						}
						else
							if (line.at(index2) == '"') {
								/*token = line.substr(index1, index2 + 1 - index1);
								tokenObjectPs.push_back(new ensc251::incorrect(token, lineNumber, index1));*/
								break;
							}
						index2++;
					}
					if (index2 >= length) { // String is not terminated!
						cout << "Error:  string is not terminated!" << endl;
						token = line.substr(index1, index2 - index1);
						tokenObjectPs.push_back(new ensc251::incorrect(token, lineNumber, index1));
						break;
					}
					index2++; // non-empty string was pushed on the result vector
					token = line.substr(index1, index2 - index1);
					tokenObjectPs.push_back(new ensc251::string(token, lineNumber, index1));
					continue;
				}

				if (line[index1] == '\'') {
					index2 = index1 + 1;
					if (index2 < length && line[index2] == '\\') {
						index2++; // skip over escape character
					}
					index2++; // skip over character
					if (index2 >= length || line.at(index2) != '\'') // Character constant is not terminated!
					{
						cout << "Error:  character constant is not terminated!" << endl;
						token = line.substr(index1, index2 - index1);
						tokenObjectPs.push_back(new ensc251::incorrect(token, lineNumber, index1));
					}
					else {
						index2++;
						token = line.substr(index1, index2 - index1);
						tokenObjectPs.push_back(new ensc251::numeric_int_const<char>(token[1], lineNumber, index1));
					}
					continue;
				}

				if(line[index1]=='_' || isalpha(line[index1])) {
					index2 = index1 + 1;
					while (index2 < length && (line[index2]=='_' || isalnum(line[index2])))
						index2 ++;

					token = line.substr(index1, index2 - index1);


					if(token == "char" || token == "int"){
						intCheck = true;
						floatCheck = false;
						pointerCheck = false;
						tokenObjectPs.push_back(new ensc251::type_spec(token, lineNumber, index1));
					}
					else if(token == "float"){
						intCheck = false;
						floatCheck = true;
						pointerCheck = false;
						tokenObjectPs.push_back(new ensc251::type_spec(token, lineNumber, index1));
					}
					else if(intCheck)
						tokenObjectPs.push_back(new ensc251::int_id(token, lineNumber, index1));
					else if(floatCheck)
						tokenObjectPs.push_back(new ensc251::numeric_id(token, lineNumber, index1));
					else if(pointerCheck)
						tokenObjectPs.push_back(new ensc251::pointer_id(token, lineNumber, index1));
					continue;
				}

				index2 = line.find_first_of(delim1andWhite, index1);
				if (index2 == index1) {
					index2++; // we found a 1-character token
					token = line.substr(index1,1);
					char charToken = line[index1];
					switch(charToken){
						case '/': tokenObjectPs.push_back(new ensc251::div_operator(token, lineNumber, index1)); break;
						case '+':
						case '-': tokenObjectPs.push_back(new ensc251::additive_operator(token, lineNumber, index1)); break;
						case '~': tokenObjectPs.push_back(new ensc251::comp_operator(token, lineNumber, index1)); break;
						case '%': tokenObjectPs.push_back(new ensc251::mod_operator(token, lineNumber, index1)); break;
						case '?': tokenObjectPs.push_back(new ensc251::conditional_operator(token, lineNumber, index1)); break;
						case '=': tokenObjectPs.push_back(new ensc251::gen_assignment_operator(token, lineNumber, index1)); break;
						case ';':
						case ':':
						case '(':
						case ')': tokenObjectPs.push_back(new ensc251::punctuator(token, lineNumber, index1)); break;
						case '*': {
							pointerCheck = true;
							intCheck = false;
							floatCheck = false;
							tokenObjectPs.push_back(new ensc251::punctuator(token, lineNumber, index1));
							break;
						}

					}

					continue;
				}
				token = line.substr(index1, index2 - index1);
				cout << "Error -- invalid token in input: " << token << endl;
				tokenObjectPs.push_back(new ensc251::incorrect(token, lineNumber, index1));
			}
		}
	//TODO: Write Code for classifying each token into an object of the proper class
	// store a pointer to the object in vector tokenObjectPs

	// For example, if you want to classify the number '10' in the input "errno = 10+2.5;":
	// where the 2nd argument is the line Number and the 3rd argument is the
	//    indexNumber (the position of the start of the token in that line).

	return tokenObjectPs;
}

