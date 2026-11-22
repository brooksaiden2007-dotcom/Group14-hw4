#include <iostream>
#include "bowling_frame.h"
#include <random>
#include <cstdlib>


using namespace std;

std::random_device dev_main;
std::mt19937 rng_main(dev_main());

const int NUM_FRAMES = 10; //num of frames
const int NUM_ROLLS = 21; //mum of rolls per player

int current_game_total(Bowling_Frame * first_frame)
{
	Bowling_Frame * temp = first_frame;
	int game_total = 0;
	while(temp)
	{
		game_total += temp->frame_score();
		temp = temp->next_frame();
	}
	return game_total;	
}

int main(int argc, char * argv[])
{
	//make syre they give player arg
	if (argc != 2) 
	{
		cout << "Usage: " << argv[0] << " <num_players>" << endl;
		return 1;
	}

	int num_players = atoi(argv[1]);
	//check if more than 1 player
	if (num_players < 1)
	{
		cout << "Usage: " << argv[0] << " <num_players>" << endl;
		return 1;
	}

	cout << "Player count is: " << num_players << endl;
	//make pointer per player
	Bowling_Frame ** players = new Bowling_Frame*[num_players];

	//p = player
	for (int p = 0; p < num_players; p++)
	{
		//10 frames
		players[p] = new Bowling_Frame(1);
		Bowling_Frame * prev = players[p];

		
		for (int f = 2; f <= NUM_FRAMES; f++)
		{
			Bowling_Frame * frame = new Bowling_Frame(f);
			prev->next_frame(frame);
			frame->prev_frame(prev);
			prev = frame;
		}
		//throw a ball for each
		for (int roll = 0; roll < NUM_ROLLS; roll++)
		{
			players[p]->update_scores();
		}
	}
	//print each players frames
	for (int p = 0; p < num_players; p++)
	{
		Bowling_Frame * traverse = players[p];

		while(traverse)
		{
			cout << "Frame " << traverse->frame_num() << ": " << endl
		 	 	<< " Score 1: " << traverse->score1() << endl
		 	 	<< " Score 2: " << traverse->score2() << endl;

			if (traverse->next_frame() == NULL && (traverse->strike() || traverse->spare()))
			{
				cout << " Score 3: " <<traverse->score3() << endl;
			}
			cout << " Frame Score: " << traverse->frame_score() << endl;
			traverse = traverse->next_frame();

		}
		cout << "\nTotal Game Score for player " << p + 1 << ": "
			 << current_game_total(players[p]) << endl << endl;
 	}
	//free frames
	for (int p = 0; p < num_players; p++)
	{
		Bowling_Frame * temp = players[p];
		while(temp)
		{
			Bowling_Frame * to_delete = temp;
			temp = temp->next_frame();
			delete to_delete;
		}
		
	}
	delete [] players;

	return 0;

	
	

}