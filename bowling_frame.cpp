#include "bowling_frame.h"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

Bowling_Frame::Bowling_Frame(int fnum)
{
	//initalize variables
	score1_ = 0;
	score2_ = 0;
	score3_ = 0;
	strike_ = false;
	spare_ = false;
	roll_one_ = false;
	roll_two_ = false;
	roll_three_ = false;


	frame_num_ = fnum;
	frame_score_ = 0;
	total_score_ = 0;

	next_frame_ = NULL;
	prev_frame_ = NULL;

}

Bowling_Frame::~Bowling_Frame()
{
}

Bowling_Frame* Bowling_Frame::next_frame() { return next_frame_; }
void Bowling_Frame::next_frame( Bowling_Frame * new_frame ) { next_frame_ = new_frame; }

Bowling_Frame* Bowling_Frame::prev_frame() { return prev_frame_; }
void Bowling_Frame::prev_frame( Bowling_Frame * new_frame ) { prev_frame_ = new_frame; }

int Bowling_Frame::score1()				{ return score1_; }
void Bowling_Frame::score1( int score )	{ score1_ = score; }

int Bowling_Frame::score2()				{ return score2_; }
void Bowling_Frame::score2( int score )	{ score2_ = score; }

int Bowling_Frame::score3()				{ return score3_; }
void Bowling_Frame::score3( int score )	{ score3_ = score; }


bool Bowling_Frame::strike() 			{ return strike_; }
void Bowling_Frame::strike(bool value )	{ strike_ = value; }

bool Bowling_Frame::spare() 			{ return spare_; }
void Bowling_Frame::spare(bool value )	{ spare_ = value; }

int Bowling_Frame::frame_num()
{
	return frame_num_;
}

int Bowling_Frame::frame_score()
{
	return frame_score_; 
}

void Bowling_Frame::update_scores()
{
	std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 10);

	if (!roll_one_)
	{
		score1_ = dist10(rng);
		roll_one_ = true;

		if(score1_ == 10) strike_ = true; 
	}
	else if ( (!strike_ || next_frame_ == NULL)&& !roll_two_ ) 	
	{
		
		if (strike_)
		{
			score2_ = dist10(rng);
		}
		else
		{
			std::uniform_int_distribution<std::mt19937::result_type> distX(0,10 - score1_);
			score2_ = distX(rng);

			if ((score1_ + score2_) == 10) spare_ = true;
		}
		roll_two_ = true;

		if ((score1_ + score2_) == 10) spare_ = true;
	}
	else if (next_frame_ == NULL) 	
	{
		if (!roll_three_ && (strike_ || spare_))
		{
			if (strike_ && score2_ < 10)
			{
				// bonus roll
				std::uniform_int_distribution<std::mt19937::result_type> distY(0,10 - score2_);
				score3_ = distY(rng);
			}
			else
			{
				// check for spare or strike
				score3_ = dist10(rng);
			}
			roll_three_ = true;
		}
	}
	else
	{
		//frame done
		next_frame_->update_scores();
	}
	if (next_frame_ == NULL)
	{
		
		frame_score_ = score1_ + score2_ + score3_;
	}
	else
	{
		frame_score_ = score1_ + score2_;

		Bowling_Frame * nxt = next_frame_;

		if (strike_)
		{
			
			frame_score_ += nxt->score1();

			if (nxt->strike() && nxt->next_frame())
				frame_score_ += nxt->next_frame()->score1();	// next was a strike, so 2nd ball is in the frame after
			else
				frame_score_ += nxt->score2();					// next frame's 2nd ball (or last frame's bonus ball)
		}
		else if (spare_)
		{
			
			frame_score_ += nxt->score1();
		}
	}
}




