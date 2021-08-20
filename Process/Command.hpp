//
// Created by phoenixflower on 3/27/21.
//

#ifndef LANDESSDEVCORE_COMMAND_HPP
#define LANDESSDEVCORE_COMMAND_HPP
#include "drain.h"
#include <memory>
namespace LD
{
    class Command
    {
    private:
        //reproc_t *reproc_destroy
        std::unique_ptr<reproc_t ,reproc_t* (*)(reproc_t*)> mHandle;
    public:
        Command() noexcept:mHandle{std::unique_ptr<reproc_t , reproc_t*(*)(reproc_t*)>(
                reproc_new(), reproc_destroy)}
        {

        }

        void operator()() noexcept
        {
            int r = REPROC_ENOMEM;
            const char * execution[] = { "wg", "showconf","wg0", NULL };
            r = reproc_start(this->mHandle.get(), execution, (reproc_options){ 0 });
            if (r < 0)
            {
                return ;
            }

            r = reproc_close(this->mHandle.get(), REPROC_STREAM_IN);

            if (r < 0)
            {
                return ;
            }

            char *output = NULL;
            reproc_sink sink = reproc_sink_string(&output);

            r = reproc_drain(this->mHandle.get(), sink, REPROC_SINK_NULL);

            if (r < 0)
            {
                return ;
            }

            r = reproc_wait(this->mHandle.get(), REPROC_INFINITE);

            if (r < 0)
            {
                return ;
            }

            if (r < 0) {
                fprintf(stderr, "%s\n", reproc_strerror(r));
            }
            printf("%s", output);
            //reproc_free(output);
            //reproc_destroy(process);
        }
    };
}
#endif //LANDESSDEVCORE_COMMAND_HPP
