#ifndef IWRITER_H
#define IWRITER_H
#include <MFRC522.h>

class IWriter
{
private:
public:
	virtual ~IWriter() = default;
    virtual void begin();
	virtual int Write();
};

#endif
