#include "Arduino.h"
#include "SerialTransfer.h"
#include "helper_3dmath.h"

class BluetoothLink
{
private:
    SerialTransfer transfer;

public:
    BluetoothLink();

    bool checkConnectionMaster();
    bool checkConnectionSlave();

    VectorInt16 getData(VectorInt16 *acc, VectorInt16* gyr);
    void sendData(VectorInt16 acc, VectorInt16 gyr);
};