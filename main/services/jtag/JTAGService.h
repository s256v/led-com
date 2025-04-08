//
// Created by sergey on 4/6/25.
//

#ifndef LED_COM_JTAGSERVICE_H
#define LED_COM_JTAGSERVICE_H

#define JTAG_READ_BUFFER_SIZE (512)

class JTAGService {
public:
    void init();

    void println(const char *str);
    void print(const char *str);
    void printf(const char *format, ...);
    void printBytes(char *buff, size_t length);

    void waitFor(const char *str);

    void readBytes(char *buff, size_t length);

private:
    char readBuffer[JTAG_READ_BUFFER_SIZE];
};


#endif //LED_COM_JTAGSERVICE_H
