#include "c4bot.h"

int main() {
	C4Bot bot;
	bot.run();


	/*int count = 0;
	while (true) {
		std::cerr << "turn: " << count++ << std::endl;
		bot.movetest();
		Player winner = getWinner(bot.state);
		if (winner != Player::None) {
			std::cerr << "Player " << winner << " won" << std::endl;
			break;
		}
	}*/
	
	//State state = { { { { Player::None } } } };
	//state[0][1] = Player::O;
	//state[0][2] = Player::O;
	//state[0][3] = Player::O;
	//state[0][4] = Player::X;
	//state[0][5] = Player::X;
	//state[0][6] = Player::X;
	//int board = getScore(state);
	return 0;
}