#include "Examples/TermBoxMenuExample.hpp"
#include "Examples/ReflectionExample.hpp"
#include "Examples/DelimeterSeperatedFileExample.h"
#include "Chrono/Timer.h"
#include "Primitives/General/Immutable.hpp"
#include "TypeTraits/IsArray.hpp"
#include "Primitives/General/ContextualVariant.h"
#include <iostream>
#include "Primitives/General/Span.hpp"
#include "IO/FetchRequest.h"
#include "Examples/ReflectionDemoTypes.h"
#include "Algorithms/FuzzySearch.h"
#include "IO/RowBackingStore.h"
#include "IO/BasicDelimeterSeperatedFile.h"
#include "Async/Thread.h"
#include <unistd.h>
#include "Primitives/General/Range.h"
#include "Algorithms/FullAdder.hpp"
#include "Algorithms/ComplimentGenerator.h"
#include "Primitives/General/Hexademical.hpp"
#include "Primitives/General/ByteBuffer.hpp"
#include "Algorithms/refl.hpp"
#include "Reflection/Reflection.hpp"
#include "Primitives/General/ctre.hpp"
#include "TypeTraits/FunctionalReflection.hpp"
#include "Unicode/utf8.h"
#include "Unicode/UTF8.hpp"
#include "TUI/ascii_art.h"
#include "Examples/FullAdderExample.hpp"
#include "IO/UnqliteDatabaseBackend.h"
#include "IO/Database.hpp"
#include "TypeTraits/TypeList.hpp"
template<typename T>
class Square
{
private:
    static constexpr auto stuffings = "abc"_ts;
};



template<>
class LD::CT::TypeDescriptor<Point>//: public LD::CT::Reflectable<LD::CT::ClassName<SquareClassName>(LD::CT::Member<stuffings,T>)>
{
private:
    static constexpr auto XName = "x"_ts;
    static constexpr auto YName = "y"_ts;
public:
    static constexpr auto ClassName = "Point"_ts;
    using MemberList = LD::CT::TypeList<
            //LD::CT::Member<LengthName,T>,
            //LD::CT::MemberDescriptor<XName,xMember>,
            LD::CT::EncapsulatedMemberDescriptor<XName,LD::CT::SelectOverload<float & (Point::*)(),&Point::X>(),LD::CT::SelectOverload<const float & (Point::*)() const,&Point::X>()>,
            LD::CT::MemberDescriptor<YName,&Point::y>
            >;
    static constexpr MemberList Members{  };
};


/* Metadata */
REFL_AUTO(
        type(Point),
        field(x, /* attributes */),
        field(y)
)





struct Fluid { };
struct LightItem { };
struct HeavyItem { };
struct FragileItem { };
//#include <variant>

template<LD::UInteger N>
constexpr LD::UInteger Size(const wchar_t (&lit)[N] )
{
    LD::UInteger size = 0;
    constexpr LD::UInteger Size = (N-1);
    for(LD::UInteger n = 0;n<Size;++n)
    {
        const wchar_t & cp = lit[n];
        if (cp < 0x80)
        {
            // one octet
            //*(result++) = static_cast<uint8_t>(cp);
            size+=1;
        }
        else if (cp < 0x800)
        {                // two octets
            //*(result++) = static_cast<uint8_t>((cp >> 6)            | 0xc0);
            //*(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            size+=2;
        }
        else if (cp < 0x10000)
        {              // three octets
            //*(result++) = static_cast<uint8_t>((cp >> 12)           | 0xe0);
            //*(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f)   | 0x80);
            //*(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            size+=3;
        }
        else
        {
            // four octets
            //*(result++) = static_cast<uint8_t>((cp >> 18)           | 0xf0);
            //*(result++) = static_cast<uint8_t>(((cp >> 12) & 0x3f)  | 0x80);
            //*(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f)   | 0x80);
            //*(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            size+=4;
        }
    }
    return size;
}


template<LD::UInteger N>
constexpr LD::UInteger Size(const ctll::fixed_string<N> & lit)
{
    LD::UInteger size = 0;
    //we don't want to count the null terminating character
    constexpr LD::UInteger Size = (N-1);
    for(LD::UInteger n = 0;n<Size;++n)
    {
        const wchar_t & cp = lit[n];
        if (cp < 0x80)
        {
            size+=1;
        }
        else if (cp < 0x800)
        {                // two octets
            size+=2;
        }
        else if (cp < 0x10000)
        {              // three octets
            size+=3;
        }
        else
        {
            size+=4;
        }
    }
    return size;
}

template< const auto &  lit>
constexpr auto ToImmutableString()
{
    LD::ImmutableString<Size(lit)-1> string;
    char * result = (char*)string.Data();
    for(LD::UInteger n = 0;n<lit.size();++n)
    {
        const wchar_t & cp = lit[n];
        if (cp < 0x80)
        {
            // one octet
            *(result++) = static_cast<uint8_t>(cp);
            //size+=1;
        }
        else if (cp < 0x800)
        {                // two octets
            *(result++) = static_cast<uint8_t>((cp >> 6)            | 0xc0);
            *(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            //size+=2;
        }
        else if (cp < 0x10000)
        {              // three octets
            *(result++) = static_cast<uint8_t>((cp >> 12)           | 0xe0);
            *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f)   | 0x80);
            *(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            //size+=3;
        }
        else
        {
            // four octets
            *(result++) = static_cast<uint8_t>((cp >> 18)           | 0xf0);
            *(result++) = static_cast<uint8_t>(((cp >> 12) & 0x3f)  | 0x80);
            *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f)   | 0x80);
            *(result++) = static_cast<uint8_t>((cp & 0x3f)          | 0x80);
            //size+=4;
        }
    }
    return string;
}

const int infoHeaderSize = 40;
const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;


unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
            0,0,0,0, /// header size
            0,0,0,0, /// image width
            0,0,0,0, /// image height
            0,0, /// number of color planes
            0,0, /// bits per pixel
            0,0,0,0, /// compression
            0,0,0,0, /// image size
            0,0,0,0, /// horizontal resolution
            0,0,0,0, /// vertical resolutionReflectionExample
            0,0,0,0, /// colors in color table
            0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);

    return infoHeader;
}

unsigned char* createBitmapFileHeader(int height, int width, int paddingSize){
    int fileSize = fileHeaderSize + infoHeaderSize + (bytesPerPixel*width+paddingSize) * height;

    static unsigned char fileHeader[] = {
            0,0, /// signature
            0,0,0,0, /// image file size in bytes
            0,0,0,0, /// reserved
            0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName){

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (width*bytesPerPixel) % 4) % 4;

    unsigned char* fileHeader = createBitmapFileHeader(height, width, paddingSize);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);

    FILE* imageFile = fopen(imageFileName, "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    int i;
    for(i=0; i<height; i++){
        fwrite(image+(i*width*bytesPerPixel), bytesPerPixel, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

int main()
{

    /*

    int height = 500;
    int width = 500;

    unsigned char image[height][width][bytesPerPixel];
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++)
        {
            image[i][j][2] = (unsigned char)((double)i/height*255); ///red
            image[i][j][1] = (unsigned char)((double)j/width*255); ///green
            image[i][j][0] = (unsigned char)(((double)i+j)/(height+width)*255); ///blue
        }
    }

    generateBitmapImage((unsigned char*)image,width,height,"stuff.bmp");
    //generateBitmapImage((unsigned char *)zBlob,width,height,"name.bmp");
    //std::cout << zBlob << std::endl;
    //fwrite(zText,sizeof(char),(width*height),stdout);



    static constexpr LD::UInteger wcharSize = Size(ctll::basic_fixed_string{L"Σὲ γνωρίζω ἀπὸ τὴν κόψη"});
    static constexpr auto reztl = ctll::basic_fixed_string{L"Σὲ γνωρίζω ἀπὸ τὴν κόψη"};
    ToImmutableString<reztl>();
    constexpr wchar_t * rawrS = L"Σὲ γνωρίζω ἀπὸ τὴν κόψη";
    std::vector<char> output;
    //utf8::distance(rawrS,rawrS+sizeof(rawrS));
    utf8::utf32to8(rawrS,rawrS+wcslen(rawrS),std::back_inserter(output));
    std::cout << output.size() <<  " : " << wcharSize << std::endl;
    auto immutableString = ToImmutableString<reztl>();

    std::cout << "Size : " << utf8::distance(immutableString.Begin(),immutableString.End()) << std::endl;
    std::cout << "Size : " << utf8::distance((const char*)output.data(),(const char*)output.data()+output.size()) << std::endl;
    //std::cout << "Size : " << utf8::distance((const char*)reztl.content,(const char*)(reztl.content+reztl.size())) << std::endl;
    auto rawrString = ctll::basic_fixed_string{u8"\\U0001F607 is O:-)"};
    //LD::QueryResult<int()> rawrbunnies = LD::QueryResult<int()>{LD::TransactionError{}};
    LD::QueryResult< LD::UInteger()> res = LD::UTF8::Distance(rawrString.begin(),rawrString.end());

    auto onDistance = [](const LD::Context<LD::TransactionResult,LD::UInteger> & context) noexcept
    {
        return LD::Get(LD::Get<1>(context));
    };

    auto onDistanceError = [](const LD::Context<LD::TransactionError> & context) noexcept
    {
        return -7;
    };

    std::cout << "Resolution : " << LD::Match(res,onDistance,onDistanceError) << std::endl;
    LD::TransactionError transactionError = LD::EncodingError{};
    transactionError();

    LD::Variant<LightItem, HeavyItem> basicPackA;
    LD::Variant<LightItem, HeavyItem> basicPackB = HeavyItem{};


    LD::MultiMatch(LD::Overload{
        [](LightItem&,LightItem&)
        {
            std::cout << "2 light items\n";
        },
        [](LightItem&,HeavyItem&)
        {
            std::cout << "light & heavy items\n";
        },
        [](HeavyItem&,LightItem&)
        {
            std::cout << "heavy & light items\n";
        },
        [](HeavyItem&,HeavyItem&)
        {
            std::cout << "2 heavy items\n";
        }}
        ,basicPackA,basicPackB);

    auto tb = LD::MapToTuple(Point{1.0f,1.0f});

    LD::Variant<int,float> variantABC = 7;
    LD::Variant<int,float> variantCDE = 9;
    LD::MultiMatch(LD::Overload{
        [](auto abc ,auto cde)
        {
            std::cout << typeid(abc).name() << " : " << typeid(cde).name() << std::endl;

        }},variantABC,variantCDE);

    LD::ImmutableString{"abc"};
    auto abc = ctll::fixed_string{LD::ImmutableString{"abc"}+LD::ImmutableString{LD::ToTypeString<5>{}}};
    for(LD::UInteger n = 0;n<abc.size();++n)
    {
        std::cout << char(abc[n]);
    }
    std::cout << std::endl;
    //LD::StringView{ctll::basic_fixed_string{"abc"}};







    LD::Example::FullAdderExample();
    */

    LD::StaticArray<char,15> array;

    LD::BackInserter<LD::StaticArray<char,15>> inserter{array};
    inserter = 'a';
    inserter = 'b';
    //ins
    //inserter = '\0';
    //inserter = '\0';

    std::cout << "Stuffings: " << array[1] << std::endl;
    ctre::fixed_string abc{L"abc"};

    std::cout << "Size : " << Size(abc) <<  " :  " << abc.real_size <<  " : " << utf8::distance((char*)abc.content,(char*)abc.content+abc.size()) << std::endl;
    std::cout << "Bytes : " << LD::UTF8::NumberOfBytes("abc"_ts) << std::endl;
    auto ts = "abc"_ts;


    LD::StaticArray<char,1024> bufferABC;
    LD::BackInserter<LD::StaticArray<char,1024>> abcInserter{bufferABC};

    for(LD::UInteger n = 0;n<3;++n)
    {
        LD::UTF8::Append(abc[n],abcInserter);
    }
    auto abcBeg = LD::UTF8::Begin(bufferABC);
    for(LD::UInteger n = 0;n<3;++n)
    {
        auto res = LD::UTF8::Next(abcBeg,LD::UTF8::End(abc));

        auto onTransaction = [](const LD::Context<LD::TransactionResult,uint32_t> & context) noexcept
        {
            printf("abc test ::%c\n",LD::Get(LD::Get<1>(context)));
        };

        auto onError = [](const LD::Context<LD::TransactionError> &) noexcept
        {

        };

        LD::Match(res,onTransaction,onError);
    }
    auto firstLength = LD::UTF8::Distance((char*)abc.content,(char*)abc.content+abc.size());

    auto secondLength = LD::UTF8::Distance(array.Begin(),array.End());


    auto onDistance = [](const LD::Context<LD::TransactionResult,LD::UInteger> & context) noexcept
    {
        std::cout << "Distance : " << LD::Get(LD::Get<1>(context)) << std::endl;
    };

    auto onError = [](const LD::Context<LD::TransactionError> & context) noexcept
    {

    };

    LD::Match(firstLength,onDistance,onError);




    LD::StaticArray<char,500> array1;
    LD::MultiMatch(
            LD::Overload{
                [](auto, auto){},
                [&](const LD::Context<LD::TransactionResult,LD::UInteger> & ctx1, const LD::Context<LD::TransactionResult,LD::UInteger> & ctx2)
                {
                    LD::BackInserter<LD::StaticArray<char,500>> backInserter{array1};

                    auto length1 = LD::Get(LD::Get<1>(ctx1));
                    auto length2 = LD::Get(LD::Get<1>(ctx2));

                    auto i1 = (char*)abc.begin();
                    for(auto i = 0;i<length1;++i)
                    {
                        auto possibleNextCharacter = LD::UTF8::Next(i1,(char*)abc.end());


                        auto onError = [](const LD::Context<LD::TransactionError> & ) noexcept
                        {

                        };

                        auto onCharacter = [](const LD::Context<LD::TransactionResult,uint32_t> & context) noexcept
                        {
                            //std::cout << LD::Get<1>(context) << std::endl;
                        };

                        LD::Match(possibleNextCharacter,LD::Overload{onError,onCharacter});
                        //std::cout << utf8::next(i1,(char*)abc.end()) << std::endl;
                    }
                    std::cout << LD::Get(LD::Get<1>(ctx1)) << " : " << LD::Get(LD::Get<1>(ctx2)) << std::endl;
                }},
            firstLength,secondLength);
    LD::UnQliteBackend<char> backingStore{"database.db",LD::OpenAndCreateIfNotExists{}};
    LD::BasicKeyedDatabase<LD::UnQliteBackend<char>> keyedDatabase{backingStore};




    //keyedDatabase.Insert("abc"_ts,LD::Pyramid{LD::Square{922},LD::Triangle{72.48,92.73}});
    auto result =  keyedDatabase.Fetch("abc"_ts,LD::CT::TypeList<LD::Pyramid>{});

    auto onFetch = [](const LD::Context<LD::TransactionResult,LD::Pyramid> & context) noexcept
    {
        LD::Pyramid & pyramid = LD::Get(LD::Get<1>(context));
        std::cout << pyramid.Side().Base() << std::endl;
        std::cout << pyramid.Side().Height() << std::endl;
        std::cout << pyramid.Base().Length() << std::endl;
    };

    auto onFetchError = [](auto) noexcept
    {

    };
    LD::Match(result,onFetch,onFetchError);
    //backingStore.Fetch(LD::StringView{"abc.Side.Height"},[](auto context)->int
    //{
       // std::cout << "Key : " << LD::Get(LD::Get<0>(context)) << "    Value : " << LD::Get(LD::Get<1>(context));
        //return 0;
    //});
    //LD::Example::DelimeterSeperatedFileExample();
    return 0;
}