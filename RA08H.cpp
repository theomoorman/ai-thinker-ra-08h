#include "RA08H.h"

RA08H::RA08H(Stream& debugSerial, Stream& loraSerial) {
    this->debugSerial = &debugSerial;
    this->loraSerial = &loraSerial;
}

void RA08H::begin() {
    this->logLevel = getLogLevel();
}

String RA08H::executeCommand(String command, String terminalText) {
    String response = "";
    if (logLevel != LogLevel_OFF) {
        debugSerial->println("DEBUG command: [" + command + "]");
    }
    loraSerial->println(command);
    long start = millis();
    while ((millis() - start) < 10000) {
        while (loraSerial->available()) {
            char c = loraSerial->read();
            response += c;
        }
        if (response.endsWith("ASR6601:~# \r\nASR6601:~# ") || 
            ((millis() - start) > 1000 && response.endsWith(terminalText))) {
            break;
        }
    }
    if (logLevel != LogLevel_OFF) {
        debugSerial->println("DEBUG response: [" + command + "]");
    }
    return response;
}

bool RA08H::executeCommandIsOK(String command, String terminalText) {
    return executeCommand(command, terminalText).indexOf("OK") != -1;
}

String RA08H::resultToValueString(String result) {
    int okPosition = result.indexOf("OK");
    if (okPosition != -1) {
        String requestResponse = result.substring(0, okPosition);
        requestResponse.trim();
        int valuePosition = requestResponse.indexOf('=');
        if (valuePosition == -1) {
            valuePosition = requestResponse.indexOf(':');
        }
        return requestResponse.substring(valuePosition + 1);
    } else {
        return "";
    }
}

int RA08H::resultToValueInt(String result) {
    return resultToValueString(result).toInt();
}

String RA08H::getManufacturerIdentification() {
    return resultToValueString(executeCommand("AT+CGMI?"));
}

String RA08H::getModelIdentification() {
    return resultToValueString(executeCommand("AT+CGMM?"));
}

String RA08H::getRevisionIdentification() {
    return resultToValueString(executeCommand("AT+CGMR?"));
}
    
String RA08H::getProductSerialNumberIdentification() {
    return resultToValueString(executeCommand("AT+CGSN?"));
}

int RA08H::getUartBaudRate() {
    return resultToValueInt(executeCommand("AT+CGBR?"));
}

bool RA08H::setUartBaudRate(int baudRate) {
    if (baudRate > 9600) {
        return false;
    }
    return executeCommandIsOK("AT+CGBR=" + String(baudRate));
}

JoinMode RA08H::getJoinMode() {
    return static_cast<JoinMode>(resultToValueInt(executeCommand("AT+CJOINMODE?")));
}

bool RA08H::setJoinMode(JoinMode joinMode) {
    return executeCommandIsOK("AT+CJOINMODE=" + String(joinMode));
}

String RA08H::getDevEui() {
    return resultToValueString(executeCommand("AT+CDEVEUI?"));
}

bool RA08H::setDevEui(String devEui) {
    if (devEui.length() != 16) {
        return false;
    }
    return executeCommandIsOK("AT+CDEVEUI=" + devEui);
}

String RA08H::getAppEui() {
    return resultToValueString(executeCommand("AT+CAPPEUI?"));
}

bool RA08H::setAppEui(String appEui) {
    if (appEui.length() != 16) {
        return false;
    }
    return executeCommandIsOK("AT+CAPPEUI=" + appEui);
}

String RA08H::getAppKey() {
    return resultToValueString(executeCommand("AT+CAPPKEY?"));
}

bool RA08H::setAppKey(String appKey) {
    if (appKey.length() != 32) {
        return false;
    }
    return executeCommandIsOK("AT+CAPPKEY=" + appKey);
}

String RA08H::getDevAddr() {
    return resultToValueString(executeCommand("AT+CDEVADDR?"));
}

bool RA08H::setDevAddr(String devAddr) {
    if (devAddr.length() != 8) {
        return false;
    }
    return executeCommandIsOK("AT+CDEVADDR=" + devAddr);
}

String RA08H::getAppSkey() {
    return resultToValueString(executeCommand("AT+CAPPSKEY?"));
}

bool RA08H::setAppSkey(String appSkey) {
    if (appSkey.length() != 32) {
        return false;
    }
    return executeCommandIsOK("AT+CAPPSKEY=" + appSkey);
}

String RA08H::getNwkSkey() {
    return resultToValueString(executeCommand("AT+CNWKSKEY?"));
}

bool RA08H::setNwkSkey(String nwkSkey) {
    if (nwkSkey.length() != 32) {
        return false;
    }
    return executeCommandIsOK("AT+CNWKSKEY=" + nwkSkey);
}

String RA08H::getFreqBankMask() {
    return resultToValueString(executeCommand("AT+CFREQBANDMASK?"));
}

bool RA08H::setFreqBankMask(String freqBankMask) {
    return executeCommandIsOK("AT+CFREQBANDMASK=" + freqBankMask);
}

UplinkDownlinkMode RA08H::getUplinkDownlinkMode() {
    return static_cast<UplinkDownlinkMode>(resultToValueInt(executeCommand("AT+CULDLMODE?")));
}

bool RA08H::setUplinkDownlinkMode(UplinkDownlinkMode uplinkDownlinkMode) {
    return executeCommandIsOK("AT+CULDLMODE=" + String(uplinkDownlinkMode));
}

bool RA08H::addMulticast(String devAddr, String appSkey, String nwkSkey, int periodicity, int datarate) {
    return executeCommandIsOK("AT+CADDMUTICAST=" + devAddr + "," + appSkey + "," + nwkSkey + "," + String(periodicity) + "," + String(datarate));
}

bool RA08H::deleteMulticast(String devAddr) {
    return executeCommandIsOK("AT+CDELMUTICAST=" + devAddr);
}

int RA08H::getMulticastCount() {
    return resultToValueInt(executeCommand("AT+CNUMMUTICAST?"));
}

WorkMode RA08H::getWorkMode() {
    return static_cast<WorkMode>(resultToValueInt(executeCommand("AT+CWORKMODE?")));
}

bool RA08H::setWorkMode(WorkMode workMode) {
    return executeCommandIsOK("AT+CWORKMODE=" + String(workMode));
}

DeviceClass RA08H::getDeviceClass() {
    return static_cast<DeviceClass>(resultToValueInt(executeCommand("AT+CCLASS?")));
}

bool RA08H::setDeviceClass(DeviceClass deviceClass) {
    return executeCommandIsOK("AT+CCLASS=" + String(deviceClass));
}

int RA08H::getBatteryLevel() {
    return resultToValueInt(executeCommand("AT+CBL?"));
}

DeviceStatus RA08H::getDeviceStatus() {
    return static_cast<DeviceStatus>(resultToValueInt(executeCommand("AT+CSTATUS?")));
}

bool RA08H::joinOtaa(bool start, bool autoJoin, int period, int attempts) {
    return executeCommand("AT+CJOIN=" + String(start) + "," + String(autoJoin) + "," + String(period) + "," + String(attempts), "Joined").endsWith("Joined");
}

int RA08H::getPingSlotInfoRequst() {
    return resultToValueInt(executeCommand("AT+CPINGSLOTINFOREQ?"));
}

bool RA08H::setPingSlotInfoRequest(int pingSlotCycle) {
    return executeCommandIsOK("AT+CPINGSLOTINFOREQ=" + String(pingSlotCycle));
}

bool RA08H::sendData(uint8_t *payload, long length, bool confirm, int attempts) {
    String response = "";
    String requestString = "AT+DTRX=" + String(confirm) + "," + String(attempts) + "," + String(length) + ",";
    for (uint8_t i = 0; i < length; i++) {
        if (payload[i] < 16) {
            requestString += "0";
        } 
        requestString += String(payload[i], HEX);
    }
    if (logLevel != LogLevel_OFF) {
        debugSerial->println("DEBUG command: [" + requestString + "]");
    }
    loraSerial->println(requestString);
    long start = millis();
    while ((millis() - start) < (10000 * attempts)) {
        while (loraSerial->available()) {
            char c = loraSerial->read();
            response += c;
        }
        if (response.indexOf("OK+RECV:") != -1) {
            delay(100);
            while (loraSerial->available()) {
                char c = loraSerial->read();
                response += c;
            }
            break;
        }
    }
    if (logLevel != LogLevel_OFF) {
        debugSerial->println("DEBUG response: [" + response + "]");
    }
    return response.indexOf("OK+RECV:") != -1;
}

ReceiveDataResponse RA08H::receiveData() {
    if (logLevel != LogLevel_OFF) {
        debugSerial->println("Check for incoming message.");
    }
    String result = resultToValueString(executeCommand("AT+DRX?"));
    ReceiveDataResponse receiveDataResponse;
    receiveDataResponse.hasReceivedData = result.substring(0, result.indexOf(',') - 1).toInt() != 0;
    if (receiveDataResponse.hasReceivedData) {
        long length = result.substring(result.indexOf(',') + 1, result.lastIndexOf(',') - 1).toInt();
        byte buf[length];
        result.substring(result.lastIndexOf(',') + 1).getBytes(buf, length);
        receiveDataResponse.dataLength = length;
        receiveDataResponse.dataBytes = buf;
    }
    return receiveDataResponse;
}

SendMessageType RA08H::getSendMessageType() {
    return static_cast<SendMessageType>(resultToValueInt(executeCommand("AT+CCONFIRM?")));
}

bool RA08H::setSendMessageType(SendMessageType sendMessageType) {
    return executeCommandIsOK("AT+CCONFIRM=" + String(sendMessageType));
}

int RA08H::getApplicationPort() {
    return resultToValueInt(executeCommand("AT+CAPPPORT?"));
}

bool RA08H::setApplicationPort(int applicationPort) {
    return executeCommandIsOK("AT+CAPPPORT=" + String(applicationPort));
}

DataRate RA08H::getDataRate() {
    return static_cast<DataRate>(resultToValueInt(executeCommand("AT+CDATARATE?")));
}

bool RA08H::setDataRate(DataRate dataRate) {
    return setAdr(false) && ("AT+CDATARATE=" + String(dataRate));
}

int* RA08H::getRssi(int frequencyBandNumber) {
    String response = executeCommand("AT+CRSSI " + String(frequencyBandNumber) + "?");
    String result = response.substring(response.indexOf(':'), response.indexOf("OK"));
    result.replace('\r', ' ');
    result.replace('\n', ' ');
    result.trim();
    int groups[8];
    groups[0] = result.substring(result.indexOf("0:") + 2, result.indexOf("1:") - 2).toInt();
    groups[1] = result.substring(result.indexOf("1:") + 2, result.indexOf("2:") - 2).toInt();
    groups[2] = result.substring(result.indexOf("2:") + 2, result.indexOf("3:") - 2).toInt();
    groups[3] = result.substring(result.indexOf("3:") + 2, result.indexOf("4:") - 2).toInt();
    groups[4] = result.substring(result.indexOf("4:") + 2, result.indexOf("5:") - 2).toInt();
    groups[5] = result.substring(result.indexOf("5:") + 2, result.indexOf("6:") - 2).toInt();
    groups[6] = result.substring(result.indexOf("6:") + 2, result.indexOf("7:") - 2).toInt();
    groups[7] = result.substring(result.indexOf("7:") + 2).toInt();
    return groups;
}

SendTimesResponse RA08H::getMaxSendTimes() {
    String response = executeCommand("AT+CNBTRIALS?");
    String result = response.substring(response.indexOf(':') + 1, response.indexOf("OK"));
    result.trim();
    SendTimesResponse sendTimesResponse;
    sendTimesResponse.sendMessageType = static_cast<SendMessageType>(result.substring(0, result.indexOf(',' - 1)).toInt());
    sendTimesResponse.maxSendTimes = result.substring(result.indexOf(',') + 1).toInt();
    return sendTimesResponse;
}

bool RA08H::setMaxSendTimes(SendMessageType sendMessageType, int maxSendTimes) {
    return executeCommandIsOK("AT+CNBTRIALS=" + String(sendMessageType) + "," + String(maxSendTimes));
}

ReportResponse RA08H::getReportMode() {
    String response = executeCommand("AT+CRM?");
    String result = response.substring(response.indexOf('=') + 1, response.indexOf("OK"));
    result.trim();
    ReportResponse reportResponse;
    reportResponse.reportMode = static_cast<ReportMode>(result.substring(0, result.indexOf(',') - 1).toInt());
    reportResponse.reportInterval = result.substring(result.indexOf(',') + 1).toInt();
    return reportResponse;
}

bool RA08H::setReportMode(ReportMode reportMode, int interval) {
    return executeCommandIsOK("AT+CRM=" + String(reportMode) + "," + String(interval));
}

TxPower RA08H::getTxPower() {
    return static_cast<TxPower>(resultToValueInt(executeCommand("AT+CTXP?")));
}

bool RA08H::setTxPower(TxPower txPower) {
    return executeCommandIsOK("AT+CTXP=" + String(txPower));
}

bool RA08H::disableConnectionCheck() {
    return executeCommandIsOK("AT+CLINKCHECK=0");
}

ConnectionCheckResult RA08H::doConnectionCheck() {
    String response = "";
    if (logLevel != LogLevel_OFF) {
        debugSerial->println("DEBUG command: [AT+CLINKCHECK=1]");
    }
    loraSerial->println("AT+CLINKCHECK=1");
    long start = millis();
    while ((millis() - start) < 30000) {
        while (loraSerial->available()) {
            char c = loraSerial->read();
            response += c;
        }
        if (response.indexOf("OK+RECV:") != -1) {
            delay(100);
            while (loraSerial->available()) {
                char c = loraSerial->read();
                response += c;
            }
            break;
        }
    }
    if (logLevel != LogLevel_OFF) {
        debugSerial->println("DEBUG response: [" + response + "]");
    }
    String result = response.substring(response.indexOf("+CLINKCHECK: ") + 13);
    result = result.substring(0, result.indexOf('\r') - 1);
    result.replace(", ", ",");
    ConnectionCheckResult connectionCheckResult;
    connectionCheckResult.success = 0 == result.substring(0, result.indexOf(',') - 1).toInt();
    result = result.substring(result.indexOf(',') + 1);
    connectionCheckResult.demodMargin = result.substring(0, result.indexOf(',') - 1).toInt();
    result = result.substring(result.indexOf(',') + 1);
    connectionCheckResult.gateways = result.substring(0, result.indexOf(',') - 1).toInt();
    result = result.substring(result.indexOf(',') + 1);
    connectionCheckResult.rssi = result.substring(0, result.indexOf(',') - 1).toInt();
    connectionCheckResult.snr = result.substring(result.indexOf(',') + 1).toInt();
    return connectionCheckResult;
}

bool RA08H::doConnectionCheckOnUplink() {
    return executeCommandIsOK("AT+CLINKCHECK=2");
}

bool RA08H::isAdr() {
    return resultToValueInt(executeCommand("AT+CADR?"));
}

bool RA08H::setAdr(bool adr) {
    return executeCommandIsOK("AT+CADR=" + String(adr));
}

ReceiveWindowParameters RA08H::getReceiveWindowParameters() {
    String response = executeCommand("AT+CRM?");
    String result = response.substring(response.indexOf('=') + 1, response.indexOf("OK"));
    result.trim();
    ReceiveWindowParameters receiveWindowParameters;
    receiveWindowParameters.rx1DrOffset = result.substring(0, result.indexOf(',') - 1).toInt();
    receiveWindowParameters.rx2DataRate = result.substring(result.indexOf(',') + 1, result.lastIndexOf(',') - 1).toInt();
    receiveWindowParameters.rx2Frequency = result.substring(result.lastIndexOf(',') + 1).toInt();
    return receiveWindowParameters;
}

bool RA08H::setReceiveWindowParameters(ReceiveWindowParameters receiveWindowParameters) {
    return executeCommandIsOK("AT+CRXP=" + String(receiveWindowParameters.rx1DrOffset) + "," + String(receiveWindowParameters.rx2DataRate) + "," + String(receiveWindowParameters.rx2Frequency));
}

int RA08H::getSendReceiveDelay() {
    return resultToValueInt(executeCommand("AT+CRX1DELAY?"));
}

bool RA08H::setSendReceiveDelay(int sendReceiveDelay) {
    return executeCommandIsOK("AT+CRX1DELAY=" + String(sendReceiveDelay));
}

bool RA08H::saveConfiguration() {
    return executeCommandIsOK("AT+CSAVE");
}

bool RA08H::resetConfiguration() {
    return executeCommandIsOK("AT+CRESTORE");
}

bool RA08H::reboot(bool immediately) {
    return executeCommandIsOK("AT+IREBOOT=" + String(immediately));
}

LogLevel RA08H::getLogLevel() {
    return static_cast<LogLevel>(resultToValueInt(executeCommand("AT+ILOGLVL?")));
}

bool RA08H::setLogLevel(LogLevel logLevel) {
    if (executeCommandIsOK("AT+ILOGLVL=" + String(logLevel))) {
        this->logLevel = logLevel;
        return true;
    }
    return false;
}

bool RA08H::isDeviceTripleTupleEncrypted() {
    return resultToValueInt(executeCommand("AT+CKEYSPROTECT?"));
}

bool RA08H::setDeviceTripleTupleEncryption(String encryptionKey) {
    if (encryptionKey.length() != 32) {
        return false;
    }
    return executeCommandIsOK("AT+CKEYSPROTECT=" + encryptionKey);
}
