#include "bluetoothLink.h"

BluetoothLink::BluetoothLink()
{
  Serial.begin(38400);
  Serial1.begin(38400); // Default communication rate of the Bluetooth module
  transfer.begin(Serial1);
  Serial.println("Avviata comunicazione bluetooth");
}

//Check Connections

bool BluetoothLink::checkConnectionMaster()
{
  Serial1.write(1);

  while (Serial1.available() == 0)
  {
    //Serial.println(Serial1.available());
    Serial1.flush();
    Serial1.write(1);
    Serial.println("Waiting for BT connection");
  }

  //When the loop ends, it checks wheter or not it's a single bit
  if (Serial1.available() == 1)
  {
    Serial1.read();

    Serial.println("Connection confirmed");
    return true;
  }
  else
  {
    return false;
  }
}

bool BluetoothLink::checkConnectionSlave()
{
  while (Serial1.available() == 0)
  {
    //Serial.println(Serial1.available());

    Serial.println("Waiting for master");
  }

  if (Serial1.available() != 0)
  {
    Serial.println("Connection confirmed");
    Serial1.flush();  //flush
    Serial1.write(1); //sends confirmation
    delay(100);
    return true;
  }
  else
  {
    return false;
  }
}

int16_t BluetoothLink::getData()
{
  int16_t tmp;
  uint16_t receivedSize = 0;

  if (transfer.available())
  {
    receivedSize = transfer.rxObj(tmp, receivedSize);
    //Serial.println(tmp);
    return tmp;
  }
}

void BluetoothLink::sendData(int16_t value)
{
  uint16_t sendSize = 0;

  sendSize = transfer.txObj(value, sendSize); //2 bytes
  transfer.sendData(sendSize);
}

