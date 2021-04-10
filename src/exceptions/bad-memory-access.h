#include <exception>
#include <sstream>
#include <string>

namespace memhax {

class BadMemoryAccess : virtual public std::exception
{
protected:
    const char* message = "accessed bad memory segment";

public:
    BadMemoryAccess() { }

    virtual const char* what() const throw ()
    {
        return this->message;
    }

    virtual ~BadMemoryAccess() throw () {}
};

}
