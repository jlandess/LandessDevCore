//
// Created by phoenixflower on 2/10/21.
//

#ifndef LANDESSDEVCORE_NDP_HPP
#define LANDESSDEVCORE_NDP_HPP
#include "Process/drain.hpp"
#include "Process/reproc.hpp"
#include "Primitives/General/ctre.hpp"
#include <memory>
#include "Process/reproc.h"
/**
 * 192.168.174.28 dev ens22 lladdr b6:1b:63:1d:6f:63 STALE
192.168.174.61 dev ens22 lladdr 14:9d:99:7b:36:25 STALE
50.7.252.138 dev ens18  FAILED
192.168.174.48 dev ens22 lladdr 3c:22:fb:6e:b6:5b STALE
192.168.174.81 dev ens22 lladdr ca:33:6d:05:a2:6f STALE
192.168.174.114 dev ens22 lladdr 50:32:37:84:d8:64 STALE
195.181.173.159 dev ens18  FAILED
192.168.174.73 dev ens22 lladdr f0:9f:c2:1d:df:dd STALE
192.168.174.78 dev ens22 lladdr 50:32:37:88:23:73 STALE
192.168.174.65 dev ens22 lladdr b0:c5:ca:33:cc:65 STALE
192.168.174.98 dev ens22 lladdr 48:b0:2d:13:ba:27 REACHABLE
192.168.174.131 dev ens22 lladdr da:15:85:2e:5f:6b STALE
192.168.152.28 dev ens19 lladdr 90:e2:ba:6d:be:a9 REACHABLE
192.168.174.50 dev ens22 lladdr 3e:84:93:3e:f4:c0 STALE
192.168.174.70 dev ens22 lladdr fc:ec:da:31:4c:9d STALE
192.168.174.128 dev ens22 lladdr 16:ce:c9:65:53:e9 STALE
195.181.173.159 dev ens19  FAILED
192.168.174.55 dev ens22 lladdr 74:c2:46:cf:b5:5d STALE
192.168.174.75 dev ens22 lladdr fc:ec:da:7d:03:6c STALE
2600:1700:6070:65df::7 dev ens22 lladdr b6:1b:63:1d:6f:63 router STALE
2600:1700:6070:65df:889e:8eff:feea:3167 dev ens22 lladdr 8a:9e:8e:ea:31:67 router STALE
2600:1700:6070:65df:6943:1ec3:ffc1:e827 dev ens22 lladdr ca:33:6d:05:a2:6f router REACHABLE
2600:1700:6070:65df::4 dev ens22 lladdr 70:85:c2:b9:18:65 router STALE
fe80::7285:c2ff:feb9:1865 dev ens22 lladdr 70:85:c2:b9:18:65 router STALE
fe80::889e:8eff:feea:3167 dev ens22 lladdr 8a:9e:8e:ea:31:67 router REACHABLE
fe80::7285:c2ff:fed6:82dc dev ens22 lladdr 70:85:c2:d6:82:dc router STALE
fe80::e2d5:5eff:fee6:b59f dev ens22 lladdr e0:d5:5e:e6:b5:9f router STALE
2600:1700:6070:65df::37 dev ens22 lladdr 70:85:c2:d6:82:dc router STALE
2600:1700:6070:65df:e44a:a4ff:fe49:d5ea dev ens22 lladdr e6:4a:a4:49:d5:ea STALE
fe80::b41b:63ff:fe1d:6f63 dev ens22 lladdr b6:1b:63:1d:6f:63 router STALE
2600:1700:6070:65df:a8ec:3fff:fe2d:f2e3 dev ens22 lladdr aa:ec:3f:2d:f2:e3 router STALE
fe80::e44a:a4ff:fe49:d5ea dev ens22 lladdr e6:4a:a4:49:d5:ea STALE
fe80::c833:6dff:fe05:a26f dev ens22 lladdr ca:33:6d:05:a2:6f router REACHABLE
fe80::a8ec:3fff:fe2d:f2e3 dev ens22 lladdr aa:ec:3f:2d:f2:e3 router STALE
2600:1700:6070:65df::13 dev ens22 lladdr e0:d5:5e:e6:b5:9f router STALE
 */

/**
 * #include "Network/NDP/ndp.h"
#include <arpa/inet.h>
enum verbosity_level {
    VERB1,
    VERB2,
    VERB3,
    VERB4,
};

#define DEFAULT_VERB VERB1
static int g_verbosity = DEFAULT_VERB;

static uint8_t flags = ND_OPT_NORMAL;
#define pr_err(args...) fprintf(stderr, ##args)
#define pr_outx(verb_level, args...)			\
	do {						\
		if (verb_level <= g_verbosity)		\
			fprintf(stdout, ##args);	\
	} while (0)
#define pr_out(args...) pr_outx(DEFAULT_VERB, ##args)
#define pr_out2(args...) pr_outx(VERB2, ##args)
#define pr_out3(args...) pr_outx(VERB3, ##args)
#define pr_out4(args...) pr_outx(VERB4, ##args)

static void pr_out_route_preference(enum ndp_route_preference pref)
{
    switch (pref) {
        case NDP_ROUTE_PREF_LOW:
            pr_out("low");
            break;
        case NDP_ROUTE_PREF_MEDIUM:
            pr_out("medium");
            break;
        case NDP_ROUTE_PREF_HIGH:
            pr_out("high");
            break;
    }
}

static void pr_out_lft(uint32_t lifetime)
{
    if (lifetime == (uint32_t) -1)
        pr_out("infinity");
    else
        pr_out("%us", lifetime);
}

static const char *str_in6_addr(struct in6_addr *addr, char * buf)
{
    return inet_ntop(AF_INET6, addr, buf, INET6_ADDRSTRLEN);
}

static void pr_out_hwaddr(unsigned char *hwaddr, size_t len)
{
    int i;

    for (i = 0; i < len; i++) {
        if (i)
            pr_out(":");
        pr_out("%02x", hwaddr[i]);
    }
    pr_out("\n");
}
#include <net/if.h>
#include <csignal>


static int msgrcv_handler_func(struct ndp *ndp, struct ndp_msg *msg, void *priv)
{
    char buf[INET6_ADDRSTRLEN];
    char ifname[IF_NAMESIZE];
    enum ndp_msg_type msg_type = ndp_msg_type(msg);
    int offset;

    if_indextoname(ndp_msg_ifindex(msg), ifname);
    pr_out("NDP payload len %zu, from addr: %s, iface: %s\n",
           ndp_msg_payload_len(msg),
           str_in6_addr(ndp_msg_addrto(msg), buf), ifname);

    //pr_out_hwaddr(ndp_msg_opt_tlladdr(msg,offset));
    int sslADDROffset = ndp_msg_next_opt_offset(msg,-1,NDP_MSG_OPT_SLLADDR);
    //pr_out_hwaddr(ndp_msg_opt_slladdr(msg, offset),ndp_msg_opt_slladdr_len(msg, sslADDROffset));


    ndp_msg_opt_for_each_offset(offset, msg, NDP_MSG_OPT_SLLADDR)
    {
        std::cout << "abc";
        pr_out("  Source linkaddr: ");
        pr_out_hwaddr(ndp_msg_opt_slladdr(msg, offset),
                      ndp_msg_opt_slladdr_len(msg, offset));
    }

    if (msg_type == NDP_MSG_RS) {
        pr_out("  Type: RS\n");
    } else if (msg_type == NDP_MSG_RA) {
        struct ndp_msgra *msgra = ndp_msgra(msg);

        pr_out("  Type: RA\n");
        pr_out("  Hop limit: %u\n", ndp_msgra_curhoplimit(msgra));
        pr_out("  Managed address configuration: %s\n",
               ndp_msgra_flag_managed(msgra) ? "yes" : "no");
        pr_out("  Other configuration: %s\n",
               ndp_msgra_flag_other(msgra) ? "yes" : "no");
        pr_out("  Default router preference: ");
        pr_out_route_preference(ndp_msgra_route_preference(msgra));
        pr_out("\n");
        pr_out("  Router lifetime: %us\n",
               ndp_msgra_router_lifetime(msgra));
        pr_out("  Reachable time: ");
        if (ndp_msgra_reachable_time(msgra)) {
            pr_out("%ums\n",
                   ndp_msgra_reachable_time(msgra));
        } else {
            pr_out("unspecified\n");
        }
        pr_out("  Retransmit time: ");
        if (ndp_msgra_retransmit_time(msgra)) {
            pr_out("%ums\n",
                   ndp_msgra_retransmit_time(msgra));
        } else {
            pr_out("unspecified\n");
        }

        ndp_msg_opt_for_each_offset(offset, msg, NDP_MSG_OPT_SLLADDR) {
            pr_out("  Source linkaddr: ");
            pr_out_hwaddr(ndp_msg_opt_slladdr(msg, offset),
                          ndp_msg_opt_slladdr_len(msg, offset));
        }
        ndp_msg_opt_for_each_offset(offset, msg, NDP_MSG_OPT_TLLADDR) {
            pr_out("  Target linkaddr: ");
            pr_out_hwaddr(ndp_msg_opt_tlladdr(msg, offset),
                          ndp_msg_opt_tlladdr_len(msg, offset));
        }
        ndp_msg_opt_for_each_offset(offset, msg, NDP_MSG_OPT_PREFIX) {
            uint32_t valid_time;
            uint32_t preferred_time;

            valid_time = ndp_msg_opt_prefix_valid_time(msg, offset);
            preferred_time = ndp_msg_opt_prefix_preferred_time(msg, offset);
            pr_out("  Prefix: %s/%u",
                   str_in6_addr(ndp_msg_opt_prefix(msg, offset), buf),
                   ndp_msg_opt_prefix_len(msg, offset));
            pr_out(", valid_time: ");
            if (valid_time == (uint32_t) -1)
                pr_out("infinity");
            else
                pr_out("%us", valid_time);
            pr_out(", preferred_time: ");
            if (preferred_time == (uint32_t) -1)
                pr_out("infinity");
            else
                pr_out("%us", preferred_time);
            pr_out(", on_link: %s",
                   ndp_msg_opt_prefix_flag_on_link(msg, offset) ? "yes" : "no");
            pr_out(", autonomous_addr_conf: %s",
                   ndp_msg_opt_prefix_flag_auto_addr_conf(msg, offset) ? "yes" : "no");
            pr_out(", router_addr: %s",
                   ndp_msg_opt_prefix_flag_router_addr(msg, offset) ? "yes" : "no");
            pr_out("\n");
        }
        ndp_msg_opt_for_each_offset(offset, msg, NDP_MSG_OPT_MTU)
            pr_out("  MTU: %u\n", ndp_msg_opt_mtu(msg, offset));
        ndp_msg_opt_for_each_offset(offset, msg, NDP_MSG_OPT_ROUTE) {
            pr_out("  Route: %s/%u",
                   str_in6_addr(ndp_msg_opt_route_prefix(msg, offset), buf),
                   ndp_msg_opt_route_prefix_len(msg, offset));
            pr_out(", lifetime: ");
            pr_out_lft(ndp_msg_opt_route_lifetime(msg, offset));
            pr_out(", preference: ");
            pr_out_route_preference(ndp_msg_opt_route_preference(msg, offset));
            pr_out("\n");
        }
        ndp_msg_opt_for_each_offset(offset, msg, NDP_MSG_OPT_RDNSS) {
            struct in6_addr *addr;
            int addr_index;

            pr_out("  Recursive DNS Servers: ");
            ndp_msg_opt_rdnss_for_each_addr(addr, addr_index, msg, offset) {
                if (addr_index != 0)
                    pr_out(", ");
                pr_out("%s", str_in6_addr(addr, buf));
            }
            pr_out(", lifetime: ");
            pr_out_lft(ndp_msg_opt_rdnss_lifetime(msg, offset));
            pr_out("\n");
        }
        ndp_msg_opt_for_each_offset(offset, msg, NDP_MSG_OPT_DNSSL) {
            char *domain;
            int domain_index;

            pr_out("  DNS Search List: ");
            ndp_msg_opt_dnssl_for_each_domain(domain, domain_index, msg, offset) {
                if (domain_index != 0)
                    pr_out(" ");
                pr_out("%s", domain);
            }
            pr_out(", lifetime: ");
            pr_out_lft(ndp_msg_opt_dnssl_lifetime(msg, offset));
            pr_out("\n");
        }
    } else if (msg_type == NDP_MSG_NS) {
        pr_out("  Type: NS\n");
    } else if (msg_type == NDP_MSG_NA) {
        pr_out("  Type: NA\n");
    } else if (msg_type == NDP_MSG_R) {
        pr_out("  Type: R\n");
    } else {
        return 0;
    }
    return 0;
}
static void empty_signal_handler(int signal)
{
}
static int run_main_loop(struct ndp *ndp)
{
    fd_set rfds;
    fd_set rfds_tmp;
    int fdmax;
    int ret;
    struct sigaction siginfo;
    sigset_t mask;
    int ndp_fd;
    int err = 0;

    sigemptyset(&siginfo.sa_mask);
    siginfo.sa_flags = 0;
    siginfo.sa_handler = empty_signal_handler;
    ret = sigaction(SIGINT, &siginfo, NULL);
    if (ret == -1) {
        pr_err("Failed to set SIGINT handler\n");
        return -errno;
    }
    ret = sigaction(SIGQUIT, &siginfo, NULL);
    if (ret == -1) {
        pr_err("Failed to set SIGQUIT handler\n");
        return -errno;
    }
    ret = sigaction(SIGTERM, &siginfo, NULL);
    if (ret == -1) {
        pr_err("Failed to set SIGTERM handler\n");
        return -errno;
    }

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTERM);

    ret = sigprocmask(SIG_BLOCK, &mask, NULL);
    if (ret == -1) {
        pr_err("Failed to set blocked signals\n");
        return -errno;
    }

    sigemptyset(&mask);

    FD_ZERO(&rfds);
    ndp_fd = ndp_get_eventfd(ndp);
    FD_SET(ndp_fd, &rfds);
    fdmax = ndp_fd + 1;

    for (;;) {
        rfds_tmp = rfds;
        ret = pselect(fdmax, &rfds_tmp, NULL, NULL, NULL, &mask);
        if (ret == -1) {
            if (errno == EINTR) {
                goto out;
            }
            pr_err("Select failed\n");
            err = -errno;
            goto out;
        }
        if (FD_ISSET(ndp_fd, &rfds_tmp)) {
            err = ndp_call_eventfd_handler(ndp);
            if (err) {
                pr_err("ndp eventfd handler call failed\n");
                return err;
            }
        }
    }
    out:
    return err;
}
static int run_cmd_monitor(struct ndp *ndp, enum ndp_msg_type msg_type,
                           uint32_t ifindex)
{
    int err;

    err = ndp_msgrcv_handler_register(ndp, &msgrcv_handler_func, msg_type,
                                      ifindex, NULL);
    if (err) {
        pr_err("Failed to register msgrcv handler\n");
        return err;
    }
    err = run_main_loop(ndp);
    ndp_msgrcv_handler_unregister(ndp, &msgrcv_handler_func, msg_type,
                                  ifindex, NULL);
    return err;
}
 */
namespace LD
{
    class NDP
    {
    private:
        std::unique_ptr<char*> mBuffer;
        static constexpr auto Regex = ctre::fixed_string{"(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])) dev [a-zA-Z0-9]+ lladr ^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$ router"};
    public:

        NDP()
        {
            this->mBuffer = std::unique_ptr<char*>{new char*[2]};
        }

        void operator()() const noexcept
        {
            int r = REPROC_ENOMEM;
            std::unique_ptr<reproc_t,reproc_t*(*)(reproc_t*)> process{reproc_new(),reproc_destroy};
            this->mBuffer.get()[0] = "ip";
            this->mBuffer.get()[1] = "neighbour";
            reproc_start(process.get(), this->mBuffer.get(), (reproc_options){ 0 });
            r = reproc_close(process.get(), REPROC_STREAM_IN);

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

            printf("%s", output);

            free(output);
            r = reproc_wait(process.get(), REPROC_INFINITE);
        }
    };
};
#endif //LANDESSDEVCORE_NDP_HPP
