//
// Created by phoenixflower on 2/14/21.
//

#ifndef LANDESSDEVCORE_SSHCOMMANDSET_HPP
#define LANDESSDEVCORE_SSHCOMMANDSET_HPP
//#include "Definitions/Common.hpp"
namespace LD
{
    template<typename ... A>
    class SSHCommandSet
    {
    private:
        LD::Tuple<LD::Detail::Decay_T<A>...> mCommands;
    public:
        SSHCommandSet() noexcept
        {

        }
        SSHCommandSet(A && ... commands) noexcept:mCommands{LD::Forward<A>(commands)...}
        {

        }

        auto Commands() const noexcept
        {
            return this->mCommands;
        }
    };

    template<typename ... T> SSHCommandSet(T && ...) -> SSHCommandSet<T...>;
}
#endif //LANDESSDEVCORE_SSHCOMMANDSET_HPP
