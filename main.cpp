// Ephimoon - 2024.3.19 - 2024.3.20
// got wordle-allowed-guesses-12546.txt" from 	https://gist.github.com/cfreshman/d5fb56316158a1575898bba1eed3b5da
// got wordle-answers-2309.txt from 			https://gist.github.com/cfreshman/a7b776506c73284511034e63af1017ee

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
//#include <unordered_set>
using namespace std;

// // Function to set text color
string setColor(string color) {
	return "\033[" + color + "m";
}

bool isValidWord(vector<string>& words, int numGuessed, string guessedWords[], string word){
	// check if the word is 5 letters
	if(word.length() != 5){
		cout << "invalid guess... " << word << " is not 5 letters" << endl << endl;
		return false;
	}
	// Check if the word has been guessed before
	for(int i = 0; i < numGuessed; i++){
		if(guessedWords[i] == word){
			cout << "invalid guess... " << word << " is already guessed" << endl;
			return false;
		}
	}
	// Check if the word is in the list of words
	for(int i = 0; i < words.size(); i++){
		if(words[i] == word){
			return true;
		}
	}
	cout << "invalid guess... " << word << " NOT in dictionary" << endl << endl;
	return false;
}

// print to share (each)
string share(string wordle, string userWord){
	string share = "";
	for(int i = 0; i < wordle.length(); i++){
		if(userWord[i] == wordle[i]){
			share += "🟩";
		}
		else{
			bool inWord = false;
			for(int j = 0; j < wordle.length(); j++){
				if(userWord[i] == wordle[j]){
					inWord = true;
					break;
				}
			}
			if(inWord){
				share += "🟨";
			}
			else{
				share += "⬜️";
			}
		}
	}
	return share;
}

// print the guessed word in certain format
string printGuess(string wordle, string userWord){
	string output = "\t| ";
	for(int i = 0; i < wordle.length(); i++){
		if(userWord[i] == wordle[i]){
			output += setColor("32");
		}
		else{
			bool inWord = false;
			for(int j = 0; j < wordle.length(); j++){
				if(userWord[i] == wordle[j]){
					inWord = true;
					break;
				}
			}
			if(inWord){
				output += setColor("33");
			}
			else{
				output += setColor("90");
			}
		}
		output += toupper(userWord[i]);
		output += setColor("0");
		output += " | ";
	}
	return output;
}

// Function to print the keyboard with highlighted letters
void printKeyboard(string& keyboard, string wordle, string userWord, bool isOperating){
	if(isOperating){
		// Highlight letters based on userWord
		for(int i = 0; i < wordle.length(); i++){
			if(isalpha(userWord[i])){
				char c = toupper(userWord[i]);
				size_t pos = keyboard.find(c);
				if (userWord[i] == wordle[i]) {
					keyboard.replace(pos, 1, setColor("32") + string(1, c) + setColor("0"));
				}
				else{
					bool inWord = false;
					for(int j = 0; j < wordle.length(); j++){
						if(userWord[i] == wordle[j]){
							inWord = true;
							break;
						}
					}
					// FIXME: i dont want to overwrite what was already highlighted
					//if(keyboard[pos + 4] != '3' && keyboard[pos + 4] != '4' && keyboard[pos + 4] != '9'){ // Check if not already G/Y/G
						if(inWord){ 
							keyboard.replace(pos, 1, setColor("33") + string(1, c) + setColor("0"));
						}
						else{
							keyboard.replace(pos, 1, setColor("90") + string(1, c) + setColor("0"));
						}
					//}
				}
			}
		}
	}
	cout << keyboard << endl;
}

int main(){
	srand(time(0));

	ifstream fin1("wordle-all-words-14855.txt"); // user can guess any of these words
	if(!fin1.is_open()){
		cerr << "Error: wordle-all-words-14855 could not open" << endl;
		return -1;
	}
	ifstream fin2("wordle-answers-2309.txt"); // these are the words that will be chosen for wordle
	if(!fin2.is_open()){
		cerr << "Error: wordle-all-words-14855 could not open" << endl;
		return -1;
	}

	// FIXME: maybe add a file that has all the words already played and check if the word is in that file
	// ofstream fout4("done-wordle-words.txt");
	// if(!fout4.is_open()){
	// 	cerr << "Error: done-wordle-words could not open" << endl;
	// 	return -1;
	// }

	// extract words from file
	string fileWord1, fileWord2, wordle;
	vector<string> validWords;
	vector<string> possibleWords;
	while(fin1 >> fileWord1){
		validWords.push_back(fileWord1);
	}
	while(fin2 >> fileWord2){
		possibleWords.push_back(fileWord2);
	}
	fin1.close();
	fin2.close();
	// randomize Wordle word
	int random = rand() % possibleWords.size();
	wordle = possibleWords[random];

	// start
	cout << "           Wordle !      " << endl;
	cout << "    ---------------------" << endl;
	string keyboard = "     Q W E R T Y U I O P\n      A S D F G H J K L\n        Z X C V B N M";
	cout << keyboard << endl;
	
	string printGuessedWords[6] = {"", "", "", "", "", ""};
	string onlyGuessedWords[6] = {"", "", "", "", "", ""};
	string shared[6] = {"", "", "", "", "", ""};
	string userWord;
	int i = 0;
	while(true){
		cout << "(" << i+1 << "/6): ";
		cin >> userWord;
		(void)system("clear"); // clear screen for better display
		bool validWord = isValidWord(validWords, i, onlyGuessedWords, userWord);
		if(validWord){
			printGuessedWords[i] = printGuess(wordle, userWord);
			onlyGuessedWords[i] = userWord;
			shared[i] = share(wordle, userWord);
			i++;
		}
		// print results
		cout << "           Wordle !      " << endl;
		cout << "    ---------------------" << endl;
		for(int k = 0; k < i; k++){
			cout << printGuessedWords[k] << endl;
			cout << "    ---------------------" << endl;
		}
		if(validWord){
			printKeyboard(keyboard, wordle, userWord, true);
		}
		else{
			printKeyboard(keyboard, wordle, userWord, false);
		}
		// end game
		if(wordle == userWord){
			cout << "\n\tyou guessed the word!" << endl;
			break;
		}
		if(i == 6){
			cout << "\n\tyou ran out of guesses :/" << endl;
			cout << "\tthe word was: " << wordle << endl;
			break;
		}
	}
	// when the game ends, print share feature
	cout << "\nshare your score!" << endl << endl;
	cout << "Ephimoon's Wordle <3" << endl << endl;
	for(int k = 0; k < i; k++){
		cout << shared[k] << endl;
	}
	cout << "\ncheck it out at: https://replit.com/@Ephimoon/Wordle?v=1";
	return 0;
}

//ALL WORDS TO ONE FILE
// ifstream fin1("wordle-allowed-guesses-12546.txt"); // https://gist.github.com/cfreshman/d5fb56316158a1575898bba1eed3b5da
// if(!fin1.is_open()){
// 	cerr << "Error: wordle-allowed-guesses-12546 could not open" << endl;
// 	return -1;
// }
// ifstream fin2("wordle-answers-2309.txt"); // https://gist.github.com/cfreshman/a7b776506c73284511034e63af1017ee
// if(!fin2.is_open()){
// 	cerr << "Error: wordle-answers-2309 could not open" << endl;
// 	return -1;
// }
// ofstream fout1("wordle-all-words.txt");
// 	if(!fout1.is_open()){
// 	cerr << "Error: wordle-all-words could not open" << endl;
// 	return -1;
// }
// unordered_set<string> uniqueWords;
// string word;
// // Read words from the first file and insert them into the set
// while (fin1 >> word) { uniqueWords.insert(word); }
// // Read words from the second file and insert them into the set
// while (fin2 >> word) { uniqueWords.insert(word); }
// // Write unique words from the set to the output file
// for (const string& uniqueWord : uniqueWords) { fout1 << uniqueWord << endl; }
// cout << "Merged files and removed duplicates successfully." << endl;
// fin1.close();
// fin2.close();
// fout1.close();
