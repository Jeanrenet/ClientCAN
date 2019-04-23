#ifndef CBUSCANINTERFACE_H
#define CBUSCANINTERFACE_H

#include <QtSerialBus/QCanBus>
#include <QTimer>
#include <QDataStream>
#include <QDebug>

class CBusCanInterface : public QObject
{
    Q_OBJECT

    //exposition au QML des variables membres
    Q_PROPERTY(QStringList devicesList  MEMBER m_devicesList    NOTIFY devicesFound())
    Q_PROPERTY(bool streamingMode       MEMBER m_streamingMode  NOTIFY dataReceived())
    Q_PROPERTY(qint32 serverRXSpeed     MEMBER m_serverRXSpeed  NOTIFY dataReceived())

public:
    CBusCanInterface();
    ~CBusCanInterface();

    typedef enum {
        StreamingMode       = 0,
        LedBlue             = 1,
        LedGreen            = 2,
        LedRed              = 3,
        CANRXSpeed          = 4,
    }RequestProtocolDefinition;

public:
    Q_INVOKABLE bool init(const QString &a_pluginType, const QString &a_interface);
    Q_INVOKABLE void sendFrame(const QCanBusFrame &frame) const;
    Q_INVOKABLE void discoverPeakDevices();

    Q_INVOKABLE void setLedBlue(bool value);
    Q_INVOKABLE void setLedGreen(bool value);
    Q_INVOKABLE void setLedRed(bool value);
    Q_INVOKABLE void swithStreamingMode();

    Q_SIGNAL void devicesFound();
    Q_SIGNAL void dataReceived();
protected:
    Q_SLOT void errorOccurred(QCanBusDevice::CanBusError error);
    Q_SLOT void framesReceived();
    Q_SLOT void framesWritten();

    template<class Type> void writeToCAN(quint32 id, Type data)
    {
        QByteArray dataArray;
        QDataStream str(&dataArray, QIODevice::WriteOnly);
        str << data;
        QCanBusFrame frame;
        frame.setFrameId(id);
        frame.setPayload(dataArray);
        sendFrame(frame);
    }

    void computeData(quint32 id, QByteArray payload);
private:
    QCanBusDevice       *mp_canDevice{nullptr};
    QStringList         m_devicesList;
    bool                m_streamingMode{false};
    qint32              m_serverRXSpeed;
};

#endif // CBUSCANINTERFACE_H
