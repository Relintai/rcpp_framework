#ifndef CRASH_HANDLER_H
#define CRASH_HANDLER_H

class CrashHandler {
public:
	virtual void enable();
	virtual void disable();

	bool is_enabled() const { return enabled; };

	static CrashHandler *get_singleton();

	CrashHandler();
	virtual ~CrashHandler();

protected:
	static CrashHandler *_self;

	bool enabled;
};

#endif
