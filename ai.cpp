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

#define DEPTH_LIMIT 7
#define MAX_NODOS 1000000

Square getBestMove(GameModel &model)
{
    Moves validMoves;
    getValidMoves(model, validMoves);

    int bestValue = INT_MIN;
    Square bestMove = validMoves[0];

    int nodosEvaluados = 0;

    for (Square move : validMoves) {
        GameModel newModel = model;  // Copia del modelo
        playMove(newModel, move);   // Simula el movimiento
        int moveValue = minimax(newModel, DEPTH_LIMIT, true, INT_MIN, INT_MAX, nodosEvaluados, MAX_NODOS);  // Llamada a minimax

        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }

    return bestMove;
}

int minimax(GameModel &model, int depth, bool maximizingPlayer, int alpha, int beta, int &nodosEvaluados, int maxNodos) {
    // Verificar si se excedió el número máximo de nodos
    if (nodosEvaluados >= maxNodos) {
        return evaluateBoard(model, getCurrentPlayer(model));  // Devolver la evaluación actual
    }

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

    // Incrementar el contador de nodos evaluados
    nodosEvaluados++;

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (Square move : validMoves) {
            GameModel newModel = model;  // Copiamos el modelo para simular el movimiento
            playMove(newModel, move);     // Simulamos el movimiento
            int eval = minimax(newModel, depth - 1, false, alpha, beta, nodosEvaluados, maxNodos);  // Llamada recursiva
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);  // Actualizamos alpha

            // Podar rama si es posible
            if (beta <= alpha) {
                break;  // No es necesario continuar explorando
            }

            // Verificar si hemos alcanzado el límite de nodos
            if (nodosEvaluados >= maxNodos) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (Square move : validMoves) {
            GameModel newModel = model;  // Copiamos el modelo para simular el movimiento
            playMove(newModel, move);     // Simulamos el movimiento
            int eval = minimax(newModel, depth - 1, true, alpha, beta, nodosEvaluados, maxNodos);   // Llamada recursiva
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);  // Actualizamos beta

            // Podar rama si es posible
            if (beta <= alpha) {
                break;  // No es necesario continuar explorando
            }

            // Verificar si hemos alcanzado el límite de nodos
            if (nodosEvaluados >= maxNodos) {
                break;
            }
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
    unsigned char white = 0;
    unsigned char black = 0;
    for (int y = 0; y < BOARD_SIZE; y++){
        for (int x = 0; x < BOARD_SIZE; x++){
            if(getBoardPiece(model, (Square){x,y}) == PIECE_WHITE){
                white++;
            }
            else if(getBoardPiece(model, (Square){x,y}) == PIECE_BLACK)
            {
                black++;
            }
        }
    }

    if(currentPlayer == PLAYER_BLACK)
    {
        return black - white;
    }
    else
    {
        return white - black;
    }
}