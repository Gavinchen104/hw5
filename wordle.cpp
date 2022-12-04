// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

//split "in" into a string and a vector
void index(const std::string& in, size_t in_size, size_t m, std::vector<size_t>& number, std::string& alphabet);

//create a set that fulfills the requirement of "floating"
std::string word_generator1(const std::string& floating, 
																		size_t floating_size, 
																		size_t in_size,
																		const std::string& word,
																		std::set<std::string>& temp_answer,
																		size_t index);


//create a set that fulfills the requirement of "in"																		
std::string word_generator2(const std::string& in, 
																		size_t in_size, 
																		const std::set<std::string>& dict,
																		size_t index,
																		const std::string& word,
																		std::vector<size_t>& number, 
																		std::string& alphabet);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{   
    //find the length of "in";
    size_t in_size = in.length();

    //find the lenght of "floating"
    size_t floating_size = floating.length();

		//create a vector that stores the index of the characters of" in"
		std::vector<size_t> number;
		//create a string that store the characters of "in"
		std::string alphabet;
		//split "in" into a vector and a stirng
		index(in, in_size, 0, number, alphabet);
	
		// set that stores the words that satisfies floating
		std::set<std::string> temp_answer;
		// set that stores final answer
		std::set<std::string> answer;

		std::set<std::string>::iterator it;

		//create the set that satisfies floating
		for(it = dict.begin(); it!= dict.end(); ++it)
		{
			std::string temp_ = word_generator2(in, in_size, dict, 0, *it, number, alphabet);
			//checking if the word has capital letters
			int count = 0;
			//make sure the string is modified
			if(temp_ != "2"){
				for(unsigned int i = 0; i < temp_.length(); i++){
					if(isupper(temp_[i])) count++;
				}
				//checking if the word has capital letters
				if (count ==0) temp_answer.insert(temp_);
			}
		}
	

		//create the final set 
		for(it = temp_answer.begin(); it!= temp_answer.end();++it)
		{
			std::string _temp = word_generator1(floating, floating_size,in_size, *it, temp_answer, 0);
			//if the returning string is modified, insert into the set
			if(_temp != "1")answer.insert(_temp);
		}
		return answer;
}

// Define any helper functions here

//find the set of word that contains floating
std::string word_generator1(const std::string& floating, 
																		size_t floating_size,
																		size_t in_size, 
																		const std::string&word,
																		std::set<std::string>& temp_answer,
																		size_t index)
{
		//initialize the string
		std::string temp = "1";

		//base case 
		if (floating_size ==0) return word;

		//check if the word contains floating characters 
		if (word.find(floating[index]) < in_size  ){
			
			//if it is the first index
			if(index ==0)
			{
				//when reached the end of the floating
				if(index +1 == floating_size)
				{
					//set the temp to word
					temp = word;
					return temp;
				}
				//otherwise recurse
				temp = word_generator1(floating, floating_size,in_size, word, temp_answer, ++index);
			}
			//if it is the not the first index
			else if(index!=0){
				//if there are repeated characters in floating
				if(floating[index] == floating[0]){
					// if in the word, the two repeated characters are not in the same index
					if(word.rfind(floating[0])!=word.find(floating[0])){
						//when reached the end of the floating
						if(index +1 == floating_size)
						{
							//set the temp to word
							temp = word;
							return temp;
						}
						//othewise recurse
						temp = word_generator1(floating, floating_size,in_size, word, temp_answer, ++index);
					}
				}
				//if there are no repeated charactes in floating
				else if(floating[index] != floating[0]){
						//when reached the end of the floating
						if(index +1 == floating_size)
						{
							//set the temp to word
							temp = word;
							return temp;
						}
						//otherwise recurse
						temp = word_generator1(floating, floating_size,in_size, word, temp_answer, ++index);
				}
			}
		}
		return temp;
}

void index(const std::string& in, size_t in_size, size_t m, std::vector<size_t>& number, std::string& alphabet)
{
	if(in_size == m) return;
	//if the character in "in" is not '-
	if(in[m]!='-'){
		//add to the number vector and alphabet string
		number.push_back(m);
		alphabet.push_back(in[m]);
	}
	//recurse
	index(in, in_size, ++m, number, alphabet);
	return;
}



//find the set of word that also contains in
std::string word_generator2(const std::string& in, 
																		size_t in_size, 
																		const std::set<std::string>& dict,
																		size_t index,
																		const std::string& word,
																		std::vector<size_t>& number,
																		std::string& alphabet)
{
	std::string temp1 = "2";
	//make sure the word has the same length as "in"
	if(in_size == word.length())
	{
		//base case
		if(number.size() == 0) return word;
		

		size_t loc = word.find(alphabet[index]);
		size_t loc_ = word.rfind(alphabet[index]);
		size_t loc_number = number[index]; 
		//if there are no repeated characters in "in"
		if(loc == loc_){
			//if the index matches to the number vector
			if(loc == loc_number){
				if(index +1 == number.size())
				{
					temp1 = word;
					return temp1;
				}
				temp1 = word_generator2(in, in_size, dict, index+1, word, number, alphabet);
			}
		}
		//if there are repeated characters in "in"
		else if(loc != loc_ && loc_< in_size){
			//if the index matches to the number vector
			if(loc_ ==loc_number || loc == loc_number){
				if(index +1 == number.size())
				{
					temp1 = word;
					return temp1;
				}
				temp1 = word_generator2(in, in_size, dict, index+1, word, number, alphabet);
			}
		}
	}
	return temp1;
}																	




