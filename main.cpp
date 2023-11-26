#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class State {
public:
    string state_id;
    vector< vector<State*> > transitons;
    bool isAcceptState = false;

    State(string state_id, size_t alphabetSize) : state_id(state_id) {
        transitons.resize(alphabetSize + 1);
    }
};

vector<State*> NextStates(string alphabets, State* currentState, char symbol) {
    vector<State*> nextStates;
    int symbolIndex = alphabets.find(symbol);

    // Add states for the given symbol
    if (symbolIndex != string::npos && symbolIndex < currentState->transitons.size()) {
        for (auto state : currentState->transitons[symbolIndex]) {
            nextStates.push_back(state);
        }
    }

    // Add states for epsilon transitions (last column in the transitions vector)
    for (auto state : currentState->transitons.back()) {
        nextStates.push_back(state);
    }

    return nextStates;
}

void PrintTransitions(const vector<State*>& states, const string& alphabets) {
    cout << "State Transitions:" << endl;
    for (const auto& state : states) {
        cout << "State " << state->state_id << ":" << endl;
        for (size_t i = 0; i < state->transitons.size(); ++i) {
            cout << "  On '" << (i < alphabets.size() ? string(1, alphabets[i]) : "ε") << "': ";
            if (!state->transitons[i].empty()) {
                for (const auto& transState : state->transitons[i]) {
                    cout << transState->state_id << " ";
                }
            } else {
                cout << "Φ";
            }
            cout << endl;
        }
    }
}

void solveNFA(string alphabets, string totalStates, string startState, vector<State*> states) {
    string inputString;

    while (true) {
        cout << "Enter the input string (type 'Done' to exit): ";
        cin >> inputString;

        if (inputString == "Done") {
            break;
        }

        vector<State*> currentStates;
        currentStates.push_back(states[stoi(startState) - stoi(startState)]); // Start state

        cout << "Processing input string: " << inputString << endl;

        for (char symbol : inputString) {
            vector<State*> nextStates;

            // Handle transitions for the current symbol
            for (auto state : currentStates) {
                int symbolIndex = alphabets.find(symbol);
                if (symbolIndex != string::npos && symbolIndex < state->transitons.size()) {
                    for (auto nextState : state->transitons[symbolIndex]) {
                        nextStates.push_back(nextState);
                        cout << "Transition from State " << state->state_id << " to State " << nextState->state_id << " on '" << symbol << "'" << endl;
                    }
                }
            }

            // Handle epsilon transitions
            bool addedNewState;
            do {
                addedNewState = false;
                vector<State*> epsilonNextStates;
                for (auto state : nextStates) {
                    for (auto nextState : state->transitons.back()) {
                        if (find(nextStates.begin(), nextStates.end(), nextState) == nextStates.end() &&
                            find(epsilonNextStates.begin(), epsilonNextStates.end(), nextState) == epsilonNextStates.end()) {
                            epsilonNextStates.push_back(nextState);
                            cout << "Epsilon Transition from State " << state->state_id << " to State " << nextState->state_id << endl;
                            addedNewState = true;
                        }
                    }
                }
                nextStates.insert(nextStates.end(), epsilonNextStates.begin(), epsilonNextStates.end());
            } while (addedNewState);

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


void ProcessLine(string &line) {
    size_t commentPos = line.find_first_of(";");
    string processedLine = line.substr(0, commentPos);
    line = processedLine.erase(processedLine.find_last_not_of(" \n\r\t") + 1);
}

int main() {
    ifstream inputFile;
    inputFile.open("NFA2.txt");

    string alphabets;
    string totalStates;
    string startState;
    vector<string> accept_states;
    vector<State*> states;

    if (inputFile.is_open()) {
        getline(inputFile, alphabets);
        ProcessLine(alphabets);

        getline(inputFile, totalStates);
        ProcessLine(totalStates);

        getline(inputFile, startState);
        ProcessLine(startState);

        size_t alphabets_size = alphabets.size();

        for (int i = stoi(startState); i < stoi(startState) + stoi(totalStates); i++) {
            states.push_back(new State(to_string(i), alphabets_size));
        }

        for (int i = stoi(startState); i < stoi(startState) + stoi(totalStates); i++) {
            string line;
            getline(inputFile, line);
            ProcessLine(line);

            stringstream lineStream(line);
            string word;

            int col = 0;

            while (lineStream >> word) {
                if (word != "Φ") {
                    stringstream transitionStream(word);
                    while (getline(transitionStream, word, ',')) {
                        states[i - stoi(startState)]->transitons[col].push_back(states[stoi(word) - stoi(startState)]);
                    }
                }
                col++;
            }
        }

        string acceptStatesLine;
        getline(inputFile, acceptStatesLine);
        ProcessLine(acceptStatesLine);

        stringstream acceptStatesStream(acceptStatesLine);
        string state;
        while (getline(acceptStatesStream, state, ',')) {
            states[stoi(state) - stoi(startState)]->isAcceptState = true;
        }

        PrintTransitions(states, alphabets);

        solveNFA(alphabets, totalStates, startState, states);
    } else {
        cout << "Failed to open file." << endl;
    }

    inputFile.close();
    return 0;
}