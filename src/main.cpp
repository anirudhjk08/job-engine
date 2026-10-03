#include <iostream>

#include "Job.h"
#include "JobEngine.h"

int main() {
    JobEngine engine(3);

    engine.start();

        engine.submit(Job([]() {
            std::cout<<"Normal Job\n";
        })); 
        engine.submit(Job([](){
          throw std::runtime_error("Something went wrong!");
        }));
        engine.submit(Job([](){
         std::cout<<"Another normal job\n";
        }));
    engine.shutdown();

    return 0;
}