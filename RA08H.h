#ifndef RA08H_h
#define RA08H_h

#include <Arduino.h>
#include <Stream.h>
#include <string>

enum JoinMode {
    OTAA = 0,
    ABP = 1
};

enum UplinkDownlinkMode {
    SAME = 1,
    DIFFERENT = 2
};

enum WorkMode {
    NORMAL = 2
};

enum DeviceClass {
    A = 0,
    B = 1,
    C = 2
};

enum DeviceStatus {
    IDLE = 0,
    SENDING = 1,
    SENDING_FAILED = 2,
    SENDING_SUCCEEDED = 3,
    JOIN_SUCCESS = 4,
    JOIN_FAILED = 5,
    NETWORK_ABNORMAL = 6,
    SENDING_SUCCEEDED_NO_DOWNLOAD = 7,
    SENDING_SUCCEEDED_DOWNLOAD_AVAILABLE = 8
};

enum SendMessageType {
    UMCONFIRMED = 0,
    CONFIRMED = 1
};

struct SendTimesResponse {
    SendMessageType sendMessageType;
    int maxSendTimes;
};

enum DataRate {
    SF12 = 0,
    SF11 = 1,
    SF10 = 2,
    SF9 = 3,
    SF8 = 4,
    SF7 = 5
};

enum ReportMode {
    NON_PERIODIC = 0,
    PERIODIC = 1
};

struct ReportResponse {
    ReportMode reportMode;
    int reportInterval;
};

enum TxPower {
    SEVENTEEN_DBM = 0,
    FIFHTEEN_DBM = 1,
    THIRDTEEN_DBM = 2,
    ELEVEN_DBM = 3,
    NINE_DBM = 4,
    SEVEN_DBM = 5,
    FIVE_DBM = 6,
    THREE_DBM = 7
};

struct ConnectionCheckResult {
    bool success;
    int demodMargin;
    int gateways;
    int rssi;
    int snr;
};

struct ReceiveWindowParameters {
    int rx1DrOffset;
    int rx2DataRate;
    long rx2Frequency;
};

enum LogLevel {
    LogLevel_OFF = 0,
    LogLevel_LOWEST = 1,
    LogLevel_LOW = 2,
    LogLevel_MEDIUM = 3,
    LogLevel_HIGH = 4,
    LogLevel_HIGHEST = 5
};

struct ReceiveDataResponse {
    bool hasReceivedData;
    long dataLength;
    byte* dataBytes;
};


class RA08H {

  public:
    RA08H(Stream& debugSerial, Stream& loraSerial);
    void begin();
    String executeCommand(String command, String terminalText = "ASR6601:~# ");
    bool executeCommandIsOK(String command, String terminalText = "ASR6601:~# ");
    String resultToValueString(String result);
    int resultToValueInt(String result);

    // 4.1.1 LoRaWAN General Command Sets
    
    String getManufacturerIdentification();
    String getModelIdentification();
    String getRevisionIdentification();
    String getProductSerialNumberIdentification();
    int getUartBaudRate();
    bool setUartBaudRate(int baudRate);
    
    // 4.1.2 LoRaWAN Network Related Parameter Setup Command Sets
    
    JoinMode getJoinMode();
    bool setJoinMode(JoinMode joinMode);
    String getDevEui();
    bool setDevEui(String devEui);
    String getAppEui();
    bool setAppEui(String appEui);
    String getAppKey();
    bool setAppKey(String appKey);
    String getDevAddr();
    bool setDevAddr(String devAddr);
    String getAppSkey();
    bool setAppSkey(String appSkey);
    String getNwkSkey();
    bool setNwkSkey(String nwkSkey);
    String getFreqBankMask();
    bool setFreqBankMask(String freqBankMask);
    UplinkDownlinkMode getUplinkDownlinkMode();
    bool setUplinkDownlinkMode(UplinkDownlinkMode uplinkDownlinkMode);
    bool addMulticast(String devAddr, String appSkey, String nwkSkey, int periodicity = 2, int datarate = 2);
    bool deleteMulticast(String devAddr);
    int getMulticastCount();
    
    // 4.1.3

    WorkMode getWorkMode();
    bool setWorkMode(WorkMode workMode);
    DeviceClass getDeviceClass();
    bool setDeviceClass(DeviceClass deviceClass);
    int getBatteryLevel();
    DeviceStatus getDeviceStatus();
    bool joinOtaa(bool start = true, bool autoJoin = true, int period = 8, int attempts = 2);
    int getPingSlotInfoRequst();
    bool setPingSlotInfoRequest(int pingSlotCycle);

    // 4.1.4

    bool sendData(uint8_t *payload, long length, bool confirm = true, int attempts = 2);
    ReceiveDataResponse receiveData();

    // 4.1.5

    SendMessageType getSendMessageType();
    bool setSendMessageType(SendMessageType sendMessageType);
    int getApplicationPort();
    bool setApplicationPort(int applicationPort);
    DataRate getDataRate();
    bool setDataRate(DataRate dataRate);
    int* getRssi(int frequencyBandNumber);
    SendTimesResponse getMaxSendTimes();
    bool setMaxSendTimes(SendMessageType sendMessageType, int maxSendTimes);
    ReportResponse getReportMode();
    bool setReportMode(ReportMode reportMode, int interval = 0);
    TxPower getTxPower();
    bool setTxPower(TxPower txPower);
    bool disableConnectionCheck();
    ConnectionCheckResult doConnectionCheck();
    bool doConnectionCheckOnUplink();
    bool isAdr();
    bool setAdr(bool adr);
    ReceiveWindowParameters getReceiveWindowParameters();
    bool setReceiveWindowParameters(ReceiveWindowParameters receiveWindowParameters);
    int getSendReceiveDelay();
    bool setSendReceiveDelay(int sendReceiveDelay);
    bool saveConfiguration();
    bool resetConfiguration();

    // 4.1.6 Other commands sets

    bool reboot(bool immediately);
    LogLevel getLogLevel();
    bool setLogLevel(LogLevel logLevel);

    // 4.1.7 Private commands sets

    bool isDeviceTripleTupleEncrypted();
    bool setDeviceTripleTupleEncryption(String encryptionKey);

  private:
    Stream* debugSerial;
    Stream* loraSerial;
    LogLevel logLevel = LogLevel_OFF;

};

#endif