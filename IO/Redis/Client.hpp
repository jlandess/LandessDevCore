#ifndef ETCDV3_CLIENT
#define ETCDV3_CLIENT

#include "Condition.hpp"
#include "Request.hpp"
//#include "rpc.grpc.pb.h"

#include <grpc++/grpc++.h>
#include <map>

using grpc::Channel;
using grpc::ClientContext;
using etcdserverpb::KV;
using etcdserverpb::Watch;
using etcdserverpb::Lease;
using etcdserverpb::PutRequest;
using etcdserverpb::PutResponse;
using etcdserverpb::DeleteRangeRequest;
using etcdserverpb::DeleteRangeResponse;
using etcdserverpb::RangeRequest;
using etcdserverpb::RangeResponse;
using etcdserverpb::WatchRequest;
using etcdserverpb::WatchCreateRequest;
using etcdserverpb::WatchResponse;
using etcdserverpb::TxnRequest;
using etcdserverpb::TxnResponse;
using grpc::Status;
using grpc::CompletionQueue;
using grpc::ClientAsyncReaderWriter;

//-------------
class Client {
//-------------

public:

    explicit Client(std::shared_ptr<Channel> channel)
    : m_kvStub(KV::NewStub(channel)),
      m_watchStub(Watch::NewStub(channel)),
      m_leaseStub(Lease::NewStub(channel))
    {
    }

    explicit Client(std::string address)
    : Client(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()))
    {
    }

    Status put(const std::string& key, const std::string& value){

        ClientContext context;
        PutResponse putResponse;

        std::unique_ptr<PutRequest> putRequest(new PutRequest());
        putRequest->set_key(key);
        putRequest->set_value(value);
        putRequest->set_prev_kv(false);
        Status putStatus = m_kvStub->Put(&context, *(putRequest.get()), &putResponse);
        return putStatus;
    }

    Status put(const std::string& key, const std::string& value, std::pair<std::string, std::string>& prevKV) {

        ClientContext context;
        PutResponse putResponse;

        std::unique_ptr<PutRequest> putRequest(new PutRequest());
        putRequest->set_key(key);
        putRequest->set_value(value);
        putRequest->set_prev_kv(true);
        Status putStatus = m_kvStub->Put(&context, *(putRequest.get()), &putResponse);

        prevKV.first = putResponse.prev_kv().key();
        prevKV.first = putResponse.prev_kv().value();
        return putStatus;
    }

    Status put(const std::string& key, const std::string& value, int64_t lease){

        ClientContext context;
        PutResponse putResponse;

        std::unique_ptr<PutRequest> putRequest(new PutRequest());
        putRequest->set_key(key);
        putRequest->set_value(value);
        putRequest->set_prev_kv(false);
        putRequest->set_lease(lease);
        Status putStatus = m_kvStub->Put(&context, *(putRequest.get()), &putResponse);
        return putStatus;
    }

    const std::string get(const std::string& key){

        ClientContext context;
        RangeRequest getRequest;
        RangeResponse getResponse;

        getRequest.set_key(key);

        Status getStatus = m_kvStub->Range(&context, getRequest , &getResponse);

        return getResponse.kvs(0).value();
    }

    Status getFromKey(const std::string& key, std::map<std::string, std::string>& pairs) {

        ClientContext context;
        RangeRequest getRequest;
        RangeResponse getResponse;

        getRequest.set_key(key);
        getRequest.set_range_end("\0", 1);

        Status getStatus = m_kvStub->Range(&context, getRequest , &getResponse);

        for(auto kv_item: getResponse.kvs()) {
            pairs[kv_item.key()] = kv_item.value();
        }

        return getStatus;
    }

    Status get(const std::string& key, std::map<std::string, std::string>& pairs) {

        ClientContext context;
        RangeRequest getRequest;
        RangeResponse getResponse;

        getRequest.set_key(key);
        std::string range_end (key);
        range_end.back()++;

        getRequest.set_range_end(range_end);

        Status getStatus = m_kvStub->Range(&context, getRequest , &getResponse);

        for(auto kv_item: getResponse.kvs()) {
            pairs[kv_item.key()] = kv_item.value();
        }

        return getStatus;
    }

    Status getKeys(const std::string& key, std::vector<std::string>& keys) {

        ClientContext context;
        RangeRequest getRequest;
        RangeResponse getResponse;

        getRequest.set_key(key);
        std::string range_end (key);
        range_end.back()++;

        getRequest.set_range_end(range_end);

        Status getStatus = m_kvStub->Range(&context, getRequest , &getResponse);

        for(auto kv_item: getResponse.kvs()) {
            keys.push_back(kv_item.key());
        }

        return getStatus;
    }
    const std::string del(const std::string key){

        ClientContext context;
        DeleteRangeRequest delRequest;
        DeleteRangeResponse delResponse;

        delRequest.set_key(key);
        delRequest.set_prev_kv(true);

        Status status = m_kvStub->DeleteRange(&context, delRequest , &delResponse);
        
        if(delResponse.prev_kvs_size()) {
            return delResponse.prev_kvs(0).value();
        } else {
            return "";
        }
    }


    Status createLease(uint64_t leaseId, uint64_t ttl) {

        ClientContext context;
        etcdserverpb::LeaseGrantRequest createLeaseRequest;
        etcdserverpb::LeaseGrantResponse createLeaseResponse;

        createLeaseRequest.set_id(leaseId);
        createLeaseRequest.set_ttl(ttl);

        Status createLeaseStatus = m_leaseStub->LeaseGrant(&context, createLeaseRequest, &createLeaseResponse);

        return createLeaseStatus;
    }

    void updateLease(uint64_t leaseId) {

        ClientContext context;
        etcdserverpb::LeaseKeepAliveRequest leaseKeepAliveRequest;
        etcdserverpb::LeaseKeepAliveResponse leaseKeepAliveResponse;


        auto stream = m_leaseStub->LeaseKeepAlive(&context);

        leaseKeepAliveRequest.set_id(leaseId);
        stream->Write(leaseKeepAliveRequest);
        stream->Read(&leaseKeepAliveResponse);
    }

    template <typename T>
    void watch(const std::string& key, T callback) {

        ClientContext context;
        CompletionQueue cq_;

        WatchRequest watchRequest;
        WatchResponse watchResponse;

        watchRequest.mutable_create_request()->set_key(key);
        auto stream = m_watchStub->Watch(&context);

        stream->Write(watchRequest);
        stream->Read(&watchResponse);

        while(true) {
            bool op = stream->Read(&watchResponse);
            if(!op) {
                std::cerr << "Connection with stream ended..."<< std::endl;
                break;
            }
            callback(stream, watchResponse);
        }
    }
    bool watchCancel(auto& stream, const int64_t watch_id) {

        WatchRequest watchRequest;
        WatchResponse watchResponse;

        watchRequest.mutable_cancel_request()->set_watch_id(watch_id);
        auto status = stream->Write(watchRequest);
        stream->Read(&watchResponse);

        return status;
    }

    template <typename Tc, typename Tsr, typename Tfr>
    Status transaction(Tc& conditions,
                       Tsr& successRequests,
                       Tfr& failureRequests) {

        ClientContext context;
        TxnRequest txnRequest;
        TxnResponse txnResponse;

        addConditions(conditions, &txnRequest);
        addRequests(successRequests, failureRequests, &txnRequest);

        Status stat = m_kvStub->Txn(&context, txnRequest , &txnResponse);
        return stat;
    }

    template <typename Tc, typename Tsr, typename Tfr, typename Cb>
    Status transaction(Tc& conditions,
                       Tsr& successRequests,
                       Tfr& failureRequests,
                       Cb callback) {

        ClientContext context;
        TxnRequest txnRequest;
        TxnResponse txnResponse;

        addConditions(conditions, &txnRequest);
        addRequests(successRequests, failureRequests, &txnRequest);

        Status stat = m_kvStub->Txn(&context, txnRequest , &txnResponse);

        callback(txnResponse, txnResponse.succeeded());

        return stat;
    }

private:

    void addConditions(auto conditions, auto txnRequest) {

        for(auto condition: conditions) {
            auto compare = txnRequest->add_compare();
            condition.populate(compare);
        }
    }

    void addRequests(auto successRequests, auto failureRequests, auto txnRequest) {

        for(auto successRequest: successRequests) {
             auto success = txnRequest->add_success();
             successRequest.populate(success);
        }

        for(auto failureRequest: failureRequests) {
            auto failure = txnRequest->add_failure();
            failureRequest.populate(failure);
        }
    }

    std::unique_ptr<KV::Stub> m_kvStub;
    std::unique_ptr<Watch::Stub> m_watchStub;
    std::unique_ptr<Lease::Stub> m_leaseStub;

};

#endif
