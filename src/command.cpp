#include "command.h"

static CmdParser cmdParser;

void ParserLoop(bool *isRunning, long *setTime, long *lightMode, long *brightness)
{
  // use own buffer from serial input
  CmdBuffer<64> myBuffer;

  Serial.println("\nSetting mode");

  // Read line from Serial until timeout
  if (myBuffer.readFromSerial(&Serial, 30000))
  {

    if (cmdParser.parseCmd(&myBuffer) != CMDPARSER_ERROR)
    {
      String param;
      uint16_t paramCnt = cmdParser.getParamCount();

      if (paramCnt > 2)
      {
        Serial.println("Parser error! Check params count");
        return;
      }
      else if (paramCnt == 2)
      {
        param = cmdParser.getCmdParam(1);
      }
      // Check end

      // Cmd Exec start
      if (cmdParser.equalCommand("Start"))
      {
        *isRunning = true;
        Serial.println("Start lighting...");
      }
      else if (cmdParser.equalCommand("Time"))
      {
        long parsedTimeVal = param.toInt();
        if ((parsedTimeVal < 90) || (parsedTimeVal > 3600))
        {
          Serial.println("Time must be 90 < t <= 3600 [s]");
        }
        else
        {
          *setTime = parsedTimeVal;
          Serial.print("Time set: ");
          Serial.println(*setTime);
        }
      }
      else if (cmdParser.equalCommand("Mode"))
      {
        long parsedModeValue = param.toInt();

        if (parsedModeValue > 3 || parsedModeValue == 0)
        {
          Serial.println("Mode must be 1(Red to Blue) or 2(Red to Red");
        }
        else
        {
          *lightMode = parsedModeValue;
          Serial.print("Mode set: ");
          Serial.println(*lightMode);
        }
      }
      else if (cmdParser.equalCommand("Brightness"))
      {
        long parsedBrightValue = param.toInt();

        if (parsedBrightValue < 0 || parsedBrightValue > 100)
        {
          Serial.println("Brightness must be between 0 to 100");
        }
        else
        {
          *brightness = parsedBrightValue;
          Serial.print("Brightness set: ");
          Serial.println(*brightness);
        }
      }
      else
      {
        Serial.println("Unknown command!");
      }
    }
    else
    {
      Serial.println("Parser error! check command format");
    }
    Serial.print("Current Mode: ");
    Serial.print(*lightMode);
    Serial.print(", Time: ");
    Serial.print(*setTime);
    Serial.print("s, Brightness: ");
    Serial.println(*brightness);
  }
  else
  {
    Serial.println("TIMEOUT!");
  }
}
