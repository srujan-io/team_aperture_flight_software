#ifndef COMMAND_INTERFACE_H
#define COMMAND_INTERFACE_H

#include <Arduino.h>

class CommandInterface
{
public:

    CommandInterface();

    bool begin();

    void update();

    bool hasCommand() const;

    String getCommand();

    void sendResponse(
        const String& response
    );

private:

    String inputBuffer;

    String receivedCommand;

    bool commandReady;
};

#endif