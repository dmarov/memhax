#pragma once

namespace memhax {

struct NTHeaders
{
    char Signature[4];
    /* public IMAGE_FILE_HEADER FileHeader; */
    /* public IMAGE_OPTIONAL_HEADER OptionalHeader; */
};

}
