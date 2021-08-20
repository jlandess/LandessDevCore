//
// Created by phoenixflower on 2/13/21.
//

#ifndef LANDESSDEVCORE_PING_HPP
#define LANDESSDEVCORE_PING_HPP
#include "Process/drain.hpp"
#include "Process/reproc.hpp"
#include "Primitives/General/ctre.hpp"
#include <memory>
#include "Process/reproc.h"
#include "Primitives/General/StringView.hpp"
#include "Process/run.h"
namespace LD
{
    class Ping
    {
    private:
    public:
        Ping() noexcept
        {

        }

        void operator()(LD::StringView host) noexcept
        {
            int r = REPROC_ENOMEM;
            std::unique_ptr<reproc_t,reproc_t*(*)(reproc_t*)> process{reproc_new(),reproc_destroy};
            //this->mBuffer.get()[1] = (char*)host.data();
            const char *args[] = { "ping", "www.google.com", NULL };
            reproc_start(process.get(), args, (reproc_options){  });
            //sleep(5);

            char *output = NULL;
            size_t size = 0;
            for (;;) {
                uint8_t buffer[4096];
                r = reproc_read(process.get(), REPROC_STREAM_OUT, buffer, sizeof(buffer));
                if (r < 0) {
                    break;
                }

                // On success, `reproc_read` returns the amount of bytes read.
                size_t bytes_read = (size_t) r;

                // Increase the size of `output` to make sure it can hold the new output.
                // This is definitely not the most performant way to grow a buffer so keep
                // that in mind. Add 1 to size to leave space for the NUL terminator which
                // isn't included in `output_size`.
                char *result = static_cast<char *>(realloc(output, size + bytes_read + 1));
                if (result == NULL)
                {
                    r = REPROC_ENOMEM;
                }

                output = result;

                // Copy new data into `output`.
                memcpy(output + size, buffer, bytes_read);
                output[size + bytes_read] = '\0';
                size += bytes_read;
            }
            r = reproc_close(process.get(), REPROC_STREAM_IN);
            printf("%s", output);

            free(output);
            r = reproc_wait(process.get(), REPROC_INFINITE);
        }
    };
}
#endif //LANDESSDEVCORE_PING_HPP
