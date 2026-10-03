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
    bool started = false;
    bool stopped = false;
    public:
    JobEngine(size_t workercount){
        for(size_t i=0; i < workercount; i++){
            workers.push_back(std::make_unique<Worker>(queue,logger));
        }
    }
    void start(){
       if(started || stopped){
        return;
       }
       started = true;
       for(auto& worker:workers){
        worker->start();
       }
    }
    bool submit(Job job){
        if(!started || stopped) return false;
        return queue.addJob(std::move(job));
    }
    void shutdown(){
         if(!started || stopped) return;
         stopped = true;
        queue.shutdownQueue();
        for(auto& worker:workers){
            worker->join();
        }
    }
    
};