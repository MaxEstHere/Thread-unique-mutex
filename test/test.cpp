#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/random/random_device.hpp>

//maximum for end of cycle
const int max = 5;

//timer for sleep
void wait(int sec)
{
	boost::this_thread::sleep_for(boost::chrono::seconds{ sec });
}
//aborting thread that shows id
void threadA()
{
	try
	{
	wait(2);
	std::cout << boost::this_thread::get_id() << std::endl;
	}
	catch (boost::thread_interrupted&) {}
}

//protected loop with random values
void threadB()
{
	boost::timed_mutex mutex;
	boost::random::random_device t;
	wait(3);
	for (int i = 0; i < max; i++)
	{
		boost::unique_lock<boost::timed_mutex> lock{ mutex };
		std::cout << "threadB: " << boost::this_thread::get_id() << " for i: " << i << std::endl;
		std::cout << "Random data for unique i: " << t() << std::endl;
		boost::timed_mutex* tm = lock.release();
		tm->unlock();
	}
}
//start threads
int main(int argc, char* argv[])
{
	boost::thread th{ threadA };
	boost::thread th2{ threadB };
	if (boost::thread::hardware_concurrency() == 4 ) th.interrupt();
	std::cout << boost::thread::hardware_concurrency() << std::endl;
	th.join();
	th2.join();
	return 0;
}