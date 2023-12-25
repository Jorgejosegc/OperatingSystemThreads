//Jorge Gutierrez Project Operating System

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>

using namespace std;

mutex mtx;   //it will avoid to a shared data accessed by multiple threads at the same time
condition_variable ConVa; //  is a synchronization primitive used with a std::mutex 
						 //to block one or more threads until another thread both modifies a shared variable (the condition) and notifies the condition_variable . 
int Coins = 0;
int NGamers = 0;
int NewCoins1 =0;

void gamer(int id)
{
	int Collectedcoins=0;
	
	while (Collectedcoins<20)
	{
		unique_lock<mutex> lock(mtx);
		
		while (NGamers > 0 || Coins <= 0)
		{
			ConVa.wait(lock);
		}
		NGamers++;
		Coins--;
		
		printf("\nGamer %d Has Entered the Room. \n", id);
		
		int CoinstoCollect = min(5,Coins);     //This means that CoinstoCollect variable will have the smallest value between 5 and Coins contador.
		Collectedcoins = Collectedcoins + CoinstoCollect;
		Coins = Coins - CoinstoCollect;
		
		printf("Gamer %d Has Collected %d Coins. \n", id,CoinstoCollect);
		
		if(Collectedcoins>=20)
		{
			printf("The gamer %d Has Collected Enough Coins ----------------> Can Move to the Next Step\n",id);
		}
		else
		{
			printf("The gamer %d Need To Collect More Coins. It will wait to enter to the room again. \n",id);
		}
		NGamers--;
		ConVa.notify_all(); //Unblocks all of the threads that are blocked on the specified condition variable at the time of the call
	}
}


void miner()
{
	random_device rd;    
    mt19937 gen(rd());   //random number generator of integers that produces non-deterministic ramdon numbers and mt19937 hich is a highly regarded pseudorandom number generator.
    uniform_int_distribution<> dis(1, 5); // Uniform_int_distribution is a class that generates random integers within a specified range. When called mt19937 is from 1 to 5.
    
    while(true)
    {
    	unique_lock<mutex> lock(mtx);
    	
    	while(NGamers>0 || Coins>=20)
    	{
    		ConVa.wait(lock);
		}
		
		printf("\nMiner Has Entered The Room.\n");
		
		int NewCoins = dis(gen);
		NewCoins1 = NewCoins;
		Coins = Coins + NewCoins;
		
		printf("Miner Has Added %d Coins to the Room.\n",NewCoins);
		ConVa.notify_all();
	}
}

int main()
{
	thread minerThread(miner);
	
	thread gamers[10];
	int Gmax=10;
	for(int i=0 ; i<Gmax ; i++)      //iteration for the 10 gamers.
	{
		gamers[i] = thread(gamer, i + 1);
	}
	for (int j = 0; j < Gmax; j++)
	{
		gamers[j].join();
	}
	minerThread.join();
	
	return 0;
}


