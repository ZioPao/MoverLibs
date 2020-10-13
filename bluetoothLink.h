#include "Arduino.h"
#include "SerialTransfer.h"

class BluetoothLink
{
private:
    SerialTransfer transfer;

public:
    BluetoothLink();

    bool checkConnectionMaster();
    bool checkConnectionSlave();

    int16_t getData();
    void sendData(int16_t value);
};