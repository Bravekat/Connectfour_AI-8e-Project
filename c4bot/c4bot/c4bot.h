#ifndef C4BOT_H
#define C4BOT_H

#include <string>
#include <vector>

#include "c4.h"

class C4Bot {
public:
	int timebank;
	int time_per_move;
	std::string player_names[2];
	std::string your_bot;
	int your_botid;
	int field_columns;
	int field_rows;

	int round;
	State state = { { { { Player::None } } } };

	std::vector<std::string> split(const std::string &s, char delim);
	void setting(std::string &key, std::string &value);
	void update(std::string &key, std::string &value);
	void move(int timeout);

	void run();
	void movetest();
};

Move AlphaBeta(const State &board, int depth);
std::pair<int, Move> AlphaBeta(const State &board, int depth, int a, int b, bool isMax, const Move bestMove, const Player player);

#endif // C4BOT_H