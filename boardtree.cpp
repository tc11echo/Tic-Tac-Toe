#include "boardtree.h"

BoardTree::BoardTree(const Board& board){
    root=new BoardNode(board);
}

BoardTree::~BoardTree(){
    delete root;
}

BoardTree* BoardTree::getSubTree(const BoardCoordinate& coords){
    // if(root==nullptr){return nullptr;}
    if(root->subTree[coords.row][coords.col].root==nullptr){ // insert
        Board new_board=root->board;
        if(new_board.play(coords)){
            root->subTree[coords.row][coords.col].root=new BoardNode(new_board);
        }
    }
    return &(root->subTree[coords.row][coords.col]);

}

BoardOptimalMove BoardTree::getOptimalMove(const unsigned int depth){

    if(root==nullptr){
    return BoardOptimalMove(); // Returns a dummy illegal move
    }

    if(depth==0 || root->board.isFinished()){
    // ADDED: save the move to the table before returning
        BoardOptimalMove move = BoardOptimalMove(root->board.getBoardScore(), BoardCoordinate(0, 0));
        //BoardHashTable::getInstance().updateTable(root->board.getID(), depth, move);
        return move;
    }

    // ADDED: check if the board is already calculated to this depth
    //if(board and depth is stored in table){
    if(BoardHashTable::getInstance().getHashedMove(root->board.getID(), depth).score != ILLEGAL){
        //return the optimal move stored in table;
        return BoardHashTable::getInstance().getHashedMove(root->board.getID(), depth);
    }

    // Else, we find the estimated score and optimal move of this node by calculating the score of each children node
    int estimatedScore=(root->board.getCurPlayer()==X)? -INFINITY: INFINITY;
    BoardOptimalMove bestMove;
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(root->subTree[i][j].root==nullptr){ //build the subtree if it is empty;
                Board new_board=root->board;
                if(new_board.play(BoardCoordinate(i, j))){
                    root->subTree[i][j].root=new BoardNode(new_board);
                }
            }

            BoardOptimalMove childMove=root->subTree[i][j].getOptimalMove(depth-1);
            if(childMove.score==ILLEGAL){ // If the move is illegal, the subtree corresponds to an invalid move/board, simply skip to the next subtree
                continue;
            }

            if((root->board.getCurPlayer()==X)? childMove.score>estimatedScore: childMove.score<estimatedScore){
                estimatedScore=childMove.score;
                bestMove=BoardOptimalMove(estimatedScore, BoardCoordinate(i, j));
            }
        }
    }

    // ADDED: save the move to the table before returning
    BoardHashTable::getInstance().updateTable(root->board.getID(), depth, bestMove);
    return bestMove;
}

//===================================================================================================

BoardOptimalMove BoardTree::getOptimalMoveAlphaBeta(const unsigned int depth, int alpha, int beta){
    if(root==nullptr){
    return BoardOptimalMove(); // Returns a dummy illegal move
    }

    if(depth==0 || root->board.isFinished()){
        return BoardOptimalMove(root->board.getBoardScore(), BoardCoordinate(0, 0));
    }

    // Else, we find the estimated score and optimal move of this node by calculating the score of each children node
    int estimatedScore=(root->board.getCurPlayer()==X)? -INFINITY: INFINITY;
    BoardOptimalMove bestMove;
    for(int i=0; i<BOARD_SIZE; i++){
        bool break_flag=false;
        
        for(int j=0; j<BOARD_SIZE; j++){
            if(root->subTree[i][j].root==nullptr){ //build the subtree if it is empty;
                Board new_board=root->board;
                if(new_board.play(BoardCoordinate(i, j))){
                    root->subTree[i][j].root=new BoardNode(new_board);
                }
            }

            // MODIFIED: Call the alpha-beta pruning variant
            BoardOptimalMove childMove = root->subTree[i][j].getOptimalMoveAlphaBeta(depth - 1, alpha, beta);
            if(childMove.score==ILLEGAL){ // If the move is illegal, the subtree corresponds to an invalid move/board, simply skip to the next subtree
                continue;
            }

            if((root->board.getCurPlayer()==X)? childMove.score>estimatedScore: childMove.score<estimatedScore){
                estimatedScore=childMove.score;
                bestMove=BoardOptimalMove(estimatedScore, BoardCoordinate(i, j));
            }

            // ADDED: Update alpha and beta according to newest child score found
            if (root->board.getCurPlayer()==X) {
                if(bestMove.score > beta){
                    // Found a move that would be worse for O, so O will not consider this subtree
                    break_flag=true;
                    break;
                }
                // Update the best move found by X
                alpha = std::max(bestMove.score, alpha);
            }
            else {
                // Same logic but for current player O
                if(bestMove.score < alpha){
                    break_flag=true;
                    break;
                }
                beta = std::min(bestMove.score, beta);
            }
        }
        if(break_flag){
            break;
        }
    }

    return bestMove;
}