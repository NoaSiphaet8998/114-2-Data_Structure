#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cctype>

using namespace std;

//弄一個類別
enum TokenType {
    NUMBER,     //數字
    OPERATOR,   //運算子
    LPAREN,     //左括號
    RPAREN,     //右括號
    IDENTIFIER  //識別名稱
};

//一個Token結構
struct Token {
    TokenType type; //type就是上面的enum
    string value;   //value是token的內容
};

//TokenNode用來實作堆疊的節點
struct TokenNode {
    Token data;         //上面定義的Token structure為data
    TokenNode* next;    //next指向下一個TokenNode的指標
};

//TokenStack是實作堆疊的類別
class TokenStack {
private:
    TokenNode* top;     //設一個private的指標變數top，指向堆疊頂端的TokenNode

public:

    //同名建構子，一執行此class就初始化
    TokenStack() {
        top = nullptr;      //top為nullptr，讓堆疊是空空ㄉ
    }

    //push function for 加入一個token到stack的top
    void push(Token tok) {      //傳入一個型別為token structure的參數tok
        TokenNode* newNode = new TokenNode;     //newNode來放新加入的token，new一個TokenNode物件
        newNode->data = tok;    //把傳入的tok放到newNode的data裡
        newNode->next = top;    //newNode的next指向原本的stack的top
        top = newNode;          //top指向剛放進來的newNode，也就是新來的token
    }

    //pop function for 從stack的top取出一個token
    Token pop() {
        if (isEmpty()) {            //如果stack是空的，就報錯，因為沒東西可以pop
            throw runtime_error("Syntax Error: Empty stack");
        }

        Token tok = top->data;      //把stack頂端的token存到tok變數裡

        TokenNode* temp = top;      //用temp暫存目前的top，因為我們要刪除它
        top = top->next;            //把top移到下一個TokenNode，因為原本的top要不見了

        delete temp;                //刪除剛剛暫存在temp的top

        return tok;                 //回傳上一個top的data，也就是我們pop出來的token
    }

    //peek function for 偷看stack的top是什麼token但不pop
    Token peek() {
        if (isEmpty()) {
            throw runtime_error("Syntax Error: Empty stack");
        }

        return top->data;   //回傳stack top的data
    }

    //檢查是不是空的function
    bool isEmpty() {
        return top == nullptr; //回傳top是nullptr
    }
};

//一個ValueNode結構
struct ValueNode {
    double data;        //double型態的data
    ValueNode* next;    //指向下一個valueNode的pointer
};

//建立一個ValueStack class
class ValueStack {
private:
    ValueNode* top;     //top指向stack頂端的valueNode

public:
    ValueStack() {
        top = nullptr; //初始化top為nullptr，讓stack一開始是空的
    }

    //push函式
    void push(double val) {
        ValueNode* newNode = new ValueNode; //newNode來放新的value，new一個ValueNode物件
        newNode->data = val;        //把傳入的val放到newNode的data裡
        newNode->next = top;        //newNode的next指向原本的stack的top
        top = newNode;          //top指向剛放進來的newNode，也就是新來的value
    }

    //pop函式
    double pop() {
        if (isEmpty()) {
            throw runtime_error("Syntax Error: Empty value stack");
        }

        double val = top->data;     //在這邊定義一個val來放top的data(double value)
        ValueNode* temp = top;      //用temp暫存目前的top，因為要刪除它
        top = top->next;            //把top弄到下一個

        delete temp;        //刪除temp，也就是刪除原本的top

        return val;         //回傳val，也就是原本被刪除的top的data(double value)
    }

    //看一下top的函式
    double peek() {
        if (isEmpty()) {    //stack is empty 就除錯
            throw runtime_error("Syntax Error: Empty value stack");
        }

        return top->data;   //回傳top的data（double value）
    }

    //檢查stack是否為空
    bool isEmpty() {
        return top == nullptr;//回傳top是空值
    }
};

//建立一個token的vector
vector<Token> tokenize(const string& expression) {

    vector<Token> tokens;

    int i = 0;
    int n = expression.length();

    while (i < n) {

        char ch = expression[i];

        //如果是空白鍵
        if (isspace(ch)) {
            i++;        
            continue; //就跳過
        }

        //如果是數字、點、負號且負號是合法的（在開頭或在運算子或左括號後面）
        if (isdigit(ch) || ch == '.' || (ch == '-' && (tokens.empty() || tokens.back().type == OPERATOR ||tokens.back().type == LPAREN)))
        {

            string numStr;  //新變數numStr來存字串

            //如果是負號
            if (ch == '-') {
                numStr += '-';   //字串加上負號
                i++;
            }

            //繼續讀取數字和小數點，直到遇到非數字或非小數點的字元
            while (i < n &&
                  (isdigit(expression[i]) ||
                   expression[i] == '.')) {

                numStr += expression[i];    //全部弄進numStr裡  
                i++;
            }

            tokens.push_back({ NUMBER, numStr });   //把numStr放到token的value裡，type是NUMBER
            continue;
        }

        //如果是alpha或底線
        if (isalpha(ch) || ch == '_') {

            string varName;   //varName來存字串

            //如果是合法的或是底線，就繼續弄！
            while (i < n &&
                  (isalnum(expression[i]) ||
                   expression[i] == '_')) {

                varName += expression[i];   //把弄到的都存進varName
                i++;
            }

            tokens.push_back({ IDENTIFIER, varName });  //把varName放到token的value裡，type是IDENTIFIER
            continue;
        }

        //如果是左括號
        if (ch == '(') {
            tokens.push_back({ LPAREN, "(" });  //把左括號放到token的value裡，type是LPAREN
            i++;
            continue;
        }

        //如果是右括號
        if (ch == ')') {
            tokens.push_back({ RPAREN, ")" });  //把右括號放到token的value裡，type是RPAREN
            i++;
            continue;
        }

        
        if (i + 1 < n) {

            string two = expression.substr(i, 2); //新的two字串是從expression的i位置開始的兩個字元

            //如果two是以下運算子之一
            if (two == "&&" ||
                two == "||" ||
                two == "==" ||
                two == "!=" ||
                two == "<=" ||
                two == ">=" ||
                two == "<<" ||
                two == ">>" ||
                two == "**") {

                tokens.push_back({ OPERATOR, two });    //把two放到token的value裡，type是OPERATOR
                i += 2; //然後把i往後移兩個字元，因為two是兩個字元的運算子
                continue;
            }
        }

        //如果是以下運算子
        if (ch == '+' ||
            ch == '-' ||
            ch == '*' ||
            ch == '/' ||
            ch == '%' ||
            ch == '<' ||
            ch == '>' ||
            ch == '&' ||
            ch == '|' ||
            ch == '^' ||
            ch == '!' ||
            ch == '~' ||
            ch == '=') {

            string opStr(1, ch);    //把ch轉成string放到opStr裡
            tokens.push_back({ OPERATOR, opStr });  //把opStr放到token的value裡，type是OPERATOR

            i++;
            continue;
        }

        throw runtime_error(
            string("Invalid character: ") + ch
        );
    }

    return tokens; //回傳token的vector
}


//建立一個ExpressionEvaluator的class
class ExpressionEvaluator {

private:

    map<string, double> variables;  //一個map

    //回傳運算子的優先權
    int precedence(const string& op) {

        if (op == "=") return 0;

        if (op == "||") return 1;

        if (op == "&&") return 2;

        if (op == "|") return 3;

        if (op == "^") return 4;

        if (op == "&") return 5;

        if (op == "==" || op == "!=") return 6;

        if (op == "<" ||
            op == ">" ||
            op == "<=" ||
            op == ">=") return 7;

        if (op == "<<" ||
            op == ">>") return 8;

        if (op == "+" ||
            op == "-") return 9;

        if (op == "*" ||
            op == "/" ||
            op == "%") return 10;

        if (op == "**") return 11;

        return 0;
    }

    //ab和運算子的運算結果
    double applyOperation(double a,
                          double b,
                          const string& op) {

        if (op == "+") return a + b;

        if (op == "-") return a - b;

        if (op == "*") return a * b;

        //如果是除法，要看b是不是0
        if (op == "/") {

            if (b == 0)
                throw runtime_error("Division by zero");    //是的話報錯

            return a / b;
        }

        //取餘數的話也是看b是不是0
        if (op == "%") {

            if ((int)b == 0)
                throw runtime_error("Modulo by zero");  //是的話報錯

            return (int)a % (int)b;
        }

        if (op == "**" || op == "^") {
            return pow(a, b);
        }

        if (op == "<") return a < b;

        if (op == ">") return a > b;

        if (op == "<=") return a <= b;

        if (op == ">=") return a >= b;

        if (op == "==") return a == b;

        if (op == "!=") return a != b;

        if (op == "&&") return a && b;

        if (op == "||") return a || b;

        if (op == "&")
            return (int)a & (int)b;

        if (op == "|")
            return (int)a | (int)b;

        if (op == "^")
            return (int)a ^ (int)b;

        if (op == "<<")
            return (int)a << (int)b;

        if (op == ">>")
            return (int)a >> (int)b;

        throw runtime_error("Unknown operator");
    }

public:

    //設定variable的函式
    void setVariable(string name, double value) {
        variables[name] = value; //讓name跟value弄在一起
    }

    //檢查是否有這個variable的函式
    bool hasVariable(string name) {
        return variables.find(name) != variables.end();  //回傳在map裡有沒有找到name這個key
    }

    //弄到variable的值的函式
    double getVariable(string name) {

        if (!hasVariable(name)) {       //如果沒有這個variable，就報錯
            throw runtime_error(
                "Undefined variable: " + name
            );
        }

        return variables[name];
    }

    //infix轉postfix的函式
    vector<Token> infixToPostfix(
        const vector<Token>& infixTokens) {

        vector<Token> postfix;

        TokenStack stack;

        for (Token tok : infixTokens) {

            //type是NUMBER或IDENTIFIER，就直接放到postfix裡
            if (tok.type == NUMBER ||
                tok.type == IDENTIFIER) {

                postfix.push_back(tok);
            }

            //如果是左括號，就放到stack裡
            else if (tok.type == LPAREN) {

                stack.push(tok);
            }

            //如果是右括號，就從stack裡拿出來放到postfix裡，直到遇到左括號
            else if (tok.type == RPAREN) {

                while (!stack.isEmpty() &&
                       stack.peek().value != "(") {

                    postfix.push_back(stack.pop());
                }

                if (!stack.isEmpty())
                    stack.pop();
            }

            //如果是OPERATOR，就從stack裡拿出來放到postfix裡，直到遇到優先權較低的運算子或左括號，然後把這個運算子放到stack裡
            else if (tok.type == OPERATOR) {

                while (!stack.isEmpty() &&
                       stack.peek().type == OPERATOR &&
                       precedence(stack.peek().value)
                       >= precedence(tok.value)) {

                    postfix.push_back(stack.pop());
                }

                stack.push(tok);
            }
        }

        //最後把stack裡剩下的運算子都弄到postfix裡
        while (!stack.isEmpty()) {
            postfix.push_back(stack.pop());
        }

        return postfix;
    }

    //評估postfix的函式
    double evaluatePostfix(
        const vector<Token>& postfixTokens) {

        ValueStack stack;

        for (Token tok : postfixTokens) {

            //如果是NUMBER，就把它的值轉成double放到stack裡
            if (tok.type == NUMBER) {

                stack.push(stod(tok.value));
            }

            //如果是IDENTIFIER，就呼叫弄到值的函式，把值放到stack裡
            else if (tok.type == IDENTIFIER) {

                stack.push(getVariable(tok.value));
            }

            //如果是OPERATOR，就從stack裡拿出來兩個值，做運算，然後把結果放回stack裡
            else if (tok.type == OPERATOR) {

                //如果是！或是～，就只需要拿出來一個值，做運算，然後把結果放回stack裡
                if (tok.value == "!" ||
                    tok.value == "~") {

                    if (stack.isEmpty()) {
                        throw runtime_error(
                            "Invalid expression"
                        );
                    }

                    double a = stack.pop();

                    double result;

                    if (tok.value == "!") {
                        result = !a;
                    }
                    else {
                        result = ~(int)a;
                    }

                    stack.push(result);

                    cout << "[Step] "
                         << tok.value
                         << a
                         << " = "
                         << result
                         << endl;

                    continue;
                }

                //如果stack是空的就報錯，因為沒東西可以pop
                if (stack.isEmpty()) {
                    throw runtime_error(
                        "Invalid expression"
                    );
                }


                //先pop出來的值是b，後pop出來的值是a，因為在postfix裡面a在b前面
                //b是右邊ㄉ，a是左邊ㄉ
                double b = stack.pop();

                if (stack.isEmpty()) {
                    throw runtime_error(
                        "Invalid expression"
                    );
                }

                double a = stack.pop();

                //result等於呼叫applyOperation的函式所回傳的結果
                double result =
                    applyOperation(a, b, tok.value);

                stack.push(result); //把結果弄進stack

                cout << "[Step] "
                     << a
                     << " "
                     << tok.value
                     << " "
                     << b
                     << " = "
                     << result
                     << endl;
            }
        }

        if (stack.isEmpty()) {
            throw runtime_error(
                "Invalid expression"
            );
        }

        return stack.pop();
    }
};

int main() {

    ExpressionEvaluator calculator;

    string input;

    while (true) {

        cout << "\nEnter expression (or exit): ";

        getline(cin, input);

        if (input == "exit") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        try {

            //找等號的位置
            size_t eqPos = input.find('=');

            //如果有等號，且不是比較運算子，就當作是ASSIGNMENT
            bool isAssignment =
                (eqPos != string::npos) &&
                (input.find("==") == string::npos);

            //如果是ASSIGNMENT
            if (isAssignment) {

                //把等號前面的當作變數名稱
                string varName =
                    input.substr(0, eqPos);

                //把等號後面的當作表達式
                string expr =
                    input.substr(eqPos + 1);

                //把變數名稱裡的空白鍵去掉，因為變數名稱不能有空白鍵
                string cleanVar;

                //把varName的字元都檢查一遍，如果不是空白鍵，就放到cleanVar裡
                for (char c : varName) {
                    if (!isspace(c)) {
                        cleanVar += c;
                    }
                }

                //把表達式弄成infix的token vector
                vector<Token> infix =
                    tokenize(expr);

                //把infix轉成postfix的token vector
                vector<Token> postfix =
                    calculator.infixToPostfix(infix);

                cout << "\nPostfix: ";

                for (Token t : postfix) {
                    cout << t.value << " ";
                }

                cout << endl;

                cout << "\n--- Evaluation Steps ---\n";

                //result等於呼叫evaluatePostfix的函式所回傳的結果
                double result =
                    calculator.evaluatePostfix(postfix);

                calculator.setVariable(cleanVar,
                                       result);

                cout << "\n"
                     << cleanVar
                     << " = "
                     << result
                     << endl;

                continue;
            }

            //infix等於呼叫tokenize的函式所回傳的結果
            vector<Token> infix =
                tokenize(input);

            //postfix等於呼叫infixToPostfix的函式所回傳的結果
            vector<Token> postfix =
                calculator.infixToPostfix(infix);

            cout << "\nPostfix: ";

            for (Token t : postfix) {
                cout << t.value << " ";
            }

            cout << endl;

            cout << "\n--- Evaluation Steps ---\n";

            double result =
                calculator.evaluatePostfix(postfix);

            cout << "\nFinal Answer: "
                 << result
                 << endl;
        }

        catch (const exception& e) {

            cerr << "\nERROR: "
                 << e.what()
                 << endl;
        }
    }

    return 0;
}
