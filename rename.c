
#include "key.h"


man of steel
hello
,,,.....
    ble_our_service_on_ble_evt(&m_our_service, p_ble_evt);
CB1n/source/CB1n_project/CB1n/pca10040/s132/iar/_build/



}
static void Output_Via_BLE(uint8_t *data) {
	data[data[1] + 2] = calculateCheckSum(data);
	sendDataToBLE(&m_our_service, data);
}

void Output_Via_UART(uint8_t *data) {
	data[data[1] + 2] = calculateCheckSum(data);

	for (int i = 0; i < data[1] +