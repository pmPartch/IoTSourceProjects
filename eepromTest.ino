#include <EEPROM.h>

/*
 * This source file is available from my GitHub repository where you can easily copy it.
 * Go to https://github.com/pmPartch/IoTSourceProjects/blob/master/eepromTest.ino
 * and click on the Raw button where you can then copy the file.
 *
 * The supplied address range is checked (and will issue a warning if out of range)
 * The supplied data for write is checked (and will issue a warning if out of byte range)
 * There are 4 possible commands:
 *    write <address> <value>
 *    read <address>
 *    clear
 *    dumpall
 * all values are in decimal. The clear command will clear all of EEPROM memory and
 * dumpall will write all address value to serial output.
 * NOTE: any trailing text following the command syntax shown above is ignored (as long
 *       as there is a space between the last parameter and the trailing text)
 *       
 * WARNING: THIS PROGRAM DOES NOT WORK WITH THE 123D SIMULATOR (apparently, the       
 *          EEPROM.read call throws an exception???)
 */

 void issueUsageWarning(String input){
  Serial.println("Error in input string. Should either read <address> or write <address> <value>");
  Serial.print("Instead it was: '");
  Serial.print(input);
  Serial.println("'");
}

bool verifyAddress(int address)
{
  bool result = true;
  if (address <0 || address > 1023)
  {
    result = false;
    Serial.print("EEPROM address is out of range: ");
    Serial.println(address);
  }
  return result;
}

bool verifyData(int data)
{
  bool result = true;
  if (data < 0 || data > 255)
  {
    result = false;
    Serial.print("EEPROM data exceeds size of a byte: ");
    Serial.println(data);
  }
  return result;
}

void setup() {
  Serial.begin(9600);
}

bool output = false;

void loop() {
  String command;
  
  while (Serial.available())
  {
    command += Serial.readString();
    output = true;
  }
  
  if (output)
  {
    //Serial.println(command);
    output = false;

    //now attempt to parse it
    //first, convert string to lowercase and trim it
    command.toLowerCase();
    command.trim();
    if (command.startsWith("read "))
    {
      String addressString = command.substring(4); 
      addressString.trim();
      int address = addressString.toInt();

      if (verifyAddress(address))
      {
        Serial.print("read from address: ");
        Serial.print(address);
        Serial.print(" is: ");
        Serial.println(EEPROM.read(address));
      }
    } else if (command.startsWith("write "))
    {
      String addressAndValue = command.substring(5); //trim off leading and trailing whitespace

      addressAndValue.trim();
      
      int address = addressAndValue.toInt(); //will convert the first ineteger off the string and ignore the rest

      int index = addressAndValue.indexOf(' ');
      if (index >0)
      {
        int byteToWrite = addressAndValue.substring(index).toInt();
  
        if (verifyData(byteToWrite))
        {
          Serial.print("write to address: ");
          Serial.print(address);
          Serial.print(" with data: ");
          Serial.println(byteToWrite);
          EEPROM.update(address,byteToWrite);
        }
      } else {
        Serial.println("write command is missing a data value to write");
      }
    } else if (command.startsWith("clear")) 
    {
      int address = 0;
      for (address = 0 ; address<1024 ; address++)
      {
        EEPROM.update(address,0);
      }
    } else if (command.startsWith("dumpall"))
    {
      int address = 0;
      for (address = 0 ; address<1024 ; address++)
      {
        Serial.print(address);
        Serial.print(":");
        Serial.println(EEPROM.read(address));
      }
    } else {
      issueUsageWarning(command);
    }
  }
}
