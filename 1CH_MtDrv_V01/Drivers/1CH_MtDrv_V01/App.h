/*
 * App.h
 *
 *  Created on: Oct 27, 2024
 *      Author: Sergen
 */
void MtDrv_Main(void);

/*
 * 1.    2.    3.    4.    5.    6.    7.    8. //SENDING
 * S_ID  D_ID  M_ROT M_ACC M_BRK ESTOP CRC_8 SYNC
 * 0-32  0-32  0-1   0-100 0-1   0-1   0-255 255 (0xFF)
 * 1.    2.    3.    4.    5.    6.    7.    8. //RECEIVING
 * S_ID  D_ID  M_ROT M_ACC ESTOP  BUS_I  BUS_V  SYNC
 * 0-32  0-32  0-1   0-255 0-1  0-60   0-31   255 (0xFF)
 *
 *
 */
