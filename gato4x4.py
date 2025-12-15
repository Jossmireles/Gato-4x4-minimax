import math

PLAYER = 'X'
AI = 'O'
EMPTY = ' '

MAX_DEPTH = 4

def create_board():
    return [[EMPTY for _ in range(4)] for _ in range(4)]

def print_board(board):
    print("\n")
    for row in board:
        print(" | ".join(row))
        print("-" * 13)

def check_winner(board):
    lines = []

    for i in range(4):
        lines.append(board[i])
        lines.append([board[j][i] for j in range(4)])

    lines.append([board[i][i] for i in range(4)])
    lines.append([board[i][3 - i] for i in range(4)])

    for line in lines:
        if all(cell == AI for cell in line):
            return AI
        if all(cell == PLAYER for cell in line):
            return PLAYER

    if all(board[i][j] != EMPTY for i in range(4) for j in range(4)):
        return "EMPATE"

    return None

def heuristic(board):
    score = 0
    lines = []

    for i in range(4):
        lines.append(board[i])
        lines.append([board[j][i] for j in range(4)])

    lines.append([board[i][i] for i in range(4)])
    lines.append([board[i][3 - i] for i in range(4)])

    for line in lines:
        if PLAYER not in line:
            score += line.count(AI)
        if AI not in line:
            score -= line.count(PLAYER)

    return score

def minimax(board, depth, alpha, beta, is_max):
    result = check_winner(board)
    if result == AI:
        return 100
    if result == PLAYER:
        return -100
    if result == "EMPATE":
        return 0
    if depth == MAX_DEPTH:
        return heuristic(board)

    if is_max:
        best = -math.inf
        for i in range(4):
            for j in range(4):
                if board[i][j] == EMPTY:
                    board[i][j] = AI
                    value = minimax(board, depth + 1, alpha, beta, False)
                    board[i][j] = EMPTY
                    best = max(best, value)
                    alpha = max(alpha, best)
                    if beta <= alpha:
                        break
        return best
    else:
        best = math.inf
        for i in range(4):
            for j in range(4):
                if board[i][j] == EMPTY:
                    board[i][j] = PLAYER
                    value = minimax(board, depth + 1, alpha, beta, True)
                    board[i][j] = EMPTY
                    best = min(best, value)
                    beta = min(beta, best)
                    if beta <= alpha:
                        break
        return best

def best_move(board):
    best_score = -math.inf
    move = None

    for i in range(4):
        for j in range(4):
            if board[i][j] == EMPTY:
                board[i][j] = AI
                score = minimax(board, 0, -math.inf, math.inf, False)
                board[i][j] = EMPTY
                if score > best_score:
                    best_score = score
                    move = (i, j)

    return move

def main():
    board = create_board()

    while True:
        print_board(board)

        r, c = map(int, input("Ingresa fila y columna (0-3): ").split())
        if board[r][c] != EMPTY:
            print("Casilla ocupada")
            continue

        board[r][c] = PLAYER

        if check_winner(board):
            break

        ai = best_move(board)
        board[ai[0]][ai[1]] = AI

        if check_winner(board):
            break

    print_board(board)
    print("Resultado:", check_winner(board))

if __name__ == "__main__":
    main()
