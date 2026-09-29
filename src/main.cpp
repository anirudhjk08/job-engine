#include<iostream>
#include "Job.h"
#include "JobQueue.h"
void calculate(){
    std::cout<<"Calculating...\n";
}

int main(){
    JobQueue queue;
    Job job(calculate);
    queue.addJob(std::move(job));
    auto receivedJob = queue.getJob();
    receivedJob->execute();
    return 0;
}