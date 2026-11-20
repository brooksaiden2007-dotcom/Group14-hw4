#include <iostream>
#include <sstream>
#include "bowling_frame.h"

using namespace std;

int current_game_total(Bowling_Frame * first_frame)
{
    int game_total = 0;

    for (Bowling_Frame * frame = first_frame;
         frame != nullptr;
         frame = frame->next_frame())
    {
        game_total += frame->frame_score();
    }

    return game_total;
}

void print_game(Bowling_Frame * first_frame, int player)
{
    cout << "\nPlayer " << player << endl;

    for (Bowling_Frame * frame = first_frame;
         frame != nullptr;
         frame = frame->next_frame())
    {
        cout << "Frame " << frame->frame_num() << ":" << endl
             << "  Score 1: " << frame->score1() << endl
             << "  Score 2: " << frame->score2() << endl
             << "  Frame Total: " << frame->frame_score() << endl;
    }

    cout << "\nTotal Game Score for player " << player << ": "
         << current_game_total(first_frame) << endl;
}

void delete_games(Bowling_Frame ** players, int player_count)
{
    for (int player = 0; player < player_count; ++player)
    {
        Bowling_Frame * frame = players[player];

        while (frame != nullptr)
        {
            Bowling_Frame * next = frame->next_frame();

            frame->next_frame(nullptr);
            frame->prev_frame(nullptr);

            delete frame;
            frame = next;
        }
    }

    delete[] players;
}

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <num_players>" << endl;
        return 1;
    }

    int player_count = 0;
    istringstream argument(argv[1]);
    char extra;

    if (!(argument >> player_count) ||
        (argument >> extra) ||
        player_count <= 0)
    {
        cerr << "Error: num_players must be a positive integer." << endl;
        return 1;
    }

    Bowling_Frame ** players = nullptr;

    try
    {
        players = new Bowling_Frame * [player_count]();

        for (int player = 0; player < player_count; ++player)
        {
            Bowling_Frame * previous = nullptr;

            for (int frame_number = 1; frame_number <= 10; ++frame_number)
            {
                Bowling_Frame * frame = new Bowling_Frame(frame_number);

                frame->prev_frame(previous);

                if (previous != nullptr)
                {
                    previous->next_frame(frame);
                }
                else
                {
                    players[player] = frame;
                }

                previous = frame;
            }
        }
    }
    catch (...)
    {
        if (players != nullptr)
        {
            delete_games(players, player_count);
        }

        cerr << "Error: unable to allocate bowling frames." << endl;
        return 1;
    }

    cout << "Player count is: " << player_count << endl;

    for (int player = 0; player < player_count; ++player)
    {
        print_game(players[player], player + 1);
    }

    delete_games(players, player_count);

    return 0;
}
