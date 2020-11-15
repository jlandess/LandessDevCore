//
// Created by phoenixflower on 9/22/20.
//
/**
 *     S3StatusInternalError                                   ,
    S3StatusOutOfMemory                                     ,
    S3StatusInterrupted                                     ,
    S3StatusInvalidBucketNameTooLong                        ,
    S3StatusInvalidBucketNameFirstCharacter                 ,
    S3StatusInvalidBucketNameCharacter                      ,
    S3StatusInvalidBucketNameCharacterSequence              ,
    S3StatusInvalidBucketNameTooShort                       ,
    S3StatusInvalidBucketNameDotQuadNotation                ,
    S3StatusQueryParamsTooLong                              ,
    S3StatusFailedToInitializeRequest                       ,
    S3StatusMetaDataHeadersTooLong                          ,
    S3StatusBadMetaData                                     ,
    S3StatusBadContentType                                  ,
    S3StatusContentTypeTooLong                              ,
    S3StatusBadMD5                                          ,
    S3StatusMD5TooLong                                      ,
    S3StatusBadCacheControl                                 ,
    S3StatusCacheControlTooLong                             ,
    S3StatusBadContentDispositionFilename                   ,
    S3StatusContentDispositionFilenameTooLong               ,
    S3StatusBadContentEncoding                              ,
    S3StatusContentEncodingTooLong                          ,
    S3StatusBadIfMatchETag                                  ,
    S3StatusIfMatchETagTooLong                              ,
    S3StatusBadIfNotMatchETag                               ,
    S3StatusIfNotMatchETagTooLong                           ,
    S3StatusHeadersTooLong                                  ,
    S3StatusKeyTooLong                                      ,
    S3StatusUriTooLong                                      ,
    S3StatusXmlParseFailure                                 ,
    S3StatusEmailAddressTooLong                             ,
    S3StatusUserIdTooLong                                   ,
    S3StatusUserDisplayNameTooLong                          ,
    S3StatusGroupUriTooLong                                 ,
    S3StatusPermissionTooLong                               ,
    S3StatusTargetBucketTooLong                             ,
    S3StatusTargetPrefixTooLong                             ,
    S3StatusTooManyGrants                                   ,
    S3StatusBadGrantee                                      ,
    S3StatusBadPermission                                   ,
    S3StatusXmlDocumentTooLarge                             ,
    S3StatusNameLookupError                                 ,
    S3StatusFailedToConnect                                 ,
    S3StatusServerFailedVerification                        ,
    S3StatusConnectionFailed                                ,
    S3StatusAbortedByCallback                               ,
    S3StatusNotSupported                                    ,


     * Errors from the S3 service

S3StatusErrorAccessDenied                               ,
S3StatusErrorAccountProblem                             ,
S3StatusErrorAmbiguousGrantByEmailAddress               ,
S3StatusErrorBadDigest                                  ,
S3StatusErrorBucketAlreadyExists                        ,
S3StatusErrorBucketAlreadyOwnedByYou                    ,
S3StatusErrorBucketNotEmpty                             ,
S3StatusErrorCredentialsNotSupported                    ,
S3StatusErrorCrossLocationLoggingProhibited             ,
S3StatusErrorEntityTooSmall                             ,
S3StatusErrorEntityTooLarge                             ,
S3StatusErrorExpiredToken                               ,
S3StatusErrorIllegalVersioningConfigurationException    ,
S3StatusErrorIncompleteBody                             ,
S3StatusErrorIncorrectNumberOfFilesInPostRequest        ,
S3StatusErrorInlineDataTooLarge                         ,
S3StatusErrorInternalError                              ,
S3StatusErrorInvalidAccessKeyId                         ,
S3StatusErrorInvalidAddressingHeader                    ,
S3StatusErrorInvalidArgument                            ,
S3StatusErrorInvalidBucketName                          ,
S3StatusErrorInvalidBucketState                         ,
S3StatusErrorInvalidDigest                              ,
S3StatusErrorInvalidEncryptionAlgorithmError            ,
S3StatusErrorInvalidLocationConstraint                  ,
S3StatusErrorInvalidObjectState                         ,
S3StatusErrorInvalidPart                                ,
S3StatusErrorInvalidPartOrder                           ,
S3StatusErrorInvalidPayer                               ,
S3StatusErrorInvalidPolicyDocument                      ,
S3StatusErrorInvalidRange                               ,
S3StatusErrorInvalidRequest                             ,
S3StatusErrorInvalidSecurity                            ,
S3StatusErrorInvalidSOAPRequest                         ,
S3StatusErrorInvalidStorageClass                        ,
S3StatusErrorInvalidTargetBucketForLogging              ,
S3StatusErrorInvalidToken                               ,
S3StatusErrorInvalidURI                                 ,
S3StatusErrorKeyTooLong                                 ,
S3StatusErrorMalformedACLError                          ,
S3StatusErrorMalformedPOSTRequest                       ,
S3StatusErrorMalformedXML                               ,
S3StatusErrorMaxMessageLengthExceeded                   ,
S3StatusErrorMaxPostPreDataLengthExceededError          ,
S3StatusErrorMetadataTooLarge                           ,
S3StatusErrorMethodNotAllowed                           ,
S3StatusErrorMissingAttachment                          ,
S3StatusErrorMissingContentLength                       ,
S3StatusErrorMissingRequestBodyError                    ,
S3StatusErrorMissingSecurityElement                     ,
S3StatusErrorMissingSecurityHeader                      ,
S3StatusErrorNoLoggingStatusForKey                      ,
S3StatusErrorNoSuchBucket                               ,
S3StatusErrorNoSuchKey                                  ,
S3StatusErrorNoSuchLifecycleConfiguration               ,
S3StatusErrorNoSuchUpload                               ,
S3StatusErrorNoSuchVersion                              ,
S3StatusErrorNotImplemented                             ,
S3StatusErrorNotSignedUp                                ,
S3StatusErrorNoSuchBucketPolicy                         ,
S3StatusErrorOperationAborted                           ,
S3StatusErrorPermanentRedirect                          ,
S3StatusErrorPreconditionFailed                         ,
S3StatusErrorRedirect                                   ,
S3StatusErrorRestoreAlreadyInProgress                   ,
S3StatusErrorRequestIsNotMultiPartContent               ,
S3StatusErrorRequestTimeout                             ,
S3StatusErrorRequestTimeTooSkewed                       ,
S3StatusErrorRequestTorrentOfBucketError                ,
S3StatusErrorSignatureDoesNotMatch                      ,
S3StatusErrorServiceUnavailable                         ,
S3StatusErrorSlowDown                                   ,
S3StatusErrorTemporaryRedirect                          ,
S3StatusErrorTokenRefreshRequired                       ,
S3StatusErrorTooManyBuckets                             ,
S3StatusErrorUnexpectedContent                          ,
S3StatusErrorUnresolvableGrantByEmailAddress            ,
S3StatusErrorUserKeyMustBeSpecified                     ,
S3StatusErrorQuotaExceeded                              ,
S3StatusErrorUnknown                                    ,


 * The following are HTTP errors returned by S3 without enough detail to
 * distinguish any of the above S3StatusError conditions

S3StatusHttpErrorMovedTemporarily                       ,
S3StatusHttpErrorBadRequest                             ,
S3StatusHttpErrorForbidden                              ,
S3StatusHttpErrorNotFound                               ,
S3StatusHttpErrorConflict                               ,
S3StatusHttpErrorUnknown
 */
#ifndef LANDESSDEVCORE_S3DATABASEBACKEND_H
#define LANDESSDEVCORE_S3DATABASEBACKEND_H
#include <s3/libs3.h>
#include "Primitives/General/Singleton.hpp"
#include "Primitives/General/StringView.hpp"
#include "Memory/ElementReference.h"
#include "IO/FetchRequest.h"
#include "Functor/FunctionView.h"
#include "Primitives/General/Immutable.hpp"
namespace LD
{
    class S3Credentials
    {
    private:
        LD::ImmutableString<64> mAccessKey;
        LD::ImmutableString<64> mSecretKey;
    public:
        S3Credentials() = default;
        S3Credentials(LD::StringView accessKey, LD::StringView secretKey) noexcept
        {
            for(LD::UInteger n = 0;n<accessKey.size() && n<64;++n)
            {
                this->mAccessKey[n] = accessKey[n];
            }

            for(LD::UInteger n = 0;n<secretKey.size() && n<64;++n)
            {
                this->mSecretKey[n] = secretKey[n];
            }
        }
        constexpr const LD::ImmutableString<64> & AccessKey() const noexcept
        {
            return this->mAccessKey;
        }
        constexpr const LD::ImmutableString<64> & SecretKey() const noexcept
        {
            return this->mSecretKey;
        }
    };

    class S3MetaData
    {
    private:
        LD::StringView mName;
        LD::StringView mValue;
    public:
    };
    class S3Response
    {
    private:
        S3MetaData mMetaData;
        LD::StringView mContentType;
        LD::StringView mTag;
        LD::StringView mRequestID[2];
        LD::StringView mServer;
        int64_t mLastModified;
        LD::UInteger mMetaDataCount;
        char mUseServerSideEncryption;
    public:
    };
    class S3State
    {
    private:

    public:
        S3State()
        {
            S3_initialize("s3", S3_INIT_ALL, "192.168.149.170:9010");
        }
        ~S3State()
        {
            S3_deinitialize();
        }
    };
    class S3BackingStore
    {
    private:
        LD::ImmutableString<64> mHost;
        LD::ImmutableString<64> mRegion;
        LD::ImmutableString<64> mSecurityToken;
        LD::ImmutableString<64> mBucketName;
        S3Credentials mCredentials;
        LD::UInteger mSelectedPath;
        static constexpr S3UriStyle S3StylePaths [2] = {S3UriStyleVirtualHost,S3UriStylePath};
    public:

        S3BackingStore(
                const LD::Variant<LD::TransactionError,LD::StringView> & hostName = {},
                const LD::Variant<LD::TransactionError,LD::StringView > & bucketName = {},
                const LD::Variant<LD::TransactionError,LD::S3Credentials> & credentials = {},
                const LD::Variant<LD::TransactionError,LD::StringView> & region = {},
                const LD::Variant<LD::TransactionError,LD::StringView> & securityToken = {},
                const bool uristylepath = true) noexcept:mSelectedPath{uristylepath}
        {
            S3State & globalState = LD::Singleton<LD::S3State>::GetInstance();


            //us-east-1
            mRegion[0] = 'u';
            mRegion[1] = 's';
            mRegion[2] = '-';
            mRegion[3] = 'e';
            mRegion[4] = 'a';
            mRegion[5] = 's';
            mRegion[6] = 't';
            mRegion[7] = '-';
            mRegion[8] = '1';

            LD::StringView generatedHostName = LD::MultiMatch(
                    LD::Overload{
                        [](const LD::TransactionError &) noexcept -> LD::StringView
                        {
                            return LD::StringView {};
                        },
                        []( LD::StringView  view) noexcept -> LD::StringView
                        {
                            return view;
                        }

                    },hostName);

            for(LD::UInteger n = 0;n<generatedHostName.size() && n < 64;++n)
            {
                this->mHost[n] = generatedHostName[n];
            }

            this->mCredentials =  LD::MultiMatch(LD::Overload{
                [](const LD::TransactionError & ) noexcept -> LD::S3Credentials
                {
                    return LD::S3Credentials{};
                },
                [](const LD::S3Credentials & credentials) noexcept -> LD::S3Credentials
                {
                    return credentials;
                }}
                ,credentials);

            LD::StringView generatedBucketName = LD::MultiMatch(LD::Overload{
                [](const LD::TransactionError &) noexcept->LD::StringView
                {
                    return {};
                },
                []( LD::StringView  bucketName) noexcept->LD::StringView
                {
                    return bucketName;
                }},bucketName);

            for(LD::UInteger n = 0;n<generatedBucketName.size() && n<64;++n)
            {
                this->mBucketName[n] = generatedBucketName[n];
            }


        }

        template<typename ... A, typename F,
                typename FunctorRet = decltype(LD::Declval<F>()(LD::Declval<LD::Context<LD::StringView,LD::StringView,A...>>())),
                typename Ret = LD::QueryResult<FunctorRet(LD::StringView,A...)>>
        LD::Enable_If_T<
                LD::Require<
                        LD::Negate<LD::Detail::IsSame<void,FunctorRet>::value>
                >
                ,
                Ret> Fetch(const LD::StringView & key, F && functor, A && ... arguments)
        {
            using UsableContext = LD::Context<LD::StringView,LD::StringView ,A...>;
            using FunctorDef = LD::FunctionView<FunctorRet(const UsableContext &)>;
            using DecayedRet = LD::Detail::Decay_T<FunctorRet>;
            using FetchContext = LD::Tuple<LD::Ref<FunctorDef>,LD::Ref<UsableContext>,DecayedRet,S3Status,LD::S3Response>;

            S3BucketContext currentBucketContext = {
                    this->mHost.Data(),
                    this->mBucketName.Data(),
                    S3ProtocolHTTP,
                    S3UriStylePath,
                    this->mCredentials.AccessKey().Data(),
                    this->mCredentials.SecretKey().Data(),
                    nullptr,
                    this->mRegion.Data()
            };

            auto getObjectDataCallBack = [](int bufferSize, const char * buffer, void * callBackData) -> S3Status
            {
                FetchContext * fetchContext = (FetchContext*)callBackData;
                FetchContext & localizedFetchContext = *fetchContext;
                UsableContext & localizedContext = LD::Get(LD::Get<1>(localizedFetchContext));
                LD::Get(LD::Get<1>(localizedContext)) = LD::StringView{buffer,LD::UInteger(bufferSize)};
                FunctorDef function = LD::Get(LD::Get<0>(localizedFetchContext));
                LD::Get(LD::Get<2>(localizedFetchContext)) =  function(localizedContext);
                return S3StatusOK;
            };

            auto propertiesCallBack = [](const S3ResponseProperties *properties,
                                         void *callbackData ) noexcept -> S3Status
            {
                FetchContext * fetchContext = (FetchContext*)callbackData;
                return S3StatusOK;
            };

            auto completionCallBack = [](S3Status status,
                                         const S3ErrorDetails *error,
                                         void *callbackData) noexcept
            {
                FetchContext * fetchContext = (FetchContext*)callbackData;
                FetchContext & localizedFetchContext = *fetchContext;
                LD::Get(LD::Get<3>(localizedFetchContext)) =  status;

            };

            S3ResponseHandler responseHandler ={
                    propertiesCallBack,
                    completionCallBack
            };
            S3GetObjectHandler getObjectHandler ={
                    responseHandler,
                    getObjectDataCallBack
            };
            UsableContext context = LD::MakeContext(LD::StringView{key},LD::StringView{},LD::Forward<A>(arguments)...);
            FetchContext fetchContext;
            LD::Get<0>(fetchContext) = FunctorDef{functor};
            LD::Get<1>(fetchContext) = LD::Ref<UsableContext>{context};

            S3_get_object(&currentBucketContext,key.data(),NULL,0,0,NULL,0,&getObjectHandler, &fetchContext);

            S3Status status = LD::Get(LD::Get<3>(fetchContext));
            if (status == S3StatusOK)
            {
                return LD::MakeContext(LD::TransactionResult{},FunctorRet{LD::Get(LD::Get<2>(fetchContext))},LD::StringView{key},LD::Forward<A>(arguments)...);
            }
            return LD::MakeContext(LD::TransactionError{},LD::StringView{key},LD::Forward<A>(arguments)...);
        }
        template<typename ... A>
        LD::QueryResult<bool(A...)> Store(LD::StringView key, LD::StringView value ,A && ... arguments) noexcept
        {
            using UsableContext = LD::Context<A...>;
            using FetchContext = LD::Tuple<LD::Ref<UsableContext>,S3Status,LD::S3Response,LD::StringView>;
            S3BucketContext currentBucketContext = {
                    this->mHost.Data(),
                    this->mBucketName.Data(),
                    S3ProtocolHTTP,
                    S3UriStylePath,
                    this->mCredentials.AccessKey().Data(),
                    this->mCredentials.SecretKey().Data(),
                    nullptr,
                    this->mRegion.Data()
            };
            auto objectPutCallBack = [](int bufferSize, char * buffer, void * callbackdata) noexcept -> int
            {
                FetchContext * fetchContext = (FetchContext*)callbackdata;
                LD::StringView data = LD::Get(LD::Get<3>(*fetchContext));

                for(LD::UInteger n = 0;n<data.size();++n)
                {
                    buffer[n] = data[n];
                }
                return data.size();
            };
            auto propertiesCallBack = [](const S3ResponseProperties *properties,
                                         void *callbackData ) noexcept -> S3Status
            {

                return S3StatusOK;
            };
            auto completionCallBack = [](S3Status status,
                                         const S3ErrorDetails *error,
                                         void *callbackData) noexcept
            {
                FetchContext * fetchContext = (FetchContext*)callbackData;
                FetchContext & localizedFetchContext = *fetchContext;
                LD::Get(LD::Get<1>(localizedFetchContext)) =  status;

            };
            S3ResponseHandler responseHandler ={
                    propertiesCallBack,
                    completionCallBack
            };
            S3PutObjectHandler putObjectHandler = {
                    responseHandler,
                    objectPutCallBack
            };
            UsableContext context = LD::MakeContext(LD::Forward<A>(arguments)...);
            FetchContext currentFetchContext;
            LD::Get<0>(currentFetchContext) = LD::Ref<UsableContext>{context};
            LD::Get<3>(currentFetchContext) = value;
            S3_put_object(&currentBucketContext,key.data(),value.size(), nullptr, nullptr,0, &putObjectHandler, &currentFetchContext);
            S3Status status = LD::Get(LD::Get<1>(currentFetchContext));
            if (status == S3StatusOK)
            {
                return LD::MakeContext(LD::TransactionResult{},true,LD::Forward<A>(arguments)...);
            }
            return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(arguments)...);
            //return LD::MakeContext(LD::TransactionResult{},true,LD::Forward<A>(arguments)...);
        }

        template<typename F, typename ... A,
                typename FunctorRet = decltype(LD::Declval<F>()(LD::Declval<LD::Context<LD::StringView,LD::StringView,LD::StringView,int64_t ,A...>>()))>
        LD::QueryResult<FunctorRet(A...)> ListBuckets(F && callback, A && ... arguments) noexcept
        {



            using UsableContext = LD::Context<LD::StringView,LD::StringView,LD::StringView ,int64_t ,A...>;
            using FunctorDef = LD::FunctionView<FunctorRet(const UsableContext &)>;
            using DecayedRet = LD::Detail::Decay_T<FunctorRet>;
            UsableContext context = LD::MakeContext(LD::StringView{},LD::StringView{},LD::StringView {},int64_t {},LD::Forward<A>(arguments)...);
            using FetchContext = LD::Tuple<LD::Ref<FunctorDef>,LD::Ref<UsableContext>,DecayedRet,S3Status,LD::S3Response>;
            FunctorDef function{LD::Forward<F>(callback)};
            FetchContext currentFetchContext;
            LD::Get<0>(currentFetchContext) = LD::Ref<FunctorDef>{function};
            LD::Get<1>(currentFetchContext) = LD::Ref<UsableContext>{context};

            auto propertiesCallBack = [](const S3ResponseProperties *properties,
                                         void *callbackData ) noexcept -> S3Status
            {

                return S3StatusOK;
            };
            auto completionCallBack = [](S3Status status,
                                         const S3ErrorDetails *error,
                                         void *callbackData) noexcept
            {
                FetchContext * fetchContext = (FetchContext*)callbackData;
                LD::Ref<UsableContext> context = LD::Get<1>(*fetchContext);
                LD::Get(LD::Get<3>(*context)) = status;
            };
            S3ResponseHandler responseHandler ={
                    propertiesCallBack,
                    completionCallBack
            };

            auto bucketListCallBack = [](const char *ownerId,
                                         const char *ownerDisplayName,
                                         const char *bucketName,
                                         int64_t creationDate, void *callbackData) noexcept-> S3Status
            {
                FetchContext * fetchContext = (FetchContext*)callbackData;
                FunctorDef function = LD::Get(LD::Get<0>(*fetchContext));
                LD::Ref<UsableContext> context = LD::Get<1>(*fetchContext);
                LD::Get(LD::Get<0>(*context)) = LD::StringView{ownerId};
                LD::Get(LD::Get<1>(*context)) = LD::StringView{ownerDisplayName};
                LD::Get(LD::Get<2>(*context)) = LD::StringView{bucketName};
                LD::Get(LD::Get<3>(*context)) = creationDate;
                LD::Get(LD::Get<2>(*fetchContext)) = function(*context);
                return S3StatusOK;
            };
            S3ListServiceHandler listServiceHandler ={
                    responseHandler,
                    bucketListCallBack
            };
            S3_list_service(
                    S3ProtocolHTTP,
                    this->mCredentials.AccessKey().Data(),
                    this->mCredentials.SecretKey().Data(),
                    nullptr,
                    this->mHost.Data(),
                    nullptr,
                    NULL,
                    10,
                    &listServiceHandler,
                    &currentFetchContext);
            return LD::MakeContext(LD::TransactionError{},LD::Forward<A>(arguments)...);
        }
    };
}
#endif //LANDESSDEVCORE_S3DATABASEBACKEND_H
