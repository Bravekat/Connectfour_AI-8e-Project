#include "c4bot.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cstdlib>

const int DEPTH = 11;

void C4Bot::run() {
	std::string line;
	while (std::getline(std::cin, line)) {
		std::vector<std::string> command = split(line, ' ');
		if (command[0] == "settings") {
			setting(command[1], command[2]);
		}
		else if (command[0] == "update" && command[1] == "game") {
			update(command[2], command[3]);
		}
		else if (command[0] == "action" && command[1] == "move") {
			move(std::stoi(command[2]));
		}
		else {
			std::cerr << "Unknown command: " << line << std::endl;
		}
	}
}

//void C4Bot::movetest()
//{
//	State newstate = state;
//	
//	int move = AlphaBeta(state, DEPTH);
//	std::cerr << "move: " <<move << std::endl;
//	doMove(state, move, newstate);
//	state = newstate;
//
//	std::cerr << state << std::endl;
//}

void C4Bot::move(int timeout) {
	/* Do something more intelligent here instead of returning a random move */
	/*std::vector<Move> moves = getMoves(state);*/
	std::cout << "place_disc " << AlphaBeta(state, 8) << std::endl;
}

int eval(const State &board, const Player &player)
{
	/* Eval connect four */
	Player opponent = (player == Player::X) ? Player::O : Player::X;

	if (getWinner(board) == player)
		return std::numeric_limits<int>::max() - 1;
	else if (getWinner(board) == opponent) {
		return std::numeric_limits<int>::min() + 1;
	}

	if (player == Player::O)
		return getOldScore(board);
	else if (player == Player::X)
		return -getOldScore(board);

	return 0;
}

std::pair<int, Move> AlphaBeta(const State &board, int depth, int a, int b, bool isMax,const Move bestMove, const Player player)
{
	if (depth == 0) {
		int score = eval(board, player);
		return std::make_pair(score, bestMove);
	}
	std::vector<Move> moves = getMoves(board);
	if (moves.empty()) {
		int score = eval(board, player);
		return std::make_pair(score, bestMove);
	}
	if (isMax)
	{
		std::pair<int, Move> v = std::make_pair(std::numeric_limits<int>::min(), -1);
		for (int move : moves)
		{
			State makeMove = board;
			bool succes = doMove(board, move, makeMove);
			if (!succes) { continue; }
			std::pair<int, Move> moveScore = AlphaBeta(makeMove, depth - 1, a, b, false, move, player);
			/*if (depth == DEPTH) 
				std::cerr << move << ": " << moveScore.first << "\t";*/
			if (moveScore.first > v.first) {
				v.second = move;
			}

			v.first = std::max(v.first, moveScore.first);
			a = std::max(a, v.first);

			if (b <= a) {
				break;
			}
		}
		//if (depth == DEPTH)
		//	std::cerr << std::endl;
		return v;
	}
	else if (!isMax)
	{
		std::pair<int, Move> v = std::make_pair(std::numeric_limits<int>::max(), -1);
		for (int move : moves)
		{
			State makeMove = board;
			bool succes = doMove(board, move, makeMove);
			if (!succes) { continue; }
			std::pair<int, Move> moveScore = AlphaBeta(makeMove, depth - 1, a, b, true, move, player);
			if (moveScore.first < v.first) {
				v.second = move;
			}

			v.first = std::min(v.first, moveScore.first);
			b = std::min(b, v.first);

			if (b <= a) {
				break;
			}
		}
		return v;
	}
}

Move AlphaBeta(const State &board, int depth)
{
	Player player = getCurrentPlayer(board);
	Move BestMove = 0;
	static int a = std::numeric_limits<int>::min();
	static int b = std::numeric_limits<int>::max();
	std::pair<int, Move> moveVal = AlphaBeta(board, depth, a, b, true, BestMove, player);
	//std::cout << moveVal.first << std::endl;
	return moveVal.second;
}

void C4Bot::update(std::string &key, std::string &value) {
	if (key == "round") {
		round = std::stoi(value);
	}
	else if (key == "field") {
		int row = 0;
		int col = 0;
		std::vector<std::string> fields = split(value, ',');
		for (std::string &field : fields) {
			if (field == "0") {
				state[row][col] = Player::X;
			}
			else if (field == "1") {
				state[row][col] = Player::O;
			}
			else {
				state[row][col] = Player::None;
			}
			col++;
			if (col == 7) {
				row++;
				col = 0;
			}
		}
	}
}

void C4Bot::setting(std::string &key, std::string &value) {
	if (key == "timebank") {
		timebank = std::stoi(value);
	}
	else if (key == "time_per_move") {
		time_per_move = std::stoi(value);
	}
	else if (key == "player_names") {
		std::vector<std::string> names = split(value, ',');
		player_names[0] = names[0];
		player_names[1] = names[1];
	}
	else if (key == "your_bot") {
		your_bot = value;
	}
	else if (key == "your_botid") {
		your_botid = std::stoi(value);
	}
	else if (key == "field_columns") {
		field_columns = std::stoi(value);
	}
	else if (key == "field_rows") {
		field_rows = std::stoi(value);
	}
}

std::vector<std::string> C4Bot::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}