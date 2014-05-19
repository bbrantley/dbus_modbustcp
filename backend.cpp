#include "backend.h"
//#define QS_LOG_DISABLE
#include "QsLog.h"

Backend::Backend(QObject *parent) :
	QObject(parent)
{
}

void Backend::modbusRequest(ADU * const modbusRequest)
{
	ADU * const reply =  modbusRequest;
	uint functionCode = modbusRequest->getFunctionCode();
	QByteArray data = modbusRequest->getData();

	switch(functionCode)
	{
	case PDU::ReadHoldingRegisters : //03 (0x03) Read Holding Registers
	{
		QLOG_TRACE() << "PDU::ReadHoldingRegisters";
		QLOG_TRACE() << "data = " << data.toHex().toUpper();
		int startAddress = modbusRequest->getAddres();
		quint16 quantity = modbusRequest->getQuantity();
		QLOG_TRACE() << "startAddress = " << startAddress;
		QLOG_TRACE() << "quantity = " << quantity;

		if (quantity == 0 || quantity > 125) {
			QLOG_ERROR() << "Illegal Data Value. Raising exception";
			modbusRequest->setExceptionCode(PDU::IllegalDataValue);
		} else {
			QByteArray replyData;
			emit getValues(startAddress, modbusRequest->getUnitID(), quantity, replyData);
			if (replyData.isEmpty()) {
				QLOG_ERROR() << "Illegal Data Address. Raising exception";
				modbusRequest->setExceptionCode(PDU::IllegalDataAddress);
			} else {
				reply->setData(replyData);
				QLOG_DEBUG() << replyData.toHex().toUpper();
			}
		}
		break;
	}
	default:
		QLOG_ERROR() << "Unknown Function. Raising exception";
		modbusRequest->setExceptionCode(PDU::IllegalFunction);
		break;
	}
	emit modbusReply(reply);
}