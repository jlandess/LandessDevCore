

#if !defined(__MAXMM__ROPE__H__)
#  define __MAXMM__ROPE__H__

//#include <Primitives/General/BasicString.h>
#include <Memory/Optional.h>
//#include <Memory/Allocator.h>
#include <Memory/UniquePointer.h>
//#include <Definitions/Common.h>
#include "Algorithms/Exponential.hpp"
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/mapboxvariant.hpp"
#include "Primitives/General/mapboxvariantvisitor.h"
#include "Primitives/General/ContextualVariant.h"

//#include <Algorithms/PDPExp.h>


#include <cassert>
namespace PDP
{


    // @brief Rope Class which implements basic functionality
    // of the Rope data structure for PDP::DataStructures::BasicString<T,Alloc>.
    //
    // https://en.wikipedia.org/wiki/Rope_(data_structure)
    template<typename T, typename Alloc>
    class Rope
    {
    private:
        using CompressedStorage = LD::Variant<
                LD::ImmutableString<1>,
                LD::ImmutableString<2>,
                LD::ImmutableString<4>,
                LD::ImmutableString<8>,
                LD::ImmutableString<16>,
                LD::ImmutableString<32>,
                LD::ImmutableString<64>,
                LD::ImmutableString<128>,
                LD::ImmutableString<256>,
                LD::ImmutableString<512>>;
    public:

        // Creators
        // --------

        // @brief Initialize the maxmm by moving a string.
        //
        // Most recommended constructor since no memory allocation
        // will be performed.
        template<LD::UInteger Size>
        explicit Rope(LD::ImmutableString<Size> && s)
                : tag_(tag::STRING),
                  height_(1),
                  size_(s.GetSize()),
                  parent_(nullptr)
        {
            string_ = LD::Move(s);
            //new (&string_) PDP::DataStructures::BasicString<T,Alloc>(std::move(s));
            //MAXMM_ASSERT_INVARIANT;
        }

        // @brief Initialize the maxmm by copying a string.
        //
        // A memory allocation + copy is done for the length
        // of the string [s].
        template<LD::UInteger Size>
        explicit Rope(const LD::ImmutableString<Size> & s)
                : tag_(tag::STRING),
                  height_(1),
                  size_(s.GetSize()),
                  parent_(nullptr)
        {


           if (Size == 1)
           {
               this->string_ = s;

           }
           else if(Size >=2 && Size < 4)
           {


           }else if(Size >=4 && Size < 7)
           {

           }else if(Size >= 8 && Size < 16)
           {

           }else if(Size >=16 && Size < 32)
           {

           }else if(Size >= 32 && Size < 64)
           {

           }else if(Size >= 64 && Size < 128)
           {

           }
           else if(Size >=128 && Size < 256)
           {

           }
           else if(Size >= 256 && Size < 512)
           {

           }
           string_ = s;
            //new (&string_) PDP::DataStructures::BasicString<T,Alloc>(s);
            //MAXMM_ASSERT_INVARIANT;
        }

        // @bief Initialize the maxmm to be an append node
        // of 2 sub maxmms.
        Rope(Rope<T,Alloc>&& lhs,
             Rope<T,Alloc> && rhs)
                noexcept: tag_(tag::APPEND),
                  height_(LD::Max(lhs.height_, rhs.height_) + 1),
                  size_(lhs.size_ + rhs.size_),
                  parent_(nullptr){



            append_ = append(LD::Move(lhs),LD::Move(rhs),this);


            //new (&append_) rope::append(std::move(lhs),
            //std::move(rhs),
            //this);
            //MAXMM_ASSERT_INVARIANT;
        }

        // @brief Move Constructor
        Rope(Rope<T,Alloc> && copy) noexcept
        {



            switch(copy.tag_)
            {
                case tag::STRING:
                {
                    this->string_ = LD::Move(copy.string_);
                    //new (&string_) PDP::DataStructures::BasicString<T,Alloc>(std::move(copy.string_));break;
                }

                case tag::APPEND:
                {
                    this->append_ = LD::Move(copy.append_);
                    //new (&append_) append(std::move(copy.append_), this);break;
                }
            }

            tag_    = copy.tag_;
            size_   = copy.size_;
            height_ = copy.height_;
            parent_ = copy.parent_;

            // -- Note --
            //
            // It's intentional that we left the [copy] unmodified besides
            // moving it's [string_] or [append_] data members.
            //
            // This pmaxmmrty is needed and will be leveraged in the
            // ::append_string() member function.

            //MAXMM_ASSERT_INVARIANT;
        }

        // Destructors
        // -----------

        // @brief Destructor
        ~Rope()
        {

        }

        // Accessors
        // ---------

        // @brief total size of the maxmm string
        //
        // It's the sum of all the sub strings (ie leaves).
        PDP::UInteger size() const {
            //MAXMM_ASSERT_INVARIANT;
            return size_;
        }

        // @brief return the height of the maxmm.
        // - Internal height of the maxmm tree data structure
        // - Only used for testing
        PDP::UInteger height() const {
            //MAXMM_ASSERT_INVARIANT;
            return height_;
        }

        // @brief non const character accessor.
        constexpr char& operator[](const PDP::UInteger & index) noexcept {
            //MAXMM_ASSERT_INVARIANT;
            switch(tag_)
            {

                case tag::STRING:
                {
                    //LD::ContextualVariant
                    auto onString = [&](auto && string)->char&
                    {
                        return string[index];
                    };
                    return LD::Match(*string_,onString);
                    //return (*string_)[index];
                } break;

                case tag::APPEND:
                {
                    PDP::UInteger lhssize_ = (*append_).lhs_->size_;
                    if(index >= lhssize_) {
                        return (*append_).rhs_->operator[](index - lhssize_);
                    }
                    else {
                        return (*append_).lhs_->operator[](index);
                    }
                } break;

            } // switch
        }

        // @brief const character accessor.
        constexpr const char & operator[](const PDP::UInteger & index) const noexcept
        {
            //MAXMM_ASSERT_INVARIANT;

            Rope<T,Alloc>& self = const_cast<Rope<T,Alloc>&>(*this);
            return self[index];
        }

        // Modifiers
        // ---------

        // @brief move and append a string to the maxmm.
        template<LD::UInteger Size>
        Rope<T,Alloc> & AppendString(LD::ImmutableString<Size> && s)
        {
            //MAXMM_ASSERT_INVARIANT;

            // -- Note --
            //
            // The tree is kept almost balance since the height
            // of the tree might at most (log2 n + 1) rather than
            // log2n for a perfectly balance tree.
            //
            // [1]
            // The key part of the algorithm is when we insert
            // into a perfectly balance tree we create a new
            // tree with the perfectly balance tree on the left and the
            // appended string on the right
            //
            //       x
            //     /   \
            //    x     x     +   new string `e`
            //   / \   / \
            //  a   b c   d
            //
            //  will become
            //
            //            x
            //         /     \
            //       x         e
            //     /   \
            //    x     x
            //   / \   / \
            //  a   b c   d
            //
            //
            // [2]
            // The seond part ofthe algorithm is when we need to
            // append a string to a leaf node. For instance if you take the
            // above tree, for the next append you need to replace the 'e' rope
            // of tag [tag::STRING] with an [tag::APPEND] in the following way:
            //
            //            x
            //         /     \
            //       x         x
            //     /   \      / \
            //    x     x    e   f
            //   / \   / \
            //  a   b c   d
            //
            // [3]
            // We then keep on following the same logic: the append node
            // composed of [e] and [f] is the left most tree which is
            // perfectly balanced and will therefore be replaced by an
            // imbalance tree following the same logic as in [1].


            PDP::UInteger ssize_ = s.GetSize();

            switch(tag_)
            {

                case tag::STRING:
                {
                    // Case [2] in notes above.

                    append tmp(LD::Move(*this),
                               Rope(LD::Move(s)),
                               this);
                    // We create a tmp node by moving
                    // [this]. This intermediate step
                    // is necessary since we need to properly
                    // call the PDP::DataStructures::BasicString<T,Alloc> destructor before
                    // assigning the [append_] variable.

                    //using PDP::DataStructures::BasicString;




                    //string_.PDP::DataStructures::~PDP::DataStructures::BasicString();


                    this->append_ = append(LD::Move(tmp),this);
                    //new (&append_) append(std::move(tmp), this);


                    tag_    = tag::APPEND;

                } break;

                case tag::APPEND:
                {

                    if((*append_).rhs_->height_ >= (*append_).lhs_->height_) {
                        // Case [1] in the note

                        append tmp(LD::Move(*this),
                                   Rope(LD::Move(s)),
                                   this);
                        // Similarly we need to create a tmp variable
                        // with [this] data so that we can properly call
                        // the [append] destructor.

                        //append_.~append();

                        this->append_ = append(LD::Move(tmp),this);
                        //new (&append_) append(std::move(tmp), this);
                        tag_    = tag::APPEND;
                    }
                    else {
                        // append on the rhs which has room.
                        (*append_).rhs_->AppendString(LD::Move(s));
                    }

                } break;

            }

            size_  += ssize_;
            height_ = LD::Max((*append_).rhs_->height_, (*append_).lhs_->height_) + 1;

            // -- Note --
            //
            // The [parent_] is unchanged due to the pmaxmmrty of the
            // move constructor [rope::(t&& copy)] which does not modify the data
            // members. So even if we called [std::move(this)] above, the
            // [parent_] is still valid.

            return *this;
        }

        // @brief append a string to the maxmm.
        //
        // allocation+copy will be made.
        template<LD::UInteger Size>
        Rope<T,Alloc> & AppendString(const LD::ImmutableString<Size> & rhs)
        {
            return this->AppendString (LD::ImmutableString<Size>(rhs));
        }
    private:


    private:

        enum class tag
        {
            STRING,
            APPEND
        };

        struct append
        {
            append(Rope&& lhs,
                   Rope&& rhs,
                   Rope*   self)
                    : lhs_(new Rope(LD::Move(lhs)))
                    ,rhs_(new Rope(LD::Move(rhs)))
            {

                lhs_->parent_ = self;
                rhs_->parent_ = self;
            }

            append(append&& copy, Rope* self)
                    : lhs_(LD::Move(copy.lhs_)),
                      rhs_(LD::Move(copy.rhs_)) {

                lhs_->parent_ = self;
                rhs_->parent_ = self;
            }

            // append(append&& copy) = delete;
            //append(append const& copy) = delete;
            //append() = delete;
            // Disabled constructor
            LD::UniquePointer<Rope<T,Alloc>> lhs_;
            LD::UniquePointer<Rope<T,Alloc>> rhs_;
        };


        /*
        union {
            PDP::DataStructures::BasicString<T,Alloc> string_;
            append      append_;
        };
         */


        PDP::Optional<CompressedStorage> string_;
        PDP::Optional<append> append_;

    private:

        // @brief return the left most string of the maxmm data structure.
        Rope* left_most_string() {
            //MAXMM_ASSERT_INVARIANT;

            if(tag_ == tag::STRING) {
                return this;
            }
            else {
                return (*append_).lhs_->left_most_string();
            }
        }

        // @brief return the next string when traversing the maxmm in a depth
        // first left to right manner.
        //
        // @return nullptr if no more leaf string
        // nodes
        Rope* next_string_leaf() {
            //MAXMM_ASSERT_INVARIANT;

            if(parent_ == nullptr) {
                // root node
                return nullptr;
            }

            MAXMM_ASSERT(parent_->tag_ == tag::APPEND);
            // Check invariant.

            bool am_i_lhs = parent_->append_->lhs_.get() == this;
            if(am_i_lhs) {
                // I am the lhs side node of my parent,
                // the next one is my sibling the rhs.
                return parent_->append_->rhs_.get ();
            }

            // I am the rhs side of my parent, we need to
            // go up a level and do this again to reach cousins/uncles/

            return parent_->next_string_leaf();
        }
    private:

        tag         tag_;
        Alloc CurrentAllocator;
        PDP::UInteger size_;
        PDP::UInteger height_;
        Rope<T,Alloc>*       parent_; // nullptr if Root node.
    };





} // namespace maxmm
#undef MAXMM_ASSERT
#undef MAXMM_ASSERT_INVARIANT
#endif // !defined(__MAXMM__ROPE__H__)
