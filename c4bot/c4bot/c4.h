#ifndef C4_H
#define C4_H

#include <array>
#include <vector>
#include <ctime>
#include <random>
#include <iterator>
#include <iostream>

constexpr int state_width = 7;
constexpr int state_height = 6;

enum class Player { None, X, O };
using Move = int;
using State = std::array<std::array<Player, state_width>, state_height>;
using ScoreBoard = std::array<std::array<int, state_width>, state_height>;

// used to get a random element from a container
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
	std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(g));
	return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return select_randomly(start, end, gen);
}

std::ostream &operator<<(std::ostream& os, const Player &p);
std::ostream &operator<<(std::ostream& os, const State &s);
Player getCurrentPlayer(const State &state);
bool doMove(const State &state, const Move &m, State & result);
Player getWinner(const State &state);
ScoreBoard getCenterBoard(const State & state);
int getScore(const State &state);
int getOldScore(const State & state);
int getScoreSum(const ScoreBoard &scoreboard);
int getVerticalScore(const State & state);
int getHorizontalScore(const State & state);
int getDiagonalScore(const State & state);
int getReverseDiagonalScore(const State & state);
std::vector<Move> getMoves(const State &state);

#endif // C4_H