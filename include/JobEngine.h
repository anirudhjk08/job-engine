#pragma once 

#include<memory>
#include<vector>

#include "JobQueue.h"
#include "Worker.h"

class JobEngine{
    private:
    JobQueue queue;
    Logger logger;
    std::vector<std::unique_ptr<Worker>> workers;
    public:
    JobEngine(size_t workercount){
        for(size_t i=0; i < workercount; i++){
            workers.push_back(std::make_unique<Worker>(queue,logger));
        }
    }
    void start(){
        for(auto& worker:workers){
            worker->start();
        }
    }
    bool submit(Job job){
        return queue.addJob(std::move(job));
    }
    void shutdown(){
        queue.shutdownQueue();
        for(auto& worker:workers){
            worker->join();
        }
    }
    
};