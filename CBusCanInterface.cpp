#include "CBusCanInterface.h"
#include <QDataStream>

CBusCanInterface::CBusCanInterface()
{
}

CBusCanInterface::~CBusCanInterface()
{
    if (!mp_canDevice)
        return;

    mp_canDevice->disconnectDevice();
    delete mp_canDevice;
    mp_canDevice = nullptr;
}

void CBusCanInterface::discoverPeakDevices()
{
    //on nettoie a liste
    m_devicesList.clear();

    //on cherche l'ensemble des devices disponibles avec le plugin PEAK can
    QList<QCanBusDeviceInfo> devices = QCanBus::instance()->availableDevices("peakcan");

    foreach(QCanBusDeviceInfo device, devices)
    {
        m_devicesList.append(device.name());
    }

    //mise à jour
    Q_EMIT devicesFound();
}

void CBusCanInterface::setLedBlue(bool value)
{
    writeToCAN(LedBlue, value);
}

void CBusCanInterface::setLedGreen(bool value)
{
    writeToCAN(LedGreen, value);
}

void CBusCanInterface::setLedRed(bool value)
{
    writeToCAN(LedRed, value);
}

void CBusCanInterface::swithStreamingMode()
{
    writeToCAN(StreamingMode, true);
}

bool CBusCanInterface::init(const QString &a_pluginType, const QString &a_interface)
{
    if (mp_canDevice)
    {
        mp_canDevice->disconnectDevice();
        delete mp_canDevice;
    }

    QString errorString;

    //tentative de création du device can
    mp_canDevice = QCanBus::instance()->createDevice(a_pluginType,
                                                     a_interface,
                                                     &errorString);    
    if (!mp_canDevice)
    {
        qDebug() << __FUNCTION__ << errorString;//affichage de l'erreur

        return false; //impossible de créer le device CAN
    }
    else
    {
        mp_canDevice->connectDevice();

        //connexion des différents signaux provenant de la classe instanciée
        connect(mp_canDevice, &QCanBusDevice::errorOccurred, this, &CBusCanInterface::errorOccurred);
        connect(mp_canDevice, &QCanBusDevice::framesReceived, this, &CBusCanInterface::framesReceived);
        connect(mp_canDevice, &QCanBusDevice::framesWritten, this, &CBusCanInterface::framesWritten);

        qDebug() << __FUNCTION__ << a_interface << " avec le plugin "<< a_pluginType<< " est ouvert!";
        return true;
    }
}

void CBusCanInterface::errorOccurred(QCanBusDevice::CanBusError error)
{
    //gestion des erreurs
    switch (error) {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        qDebug()<< __FUNCTION__ << "Une erreur est apparue : "<< mp_canDevice->errorString();
        break;
    default:
        break;
    }
}

void CBusCanInterface::framesReceived()
{
    if (!mp_canDevice) // si null, on s'arrête
        return;

    //sinon on parcourt tous les messages disponibles
    while (mp_canDevice->framesAvailable())
    {
        //lecture de la donnée
        const QCanBusFrame frame = mp_canDevice->readFrame();

        QString data;

        //vérification du type de la donnée
        switch(frame.frameType())
        {
        case QCanBusFrame::ErrorFrame:
            data = mp_canDevice->interpretErrorFrame(frame);
            break;
        case QCanBusFrame::UnknownFrame:
        case QCanBusFrame::InvalidFrame:
            qDebug() << __FUNCTION__ << "Invalid/error Frame";
            break;
        case QCanBusFrame::DataFrame:
            computeData(frame.frameId(), frame.payload());
            break;
        case QCanBusFrame::RemoteRequestFrame:
            break;
        }
    }
}

void CBusCanInterface::computeData(quint32 id, QByteArray payload)
{
    switch(id)
    {
    case StreamingMode:
        m_streamingMode = payload.toInt();
        break;
    case CANRXSpeed:
        m_serverRXSpeed = payload.toInt();
        break;
    default:
        break;
    }

    Q_EMIT dataReceived();
}

void CBusCanInterface::framesWritten()
{
    //qDebug() << __FUNCTION__;
}

void CBusCanInterface::sendFrame(const QCanBusFrame &frame) const
{
    if (!mp_canDevice)
        return;

    mp_canDevice->writeFrame(frame);
}
