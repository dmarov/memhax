#ifndef __DATA_MAPPER_H__
#define __DATA_MAPPER_H__

#include <string>
#include <sqlite3.h>
#include <vector>

class DataMapper {

private:
    sqlite3 *handle;

public:
    DataMapper() throw(std::exception);

    void appendSample(std::string id, std::byte* bytes, unsigned len, int offset, unsigned size) throw(std::exception);
    std::vector<std::byte*> selectSamples(std::string session_id, unsigned len);

    ~DataMapper();
private:
};

#endif
