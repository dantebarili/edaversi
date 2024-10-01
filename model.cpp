/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <raylib.h>

#include "model.h" 

void initModel(GameModel &model)
{
    model.gameOver = true;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
}

void startModel(GameModel &model)
{
    model.gameOver = false;

    model.currentPlayer = PLAYER_BLACK;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;
    model.turnTimer = GetTime();

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2 - 1] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2] = PIECE_BLACK;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2 - 1] = PIECE_BLACK;
}

Player getCurrentPlayer(GameModel &model)
{
    return model.currentPlayer;
}

int getScore(GameModel &model, Player player)
{
    int score = 0;

    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (((model.board[y][x] == PIECE_WHITE) &&
                 (player == PLAYER_WHITE)) ||
                ((model.board[y][x] == PIECE_BLACK) &&
                 (player == PLAYER_BLACK)))
                score++;
        }

    return score;
}

double getTimer(GameModel &model, Player player)
{
    double turnTime = 0;

    if (!model.gameOver && (player == model.currentPlayer))
        turnTime = GetTime() - model.turnTimer;

    return model.playerTime[player] + turnTime;
}

Piece getBoardPiece(GameModel &model, Square square)
{
    return model.board[square.y][square.x];
}

void setBoardPiece(GameModel &model, Square square, Piece piece)
{
    model.board[square.y][square.x] = piece;
}

bool isSquareValid(Square square)
{
    return (square.x >= 0) &&
           (square.x < BOARD_SIZE) &&
           (square.y >= 0) &&
           (square.y < BOARD_SIZE);
}

void getValidMoves(GameModel &model, Moves &validMoves)
{

    Player player = getCurrentPlayer(model);
    //bool adyascencia;
    bool alineado;
    std::vector<Square> direcciones = {{1,0},{0,1},{-1,0},{0,-1},{1,1},{-1,1},{-1,-1},{1,-1}};


    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            Square move = {x, y};
            
            alineado = false;
            
            if (getBoardPiece(model, move) == PIECE_EMPTY)
            {
                Square moveAux;

                if(player == PLAYER_BLACK)
                {
                        for(auto& direccion : direcciones)
                        {
                            bool oponenteEncontrado = false;
                            moveAux = move;
                            moveAux.x += direccion.x;
                            moveAux.y += direccion.y;
                            while(isSquareValid(moveAux))
                            {
                                if(getBoardPiece(model,moveAux) == PIECE_WHITE)
                                {
                                    oponenteEncontrado = true;
                                }else if(getBoardPiece(model,moveAux) == PIECE_BLACK && oponenteEncontrado)
                                {
                                    alineado = true;
                                    break;
                                }
                                else
                                {
                                    break;
                                }
                                moveAux.x += direccion.x;
                                moveAux.y += direccion.y;
                            }
                            if(alineado)
                            {
                                validMoves.push_back(move);
                                break;
                            }
                        }
                    
                }else
                {
                        for(auto& direccion : direcciones)
                        {
                            bool oponenteEncontrado = false;
                            moveAux = move;
                            moveAux.x += direccion.x;
                            moveAux.y += direccion.y;
                            while(isSquareValid(moveAux))
                            {
                                if(getBoardPiece(model,moveAux) == PIECE_BLACK)
                                {
                                    oponenteEncontrado = true;
                                }else if(getBoardPiece(model,moveAux) == PIECE_WHITE && oponenteEncontrado)
                                {
                                    alineado = true;
                                    break;
                                }
                                else
                                {
                                    break;
                                }
                                moveAux.x += direccion.x;
                                moveAux.y += direccion.y;
                            }
                            if(alineado)
                            {
                                validMoves.push_back(move);
                                break;
                            }
                        }
                    
                }
            }
        }

}

bool playMove(GameModel &model, Square move)
{
    bool terminar = 0;
    bool propia = 0;

    // Set game piece
    Piece piece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
            ? PIECE_WHITE
            : PIECE_BLACK;

    setBoardPiece(model, move, piece);

    // cambio de color las intermedias 

    // a la derecha
    for (int x = 1; x < BOARD_SIZE && terminar == 0; x++) {
        Square pos = {move.x + x, move.y};

        if (getCurrentPlayer(model) == PLAYER_WHITE) {

            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_BLACK)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_WHITE) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < x; i++) {
                        Square intermedia = {move.x + i, move.y};
                        setBoardPiece(model, intermedia, PIECE_WHITE); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }

        } else { // Caso de que el jugador actual sea negro
            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_WHITE)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_BLACK) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < x; i++) {
                        Square intermedia = {move.x + i, move.y};
                        setBoardPiece(model, intermedia, PIECE_BLACK); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }
        }
    }
    // a la izquierda
    for (int x = 1; x < BOARD_SIZE && terminar == 0; x++) {
        Square pos = {move.x - x, move.y};

        if (getCurrentPlayer(model) == PLAYER_WHITE) {

            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_BLACK)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_WHITE) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < x; i++) {
                        Square intermedia = {move.x - i, move.y};
                        setBoardPiece(model, intermedia, PIECE_WHITE); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }

        } else { // Caso de que el jugador actual sea negro
            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_WHITE)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_BLACK) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < x; i++) {
                        Square intermedia = {move.x - i, move.y};
                        setBoardPiece(model, intermedia, PIECE_BLACK); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }
        }
    }
    // arriba
    for (int y = 1; y < BOARD_SIZE && terminar == 0; y++) {
        Square pos = {move.x, move.y + y};

        if (getCurrentPlayer(model) == PLAYER_WHITE) {

            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_BLACK)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_WHITE) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x, move.y + i};
                        setBoardPiece(model, intermedia, PIECE_WHITE); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }

        } else { // Caso de que el jugador actual sea negro
            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_WHITE)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_BLACK) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x, i+ move.y};
                        setBoardPiece(model, intermedia, PIECE_BLACK); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }
        }
    }
    // abajo
    for (int y = 1; y < BOARD_SIZE && terminar == 0; y++) {
        Square pos = {move.x, move.y - y};

        if (getCurrentPlayer(model) == PLAYER_WHITE) {

            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_BLACK)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_WHITE) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x, move.y - i};
                        setBoardPiece(model, intermedia, PIECE_WHITE); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }

        } else { // Caso de que el jugador actual sea negro
            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_WHITE)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_BLACK) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x, move.y - i};
                        setBoardPiece(model, intermedia, PIECE_BLACK); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }
        }
    }

    for (int y = 1; y < BOARD_SIZE && terminar == 0; y++) {
        Square pos = {move.x + y, move.y + y};

        if (getCurrentPlayer(model) == PLAYER_WHITE) {

            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_BLACK)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_WHITE) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x + i, move.y + i};
                        setBoardPiece(model, intermedia, PIECE_WHITE); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }

        } else { // Caso de que el jugador actual sea negro
            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_WHITE)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_BLACK) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x + i, move.y + i};
                        setBoardPiece(model, intermedia, PIECE_BLACK); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }
        }
    }

    for (int y = 1; y < BOARD_SIZE && terminar == 0; y++) {
        Square pos = {move.x - y, move.y - y};

        if (getCurrentPlayer(model) == PLAYER_WHITE) {

            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_BLACK)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_WHITE) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x - i, move.y - i};
                        setBoardPiece(model, intermedia, PIECE_WHITE); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }

        } else { // Caso de que el jugador actual sea negro
            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_WHITE)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_BLACK) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x - i, move.y - i};
                        setBoardPiece(model, intermedia, PIECE_BLACK); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }
        }
    }

    for (int y = 1; y < BOARD_SIZE && terminar == 0; y++) {
        Square pos = {move.x + y, move.y - y};

        if (getCurrentPlayer(model) == PLAYER_WHITE) {

            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_BLACK)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_WHITE) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x + i, move.y - i};
                        setBoardPiece(model, intermedia, PIECE_WHITE); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }

        } else { // Caso de que el jugador actual sea negro
            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_WHITE)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_BLACK) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x + i, move.y - i};
                        setBoardPiece(model, intermedia, PIECE_BLACK); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }
        }
    }

    for (int y = 1; y < BOARD_SIZE && terminar == 0; y++) {
        Square pos = {move.x - y, move.y + y};

        if (getCurrentPlayer(model) == PLAYER_WHITE) {

            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_BLACK)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_WHITE) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x - i, move.y + i};
                        setBoardPiece(model, intermedia, PIECE_WHITE); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }

        } else { // Caso de que el jugador actual sea negro
            if (isSquareValid(pos)) {
                // Si encuentra una casilla vacía, termina el ciclo
                if (getBoardPiece(model, pos) == PIECE_EMPTY)
                    break;

                if (getBoardPiece(model, pos) == PIECE_WHITE)
                    continue;

                // Si encuentra una pieza negra, verifica si ya había una pieza blanca al otro lado
                if (getBoardPiece(model, pos) == PIECE_BLACK) {
                    
                    // Cambiar de color las del medio
                    for (int i = 1; i < y; i++) {
                        Square intermedia = {move.x - i, move.y + i};
                        setBoardPiece(model, intermedia, PIECE_BLACK); // Cambia las piezas negras a blancas
                    }
                    break;  // Termina el ciclo
                    
                }

            } else {
                // Si la casilla no es válida, termina
                break;
            }
        }
    }

    // Update timer
    double currentTime = GetTime();
    model.playerTime[model.currentPlayer] += currentTime - model.turnTimer;
    model.turnTimer = currentTime;

    // Swap player
    model.currentPlayer =
        (model.currentPlayer == PLAYER_WHITE)
            ? PLAYER_BLACK
            : PLAYER_WHITE;

    // Game over?
    Moves validMoves;
    getValidMoves(model, validMoves);

    if (validMoves.size() == 0)
    {
        // Swap player
        model.currentPlayer =
            (model.currentPlayer == PLAYER_WHITE)
                ? PLAYER_BLACK
                : PLAYER_WHITE;

        Moves validMoves;
        getValidMoves(model, validMoves);

        if (validMoves.size() == 0)
            model.gameOver = true;
    }

    return true;
}
