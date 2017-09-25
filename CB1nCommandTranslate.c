#include <string.h>
#include "CB1nCommandTranslate.h"
#include "app_uart.h"
#include "motorDriver.h"
#include "buzzer.h"
#include "LED.h"
#include "misc.h"
#include "nrf_delay.h"

// #include "nrf_sdm.h"
#include "ble_gap.h"

// #define FPU_EXCEPTION_MASK 0x0000009F

static uint8_t calculateCheckSum(uint8_t *data);
bool is_advertising = true;
uint8_t command;
uint8_t BLEData[20];

uint8_t Motor, value;
extern uint8_t write_flag;
uint8_t write_BLE_Name = 0;
uint8_t BLE_Name[14]; //Max 14 byte

//motor
uint8_t indexPoint;
uint8_t dataLength;
uint8_t direction;
uint16_t runTime;
// extern APP_TIMER_DEF(m_BLE_sleep_timer_id);

//CommandTranslate(...) With perfix and checksum
void CommandTranslate(uint8_t *rawData, uint8_t rawDataLength) {
	// log("\r\n");
	// for (uint32_t i = 0; i < rawDataLength; i++)
	// {
	// 	log("%#x ", rawData[i]);
	// }
	// log("\r\n");

	// log("checksum = %#x ", calculateCheckSum(rawData));

	uint8_t dataLen = rawData[1];
	//TODO must to recognize where data come from
	if ( (calculateCheckSum(rawData) == rawData[dataLen + 2] ) ) {

		log("CKS OK: %x\r\n", calculateCheckSum(rawData));

		memset(BLEData, 0, sizeof(BLEData));
		memcpy( BLEData, rawData + 2, 17);

		switch (BLEData[COMMAND]) {
		case LED_FUNCTION: //0xE6
			log("LED_FUNCTION\r\n");
			set_LED(BLEData[1], BLEData[2]);
			// nrf_gpio_cfg_output(16);
			// nrf_gpio_pin_set(16);
			// nrf_gpio_pin_clear(16);
			break;

		case 0xE3:			//0xE0~ 0xE3
		case 0xE2:
		case 0xE1:
		case ANALOG_MESSAGE:
			log("ANALOG_MESSAGE\r\n");

			Motor = BLEData[COMMAND] & 0x0F;
			value = BLEData[LSB] + (BLEData[HSB] << 7); //LSB +HSB

			log("set motor %#x \r\n", Motor);
			log("set value %#x \r\n", value);

			setMotor(Motor, value);

			break;
		case MOTOR_SENSE_FUNCTION:
			uint8_t senseTime;
			senseTime =  BLEData[1];
			log("MSF: %d\r\n", senseTime * 100);
			setMotorSenseTimer(senseTime);

			break;
		case MULTE_MOTOR_TIMER_FUNCTION:
			log("MMT_F\r\n");

			dataLength = rawData[1];
			if (dataLength != 0)
			{
				indexPoint = 0;
				while ( indexPoint + 1 < dataLength) {

					value = BLEData[2 + indexPoint];
					Motor = BLEData[COMMAND + 1 + indexPoint] & 0x07;
					runTime = (BLEData[3 + 1 + indexPoint] << 8) | (BLEData[2 + 1 + indexPoint]) ;
					direction = (BLEData[COMMAND + 1 + indexPoint] & 0x08) >> 3;

					// log("MMT_V: %d\r\n", value);
					// log("MMT_M: %d\r\n", Motor);
					// log("MMT_LSB: %d\r\n", BLEData[2 + 1 + indexPoint]);
					// log("MMT_HSB: %d\r\n", BLEData[3 + 1 + indexPoint]);

					switch (Motor) {
					case 0:
						setMotor(Motor, value);
						setMotorDirection(Motor, direction);
						setMotorRunTime(Motor, runTime);
						break;
					case 1:
						setMotor(Motor, value);
						setMotorDirection(Motor, direction);
						setMotorRunTime(Motor, runTime);
						break;
					case 2:
						setMotor(Motor, value);
						setMotorDirection(Motor, direction);
						setMotorRunTime(Motor, runTime);
						break;
					case 3:
						setMotor(Motor, value);
						setMotorDirection(Motor, direction);
						setMotorRunTime(Motor, runTime);
						break;
					}
					indexPoint += 4;//per motor control data length.
				}
			}
			break;
		case MULTE_MOTOR_FUNCTION:
			log("MULTE_MOTOR_FUNCTION\r\n");
			// for multi motor
			// uint8_t indexPoint;
			// uint8_t dataLength;
			// uint8_t direction;

			dataLength = rawData[1];
			if (dataLength != 0)
			{
				indexPoint = 0;
				while ( indexPoint + 1 < dataLength) {

					value = BLEData[2 + indexPoint];
					Motor = BLEData[COMMAND + 1 + indexPoint] & 0x07;
					direction = (BLEData[COMMAND + 1 + indexPoint] & 0x08) >> 3;

					switch (Motor) {
					case 0:
						setMotor(Motor, value);
						setMotorDirection(Motor, direction);
						break;
					case 1:
						setMotor(Motor, value);
						setMotorDirection(Motor, direction);
						break;
					case 2:
						setMotor(Motor, value);
						setMotorDirection(Motor, direction);
						break;
					case 3:
						setMotor(Motor, value);
						setMotorDirection(Motor, direction);
						break;
					}
					indexPoint += 2;
				}
			}

			break;
		case SET_PIN_VALUE:
			setMotorDirection(BLEData[1], BLEData[2]);
			break;
		case BUZZER_FUNCTION:
			// log("BUZZER_FUNCTION\r\n");
			uint32_t buzzerfreq;
			uint32_t buzzerTime;
			buzzerfreq = (BLEData[2] << 8) + BLEData[1];
			buzzerTime = BLEData[3];
			// log("b frq = %d\r\n",buzzerfreq);
			// log("delay = %d\r\n",buzzerTime*10);
			setBuzzerFreq(buzzerfreq, buzzerTime);

			break;
		case START_SYSEX:
			switch (BLEData[1]) {
			case REPORT_FIRMWARE:
				log("REPORT_FIRMWARE\r\n");
				// app_uart_put(DEVICE_NAME);
				Output_FW_Version();
				break;
			case REPORT_UUID:
				log("REPORT_UUID\r\n");
				Output_UUID();
				break;
			case REPORT_MAC_ADDR:
				log("REPORT_MAC_ADDR\r\n");
				Output_MAC_ADDR();
				break;
			case REPORT_SIGNATURE:
				log("REPORT_SIGNATURE\r\n");
				break;
			}
			break;
		case SET_PIN_MODE:
			log("SET_PIN_MODE\r\n");
			break;
		case SET_BLE_NAME:
			log("SET_BLE_NAME\r\n");

			uint8_t printBuffer[50];
			memcpy( BLE_Name, BLEData + 2 , dataLen - 2);
			sprintf(printBuffer, "%s", BLE_Name);
			log("%s \r\n", printBuffer);
			write_BLE_Name = 1;

			// FLASH_EreasePage(ADDR_SAVEDATA);
			// FLASH_WriteByte(ADDR_BLE_NAME , BLE_Name , 14);
			// restoreFlashData();

			break;
		case FUNCTION_OUTPUT_ENABLE:
			FUNCTION_OUTPUT_STATUS( (BLEData[2] << 8) | BLEData[1]);
			log("data_filter:%x\r\n", (BLEData[2] << 8) | BLEData[1]);
			break;
		case FW_UPDATE_FLAG_FUCTION:
			log("NVIC_SystemReset\r\n");
			if (BLEData[1] == 0x01)
			{
				NVIC_SystemReset();
			}
			break;
		}

		log("Finish\r\n");

	} else { // check sum is not correct
		log("check sum FAIL\r\n");
		return;
	}


}
void verifyUARTinData(app_uart_evt_t * p_event)
{
	static uint8_t data_array[20];//20 is to match BLE per package
	static uint8_t index = 0;

	if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
	{
		APP_ERROR_HANDLER(p_event->data.error_communication);
	}
	else if (p_event->evt_type == APP_UART_FIFO_ERROR)
	{
		APP_ERROR_HANDLER(p_event->data.error_code);
	}
	else if (p_event->evt_type ==  APP_UART_DATA_READY)
	{
		uint8_t cr;
		UNUSED_VARIABLE(app_uart_get(&data_array[index]));
		// while (app_uart_get(&data_array[index]) != NRF_SUCCESS);

		if ((data_array[0] == 0x55) || (data_array[0] == 0x77)) {
			if (index == 1)
			{
				if ((data_array[1] == 0x00) || (data_array[1] > 17))
				{
					index = 0;
					return;
				}
			}
			index++;
		}

		if ((data_array[1] == (index - 3)) && (data_array[1] <= 17))
		{
			//print uart received data
			// for (int i = 0; i < index; i++)
			// {
			// 	while (app_uart_put(data_array[i]) != NRF_SUCCESS);
			// }//------

			//disable BLE advertising
			if (is_advertising)
			{
				uint32_t err_code;
				err_code = sd_ble_gap_adv_stop();
				if (err_code == NRF_SUCCESS)
				{
					is_advertising = false;
					BleStatusLed(LED_ON);
					stopSleepTimer();
				}
			}

			//set data from UART and to disable out data via BLE
			//set out via uart
			Set_Out_Channel(COMMAND_FROM_USB);
			CommandTranslate(data_array, data_array[1]);

			// __set_FPSCR(__get_FPSCR()  & ~(FPU_EXCEPTION_MASK));
			// (void) __get_FPSCR();
			// NVIC_ClearPendingIRQ(FPU_IRQn);
			// sd_softdevice_disable();

			memset(&data_array, 0, sizeof(data_array));
			index = 0;
		}

		// index++;
		// if (index == 5)
		// {
		//     log("%s", data_array);
		//     // memset(&data_array, 0, sizeof(data_array));

		//     index = 0;
		// }


		// index++;
		// if (data_array[index - 1] == '\n') {
		//     log("%s", data_array);
		//     log("%d", index - 2);
		//     memset(&data_array, 0, sizeof(data_array));
		//     index = 0;
		// }


		// while (app_uart_get(&cr) != NRF_SUCCESS);
		// // log("%x", cr);
		// log("%c", cr);
		// if (cr == 'q') {
		//     log("\n\rquit\n\r");
		// }
	}
}

uint8_t calculateCheckSum(uint8_t *data)
{
	uint8_t checkSum = 0;
	uint8_t length = data[1] + 2;
	for (uint8_t i = 0; i < length; i++) {
		checkSum = checkSum + data[i];
	}
	return checkSum;
}

// bool isAdvertising(void) {
// 	if (is_advertising)
// 	{
// 		return true;
// 	} else {
// 		return false;
// 	}
// }

void BLE_Rename_Handler(void) {
	if (write_BLE_Name == 1)
	{
		fds_test_find_and_delete();
		// uint8_t sData1[14] = "0.9.2B1    ABC";
		fds_test_write(0x1111, 0x2222, BLE_Name, BYTES_TO_WORDS(sizeof(BLE_Name)));
		write_BLE_Name = 0;
	}
	if (write_flag == 1)
	{
		Output_BLE_ACK();
		// uint8_t gData[14];
		// fds_read(0x1111, 0x2222, gData);
		// log("gData = %s\r\n", gData);
		write_flag = 0;
	}
}