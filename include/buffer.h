#define BUFFER_END -1
#define BUFFER_FULL -2

class Buffer{
public:
    Buffer(int bufferSize)
        : bufferSize(bufferSize) {
        rxBuffer = new char[bufferSize];
    }
    ~Buffer() {
        delete[] rxBuffer;
    }

    int read();
    int write(char c);
    int length();
    int remove(int count);
    void clear();
private:
    int bufferSize;
    char* rxBuffer;
    volatile int head = 0;
    volatile int tail = 0;
};
