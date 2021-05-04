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
    //Serial.println("Waiting for BT connection");
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
    ; //just wait
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

VectorInt16 BluetoothLink::getData(VectorInt16 * acc, VectorInt16 *gyr)
{

 // Serial.print("Entered getData");

  VectorInt16 tmp;
  int16_t test;
  int16_t receivedSize;


  if (transfer.available())   //todo force 12 bytes read only
  {
    transfer.rxObj(tmp, receivedSize);
    
    acc -> x = tmp.x;
    acc -> y = tmp.y;
    acc -> z = tmp.z;
    
    tmp = VectorInt16(0,0,0);  //force reset
    transfer.rxObj(tmp, receivedSize);

    gyr -> x = tmp.x;
    gyr -> y = tmp.y;
    gyr -> z = tmp.z;
  }
}

void BluetoothLink::sendData(VectorInt16 acc, VectorInt16 gyr)
{
  uint16_t sendSize = 0;
  //6 bytes to send a full vector
  //12 bytes in total for acc and gyr values

  sendSize = transfer.txObj(acc, sendSize); //6 bytes     
  sendSize = transfer.txObj(gyr, sendSize);
 // Serial.println(sendSize);

  sendSize = transfer.sendData(sendSize);

  // Serial.println();
  // Serial.print("Sent bytes, current bytes ->");
  // Serial.print(sendSize);

}

