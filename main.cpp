#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

struct StateTransition {
    string stateID;
    vector< vector<string> > transitionsForSymbols;
};

// Function to process a line by removing comments and trimming whitespace from the end of the line
string ProcessLine(const string& line) {
    size_t commentPos = line.find_first_of(";"); // Find the postion of the comment symbol i.e.,';'
    string processedLine = line.substr(0, commentPos); // Remove the comment from the line by taking a substring from 0 to the comment position
    processedLine.erase(processedLine.find_last_not_of(" \n\r\t") + 1); // Trim the whitespaces from the end of the line
    return processedLine;
}

void NextStates(string alphabetSymbols, StateTransition currentState, StateTransition* StateTransition, char symbol) {

    for (const string state: currentState.transitionsForSymbols[alphabetSymbols.find(symbol)]) {
        


    }

}

void SolveNFA(string alphabetSymbols, string totalStates, vector<string> acceptStates, StateTransition* stateTransitions) {

    string input;
    while(input != "Done") {

        vector<StateTransition> activeStates;

        activeStates.push_back(stateTransitions[0]);

        for (char symbol: input) {
            
            for (const StateTransition currentState: activeStates) {
                NextStates(alphabetSymbols, currentState, stateTransitions, symbol);
            }
        }

        cout << "Input language: " << endl;
        cin >> input;
        cout << endl;
    }

}

int main() {
    // Opem the input file
    ifstream inputFile;
    inputFile.open("NFA.txt");

    string alphabetSymbols;              // String to store the alphabets
    string totalStates;                  // String to store the total number of states
    vector<string> acceptStates;         // Vector to store the accept states
    StateTransition* stateTransitions;   // Array of StateTransition structs to store the transition table

    if (inputFile.is_open()) {
        // Get and processed alphabet symbols
        getline(inputFile, alphabetSymbols);
        alphabetSymbols = ProcessLine(alphabetSymbols);

        // Get and processed number of states
        getline(inputFile, totalStates);
        totalStates = ProcessLine(totalStates);

        // Allocate memory for the transition table
        stateTransitions = new StateTransition[stoi(totalStates)];

        // Read and process the transition table from the input file
        for (int i = 0; i < stoi(totalStates); i++) {
            string line;
            getline(inputFile, line);
            line = ProcessLine(line);

            vector< vector<string> > symbolTransitions; // Vector to store the transitions for each state
            stringstream lineStream(line);              // String stream to read the line word by word
            string word;

            while (lineStream >> word) {
                vector<string> transitions;             // Vector to store the transitions for each symbol

                // Split the word by comma and store the transitions
                if (word != "Φ") {
                    stringstream transitionStream(word);
                    while (getline(transitionStream, word, ',')) {
                        transitions.push_back(word);
                    }
                }
                symbolTransitions.push_back(transitions);
            }

            // Ensure there are transitions for all symbols in the alphabet
            while (symbolTransitions.size() < 3) {
                symbolTransitions.push_back(vector<string>());
            }

            // Store the transitions for the current state
            stateTransitions[i].stateID = to_string(i);
            stateTransitions[i].transitionsForSymbols = symbolTransitions;
        }

        // Read and process the accept states
        string acceptStatesLine;
        getline(inputFile, acceptStatesLine);
        acceptStatesLine = ProcessLine(acceptStatesLine);

        // Split the accept states by comma and store them
        stringstream acceptStatesStream(acceptStatesLine);
        string state;
        while (getline(acceptStatesStream, state, ',')) {
            acceptStates.push_back(state);
        }

        /*
            If any of the accept states are greater than number of states provided,
            that means the state id is started from 1 instead of 0 (i.e. initial state is 1 and not 0). So, in order to
            compensate for that, we add 1 to all the state ids.
        */
        for (const string state : acceptStates) {
            if (stoi(state) >= stoi(totalStates)) {
                for (int i = 0; i < stoi(totalStates); i++) {
                    stateTransitions[i].stateID = to_string(stoi(stateTransitions[i].stateID) + 1);
                }
                break;
            }
        }

        SolveNFA(alphabetSymbols, totalStates, acceptStates, stateTransitions);

    } else {
        cout << "Failed to open file." << endl;
    }

    inputFile.close();
    delete[] stateTransitions; // Freeing the allocated memory
    return 0;
}
