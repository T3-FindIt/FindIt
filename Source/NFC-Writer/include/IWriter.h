#ifndef IWRITER_H
#define IWRITER_H

class IWriter
{
private:
public:
	virtual ~IWriter() = default;
	virtual void begin();
	virtual int Write();
};

#endif
