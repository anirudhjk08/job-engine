#pragma once

#include<queue>
#include<mutex>
#include<condition_variable>
#include "Job.h"
#include<optional>

class JobQueue{
  private:
  std::queue<Job> jobs;
  std::mutex mtx;
  std::condition_variable cv;
  bool shutdown = false;
  public:
  void addJob(Job job){
    std::lock_guard<std::mutex> lock(mtx);
    jobs.push(std::move(job));
    cv.notify_one();
  }
  std::optional<Job> getJob(){
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock,[this]{
        return !jobs.empty()||shutdown;
    });
    if(jobs.empty() && shutdown){
        return std::nullopt;
    }
    Job job = std::move(jobs.front());
    jobs.pop();
    return job;
  }
  void shutdownQueue(){
    std::lock_guard<std::mutex> lock(mtx);
      shutdown = true;
      cv.notify_all();
  }
};