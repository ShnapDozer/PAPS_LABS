#include <string>
#include <regex>
#include <iostream>

class Expression {
public:
    Expression() {}
    virtual std::string interpret() = 0;
private:
};

class SpaceExpression : public Expression { //  Множественные пробелы
public:
    SpaceExpression(const std::string& input) : input_(input), Expression() {}
    std::string interpret() override {
        std::regex regex("\\s+");
        return std::regex_replace(input_, regex, " ");
    }
private:
    std::string input_;
};

class DashExpression : public Expression { // Использование дефиса вместо тире
public:
  DashExpression(const std::string & input) : input_(input), Expression() {}
  std::string interpret() override {
    std::regex regex("\\b-\\b");
    return std::regex_replace(input_, regex, "—");
  }
private:
  std::string input_;
};

class QuoteExpression : public Expression { // Использование в качестве кавычек символов
public:
    QuoteExpression(const std::string& input) : input_(input), Expression() {}
    std::string interpret() override {
        std::regex regex("\"([^\"]*)\"");
        return std::regex_replace(input_, regex, "«$1»");
    }
private:
    std::string input_;
};

class TabExpression : public Expression { // Неправильное использование табуляторов
public:
    TabExpression(const std::string& input) : input_(input), Expression() {}
    std::string interpret() override {
        std::regex regex("\\t");
        return std::regex_replace(input_, regex, "  ");
    }
private:
    std::string input_;
};

class ParenthesisExpression : public Expression { //  Наличие «лишнего» пробела после открывающей скобки, перед закрывающей скобкой, перед запятой, перед точкой

public:
    ParenthesisExpression(const std::string& input) : input_(input), Expression() {}
    std::string interpret() override {
        std::regex regex("\\( +");
        input_ = std::regex_replace(input_, regex, "(");
        regex = std::regex(" +\\)");
        input_ = std::regex_replace(input_, regex, ")");
        regex = std::regex(", +");
        input_ = std::regex_replace(input_, regex, ",");
        regex = std::regex("\\. +");
        input_ = std::regex_replace(input_, regex, ".");
        return input_;
    }
private:
    std::string input_;
};

class NewlineExpression : public Expression { // Наличие множественных символов перевода строки
public:
    NewlineExpression(const std::string& input) : input_(input), Expression() {}
    std::string interpret() override {
        std::regex regex("\\n+");
        return std::regex_replace(input_, regex, "\n");
    }
private:
    std::string input_;
};

class FullGrammarExpression : public Expression { 
public:
    FullGrammarExpression(const std::string& input) : input_(input), Expression() {}
    std::string interpret() override {
        Expression* expression = new SpaceExpression(input_);
        input_ = expression->interpret();
        delete expression;

        expression = new DashExpression(input_);
        input_ = expression->interpret();
        delete expression;

        expression = new QuoteExpression(input_);
        input_ = expression->interpret();
        delete expression;

        expression = new TabExpression(input_);
        input_ = expression->interpret();
        delete expression;

        expression = new ParenthesisExpression(input_);
        input_ = expression->interpret();
        delete expression;

        expression = new NewlineExpression(input_);
        input_ = expression->interpret();
        delete expression;

        return input_;
    }
private:
    std::string input_;
};

int main() {
    std::string input = "   This is    a     test    \n\n\nwith   multiple - errors. v( \"test\" )   \n";

    Expression* expression = new FullGrammarExpression(input);
    std::cout << expression->interpret() << std::endl;

    delete expression;
    return 0;
}