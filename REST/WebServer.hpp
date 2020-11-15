//
// Created by phoenixflower on 11/14/20.
//

#ifndef LANDESSDEVCORE_WEBSERVER_HPP
#define LANDESSDEVCORE_WEBSERVER_HPP
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <chrono>
#include <cstring>
#include <list>
#include <string>
#include <ctime>
#include <iostream>
#include "IO/EtcdBackingStore.h"
#include "IO/etcd/etcdcpp.h"
#include "IO/Database.hpp"
namespace LD
{
    class ProgramCount
    {
    private:
        LD::UInteger mCount;
    public:
        ProgramCount() = default;


        ProgramCount(const LD::UInteger & count) noexcept:mCount{count}
        {

        }
        LD::UInteger & Count() noexcept
        {
            return this->mCount;
        }
        const LD::UInteger & Count()  const noexcept
        {
            return this->mCount;
        }
    };
}
template<>
struct LD::CT::TypeDescriptor<LD::ProgramCount>
{
private:
    static constexpr auto AmountName = ctll::basic_fixed_string("Amount");
public:
    static constexpr auto ClassName = ctll::fixed_string{"ProgramCount"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<AmountName,LD::CT::SelectOverload<LD::UInteger & (ProgramCount::*)(),&ProgramCount::Count>(),LD::CT::SelectOverload<const LD::UInteger & (ProgramCount::*)() const,&ProgramCount::Count>()>
    >;


    static constexpr MemberList Members{  };

};
namespace LD
{

    namespace detail {

        struct static_pool
        {
            std::size_t size_;
            std::size_t refs_ = 1;
            std::size_t count_ = 0;
            char* p_;

            char*
            end()
            {
                return reinterpret_cast<char*>(this + 1) + size_;
            }

            explicit
            static_pool(std::size_t size)
                    : size_(size)
                    , p_(reinterpret_cast<char*>(this + 1))
            {
            }

        public:
            static
            static_pool&
            construct(std::size_t size)
            {
                auto p = new char[sizeof(static_pool) + size];
                return *(::new(p) static_pool{size});
            }

            static_pool&
            share()
            {
                ++refs_;
                return *this;
            }

            void
            destroy()
            {
                if(refs_--)
                    return;
                this->~static_pool();
                delete[] reinterpret_cast<char*>(this);
            }

            void*
            alloc(std::size_t n)
            {
                auto last = p_ + n;
                if(last >= end())
                    BOOST_THROW_EXCEPTION(std::bad_alloc{});
                ++count_;
                auto p = p_;
                p_ = last;
                return p;
            }

            void
            dealloc()
            {
                if(--count_)
                    return;
                p_ = reinterpret_cast<char*>(this + 1);
            }
        };

    } // detail


    template<class T>
    struct fields_alloc
    {
        detail::static_pool* pool_;

    public:
        using value_type = T;
        using is_always_equal = std::false_type;
        using pointer = T*;
        using reference = T&;
        using const_pointer = T const*;
        using const_reference = T const&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        template<class U>
        struct rebind
        {
            using other = fields_alloc<U>;
        };

#if defined(_GLIBCXX_USE_CXX11_ABI) && (_GLIBCXX_USE_CXX11_ABI == 0)
        // Workaround for g++
    // basic_string assumes that allocators are default-constructible
    // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56437
    fields_alloc() = default;
#endif

        explicit
        fields_alloc(std::size_t size)
                : pool_(&detail::static_pool::construct(size))
        {
        }

        fields_alloc(fields_alloc const& other)
                : pool_(&other.pool_->share())
        {
        }

        template<class U>
        fields_alloc(fields_alloc<U> const& other)
                : pool_(&other.pool_->share())
        {
        }

        ~fields_alloc()
        {
            pool_->destroy();
        }

        value_type*
        allocate(size_type n)
        {
            return static_cast<value_type*>(
                    pool_->alloc(n * sizeof(T)));
        }

        void
        deallocate(value_type*, size_type)
        {
            pool_->dealloc();
        }

#if defined(BOOST_LIBSTDCXX_VERSION) && BOOST_LIBSTDCXX_VERSION < 60000
        template<class U, class... Args>
    void
    construct(U* ptr, Args&&... args)
    {
        ::new(static_cast<void*>(ptr)) U(
            std::forward<Args>(args)...);
    }

    template<class U>
    void
    destroy(U* ptr)
    {
        ptr->~U();
    }
#endif

        template<class U>
        friend
        bool
        operator==(
                fields_alloc const& lhs,
                fields_alloc<U> const& rhs)
        {
            return &lhs.pool_ == &rhs.pool_;
        }

        template<class U>
        friend
        bool
        operator!=(
                fields_alloc const& lhs,
                fields_alloc<U> const& rhs)
        {
            return ! (lhs == rhs);
        }
    };


    namespace beast = boost::beast;         // from <boost/beast.hpp>
    namespace http = beast::http;           // from <boost/beast/http.hpp>
    namespace net = boost::asio;            // from <boost/asio.hpp>
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

    namespace my_program_state
    {
        std::size_t
        request_count()
        {
            static std::size_t count = 0;
            return ++count;
        }

        std::time_t
        now()
        {
            return std::time(0);
        }
    }


    class http_connection : public std::enable_shared_from_this<http_connection>
    {
    private:
        etcdcpp::etcd_host etcdHost{"192.168.149.170",2379};
        etcdcpp::etcd_session session{{etcdHost}};
    public:
        http_connection(tcp::socket socket)
                : socket_(std::move(socket))
        {
            std::string s = socket_.remote_endpoint().address().to_string();
            LD::BasicKeyedDatabase<etcdcpp::etcd_session> etcdKeyedDatabase{session};
            LD::QueryResult<LD::Variant<ProgramCount>(LD::StringView,LD::BasicKeyedDatabase<etcdcpp::etcd_session>&)> result =  etcdKeyedDatabase.Fetch(LD::StringView {s.c_str()},LD::CT::TypeList<ProgramCount>{},LD::StringView{s.c_str()},etcdKeyedDatabase);

            //etcdKeyedDatabase.Fetch(LD::StringView{},LD::CT::TypeList<ProgramCount>{},LD::StringView{});
            std::cout <<"Remote : " << s << std::endl;
            auto onError = [](const LD::Context<LD::TransactionError,LD::StringView,LD::BasicKeyedDatabase<etcdcpp::etcd_session>&> & context) noexcept
            {
                std::cout << "not found" << std::endl;
                auto & db = LD::Get(LD::Get<2>(context));
                const auto & viewOfKey = LD::Get(LD::Get<1>(context));
                db.Insert(LD::StringView{viewOfKey},ProgramCount{});
            };

            auto onSuccess = [](const LD::Context<LD::TransactionResult,ProgramCount,LD::StringView,LD::BasicKeyedDatabase<etcdcpp::etcd_session>&> & context) noexcept
            {

            };

            LD::MultiMatch(LD::Overload{onError,onSuccess},result);
        }

        // Initiate the asynchronous operations associated with the connection.
        void
        start()
        {
            read_request();
            check_deadline();
        }

    private:
        // The socket for the currently connected client.
        tcp::socket socket_;

        // The buffer for performing reads.
        beast::flat_buffer buffer_{8192};

        // The request message.
        http::request<http::dynamic_body> request_;

        // The response message.
        http::response<http::dynamic_body> response_;

        // The timer for putting a deadline on connection processing.
        net::steady_timer deadline_{
                socket_.get_executor(), std::chrono::seconds(60)};

        // Asynchronously receive a complete request message.

        void
        read_request()
        {
            auto self = shared_from_this();

            http::async_read(
                    socket_,
                    buffer_,
                    request_,
                    [self](beast::error_code ec,
                           std::size_t bytes_transferred)
                    {
                        boost::ignore_unused(bytes_transferred);
                        if(!ec)
                            self->process_request();
                    });
        }

        // Determine what needs to be done with the request message.
        void
        process_request()
        {
            response_.version(request_.version());
            response_.keep_alive(false);


            switch(request_.method())
            {
                case http::verb::get:
                    response_.result(http::status::ok);
                    response_.set(http::field::server, "Beast");
                    create_response();
                    break;

                default:
                    // We return responses indicating an error if
                    // we do not recognize the request method.
                    response_.result(http::status::bad_request);
                    response_.set(http::field::content_type, "text/plain");
                    beast::ostream(response_.body())
                            << "Invalid request-method '"
                            << std::string(request_.method_string())
                            << "'";
                    break;
            }

            write_response();
        }

        // Construct a response message based on the program state.
        void
        create_response()
        {
            std::cout << request_.target() << std::endl;

            std::string s = socket_.remote_endpoint().address().to_string();
            std::string t = socket_.local_endpoint().address().to_string();

            LD::BasicKeyedDatabase<etcdcpp::etcd_session> etcdKeyedDatabase{session};

            if(request_.target() == "/count")
            {
                LD::QueryResult<LD::Variant<ProgramCount>(LD::StringView,LD::BasicKeyedDatabase<etcdcpp::etcd_session>&)> result =  etcdKeyedDatabase.Fetch(LD::StringView {s.c_str()},LD::CT::TypeList<ProgramCount>{},LD::StringView{s.c_str()},etcdKeyedDatabase);

                auto onError = [](const LD::Context<LD::TransactionError,LD::StringView,LD::BasicKeyedDatabase<etcdcpp::etcd_session>&> & context) noexcept ->LD::UInteger
                {
                    std::cout << "not found" << std::endl;
                    auto & db = LD::Get(LD::Get<2>(context));
                    const auto & viewOfKey = LD::Get(LD::Get<1>(context));
                    db.Insert(LD::StringView{viewOfKey},ProgramCount{});
                    return 0;
                };

                auto onSuccess = [](const LD::Context<LD::TransactionResult,ProgramCount,LD::StringView,LD::BasicKeyedDatabase<etcdcpp::etcd_session>&> & context) noexcept -> LD::UInteger
                {



                    std::cout << "found program counter" << std::endl;
                    return LD::Get(LD::Get<1>(context)).Count();
                };

                auto count = LD::MultiMatch(LD::Overload{onError,onSuccess},result);
                response_.set(http::field::content_type, "text/html");
                //" <button name=\"subject\" type=\"submit\" value=\"fav_HTML\">button</button>"
                //<input type="text" id="name" name="name" required minlength="4" maxlength="8" size="10">
                beast::ostream(response_.body())
                        << "<!DOCTYPE html>\n"
                        << "<html lang=\"en-US\">\n"
                        << "<input type=\"date\" id=\"name\" value=\"name\" required minlength=\"4\" maxlength=\"8\" size=\"10\">"
                        <<  "<head><title>Request count</title></head>\n"
                        <<  "<body>\n"
                        <<  "<h1>Request count</h1>\n"
                        <<  "<p>There have been "
                        <<  count
                        << "\n"
                        <<  s
                        << "\n"
                        << t
                        <<  " requests so far.</p>\n"
                        <<  "</body>\n"
                        <<  "</html>\n";

                etcdKeyedDatabase.Insert(LD::StringView{s.c_str()},ProgramCount{count+1});
            }
            else if(request_.target() == "/time")
            {
                response_.set(http::field::content_type, "text/html");
                beast::ostream(response_.body())
                        <<  "<html>\n"
                        <<  "<head><title>Current time</title></head>\n"
                        <<  "<body>\n"
                        <<  "<h1>Current time</h1>\n"
                        <<  "<p>The current time is "
                        <<  my_program_state::now()
                        <<  " seconds since the epoch.</p>\n"
                        <<  "</body>\n"
                        <<  "</html>\n";
            }
            else
            {
                response_.result(http::status::not_found);
                response_.set(http::field::content_type, "text/plain");
                beast::ostream(response_.body()) << "File not found\r\n";
            }
        }

        // Asynchronously transmit the response message.
        void
        write_response()
        {
            auto self = shared_from_this();

            response_.content_length(response_.body().size());

            http::async_write(
                    socket_,
                    response_,
                    [self](beast::error_code ec, std::size_t)
                    {
                        self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                        self->deadline_.cancel();
                    });
        }

        // Check whether we have spent enough time on this connection.
        void
        check_deadline()
        {
            auto self = shared_from_this();

            deadline_.async_wait(
                    [self](beast::error_code ec)
                    {
                        if(!ec)
                        {
                            // Close socket to cancel any outstanding operation.
                            self->socket_.close(ec);
                        }
                    });
        }
    };

// "Loop" forever accepting new connections.
    void
    http_server(tcp::acceptor& acceptor, tcp::socket& socket)
    {
        acceptor.async_accept(socket,
                              [&](beast::error_code ec)
                              {
                                  if(!ec)
                                      std::make_shared<http_connection>(std::move(socket))->start();
                                  http_server(acceptor, socket);
                              });
    }
}
#endif //LANDESSDEVCORE_WEBSERVER_HPP
