/*Author: Brandon Van Philips
  Date: 3/29/16
  Title: Project Goid Engrish
  Discription: Its a Spell checker
  File Name: vanPhilips.cpp
  Honor statement: I have neither given nor received any unauthorized help on this assignment.
  Resources: 
	http://computing.southern.edu/halterman/Courses/Winter2017/318/Assignments/spellcheck.html
	http://stackoverflow.com/questions/23418390/how-to-convert-a-c-string-to-uppercase
*/


#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>


class Hashtable
{
public:
	Hashtable(std::string address, int tableSize);
	std::vector<std::string> allOfHumanUnderstanding; //all of the words imported from the doc

	std::vector<std::string> corrections;
	int size();

	int hash(std::string& lookUp, std::vector<std::string>& list);
	bool insert(std::string& word, std::vector<std::string>& list);
	bool contains(std::string& word, std::vector<std::string>& list);
	bool remove(std::string& word, std::vector<std::string>& list);

	void omitLetter(std::string& word);
	void addLetter(std::string& word);
	void replaceLetter(std::string& word);
	void transposeLetters(std::string& word);
	void addSpace(std::string& word);
private:
	int tableSize;
	std::string listAddress;
	std::string alphabet;
};

Hashtable::Hashtable(std::string address, int tableSize)
{
	this->tableSize = tableSize;
	this->corrections = {};
	this->alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	std::ifstream fin(address); //  Open input stream from file
	//  Do (or continue to do) loop while the stream has data
	while (fin)
	{
		std::string word;
		fin >> word; //  Read a word
		if (fin)
		{
			//  Ensure all letters are uppercase
			transform(begin(word), end(word), begin(word), toupper);
			//  Add word to the list
			this->allOfHumanUnderstanding.push_back(word);
		}
	}
}

int Hashtable::size()
{
	return this->tableSize;
}

int Hashtable::hash(std::string& lookUp, std::vector<std::string>& list)
{
	for (auto i = 0; i < list.size(); i++)
	{
		if (list[i] == lookUp)
		{
			return i;
		}
	}
	return -1;
}

bool Hashtable::insert(std::string& word, std::vector<std::string>& list) //tested
{
	if (!contains(word, list))
	{
		list.push_back(word);
		return true;
	}
	return false;
}

//checks to see if word is contained in chosen vector
bool Hashtable::contains(std::string& word, std::vector<std::string>& list)
{
	return (find(list.begin(), list.end(), word) != list.end());
}

//removes a word from the chosen vector, returns true if sucessful
bool Hashtable::remove(std::string& word, std::vector<std::string>& list)
{
	auto place = find(list.begin(), list.end(), word);
	if (place == list.end())
	{
		return false;
	}
	list.erase(place);
	return true;
}

//omits a letter from the current word to see if that is a real word
void Hashtable::omitLetter(std::string& word)
{
	for (auto i = 0; i < word.size(); i++)
	{
		auto workingWord = word;
		workingWord.erase(workingWord.begin() + i);
		auto place = find(allOfHumanUnderstanding.begin(), allOfHumanUnderstanding.end(), workingWord);
		if ((place != allOfHumanUnderstanding.end()) || (workingWord == *(allOfHumanUnderstanding.end() - 1)))
		{
			insert(workingWord, corrections);
		}
	}
}

//adds a letter in various places in the word to see if that is a word
void Hashtable::addLetter(std::string& word) //tested
{
	for (auto i = 0; i < word.size() + 1; i++)
	{
		auto workingWord = word;
		for (auto j = 0; j < alphabet.size() + 1; j++)
		{
			workingWord = word;
			workingWord.insert(i, 1, alphabet[j]);
			auto place = find(allOfHumanUnderstanding.begin(), allOfHumanUnderstanding.end(), workingWord);
			if ((place != allOfHumanUnderstanding.end()) || (workingWord == *(allOfHumanUnderstanding.end() - 1)))
			{
				insert(workingWord, corrections);
			}
		}
	}
}

//goes through the word and replaces the selected letter with every letter in the alphabet and checks if its a word
void Hashtable::replaceLetter(std::string& word)
{
	for (auto i = 0; i < word.size() + 1; i++)
	{
		auto workingWord = word;
		for (auto j = 0; j < alphabet.size(); j++)
		{
			workingWord[i] = alphabet[j];
			auto place = find(allOfHumanUnderstanding.begin(), allOfHumanUnderstanding.end(), workingWord);
			if ((place != allOfHumanUnderstanding.end()) || (workingWord == *(allOfHumanUnderstanding.end() - 1)))
			{
				insert(workingWord, corrections);
			}
		}
	}
}

//Moves through the words and swaps letters next to each other and checks if its a word
void Hashtable::transposeLetters(std::string& word) //fixed with find function
{
	for (auto i = 0; i < word.size() - 1; i++)
	{
		auto workingWord = word;
		auto firstLetter = workingWord[i];
		auto secondLetter = workingWord[i + 1];
		workingWord[i] = secondLetter;
		workingWord[i + 1] = firstLetter;

		auto place = find(allOfHumanUnderstanding.begin(), allOfHumanUnderstanding.end(), workingWord);
		if (place != allOfHumanUnderstanding.end() || (*(allOfHumanUnderstanding.end() - 1) == workingWord))
		{
			insert(workingWord, corrections);
		}
	}
}

//adds a space to the word to see if it is actually multiple words together
void Hashtable::addSpace(std::string& word)
{
	for (auto i = 0; i < word.size(); i++)
	{
		auto workingWord = word;
		workingWord.insert(i, 1, ' ');
		auto firstWord = workingWord.substr(0, workingWord.find(' '));
		auto secondWord = workingWord.substr(workingWord.find(' ') + 1, workingWord.size() - workingWord.find(' '));
		auto foundWordOne = false;
		auto foundWOrdTwo = false;

		auto place = find(allOfHumanUnderstanding.begin(), allOfHumanUnderstanding.end(), firstWord);

		if (place != allOfHumanUnderstanding.end() || (*(allOfHumanUnderstanding.end() - 1) == workingWord))
		{
			foundWordOne = true;
		}

		place = find(allOfHumanUnderstanding.begin(), allOfHumanUnderstanding.end(), secondWord);

		if ((place != allOfHumanUnderstanding.end()) || (*(allOfHumanUnderstanding.end() - 1) == workingWord))
		{
			foundWOrdTwo = true;


			if (foundWordOne & foundWOrdTwo)
			{
				insert(workingWord, corrections);
			}
		}
	}
}


int main()
{
	Hashtable table("wordsEn.txt", 109582);
	std::string input = "";

	do
	{
		std::cout << "Enter a word: ";
		std::cin >> input;
		transform(input.begin(), input.end(), input.begin(), toupper);
		if (input == ".")
			break;

		table.addLetter(input);
		table.omitLetter(input);
		table.replaceLetter(input);
		table.transposeLetters(input);
		table.addSpace(input);


		sort(table.corrections.begin(), table.corrections.end());
		for (auto i : table.corrections)
			std::cout << i << std::endl;
		table.corrections = {};
	}
	while (input != ".");
}
