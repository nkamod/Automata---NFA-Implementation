
---

# NFA Solver README

## Overview
This program is designed to solve a Nondeterministic Finite Automaton (NFA) given a specific input string. It reads the NFA's states, transitions, and accept states from a file, then processes an input string to determine if it is accepted by the NFA.

## NFA Representation
The NFA is represented using the `State` class, where each state has an ID, a list of transitions for each alphabet symbol, and a flag indicating if it's an accept state. Transitions are stored in a 2D vector, with the last index reserved for epsilon transitions.

### State Class
```cpp
class State {
public:
    string state_id;
    vector<vector<State*>> transitions;
    bool isAcceptState = false;

    State(string state_id, size_t alphabetSize);
};
```

## Processing the NFA
The `solveNFA` function takes the NFA's alphabet, states, start state, and processes an input string. It iterates through each character in the string, determining the next states based on the current states and the input symbol.

### Solving the NFA
```cpp
void solveNFA(string alphabets, string totalStates, string startState, vector<State*> states);
```

## Main Function
The `main` function reads the NFA configuration from a file, constructs the states and transitions, and then calls `solveNFA`.

### Main Function Code
```cpp
int main() {
    // ... File reading and NFA construction ...
    solveNFA(alphabets, totalStates, startState, states);
    return 0;
}
```

## NFA Configuration File Format
The input file for the NFA should be formatted as follows:
1. Alphabets (separated by space)
2. Total number of states
3. Start state
4. Transitions for each state
5. Accept states

### Sample Input File
```
01      ; Alphabets
3       ; Number of states: 1, 2, 3
1       ; Start state is 1
2 1     ; State 1 transitions: on 0 = {2}, on 1 = {1}
3 Φ     ; State 2 transitions: on 0 = {3}, on 1 = Φ
Φ 1     ; State 3 transitions: on 0 = Φ, on 1 = {1}
1       ; Accept state is {1}
```

## User Input
The user is prompted to enter an input string. The program processes this string and determines if it is accepted by the NFA.

### User Input Handling
```cpp
while (true) {
    cout << "Enter the input string (type 'Done' to exit): ";
    cin >> inputString;
    // Process the string...
}
```

## Creating Test Files and User Guidelines
- Ensure the test file follows the specified format.
- Each line should describe one aspect of the NFA (alphabets, states, transitions, accept states).
- Use semicolons (`;`) for comments.
- State transitions are space-separated, with `Φ` representing no transition.

<!-- ## Diagrams and Visuals
(Here, you can add images or diagrams that explain how NFAs work, how transitions are processed, or any other relevant visual aid.) -->

---