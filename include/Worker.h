#pragma once

#include<iostream>
#include<thread>
#include "JobQueue.h"

class Worker{
   JobQueue& queue;
   std::thread thread;
   public:
   Worker(JobQueue& queue):queue(queue){}
   void run(){
    while(true){
       auto job = queue.getJob();
       if(!job){
        break;
       }
       job->execute();
    }
   }
   void start(){
    thread = std::thread(&Worker::run,this);
   }
   void join(){
    thread.join();
   }
};