#include <iostream>
#include "../src/levesteinDFA.h"

int main(int argc, char* argv[]){
    auto dfa = new LevesteinDFA();
    if(argc == 3){
        std::string text = argv[1];
        int dist = std::stoi(argv[2]);
        dfa->build(text, dist);
        dfa->write_to_file("../dfa.txt");
        std::cout << "Writed dfa to dfa.txt" << std::endl;
    }
    else
        std::cout << "Usage: ./levestein <prefix> <dist>\n";
    return 0;
}