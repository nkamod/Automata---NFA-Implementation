#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/**
 * Class to store the state of the NFA
 * 
 * @param state_id The id of the state
 * @param transitons The transitions for each alphabet
 * @param isAcceptState Whether the state is an accept state or not
 * 
 * @details The transitions are stored in a 2D vector, the first dimension represents the alphabet and the second dimension represents the states the current state transitions to on that alphabet.
 *          The last index of the transitions array represents the epsilon transitions for the state.
*/
class State {
public:
    string state_id;
    vector< vector<State*> > transitons;
    bool isAcceptState = false;

    /**
     * Constructor for the State class
     * 
     * @param state_id The id of the state
     * @param alphabetSize The size of the alphabet
    */
    State(string state_id, size_t alphabetSize) : state_id(state_id) {
        transitons.resize(alphabetSize + 1);  // Create a 2D vector for the transitions, the last index is for epsilon transitions
    }
};

/**
 * Solves the NFA to find if the input string is accepted by the NFA
 * 
 * @param alphabets The alphabets for the NFA
 * @param totalStates The total number of states in the NFA
 * @param startState The start state of the NFA
 * @param states The array of addresses of all states in the NFA
*/
void solveNFA(string alphabets, string totalStates, string startState, vector<State*> states) {
    string inputString;   // The input string

    while (true) {
        cout << "Enter the input string (type 'Done' to exit): ";
        cin >> inputString;
        
        // Exit if the user enters 'Done'
        if (inputString == "Done") {
            break;
        }


        vector<State*> currentStates;
        currentStates.push_back(states[0]); // Add the start state to the current states
        
        cout << "Processing input string: " << inputString << endl;

        // Iterate over each symbol in the input string
        for (char symbol : inputString) {
            vector<State*> nextStates; // Stores the next states for the current symbol

            // Iterate over each current state
            for (auto state : currentStates) {
                // Find the index of the symbol in the alphabets
                int symbolIndex = alphabets.find(symbol);

                // Check if the symbol is present in the alphabets and if the state has a transition for the symbol
                if (symbolIndex != string::npos && symbolIndex < state->transitons.size()) {

                    // With the symbol index, find the next states for the current state
                    for (auto nextState : state->transitons[symbolIndex]) {

                        // Add the next state to the next states
                        nextStates.push_back(nextState);
                        cout << "Transition from State " << state->state_id << " to State " << nextState->state_id << " on '" << symbol << "'" << endl;
                    }
                }
            }

            // There can be epsilon transitions from the current states and they can be chained one after the other hence we need to find all the epsilon transitions using a loop,
            // This loop will run until no new epsilon transitions are found which are consecutively chained
            bool addedNewState;
            do {
                addedNewState = false;
                vector<State*> epsilonNextStates;

                // Iterate over each next state which was found in the previous loop
                for (auto state : nextStates) {

                    // Find the epsilon transitions for each next state, epsilon transitions are stored in the last index of the transitions array
                    for (auto nextState : state->transitons.back()) {
                        
                        // Check if the state reached by the epsilon transition is not already in nextStates or epsilonNextStates
                        if (find(nextStates.begin(), nextStates.end(), nextState) == nextStates.end() &&
                            find(epsilonNextStates.begin(), epsilonNextStates.end(), nextState) == epsilonNextStates.end()) {
                            // Add the new state to the list of epsilon next states
                            epsilonNextStates.push_back(nextState);
                            cout << "Epsilon Transition from State " << state->state_id << " to State " << nextState->state_id << endl;
                            // Mark that a new state has been added in this iteration
                            addedNewState = true;
                        }
                    }
                }
                // Add all newly discovered states to nextStates for the next iteration
                nextStates.insert(nextStates.end(), epsilonNextStates.begin(), epsilonNextStates.end());
            } while (addedNewState); // Run the loop until no new epsilon transitions are found consecutively

            // Update the current states to the next states
            currentStates = nextStates;
        }

        // Check if any of the current states is an accept state
        bool isAccepted = false;
        for (auto state : currentStates) {
            if (state->isAcceptState) {
                isAccepted = true;
                break;
            }
        }

        if (isAccepted) {
            cout << "The input string is accepted by the NFA." << endl;
        } else {
            cout << "The input string is not accepted by the NFA." << endl;
        }
    }
}

/**
 * Removes comments and trailing spaces from a line
 * 
 * @param line The line to be processed
*/
void ProcessLine(string &line) {
    size_t commentPos = line.find_first_of(";");                                 // Find the position of the comment
    string processedLine = line.substr(0, commentPos);                           // Remove the comment from the line
    line = processedLine.erase(processedLine.find_last_not_of(" \n\r\t") + 1);   // Remove trailing spaces
}

int main() {
    // Read input file
    ifstream inputFile;
    inputFile.open("NFA.txt");

    // Variables to store the input file data
    string alphabets;                    // Alphabets
    string totalStates;                  // Total number of states
    string startState;                   // Start state
    vector<string> accept_states;        // Accept states
    vector<State*> states;               // Array of addresses of all states

    // Read the input file line by line
    if (inputFile.is_open()) {
        // The first line contains the alphabets
        getline(inputFile, alphabets);
        ProcessLine(alphabets);                 // Remove comments and trailing spaces

        // The second line contains the total number of states
        getline(inputFile, totalStates);
        ProcessLine(totalStates);

        // The third line contains the start state
        getline(inputFile, startState);
        ProcessLine(startState);

        // Find the number of possible alphabets a state can transition on
        size_t alphabets_size = alphabets.size();
        
        // Create an object for each state and store its address in the states array
        for (int i = stoi(startState); i < stoi(startState) + stoi(totalStates); i++) states.push_back(new State(to_string(i), alphabets_size));

        // Read the transitions for each state
        for (int i = stoi(startState); i < stoi(startState) + stoi(totalStates); i++) {
            // Read the line
            string line;
            getline(inputFile, line);
            ProcessLine(line);

            // Each line consists of space separated transitions for each alphabet
            stringstream lineStream(line);
            string word;

            // Here a column represents an alphabet
            int col = 0;

            // Read the transitions for each alphabet
            while (lineStream >> word) {

                // If the transition is not empty, add the transition to the state
                if (word != "Φ") {
                    // For a state, each alphabet can have multiple transitions which are separated by a comma
                    stringstream transitionStream(word);
                    while (getline(transitionStream, word, ',')) {
                        // Link the tranisitioning state to the state it transitions to to a particular alphabet
                        states[i - stoi(startState)]->transitons[col].push_back(states[stoi(word) - stoi(startState)]);
                    }
                }

                // Move to the next column
                col++;
            }
        }

        // Read the accept states
        string acceptStatesLine;
        getline(inputFile, acceptStatesLine);
        ProcessLine(acceptStatesLine);

        // There can be multiple accept states for a machine which are separated by a comma
        stringstream acceptStatesStream(acceptStatesLine);
        string state;
        while (getline(acceptStatesStream, state, ',')) {
            states[stoi(state) - stoi(startState)]->isAcceptState = true;
        }


        // Solve the NFA
        solveNFA(alphabets, totalStates, startState, states);
    } else {
        // If the file cannot be opened, print an error message
        cout << "Failed to open file." << endl;
    }

    // Close the input file
    inputFile.close();
    return 0;
}