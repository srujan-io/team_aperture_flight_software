#include "commandInterface.h"


CommandInterface::CommandInterface()
{
    inputBuffer = "";
    receivedCommand = "";

    commandReady = false;
}


// ====================================================
// BEGIN
// ====================================================

bool CommandInterface::begin()
{
    inputBuffer = "";
    receivedCommand = "";

    commandReady = false;

    Serial.println(
        "Command Interface initialized."
    );

    return true;
}


// ====================================================
// UPDATE
// ====================================================

void CommandInterface::update()
{
    while (Serial.available())
    {
        char c = Serial.read();


        // --------------------------------------------
        // Carriage return / newline
        // --------------------------------------------

        if (c == '\r' || c == '\n')
        {
            if (inputBuffer.length() > 0)
            {
                receivedCommand =
                    inputBuffer;

                inputBuffer = "";

                commandReady = true;
            }

            continue;
        }


        // --------------------------------------------
        // Prevent runaway input
        // --------------------------------------------

        if (inputBuffer.length() < 200)
        {
            inputBuffer += c;
        }
        else
        {
            inputBuffer = "";

            Serial.println(
                "Command too long."
            );
        }
    }
}


// ====================================================
// COMMAND AVAILABLE
// ====================================================

bool CommandInterface::hasCommand() const
{
    return commandReady;
}


// ====================================================
// GET COMMAND
// ====================================================

String CommandInterface::getCommand()
{
    String command =
        receivedCommand;

    receivedCommand = "";

    commandReady = false;

    return command;
}


// ====================================================
// SEND RESPONSE
// ====================================================

void CommandInterface::sendResponse(
    const String& response)
{
    Serial.print(response);
}