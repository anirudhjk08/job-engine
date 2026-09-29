#pragma once
#include<functional>

class Job{
 private:
 //store the actual work this job needs to perform
 std::function<void()> work;
  public:
  Job(std::function<void()> work){
    this->work = work;
  }
  //execute the work stored inside the job
   void execute(){
    work();
  }
};