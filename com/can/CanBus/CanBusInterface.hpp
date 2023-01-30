#pragma once

class CanBusInterface {
	
	virtual void joinWriter() = 0;
	virtual void detachWriter() = 0;
	
	virtual void joinReader() = 0;
	virtual void detachReader() = 0;

};
