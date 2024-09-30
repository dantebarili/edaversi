/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <cstdlib>

#include "ai.h"
#include "controller.h"
#include <limits.h>

#define DEPTH_LIMIT 150

Square getBestMove(GameModel &model)
{
    Moves validMoves;
    getValidMoves(model, validMoves);

    int bestValue = INT_MIN;
    Square bestMove = validMoves[0];

    for (Square move : validMoves) {
        GameModel newModel = model;  // Copia del modelo
        playMove(newModel, move);   // Simula el movimiento
        int moveValue = minimax(newModel, DEPTH_LIMIT, false);  // Llamada a minimax

        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }

    return bestMove;

    /* +++ TEST
    // Returns a random valid move...
    Moves validMoves;
    getValidMoves(model, validMoves);

    int index = rand() % validMoves.size();
    return validMoves[index];
    */ 
}

int minimax(GameModel &model, int depth, bool maximizingPlayer) {
    Player currentPlayer = getCurrentPlayer(model);

    // Condición de parada: Si llegamos al límite de profundidad o si el juego ha terminado
    if (depth == 0 || gameIsOver(model)) {
        return evaluateBoard(model, currentPlayer);
    }

    Moves validMoves;
    getValidMoves(model, validMoves);

    if (validMoves.size() == 0) {
        // Si no hay movimientos válidos, simplemente devuelve la evaluación del tablero actual
        return evaluateBoard(model, currentPlayer);
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (Square move : validMoves) {
            GameModel newModel = model;  // Copiamos el modelo para simular el movimiento
            playMove(newModel, move);     // Simulamos el movimiento
            int eval = minimax(newModel, depth - 1, false);  // Llamada recursiva
            maxEval = std::max(maxEval, eval);
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (Square move : validMoves) {
            GameModel newModel = model;  // Copiamos el modelo para simular el movimiento
            playMove(newModel, move);     // Simulamos el movimiento
            int eval = minimax(newModel, depth - 1, true);   // Llamada recursiva
            minEval = std::min(minEval, eval);
        }
        return minEval;
    }
}

bool gameIsOver(GameModel& model){

    for (int y = 0; y < BOARD_SIZE; y++){
        for (int x = 0; x < BOARD_SIZE; x++){
            if(getBoardPiece(model, (Square){x,y}) == PIECE_EMPTY){
                return false;
            }
        }
    }
    return true;

}

int evaluateBoard(GameModel& model, Player currentPlayer){

}