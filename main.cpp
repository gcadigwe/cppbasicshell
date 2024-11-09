//
//  main.cpp
//  cppshell
//
//  Created by Godswill Adigwe on 09/11/2024.
//

#include <iostream> //provides basic input and output stream objects
#include <unistd.h> // provides access to POSIX operating system API, fork(), execvp(), chdir()
#include <sys/wait.h> // provides waitpid() function to wait for child processes to finish
#include <string> // provides std::string class for handling strings
#include <vector> // provides std::vector container for handling dynamic arrays
#include <sstream> // provides std::istringstream for parsing string


bool isBuiltinCommand(const std::vector<std::string> &args) {
    return args[0] == "cd";
}

void executeBuiltinCommand(const std::vector<std::string> &args) {
    if(args[0] == "cd") {
        if(args.size() < 2) {
            std::cerr << "cd requires an argument" << std::endl;
        }else if(chdir(args[1].c_str()) != 0){
            perror("cd failed");
        }
    }
}

std::vector<std::string> parseCommand(const std::string &input) {
    std::vector<std::string> args;
    std::istringstream iss(input);
    std::string arg;
    
    while (iss >> arg) {
        args.push_back(arg);
    }
    
    return args;
}

void executeCommand(const std::vector<std::string> &args) {
    if (args.empty()) return;
    
    if (isBuiltinCommand(args)) {
        executeBuiltinCommand(args);
        return;
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        return;
    }
    
    if (pid == 0) {
        std::vector<char*> cArgs;
        for (const auto& arg : args) {
            cArgs.push_back(const_cast<char*>(arg.c_str()));
        }
        cArgs.push_back(nullptr);
        execvp(cArgs[0], cArgs.data());
        std::cerr << "Unknown command" << std::endl;
        exit(1);
    }else{
        waitpid(pid, nullptr, 0);
    }
}


void shellloop() {
    std::string input;
    
    while (true) {
        std::cout << "SarzyShell 🐚 ";
        std::getline(std::cin, input);
        
        if(input == "exit") break;
        if (input.empty()) continue;
        
        std::vector<std::string> args = parseCommand(input);
        executeCommand(args);
    }
}




int main(int argc, const char * argv[]) {
    shellloop();
    return 0;
}
