#include <exception>
#include <sstream>
#include <string>

class BadMemoryAccress : virtual public std::exception
{
protected:
    const char* message = "accessed bad memory segment";

public:
    BadMemoryAccress() { }

    virtual const char* what() const throw ()
    {
        return this->message;
    }

    virtual ~BadMemoryAccress() throw () {}
};
