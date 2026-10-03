#pragma once 

#include<iostream>
#include<mutex>
#include<string>

class Logger{
private:
std::mutex mtx;
public:
void info(const std::string& message){
std::lock_guard<std::mutex> lock(mtx);
std::cout<<"[INFO] "<<message<<"\n";
}
void error(const std::string& message){
    std::lock_guard<std::mutex> lock(mtx);
    std::cerr<<"[ERROR] "<<message<<"\n";
}
}; 