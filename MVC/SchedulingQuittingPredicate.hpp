//
// Created by James Landess on 2/8/20.
//

#ifndef LANDESSDEVCORE_SCHEDULINGQUITTINGPREDICATE_HPP
#define LANDESSDEVCORE_SCHEDULINGQUITTINGPREDICATE_HPP
namespace LD
{
    class ApplicationQuittingPredicate
    {
    private:
        LD::UInteger mShouldQuit;
    public:

        inline explicit constexpr ApplicationQuittingPredicate() noexcept :mShouldQuit(false) {};
        inline explicit constexpr ApplicationQuittingPredicate(const LD::UInteger & shouldQuit) noexcept :mShouldQuit(shouldQuit == false){}
        constexpr const LD::UInteger  & Value() const noexcept {return this->mShouldQuit;}

        constexpr operator bool() const noexcept { return this->mShouldQuit; }
        //explicit constexpr operator bool&&() const noexcept { return this->mShouldQuit; }
    };

    class ApplicationRunningPredicate
    {
    private:
        LD::UInteger mShouldRun;
    public:

        inline explicit constexpr ApplicationRunningPredicate() noexcept :mShouldRun(true) {};
        inline explicit constexpr ApplicationRunningPredicate(const LD::UInteger & shouldRun) noexcept :mShouldRun(shouldRun == true){}
        constexpr const LD::UInteger  & Value() const noexcept {return this->mShouldRun;}

        constexpr operator bool() const noexcept { return this->mShouldRun; }
    };
}
#endif //LANDESSDEVCORE_SCHEDULINGQUITTINGPREDICATE_HPP
