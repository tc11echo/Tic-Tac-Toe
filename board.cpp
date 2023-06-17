#include "board.h"

Board::Board(const int score[][BOARD_SIZE]): curPlayer(X), id(0) {
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            cells[i][j]=EMPTY;
            this->score[i][j]=score[i][j];
        }
    }
}

bool Board::isFull() const{
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(cells[i][j]==EMPTY){return false;}
        }
    }
    return true;
}

bool Board::isFinished() const{
    if(isFull()){return true;}

    // check row, col, diag and anti-diag
    int diag=0, anti_diag=0;
    for(int i=0; i<BOARD_SIZE; i++){
        
        // check diag and anti-diag
        diag+=cells[i][i];
        anti_diag+=cells[i][BOARD_SIZE-1-i];

        // check row and col
        int row=0, col=0;
        for(int j=0; j<BOARD_SIZE; j++){
            row+=cells[i][j];
            col+=cells[j][i];
        }
        if(abs(row)==BOARD_SIZE){return true;} // whole row same
        if(abs(col)==BOARD_SIZE){return true;} // whole col same
    }
    // check diag and anti-diag
    if(abs(diag)==BOARD_SIZE){return true;} // whole diag same
    if(abs(anti_diag)==BOARD_SIZE){return true;} // whole anti-diag same

    return false;
}

int Board::getBoardScore() const{
    if(isFinished()){

        // check row, col, diag and anti-diag
        int diag=0, anti_diag=0;
        for(int i=0; i<BOARD_SIZE; i++){
            // check diag and anti-diag
            diag+=cells[i][i];
            anti_diag+=cells[i][BOARD_SIZE-1-i];

            // check row and col
            int row=0, col=0;
            for(int j=0; j<BOARD_SIZE; j++){
                row+=cells[i][j];
                col+=cells[j][i];
            }
            if(abs(row)==BOARD_SIZE){return (cells[i][0]==X)? WIN_SCORE: -WIN_SCORE;} // whole row same
            if(abs(col)==BOARD_SIZE){return (cells[0][i]==X)? WIN_SCORE: -WIN_SCORE;} // whole col same
        }
        // check diag and anti-diag
        if(abs(diag)==BOARD_SIZE){return (cells[0][0]==X)? WIN_SCORE: -WIN_SCORE;} // whole diag same
        if(abs(anti_diag)==BOARD_SIZE){return (cells[0][BOARD_SIZE-1]==X)? WIN_SCORE: -WIN_SCORE;} // whole anti-diag same
    }

    int x_score=0, o_score=0;
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(cells[i][j]==X){
                x_score+=score[i][j];
            }
            else if(cells[i][j]==O){
                o_score+=score[i][j];
            }
        }
    }

    int x_weighting=BOARD_SIZE*BOARD_SIZE/2;
    int o_weighting=BOARD_SIZE*BOARD_SIZE-x_weighting;

    return x_score*x_weighting-o_score*o_weighting;

}

bool Board::play(const BoardCoordinate& coords){
    if(!coords.isValid()){return false;}
    if(cells[coords.row][coords.col]!=EMPTY){return false;}

    cells[coords.row][coords.col]=curPlayer;

    int cellValue=(cells[coords.row][coords.col]==X)? 1: 2;
    id += cellValue*pow(3, (coords.row*BOARD_SIZE+coords.col));

    curPlayer=(curPlayer==X)? O: X;

    return true;
}


