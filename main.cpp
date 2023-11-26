#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include <algorithm>

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

void removeDuplicates(vector<StateTransition>& vec) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

StateTransition GetStateTransition(string state, vector<StateTransition> stateTransitions) {
    for (const StateTransition nextState: stateTransitions)
        if (nextState.stateID == state) return nextState;

    return StateTransition(); 
}

/**
 * Function to find next states after consuming a character input from the language
 * 
 * @param alphabetSymbols Language/input provided to the NFA
 * @param currentState current state accepting the input
 * @param stateTransitions transition table for the machine
 * @param symbol current character in the language which is being provided to the machine
 * 
 * @return next states after consuming the input
*/
vector<StateTransition> NextStates(string alphabetSymbols, StateTransition currentState, vector<StateTransition> stateTransitions, char symbol) {

    vector<StateTransition> activeStatesForCurrentSymbol;

    /*
        The first loop finds the next states after transitions for a state after consuming the charater

        e.g.
        01      ; The alphabets
        4       ; 4 states to read 1, 2, 3 and 4
        1 1,2 Φ ; state 1 on 0 = {1} ; 1 on 1 = {1, 2} ; 1 on ε = Φ 
        3 Φ 3   ; state 2 on 0 = {3} ; 2 on 1 = Φ      ; 2 on ε = {3}
        Φ 4 Φ   ; state 3 on 0 = Φ   ; 3 on 1 = {4}    ; 3 on ε = Φ
        4 4 Φ   ; state 4 on 0 = {4} ; 4 on 1 = Φ      ; 4 on ε = Φ
        4       ; accept states are {q4}

        For the given NFA, lets the input language is 1001
        and we are reading the first character i.e. 1

        Then the next transition from the start state will be {1, 2}

        01      ; The alphabets
        4       ; 4 states to read 1, 2, 3 and 4
        
                |
               \/
             0 1   ε
      ->q1 | 1 1,2 Φ ; state 1 on 0 = {1} ; 1 on 1 = {1, 2} ; 1 on ε = Φ 
        q2 | 3 Φ   3 ; state 2 on 0 = {3} ; 2 on 1 = Φ      ; 2 on ε = {3}
        q3 | Φ 4   Φ ; state 3 on 0 = Φ   ; 3 on 1 = {4}    ; 3 on ε = Φ
        q4 | 4 4   Φ ; state 4 on 0 = {4} ; 4 on 1 = Φ      ; 4 on ε = Φ

        4            ; accept states are {q4}

    */
    for (const string state: currentState.transitionsForSymbols[alphabetSymbols.find(symbol)]) {
        
        // Since we are storing these transitions in string form and not creating an actual graph, we need to find the object for this state in the transition table
        StateTransition tr = GetStateTransition(state, stateTransitions);

        if (!tr.stateID.empty()) {
            activeStatesForCurrentSymbol.push_back(tr);
        }

    }

    // Since its a bad approach to modify an array during its iteration we will create an temporary epsilon transition array
    vector<StateTransition> tempEpsilonTransitions;

    // Sometimes on a input a the machine might transition multiple times due to ε transition.
    for (const StateTransition active: activeStatesForCurrentSymbol) {
        if (!active.transitionsForSymbols.back().empty()) {
            
            for (const string state: active.transitionsForSymbols.back()) {
                
                StateTransition tr = GetStateTransition(state, stateTransitions);

                tempEpsilonTransitions.push_back(tr);

            }

            
        }
    }

    // Remove duplicates from tempEpsilonTransitions
    removeDuplicates(tempEpsilonTransitions);

    for (const StateTransition tr: tempEpsilonTransitions) {
        cout << tr.stateID << endl;
    }

    return activeStatesForCurrentSymbol;

}

void SolveNFA(string alphabetSymbols, string totalStates, vector<string> acceptStates, vector<StateTransition> stateTransitions) {

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
    vector<StateTransition> stateTransitions;   // Array of StateTransition structs to store the transition table

    if (inputFile.is_open()) {
        // Get and processed alphabet symbols
        getline(inputFile, alphabetSymbols);
        alphabetSymbols = ProcessLine(alphabetSymbols);

        // Get and processed number of states
        getline(inputFile, totalStates);
        totalStates = ProcessLine(totalStates);

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
            StateTransition tr = StateTransition();

            tr.stateID = to_string(i);
            tr.transitionsForSymbols = symbolTransitions;

            stateTransitions.push_back(tr);
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
    return 0;
}
