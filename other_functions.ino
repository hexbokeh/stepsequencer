void updateChannel()
{
  for (int i = 4; i < 6; i++)
  {
    lastButtonStates[i] = buttonStates[i];
    buttonStates[i] = digitalRead(buttonPins[i]);

    if (buttonStates[i] == HIGH && lastButtonStates[i] == LOW)
    {
      // do something in here.
      if (i == 4) // not 5!
      {
        Serial.print("i = ");
        Serial.println(i);
        Serial.println("going up");
        // up button code
        currentChannel++;
        currentChannel %= 3;
        updateLEDChannelIndicators();
      }
      if (i == 5) // not 6!
      {
        Serial.print("i = ");
        Serial.println(i);
        Serial.println("going down");
        // down button code
        currentChannel--;
        if (currentChannel < 0)
        {
          currentChannel = 2;
        }
        updateLEDChannelIndicators();
      }
    }
  }

  //debug
  //  Serial.print("CURRENT CHANNEL = ");
  //  Serial.println(currentChannel);
}

void checkButtons()
{
  for (int i = 0; i < 4; i++)
  {
    lastButtonStates[i] = buttonStates[i];
    buttonStates[i] = digitalRead(buttonPins[i]);

    if (buttonStates[i] == HIGH && lastButtonStates[i] == LOW)
    {
      if (stepStates[currentChannel][i] == false)
      {
        stepStates[currentChannel][i] = true;
      }
      else if (stepStates[currentChannel][i] == true)
      {
        stepStates[currentChannel][i] = false;
      }

      // if a change is made to any part of the sequence, EEPROM will update the entire multidimensional array.
      saveToEEPROM();
      // debug
      //      Serial.println("Change made to sequence; EEPROM updated");
      testEEPROM();
    }
  }
}

void updateLEDChannelIndicators() // seems to work properly.
{
  for (int i = 0; i < sizeof(ledChannelIndicators) / sizeof(ledChannelIndicators[0]); i++)
  {
    if (i == currentChannel)
    {
      digitalWrite(ledChannelIndicators[i], HIGH);
    }
    else
    {
      digitalWrite(ledChannelIndicators[i], LOW);
    }
  }
}

void saveToEEPROM()
{
  // debug
  Serial.println("new array values: ");

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      EEPROM.write(4*i+j, stepStates[i][j]); // arguments --> (address in memory, value)
      // debug
      Serial.print(stepStates[i][j]);
      Serial.print(" ");
    }
  }
  //debug
  Serial.println("I'm saved!");
}

void loadFromEEPROM()
{
  // debug
  Serial.println("EEPROM LOADED!");
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      stepStates[i][j] = EEPROM.read(4*i+j); // arguments --> (address in memory, value)

      //debug
      Serial.print(stepStates[i][j]);
      Serial.print(" ");
    }
  }
  // debug
  Serial.println();
}

// debugging
void testEEPROM()
{
  // debug
  Serial.println("TESTING EEPROM:");
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      //debug
      Serial.print(stepStates[i][j]);
      Serial.print(" ");
    }
  }
  // debug
  Serial.println();
}

