#include<cassert>
#include<iostream>
#include "Job.h"
#include "JobEngine.h"

int main(){
    JobEngine engine(3);
    engine.start();
    bool accepted = engine.submit(Job([]{
        std::cout<<"Test Job executed\n";
    }));
    assert(accepted);
    engine.shutdown();
   bool rejected = engine.submit(Job([]{
    std::cout<<"This should never execute\n";
   }));
   assert(!rejected);
   std::cout<<"All tests passed!\n";
   return 0;
}