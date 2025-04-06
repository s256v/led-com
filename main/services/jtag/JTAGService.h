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

    void waitFor(const char *str);

private:
    char readBuffer[JTAG_READ_BUFFER_SIZE + 1];
};


#endif //LED_COM_JTAGSERVICE_H
