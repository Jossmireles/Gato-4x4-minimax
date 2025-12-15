#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const char PLAYER = 'X';
const char AI = 'O';
const char EMPTY = ' ';
const int SIZE = 4;
const int MAX_DEPTH = 4;

vector<vector<char>> board(SIZE, vector<char>(SIZE, EMPTY));

void printBoard() {
    cout << "\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << board[i][j];
            if (j < SIZE - 1) cout << " | ";
        }
        cout << "\n";
        cout << "---------------\n";
    }
}

char checkWinner() {
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] != EMPTY &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2] &&
            board[i][2] == board[i][3])
            return board[i][0];

        if (board[0][i] != EMPTY &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i] &&
            board[2][i] == board[3][i])
            return board[0][i];
    }

    if (board[0][0] != EMPTY &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2] &&
        board[2][2] == board[3][3])
        return board[0][0];

    if (board[0][3] != EMPTY &&
        board[0][3] == board[1][2] &&
        board[1][2] == board[2][1] &&
        board[2][1] == board[3][0])
        return board[0][3];

    return ' ';
}

int heuristic() {
    int score = 0;

    for (int i = 0; i < SIZE; i++) {
        int aiCount = 0, playerCount = 0;
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == AI) aiCount++;
            if (board[i][j] == PLAYER) playerCount++;
        }
        if (playerCount == 0) score += aiCount;
        if (aiCount == 0) score -= playerCount;
    }
    return score;
}

int minimax(int depth, bool isMax, int alpha, int beta) {
    char winner = checkWinner();
    if (winner == AI) return 100 - depth;
    if (winner == PLAYER) return depth - 100;
    if (depth == MAX_DEPTH) return heuristic();

    bool movesLeft = false;
    for (auto &r : board)
        for (char c : r)
            if (c == EMPTY) movesLeft = true;

    if (!movesLeft) return 0;

    if (isMax) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = AI;
                    int value = minimax(depth + 1, false, alpha, beta);
                    board[i][j] = EMPTY;
                    best = max(best, value);
                    alpha = max(alpha, best);
                    if (beta <= alpha) return best;
                }
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER;
                    int value = minimax(depth + 1, true, alpha, beta);
                    board[i][j] = EMPTY;
                    best = min(best, value);
                    beta = min(beta, best);
                    if (beta <= alpha) return best;
                }
            }
        }
        return best;
    }
}

pair<int, int> bestMove() {
    int bestScore = numeric_limits<int>::min();
    pair<int, int> move = {-1, -1};

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = AI;
                int score = minimax(0, false,
                                    numeric_limits<int>::min(),
                                    numeric_limits<int>::max());
                board[i][j] = EMPTY;

                if (score > bestScore) {
                    bestScore = score;
                    move = {i, j};
                }
            }
        }
    }
    return move;
}

int main() {
    while (true) {
        printBoard();

        int r, c;
        cout << "Ingresa fila y columna (0-3): ";
        cin >> r >> c;

        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || board[r][c] != EMPTY) {
            cout << "Movimiento invalido\n";
            continue;
        }

        board[r][c] = PLAYER;

        if (checkWinner() != ' ') break;

        auto ai = bestMove();
        board[ai.first][ai.second] = AI;

        if (checkWinner() != ' ') break;
    }

    printBoard();
    char winner = checkWinner();
    if (winner == PLAYER) cout << "Ganaste\n";
    else if (winner == AI) cout << "Gana la computadora\n";
    else cout << "Empate\n";

    return 0;
}
