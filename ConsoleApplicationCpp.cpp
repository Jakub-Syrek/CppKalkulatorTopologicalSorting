#include <iostream>
#include <stack>
#include <vector>
//klasa Kalkulator, wykonującą obliczenia arytmetyczne + , -, *, / na liczbach całkowitych w tym ujemnych.
//Klasa zawiera metodę int calc(std::string expr), w której parametrem wejściowym expr jest łańcuch znakowy, 
//zawierający dowolne wyrażenie arytmetyczne, które należy obliczyć, np. "-12 - 25/4 + 112", 
//a wartością zwracaną przez funkcję jest wynik tego działania.
//Klasa powinna obsługiwać liczby ujemne, wyjątki w sytuacji wykrycia błędów syntaktycznych w wyrażeniu,
//Klasa powinna obsługiwać wyjątki w sytuacji wykrycia błędów syntaktycznych w wyrażeniu, wykrycia niedozwolonych znaków lub liczb niecałkowitych. 
//Powinna również obsługiwać wyjątek, kiedy wystąpi dzielenie przez zero.Pamiętaj o zachowaniu kolejności działań.
using namespace std;

struct Operator {
    char symbol;
    int priority;

    bool operator<(const Operator& other) const {
        return priority < other.priority;
    }
};

class Kalkulator {
public:
    int calc(string expr) {
        stack<int> numbers;
        stack<char> operators;

        for (size_t i = 0; i < expr.length(); i++) {
            if (isspace(expr[i])) {
                continue;
            }
            else if (isdigit(expr[i])) {
                int num = 0;
                while (i < expr.length() && isdigit(expr[i])) {
                    num = num * 10 + (expr[i] - '0');
                    i++;
                }
                i--;
                numbers.push(num);
            }
            else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
                while (!operators.empty() && numbers.size() > 1 && priority(operators.top()) >= priority(expr[i])) {
                    int b = numbers.top();
                    numbers.pop();
                    int a = numbers.top();
                    numbers.pop();
                    char op = operators.top();
                    operators.pop();
                    numbers.push(applyOp(a, b, op));
                }
                operators.push(expr[i]);
            }
            else if (expr[i] == '(') {
                operators.push(expr[i]);
            }
            else if (expr[i] == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    int b = numbers.top();
                    numbers.pop();
                    int a = numbers.top();
                    numbers.pop();
                    char op = operators.top();
                    operators.pop();
                    numbers.push(applyOp(a, b, op));
                }
                if (operators.empty()) {
                    throw invalid_argument("Mismatched parentheses");
                }
                operators.pop();
            }
            else {
                throw invalid_argument("Invalid character");
            }
        }

        while (!operators.empty() && numbers.size() > 1) {
            
            int a = numbers.top();
            numbers.pop();
            int b = numbers.top();
            numbers.pop();
            char op = operators.top();
            operators.pop();

            numbers.push(applyOp(a, b, op));
        }

        if (!operators.empty() && numbers.size() == 1) {
            int b = numbers.top();
            numbers.pop();
            char op = operators.top();
            operators.pop();

            if (op == '-') {
                numbers.push(applyOp(0, b, op));
            }
            else {
				throw invalid_argument("Invalid expression");
            }

        }

        if (numbers.size() != 1 || !operators.empty()) {


            throw invalid_argument("Invalid expression");
        }

        return numbers.top();
    }

private:
    int priority(char op) {
        Operator operators[] = {
            {'+', 1},
            {'-', 1},
            {'*', 2},
            {'/', 2}
        };
        int n = sizeof(operators) / sizeof(operators[0]);
        Operator* p = find_if(operators, operators + n, [=](const Operator& x) { return x.symbol == op; });
        return p != operators + n ? p->priority : 0;
    }

    int applyOp(int a, int b, char op) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (a == 0) {
                throw invalid_argument("Division by zero");
            }
            return b / a;
        default:
            throw invalid_argument("Invalid operator");
        }
    }
};


int main() {
    Kalkulator kalkulator;

    std::vector<std::string> wyrazenia = {
        
        "2 + 3 * (4 - 1)",
        "5+7",
        "1/2",
        "4/4",        
        "3*5+2",
        "3/0",
        "3.14+2"
    };

    for (const auto& wyrazenie : wyrazenia) {
        std::cout << "Wyrazenie: " << wyrazenie << std::endl;

        try {
        
            int wynik = kalkulator.calc(wyrazenie);
            std::cout << "Wynik: " << wynik << std::endl;
        }
        catch (std::invalid_argument& e) {
            std::cout << "Blad: " << e.what() << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}


