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
template<typename T>
class Square
{
private:
    static constexpr auto stuffings = "abc"_ts;
};


struct Point
{
    float x;
    float y;

    constexpr Point(const float & x1 = 0, const float & x2 = 0) noexcept :x(x1),y(x2){}
    constexpr const float & X() const noexcept
    {
        return this->x;
    }

    constexpr float & X() noexcept
    {
        return this->x;
    }

    void SetX(const float & x) noexcept
    {
        this->x = x;
    }
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
    for(LD::UInteger n = 0;n<N;++n)
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
constexpr LD::UInteger Size(const ctll::basic_fixed_string<wchar_t,N> & lit)
{
    LD::UInteger size = 0;
    for(LD::UInteger n = 0;n<N;++n)
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
            0,0,0,0, /// vertical resolution
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
    LD::Example::DelimeterSeperatedFileExample();
    //LD::Example::TermBoxMenuExample();
    /*

    Point pt{5,7};


    pt.X() = 97;

    LD::Tuple<float,float> point =  MapToTuple(pt);

    std::cout << "Point : " << LD::Get<0>(point) << "," << LD::Get<1>(point) << std::endl;


    //LD::Hexademical<2> address{LD::BitSet<8>{1,1,1,0,1,1,1,1}};
    //LD::Hexademical<16> number{LD::GenerateBitSet(92392423216,LD::CT::Range<0,64>{})};

    LD::ImmutableByteBuffer<8> numberAsBytes{LD::GenerateBitSet(92392423216,LD::CT::Range<0,64>{})};
    for(LD::UInteger n = 0;n<8;++n)
    {
        std::cout  << LD::UInteger (numberAsBytes[n]) << ",";
    }
    std::cout << "\n";
    for(LD::UInteger n = 0;n<16;++n)
    {
        std::cout << number[n];
    }
    std::cout << "\n";
    for (int i = 0; i < 2; ++i) {
        std::cout << address[i];
    }
    std::cout << "\n";
    std::cout << "Test 5" << std::endl;
    for(LD::UInteger n = 0;n<8;++n)
    {
        std::cout << setResult[n];
    }

    auto bits = LD::GenerateBitSet(-8,LD::CT::Range<0,11>{});

    std::cout << std::endl;
    for(LD::UInteger n = 0;n<11;++n)
    {
        std::cout << bits[n];
    }
    std::cout << std::endl;

    auto doubleAsBits = LD::GenerateBitSet(0.043);
    for(LD::UInteger n = 0;n<18;++n)
    {
        std::cout << doubleAsBits[n];
    }
    /*
     */
    //std::printf("x = %.2lf = %.2lf * 2^%d\n", x, fraction, e);
    /*
    LD::RowBackingStore backingStore{"/proc/stat"};
    LD::SpaceSpeerateValueFile<LD::RowBackingStore> file{backingStore};
    //using ObjectPack = LD::CT::TypeList<LD::CPUPackageMetric,LD::CPUCoreMetric<0>,LD::CPUCoreMetric<1>,LD::CPUCoreMetric<2>>;
    using ObjectPack = LD::GenerateSystemRepresentation<16>;
    using ObjectPackVariant = LD::Rebind<ObjectPack,LD::Variant>;
    LD::UInteger previous_idle_time=0, previous_total_time=0;
    LD::SpaceSpeerateValueFile<LD::RowBackingStore>::Iterator<ObjectPackVariant (LD::UInteger&,LD::UInteger&)> it{previous_idle_time,previous_total_time};// = file.Begin(LD::CT::TypeList<LD::CPUPackageMetric>{},previous_idle_time,previous_total_time);
    LD::SpaceSpeerateValueFile<LD::RowBackingStore>::Iterator<ObjectPackVariant (LD::UInteger&,LD::UInteger&)> end = file.End(ObjectPack{},previous_idle_time,previous_total_time);
    std::cout << LD::CPUCoreMetric<0>::GetClassName() << std::endl;
    for (LD::UInteger i = 0; i < 10; ++i)
    {

        for (auto it = file.Begin(ObjectPack{},previous_idle_time,previous_total_time); it != end; ++it)
        {


            //called when we get a LD::CPUPackageMetric Object
            auto onCPUPackageMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::CPUPackageMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {

                std::cout << "found package " << std::endl;
                LD::CPUPackageMetric & metric = LD::Get(LD::Get<1>(context));
                LD::UInteger & previous_idle_time = LD::Get(LD::Get<2>(context));
                LD::UInteger & previous_total_time = LD::Get(LD::Get<3>(context));
                LD::Float total_time = metric.User() + metric.Nice() + metric.System() + metric.Idle() +metric.IOWait() + metric.IRQ() + metric.SoftIRQ();

                //std::cout << "Previous Idle Time = " << previous_idle_time << std::endl;
                //std::cout << "Previous Total Time = " << previous_idle_time << std::endl;
                const LD::UInteger idle_time = metric.Idle();
                //std::cout << "Idle Time : " << idle_time << std::endl;
                const double idle_time_delta = idle_time - previous_idle_time;
                const double total_time_delta = total_time - previous_total_time;
                const double utilization = (idle_time_delta!=total_time_delta)*(100.0 * (1.0 - idle_time_delta / total_time_delta));
                std::cout << "Utilizaiton : " <<  utilization << std::endl;
                //std::cout << "Sum : " << total_time << std::endl;
                LD::Float idlePercentage = (metric.Idle()*100)/total_time;
                //std::cout << "Usage: " << ((sum-metric.Idle())*100.0)/sum << std::endl;

                previous_idle_time = idle_time;
                previous_total_time = total_time;
            };

            //called when we find an object that matches LD::CPUCoreMetric<LD::UInteger>
            auto onCPUCoreMetric = [](auto  && stuff) noexcept -> LD::Enable_If_T<LD::Require<LD::IsCPUCore<LD::GetType<decltype(LD::GetQueryType(stuff))>>>,void>
            {
                using CoreType = decltype(LD::GetQueryType(stuff));
                using Underlyingcore = LD::GetType<CoreType >;
                constexpr static const LD::UInteger Core = Underlyingcore::Core;
                std::cout << "found core " << Core <<  std::endl;


                if constexpr(LD::IsCPUCore<Type>)
                {
                    using CoreType = decltype(LD::GetQueryType(stuff));
                    using Underlyingcore = typename CoreType::type;
                    constexpr static const LD::UInteger Core = Underlyingcore::Core;
                    std::cout << "found core " << Core <<  std::endl;
                }


            };

            //called when we get a LD::CPUPackageMetric Object
            auto onContextSwitchMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ContextSwitchMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onInterruptServiceMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::InterruptServiceMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onBootUpTimeMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::BootUpTimeMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onProcessMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ProcessesMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onProcessRunningMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ProcessesRunningMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            auto onProcessBlockedMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::ProcessesBlockedMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            //LD::SoftIRQMetric

            auto onSoftIRQMetric = [](const LD::Context<LD::DatabaseTransactionResult,const LD::SoftIRQMetric,LD::UInteger&,LD::UInteger&> & context) noexcept
            {


            };

            //called if we get a Database Error, such as getting a row with no associated objectyghtytynonneaew WQQq
            auto onDatabaseError = [](const LD::Context<LD::DatabaseError,LD::UInteger&,LD::UInteger&> & context) noexcept
            {

                //std::cout << "found core 0 " << std::endl;
            };


            LD::Match(*it,
                    onCPUPackageMetric,
                    onDatabaseError,
                    onCPUCoreMetric,
                    onContextSwitchMetric,
                    onInterruptServiceMetric,
                    onBootUpTimeMetric,
                    onProcessMetric,
                    onProcessRunningMetric,
                    onProcessBlockedMetric,
                    onSoftIRQMetric);
           //sleep(1);
        }
        sleep(1);
    }
     */
    //LD::Example::TermBoxMenuExample();
    LD::Timer currentTimer;
    currentTimer.Start();
    //LD::Example::ReflectionExample();
    //LD::Example::IMGUITUIExample();
    currentTimer.Stop();
    //std::cout << "Execution Time: " << currentTimer.Time()/1.0_us<< std::endl;
    return 0;
}