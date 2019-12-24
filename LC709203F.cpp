#include <LC709203F.h>

byte LC709203F::crc8ccitt(const void *data, size_t size) {
  byte val = 0;
  byte *pos = (byte *)data;
  byte *end = pos + size;
  while (pos < end) {
    val = CRC_TABLE[val ^ *pos];
    pos++;
  }
  return val;
}

uint16_t LC709203F::readReg16(uint8_t deviceAddress, uint8_t regAddress) {
  word data = 0;
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(deviceAddress, 2);
  byte lowByteData = Wire.read();
  byte highByteData = Wire.read();
  // Serial.print(lowByteData);
  // Serial.print(" ");
  // Serial.println(highByteData);
  data = word(highByteData, lowByteData);
  return uint16_t(data);
}

void LC709203F::writeReg16(uint8_t deviceAddress, uint8_t regAddress,
                           uint16_t data) {
  byte crcCalcArray[5];
  crcCalcArray[0] = 0x16;
  crcCalcArray[1] = regAddress;
  crcCalcArray[2] = lowByte(data);
  crcCalcArray[3] = highByte(data);
  crcCalcArray[4] = crc8ccitt(crcCalcArray, 4);
  // Point to device
  Wire.beginTransmission(deviceAddress);
  // Point to register
  Wire.write(regAddress);
  // Send low byte
  Wire.write(crcCalcArray[2]);
  // Send high byte
  Wire.write(crcCalcArray[3]);
  // Send CRC-8 byte
  Wire.write(crcCalcArray[4]);
  // Transmit
  Wire.endTransmission();
}

float LC709203F::getBatteryVoltage() {
  uint16_t data = 0;
  data = readReg16(LC709203F_ADDRESS, LC709203F_REG_CELL_VOLT);
  return float(data) / 1000;
}

uint16_t LC709203F::getBatteryLevel() {
  uint16_t data = 0;
  data = readReg16(LC709203F_ADDRESS, LC709203F_REG_RSOC);
  return data;
}

float LC709203F::getBatteryLevelAccu() {
  uint16_t data = 0;
  data = readReg16(LC709203F_ADDRESS, LC709203F_REG_ITE);
  return float(data) / 10.0;
}

void LC709203F::setProfile(uint16_t profile) {
  writeReg16(LC709203F_ADDRESS, LC709203F_REG_BAT_PROF, profile);
}

void LC709203F::setApa(uint16_t apa) {
  writeReg16(LC709203F_ADDRESS, LC709203F_REG_APA, apa);
}

void LC709203F::begin() {
  writeReg16(LC709203F_ADDRESS, LC709203F_REG_POWER, 0x0001);
  writeReg16(LC709203F_ADDRESS, LC709203F_REG_APA, 0x0001);
  writeReg16(LC709203F_ADDRESS, LC709203F_REG_BAT_PROF, 0x0000);
  writeReg16(LC709203F_ADDRESS, LC709203F_INIT_RSOC, 0xAA55);
  writeReg16(LC709203F_ADDRESS, LC709203F_REG_TEMP_METH, 0x0000);
  writeReg16(LC709203F_ADDRESS, LC709203F_REG_TEMP, 0x0BA6);
}
LC709203F::LC709203F() {}
