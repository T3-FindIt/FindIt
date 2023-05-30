#ifndef IWRITER_HPP
#define IWRITER_HPP

class IWriter
{
public:
	virtual ~IWriter() = default;
	virtual void Begin();
	virtual int Write(char *message);

private:
};

#endif
