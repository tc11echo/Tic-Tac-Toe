#include "hashtable.h"

BoardOptimalMove BoardHashTable::getHashedMove(const unsigned long long id, const int depth){
    for(BoardHashNode* cur=table[id%TABLE_SIZE]; cur!=nullptr; cur=cur->next){
        if(cur->id==id && cur->depth>=depth){
            return cur->optimalMove;
        }
    }
    return BoardOptimalMove(); // for illegal
}

void BoardHashTable::updateTable(const unsigned long long id, const int depth, const BoardOptimalMove& optimalMove){
    if(table[id%TABLE_SIZE]==nullptr){ // first
        table[id%TABLE_SIZE]=new BoardHashNode(id, depth, optimalMove);
        return;
    }

    BoardHashNode* cur=table[id%TABLE_SIZE];
    BoardHashNode* prev=nullptr;
    for (; cur!=nullptr; prev=cur, cur=cur->next){
        if(cur->id==id && cur->depth<=depth){ // update exist
            cur->depth=depth;
            cur->optimalMove=optimalMove;
            return;
        }
    }

    // insert
    prev->next=new BoardHashNode(id, depth, optimalMove);
}

void BoardHashTable::clearTable(){
    for(int i=0; i<TABLE_SIZE;i++){
        BoardHashNode* cur=table[i];
        while(cur!=nullptr){
            BoardHashNode* temp=cur;
            cur=cur->next;
            delete temp;
        }
        table[i]=nullptr;
    }
}


