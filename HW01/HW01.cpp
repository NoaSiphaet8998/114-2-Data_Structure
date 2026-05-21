#include <iostream>
#include <string>
#include <vector>
using namespace std;

// define a token's type
enum TokenType { NUMBER, OPERATOR, LPAREN, RPAREN };

// define a token
struct Token {
    TokenType type;
    string value;
};

// define a node within a TokenStack
struct TokenNode {
    Token data;  // define the data of the node
    TokenNode* next;   // point to the next node in the stack
};

// make a stack with linked list, using TokenNodes
class TokenStack {
private:
    TokenNode* top; // point to the top node of the stack
public:
    TokenStack() { top = nullptr; } // initialize the stack

    // push function: add a token to the top of the stack
    void push(Token tok) {
        TokenNode* newNode = new TokenNode; // create new node
        newNode->data = tok;      // save token
        newNode->next = top;      // new node points to next in stack(the old top)
        top = newNode;            // update top node
    }

    // pop function: remove and return top of the stack
    Token pop() {
        if (isEmpty()) {
            throw runtime_error("Syntax Error: Unexpected empty stack during evaluation.");
        }                           // if stack is empty, throw error
        Token tok = top->data;      // retrieve top token
        TokenNode* temp = top;           // temporarily save top node
        top = top->next;            // move top to new top
        delete temp;                // free memory
        return tok;                 // return popped token
    }

    // peek function: retrieve top value but don't remove node
    Token peek() {
        if (isEmpty()) { // if stack is empty, throw error
            throw runtime_error("Syntax Error: Unexpected empty stack during evaluation.");
        }
        return (top->data);
    }

    // isEmpty function: check if stack is empty
    bool isEmpty() {
        return top == nullptr;
    }
};

// define a node within a ValueStack
struct ValueNode {
    double data;  // define the data of the node
    ValueNode* next;   // point to the next node in the stack
};

// make a stack with linked list, using ValueNodes
class ValueStack {
private:
    ValueNode* top; // point to the top node of the stack
public:
    ValueStack() { top = nullptr; } // initialize the stack

    // push function: add a value to the top of the stack
    void push(double val) {
        ValueNode* newNode = new ValueNode; // create new node
        newNode->data = val;      // save value
        newNode->next = top;      // new node points to next in stack(the old top)
        top = newNode;            // update top node
    }

    // pop function: remove and return top of the stack
    double pop() {
        if (isEmpty()) {
            throw runtime_error("Syntax Error: Unexpected empty stack during evaluation.");
        }                           // if stack is empty, throw error
        double val = top->data;     // retrieve top value
        ValueNode* temp = top;      // temporarily save top node
        top = top->next;            // move top to new top
        delete temp;                // free memory
        return val;                 // return popped value
    }

    // peek function: retrieve top value but don't remove node
    double peek() {
        if (isEmpty()) { // if stack is empty, throw error
            throw runtime_error("Syntax Error: Unexpected empty stack during evaluation.");
        }
        return (top->data);
    }

    // isEmpty function: check if stack is empty
    bool isEmpty() {
        return top == nullptr;
    }
};

// reconstruct input into tokens
vector<Token> tokenize(const string& expression) {
    vector<Token> tokens;
    int i = 0;
    int n = expression.length();

    while (i < n) {
        char ch = expression[i];

        // skip whitespace
        if (isspace(ch)) {
            i++;
            continue;
        }

        // parse multi-digit numbers, decimals and negative numbers
        // minus signs not part of a negative number will not be handled here
        if (isdigit(ch) || ch == '.' || ch == '-') {
            string numStr = "";
            if (ch == '-' && !tokens.empty()) {     // if this is a minus sign and is not the first character
                TokenType _ = tokens.back().type;   // retrieve last character's type
                if (_ == LPAREN || _ == OPERATOR) { // if the last character is a left parentheses or an operator
                    numStr += ch;                   // this is a negative number, add to string
                    i++;
                }
            }
            else if (ch == '-' && tokens.empty()) { // if this is a minus sign and the first character
                numStr += ch;                       // this is a negative number, add to string
                i++;
            }
            if (expression[i] != '-') { // do not handle minus signs here
                // keep adding characters to the string if it belongs to a number
                while (i < n && (isdigit(expression[i]) || expression[i] == '.')) {
                    numStr += expression[i];
                    i++;
                }

                // push the complete number into vector
                tokens.push_back({ NUMBER, numStr });
                continue;
            }

        }

        // handle parentheses
        if (ch == '(') {
            tokens.push_back({ LPAREN, "(" });
            i++;
            continue;
        }
        if (ch == ')') {
            tokens.push_back({ RPAREN, ")" });
            i++;
            continue;
        }

        // handle operators
        // minus signs not recognized as part of a negative number will be handled here
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '%') {
            string opStr(1, ch); // convert char to string
            tokens.push_back({ OPERATOR, opStr });
            i++;
            continue;
        }

        // if character does not fulfill any of the above, throw error
        throw runtime_error(string("Syntax Error: Invalid character encountered: ") + ch);
    }

    return tokens;
}

// evaluates tokenized expressions
class ExpressionEvaluator {
private:
    // precedence function: determines operator precedence
    int precedence(const string& op) {
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/" || op == "%") return 2;
        if (op == "^") return 3;
        return 0;
    }

    // applyOperation function: safely applies operations, otherwise throws errors
    double applyOperation(double a, double b, const string& op) {
        if (op == "+") return a + b;
        if (op == "-") return a - b;
        if (op == "*") return a * b;
        if (op == "/") {
            if (b == 0) throw runtime_error("Error: Division by zero");
            return a / b;
        }
        if (op == "%") {
            if (b == 0) throw runtime_error("Error: Modulo by zero");
            return (int)a % (int)b; // cast to integer for standard modulo
        }
        if (op == "^") return pow(a, b);
        throw runtime_error("Error: Unknown operator");
    }

public:
    // infixToPostfix function: translates vector of infix tokens to vector of postfix tokens
    vector<Token> infixToPostfix(const vector<Token>& infixTokens) {
        vector<Token> postfix;
        TokenStack stack;
        
        for (Token tok : infixTokens) { // loop through all of infix
            if (tok.type == NUMBER) { // determine if current character is a number
                postfix.push_back(tok); // put directly into postfix
            }
            else if (tok.type == LPAREN) { // put ( directly into stack
                stack.push(tok);
            }
            else if (tok.type == RPAREN) {
                while (!stack.isEmpty() && stack.peek().value != "(") { // pop until (
                    postfix.push_back(stack.pop()); // put popped into postfix
                }
                stack.pop(); // pop ( without putting into postfix
            }
            else { //  process +-*/
                while (!stack.isEmpty() && precedence(stack.peek().value) >= precedence(tok.value)) { // check precedence
                    postfix.push_back(stack.pop()); // pop into postfix
                }
                stack.push(tok); // push current into stack
            }
        }
        // pop all remaining characters in stack
        while (!stack.isEmpty()) {
            postfix.push_back(stack.pop());
        }
         // debugging :P
        for (Token _ : postfix) {
            cout << _.value << " ";
        }
        cout << endl;
        return postfix;
    }

    // evaluatePostfix function: evaluates vector of postfix tokens and prints process
    double evaluatePostfix(const vector<Token>& postfixTokens) {
        ValueStack stack;

        for (Token tok : postfixTokens) {
            if (tok.type == NUMBER) {        // if token is a number
                stack.push(stod(tok.value)); // push value as a double into stack
            }
            else if (tok.type == OPERATOR) { // if token is an operator, attempt applying operation
                // if there are no numbers left in stack, throw error, otherwise retrieve second number in operation
                if (stack.isEmpty()) throw runtime_error("Error: Invalid Expression");
                double b = stack.pop();

                // if there was only one number left in stack, throw error, otherwise retrieve first number in operation
                if (stack.isEmpty()) throw runtime_error("Error: Invalid Expression");
                double a = stack.pop();

                // attempt to apply operation and push result back into stack
                double result = applyOperation(a, b, tok.value);
                stack.push(result);

                // print process if successful
                cout << "[Step] " << a << " " << tok.value << " " << b << " = " << result << endl;
            }
        }
        // if there is nothing in stack, throw error, otherwise return result
        if (stack.isEmpty()) throw runtime_error("Error: Invalid Expression");
        return stack.pop();
    }
};

int main() {
    ExpressionEvaluator calculator;
    string input;

    cout << "=== Stack-Based Expression Calculator ===\n";
    while (true) {
        cout << "\nEnter an expression (or 'exit' to quit): ";
        getline(cin, input);

        if (input == "exit") break;
        if (input.empty()) continue;

        try {
            cout << "Attempting tokenization...\n";
            vector<Token> infix = tokenize(input);
            cout << "Translating infix to postfix...\n";
            vector<Token> postfix = calculator.infixToPostfix(infix);
            cout << "\n--- Evaluation Steps ---\n";
            double finalResult = calculator.evaluatePostfix(postfix);

            cout << "\nFinal Answer: " << finalResult << endl;
        }
        catch (const exception& e) { // handle 
            cerr << e.what() << endl;
        }
    }

    return 0;
}
