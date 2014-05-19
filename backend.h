#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QTcpSocket>

#include "adu.h"

class Backend : public QObject
{
	Q_OBJECT
public:
	explicit Backend(QObject *parent = 0);

public slots:
	void modbusRequest(ADU * const request);

signals:
	void getValues(const int modbusAddress, const int unitID, const int quantity, QByteArray &replyData);
	void modbusReply(ADU * const reply);
};

#endif // BACKEND_H