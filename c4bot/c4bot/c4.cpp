#include "c4.h"

std::ostream &operator<<(std::ostream& os, const Player &p) {
	if (p == Player::None) {
		os << ".";
	}
	else if (p == Player::X) {
		os << "X";
	}
	else if (p == Player::O) {
		os << "O";
	}
	return os;
}

std::ostream &operator<<(std::ostream& os, const State &s) {
	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < 7; c++) {
			os << s[r][c];
		}
		os << std::endl;
	}
	return os;
}

Player getCurrentPlayer(const State &state)
{
	int countX = 0;
	int countO = 0;
	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < 7; c++) {
			if (state[r][c] == Player::X) countX++;
			else if (state[r][c] == Player::O) countO++;
		}
	}
	return (countX > countO ? Player::O : Player::X);
}

bool doMove(const State &state, const Move &m, State & result)
{
	for (int r = 0; r < state_height; r++) {
		if (r == 5 || state[r + 1][m] != Player::None) {
			result[r][m] = getCurrentPlayer(state);
			return true;
		}
	}
	return false;
}

Player getWinner(const State &state)
{
	for (int r = 0; r < state_height; r++) {
		for (int c = 0; c < state_width; c++) {
			if (state[r][c] != Player::None) {
				if (c < 4) {
					if (state[r][c] == state[r][c + 1] && state[r][c] == state[r][c + 2] && state[r][c] == state[r][c + 3])
						return state[r][c];
					if (r < 3) {
						if (state[r][c] == state[r + 1][c + 1] && state[r][c] == state[r + 2][c + 2] && state[r][c] == state[r + 3][c + 3])
							return state[r][c];
					}
				}
				if (r < 3) {
					if (state[r][c] == state[r + 1][c] && state[r][c] == state[r + 2][c] && state[r][c] == state[r + 3][c])
						return state[r][c];
					if (c > 2) {
						if (state[r][c] == state[r + 1][c - 1] && state[r][c] == state[r + 2][c - 2] && state[r][c] == state[r + 3][c - 3])
							return state[r][c];
					}
				}
			}

		}
	}
	return Player::None;
}

ScoreBoard getCenterBoard(const State & state) {
	ScoreBoard board{};
	for (int y = 0; y < state_height; y++) {
		if (Player::O == state[y][3]) {
			board[y][3] = 4;
		}
		else if (Player::X == state[y][3]) {
			board[y][3] = -4;
		}
	}
	return board;
}


int getScoreSum(const ScoreBoard & scoreboard)
{
	int sum = 0;
	for (int y = 0; y < state_height; y++) {
		for (int x = 0; x < state_width; x++) {
			sum += scoreboard[y][x];
		}
	}
	return sum;
}

int oldVerticalScore(const State & state)
{
	int sum = 0;
	ScoreBoard vertical_score{};
	for (int x = 0; x < state_width; x++) {
		for (int y = 0; y < state_height; y++) {
			int prev = y > 0 ? vertical_score[y - 1][x] : 0;
			if (state[y][x] == Player::O) {
				vertical_score[y][x] = std::max(prev, 0) + 1;
			}
			else if (state[y][x] == Player::X) {
				vertical_score[y][x] = std::min(prev, 0) - 1;
			}
		}
	}

	for (int x = 0; x < state_width; x++) {
		for (int y = state_height - 1; y >= 0; y--) {
			int prev = y < state_height - 1 ? vertical_score[y + 1][x] : 0;
			int cur = vertical_score[y][x];
			if (state[y][x] == Player::O) {
				vertical_score[y][x] = std::max(cur, prev);
			}
			else if (state[y][x] == Player::X) {
				vertical_score[y][x] = std::min(cur, prev);
			}
		}
	}
	sum += getScoreSum(vertical_score);
	return sum;
}

int getVerticalScore(const State & state)
{
	int sum = 0;
	/* Vertical pass */	
	for (int x = 0; x < state_width; x++){
		int points = 0;
		int whitespots = 0;
		for (int y = 0; y < state_height; y++){
			if (state[y][x] == Player::None)
				whitespots++;
			if (state[y][x] == Player::X){
				if (points > 0)
					break;
				points--;
			}
			else if (state[y][x] == Player::O){
				if (points < 0)
					break;
				points++;
			}
		}
		if (whitespots + abs(points) > 3){
			points *= abs(points);
			sum += points;
		}
	}
	return sum;
}

int getHorizontalScore(const State & state)
{
	/* Horizontal pass */
	ScoreBoard horizontal_score{};
	for (int y = 0; y < state_height; y++) {
		for (int x = 0; x < state_width; x++) {
			int prev = x > 0 ? horizontal_score[y][x - 1] : 0;
			if (state[y][x] == Player::O) {
				horizontal_score[y][x] = std::max(prev, 0) + 1;
			}
			else
				if (state[y][x] == Player::X) {
					horizontal_score[y][x] = std::min(prev, 0) - 1;
				}
		}
	}

	for (int y = 0; y < state_height; y++) {
		for (int x = state_width - 1; x >= 0; x--) {
			int prev = x < state_width - 1 ? horizontal_score[y][x + 1] : 0;
			int cur = horizontal_score[y][x];
			if (state[y][x] == Player::O) {
				horizontal_score[y][x] = std::max(cur, prev);
			}
			else
				if (state[y][x] == Player::X) {
					horizontal_score[y][x] = std::min(cur, prev);
				}
		}
	}
	return getScoreSum(horizontal_score);
}

int getHorizontalScoreNew(const State & state) {
	int sum = 0;

	for (int y = 0; y < state_height; y++) {
		for (int x = 0; x < state_width; x++) {
			if (state[y][x] != Player::None) {
				int empty_spots = 0, xo_spots = 0;

				for (int z = x - 1; z >= std::max(x - 3, 0); z--) {
					if (state[y][x] == state[y][z]) {
						xo_spots++;
					}
					else if (state[y][z] == Player::None) {
						empty_spots++;
					}
					else {
						break;
					}
				}

				for (int z = x + 1; z <= std::min(x + 3, state_width - 1); z++) {
					if (state[y][x] == state[y][z]) {
						xo_spots++;
					}
					else if (state[y][z] == Player::None) {
						empty_spots++;
					}
					else {
						break;
					}
				}

				if (empty_spots + xo_spots + 1 >= 4) {
					if (state[y][x] == Player::O) {
						sum += xo_spots + 1;
					}
					else {
						sum -= xo_spots + 1;
					}
				}

			}
		}
	}

	return sum;

}

int getDiagonalScore(const State & state)
{
	/* Diagonal pass */
	ScoreBoard diagonal_score{};
	for (int x = 0; x < state_width; x++) {
		for (int y = 0; y < state_height; y++) {
			int prev = (x > 0 && y > 0) ? diagonal_score[y - 1][x - 1] : 0;

			if (state[y][x] == Player::O) {
				diagonal_score[y][x] = std::max(prev, 0) + 1;
			}
			else if (state[y][x] == Player::X) {
				diagonal_score[y][x] = std::min(prev, 0) - 1;
			}
		}
	}

	for (int x = state_width - 1; x >= 0; x--) {
		for (int y = state_height - 1; y >= 0; y--) {
			int cur = diagonal_score[y][x];
			int prev = (x < state_width - 1 && y < state_height - 1) ? diagonal_score[y + 1][x + 1] : 0;
			if (state[y][x] == Player::O) {
				diagonal_score[y][x] = std::max(cur, prev);
			}
			else if (state[y][x] == Player::X) {
				diagonal_score[y][x] = std::min(cur, prev);
			}
		}
	}
	return getScoreSum(diagonal_score);
}

int getReverseDiagonalScore(const State & state)
{
	/* Reverse Diagonal pass */
	ScoreBoard reverse_diagonal_score{};
	for (int x = state_width - 1; x >= 0; x--) {
		for (int y = 0; y < state_height; y++) {
			int prev = (x < state_width - 1 && y > 0) ? reverse_diagonal_score[y - 1][x + 1] : 0;

			if (state[y][x] == Player::O) {
				reverse_diagonal_score[y][x] = std::max(prev, 0) + 1;
			}
			else if (state[y][x] == Player::X) {
				reverse_diagonal_score[y][x] = std::min(prev, 0) - 1;
			}
		}
	}

	for (int x = 0; x < state_width; x++) {
		for (int y = state_height - 1; y >= 0; y--) {
			int cur = reverse_diagonal_score[y][x];
			int prev = (x > 0 && y < state_height - 1) ? reverse_diagonal_score[y + 1][x - 1] : 0;
			if (state[y][x] == Player::O) {
				reverse_diagonal_score[y][x] = std::max(cur, prev);
			}
			else if (state[y][x] == Player::X) {
				reverse_diagonal_score[y][x] = std::min(cur, prev);
			}
		}
	}
	
	return getScoreSum(reverse_diagonal_score);
}

int getScore(const State & state)
{
	int centerScore = getScoreSum(getCenterBoard(state));
	int vertScore = getVerticalScore(state);
	int horScore = getHorizontalScore(state);
	int diagonalScore = getDiagonalScore(state);
	int reversediagonalScore = getReverseDiagonalScore(state);
	int sum = centerScore + vertScore + horScore + diagonalScore + reversediagonalScore;
	return sum;

}

int getOldScore(const State & state)
{
	int centerScore = getScoreSum(getCenterBoard(state));
	int vertScore = oldVerticalScore(state);
	int horScore = getHorizontalScore(state);
	int diagonalScore = getDiagonalScore(state);
	int reversediagonalScore = getReverseDiagonalScore(state);
	int sum = centerScore + vertScore + horScore + diagonalScore + reversediagonalScore;
	return sum;
}

std::vector<Move> getMoves(const State &state)
{
	std::vector<Move> moves;
	if (getWinner(state) == Player::None) {
		for (int x = 0; x < state_width; x++)
			if (state[0][x] == Player::None)
				moves.push_back(x);
	}

	return moves;
}