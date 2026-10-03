#pragma once

#include<iostream>
#include<thread>
#include "JobQueue.h"
#include "Logger.h"

class Worker{
   JobQueue& queue;
   Logger& logger;
   std::thread thread;
   public:
   Worker(JobQueue& queue, Logger& logger):queue(queue),logger(logger){}
   void run(){
    while(true){
       auto job = queue.getJob();
       if(!job){
        logger.info("Worker shutting down");
        break;
       }
       logger.info("Worker picked up a job");
       try{
         job->execute();
         logger.info("Job completed");
       }
       catch(const std::exception& e){
         logger.error("Job failed: "+ std::string(e.what()));
       }
       catch(...){
         logger.error("Job failed: unknown exception");
       }
    }
   }
   void start(){
    thread = std::thread(&Worker::run,this);
   }
   void join(){
      if(thread.joinable())
    thread.join();
   }
};