//
// Created by phoenixflower on 8/12/20.
//

#ifndef LANDESSDEVCORE_STRAND_HPP
#define LANDESSDEVCORE_STRAND_HPP
#include "Thread.h"
namespace LD
{
    template <class T>
    class Monitor
    {
    private:
        mutable T m_t;
        mutable LD::Mutex m_mtx;

    public:
        using Type = T;
        Monitor() {}
        Monitor(T t_) : m_t(LD::Move(t_)) {}
        template <typename F>
        auto operator()(F && f) const -> decltype(f(m_t)) {
            //std::lock_guard<std::mutex> hold{m_mtx};
            LD::ScopeLock<LD::Mutex> hold{this->m_mtx};
            return f(m_t);
        }
    };

    template <typename Processor>
    class Strand
    {
    private:
        Processor& m_proc;
        struct Data
        {
            bool running = false;
            //std::queue<std::function<void()>> q;
        };
        Monitor<Data> m_data;
    public:
        Strand(Processor& proc) : m_proc(proc) {}

        Strand(const Strand&) = delete;
        Strand& operator=(const Strand&) = delete;
    };
}
#endif //LANDESSDEVCORE_STRAND_HPP
