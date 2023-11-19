#include <iostream>
#include "../src/levesteinDFA.h"

int main(){
    auto dfa = new LevesteinDFA();
    dfa->build("woof", 1);
    dfa->write_to_file("../dfa.txt");
    std::vector<std::string> tests;
    tests.push_back("");
    tests.push_back("w");
    tests.push_back("ww");
    tests.push_back("woo");
    tests.push_back("woof");
    tests.push_back("wooo");
    tests.push_back("wooof");
    tests.push_back("wwf");
    for(auto text : tests){
        std::cout << text << " " << dfa->check(text) << std::endl; 
    }
    return 0;
}