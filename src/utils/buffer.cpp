#include "buffer.h"

int Buffer::read(){
    if(head == tail)
        return BUFFER_END;
    char c = rxBuffer[tail];
    tail = (tail + 1) % bufferSize;
    return c;
}

int Buffer::write(char c){
    int next = (head + 1) % bufferSize;
    if(next == tail) 
        return BUFFER_FULL; // full buffer

    rxBuffer[head] = c;
    head = next;
    return 0;
}

int Buffer::length(){
    return (head - tail + bufferSize) % bufferSize;
}

int Buffer::remove(int count){
    if(this->length() < count) count = this->length();
    
    head = (head - count + bufferSize) % bufferSize;
    return count;
}

void Buffer::clear(){
    head = tail;
}
