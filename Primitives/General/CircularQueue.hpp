//
// Created by phoenixflower on 9/21/21.
//

#ifndef LANDESSDEVCORE_CIRCULARQUEUE_HPP
#define LANDESSDEVCORE_CIRCULARQUEUE_HPP
#include "Algorithms/Forward.hpp"
#include "Definitions/Integer.hpp"
#include "Algorithms/Move.hpp"
#include "TypeTraits/Iterable.h"
#include "TypeTraits/Detection.hpp"
#include "TypeTraits/IsConvertible.hpp"
//#include <type_traits>
/*
namespace LD
{
    template<class T, bool B> union Cell; //bool B == std::is_trivially_destructible<T>::value
    template<class T>
    union Cell<T, true>{
        class emptyClass{} forConstexprCtor;
        T value;
        //Initializes forConstexprCtor because constexpr union constructors must initialize a member
        constexpr Cell() : forConstexprCtor{} {}
        //Initializes value with the provided parameter arguments
        template<typename... Args>
        constexpr Cell(Args&&... args) : value(LD::Forward<Args>(args)...) {}
    };
    template<class T>
    union Cell<T, false>{
        class emptyClass{} forConstexprCtor;
        T value;
        constexpr Cell() : forConstexprCtor{} {}
        template<typename... Args>
        constexpr Cell(Args&&... args) : value(LD::Forward<Args>(args)...) {}
        ~Cell(){} //Included because Cell<T, false>'s destructor is deleted
    };
    template<class T, LD::UInteger N, bool B, typename Idxtype> struct theQueue;

    template<class T, LD::UInteger N, typename Idxtype>
    struct theQueue<T, N, true, Idxtype>{
        constexpr theQueue() = default; //Default constructor
        //Copy constructor
        constexpr theQueue(const theQueue& other) : head(other.head), tail(other.tail), theSize(other.theSize){
            std::size_t originalHead(other.head);
            //If other is full, there's a chance that other.head == other.tail
            if(other.head > other.tail || (other.head == other.tail && other.theSize == N)){
                for(; originalHead < N; ++originalHead){
                    if constexpr(std::is_trivially_copy_assignable<T>::value){
                        theArray[originalHead] = other.theArray[originalHead];
                    } else {
                        new(&theArray[originalHead].value)T(other.theArray[originalHead].value);
                    }
                }
                originalHead = 0;
            }
            for(; originalHead < other.tail; ++originalHead){
                if constexpr(std::is_trivially_copy_assignable<T>::value){
                    theArray[originalHead] = other.theArray[originalHead];
                } else {
                    new(&theArray[originalHead].value)T(other.theArray[originalHead].value);
                }
            }
        }
        constexpr theQueue(theQueue&& other) : head(other.head), tail(std::move(other.tail)),
                                               theSize(std::move(other.theSize)){ //Move constructor
            std::size_t originalHead(std::move(other.head));
            //If other is full, there's a chance that other.head == other.tail
            if(other.head > other.tail || (other.head == other.tail && other.theSize == N)){
                for(; originalHead < N; ++originalHead){
                    if constexpr(std::is_trivially_copy_assignable<T>::value){
                        theArray[originalHead] = std::move(other.theArray[originalHead]);
                    } else {
                        new(&theArray[originalHead].value)T(std::move(other.theArray[originalHead].value));
                    }
                }
                originalHead = 0;
            }
            for(; originalHead < other.tail; ++originalHead){
                if constexpr(std::is_trivially_copy_assignable<T>::value){
                    theArray[originalHead] = std::move(other.theArray[originalHead]);
                } else {
                    new(&theArray[originalHead].value)T(std::move(other.theArray[originalHead].value));
                }
            }
        }
        //Constructor which accepts arguments to construct theArray
        template<typename... Args, typename =
        typename std::enable_if<(... && std::is_constructible_v<T,Args>)>::type >
        explicit constexpr theQueue(Args&&... theList) : head{0}, tail(sizeof...(theList)), theSize(sizeof...(theList)),
                                                         theArray{LD::Forward<Args>(theList)...}{}
        constexpr theQueue& operator=(const theQueue& other){//Copy assignment
            std::size_t originalHead(head = other.head);
            if constexpr(!std::is_trivially_destructible<T>::value){
                clear();
            }
            if(other.head > other.tail || (other.head == other.tail && other.theSize == N)){
                for(; originalHead < N; ++originalHead){
                    if constexpr(std::is_trivially_copy_assignable<T>::value){
                        theArray[originalHead] = other.theArray[originalHead];
                    } else {
                        new(&theArray[originalHead].value)T(other.theArray[originalHead].value);
                    }
                }
                originalHead = 0;
            }
            for(; originalHead < other.tail; ++originalHead){
                if constexpr(std::is_trivially_copy_assignable<T>::value){
                    theArray[originalHead] = other.theArray[originalHead];
                } else {
                    new(&theArray[originalHead].value)T(other.theArray[originalHead].value);
                }
            }
            tail = other.tail;
            theSize = other.theSize;
            return *this;
        }
        constexpr theQueue& operator=(theQueue&& other){//Move assignment
            std::size_t originalHead(head = other.head);
            if constexpr(!std::is_trivially_destructible<T>::value){
                clear();
            }
            if(other.head > other.tail || (other.head == other.tail && other.theSize == N)){
                for(; originalHead < N; ++originalHead){
                    if constexpr(std::is_trivially_copy_assignable<T>::value){
                        theArray[originalHead] = std::move(other.theArray[originalHead]);
                    } else {
                        new(&theArray[originalHead].value)T(LD::Move(other.theArray[originalHead].value));
                    }
                }
                originalHead = 0;
            }
            for(; originalHead < other.tail; ++originalHead){
                if constexpr(std::is_trivially_copy_assignable<T>::value){
                    theArray[originalHead] = LD::Move(other.theArray[originalHead]);
                } else {
                    new(&theArray[originalHead].value)T(std::move(other.theArray[originalHead].value));
                }
            }
            tail = LD::Move(other.tail);
            theSize = LD::Move(other.theSize);
            return *this;
        }
        //Container modifying functions
        constexpr bool push(const T& theObj){//Pushes the given element value to the end of the queue
            if(!checkSizeAndIndex()) return false;
            if constexpr(std::is_trivially_copy_assignable<T>::value){
                theArray[tail++] = Cell<T,true>(theObj);
            } else {
                new(&theArray[tail++].value)T(theObj);
            }
            return ++theSize; //++theSize always > 0. Return true
        }
        constexpr bool push(T&& theObj){//Pushes the given element value to the end of the queue
            if(!checkSizeAndIndex()) return false;
            if constexpr(std::is_trivially_copy_assignable<T>::value){
                theArray[tail++] = Cell<T,true>(LD::Move(theObj));
            } else {
                new(&theArray[tail++].value)T(std::move(theObj));
            }
            return ++theSize; //++theSize always > 0. Return true
        }
        template<typename ...Args>
        constexpr bool emplace(Args&&... args){ //Same as push, but the element is constructed in-place
            if(!checkSizeAndIndex()) return false;
            if constexpr(std::is_trivially_copy_assignable<T>::value){
                theArray[tail++] = Cell<T,true>(LD::Forward<Args>(args)...);
            } else {
                new(&theArray[tail++].value)T(LD::Forward<Args>(args)...);
            }
            return ++theSize;
        }
        constexpr bool pop() noexcept{ //Removes the element at the queue's front
            if(!theSize) return false; //If it's empty, pop fails
            if constexpr(std::is_trivially_destructible<T>::value){
                (head == N - 1 ? head = 0 : ++head);
            } else {
                if(head == N - 1){
                    theArray[head].value.~T();
                    head = 0;
                } else {
                    theArray[head++].value.~T();
                }
            }
            --theSize;
            return true;
        }

        //Capacity Methods
        constexpr bool full() const noexcept {return theSize == N;} //Check if queue is full
        constexpr bool empty() const noexcept {return !theSize;} //Check if queue is empty
        constexpr Idxtype size() const noexcept {return theSize;} //Returns the queue's current size
        //Element Access functions
        //Returns the max number of elements the queue may hold
        constexpr LD::UInteger capacity() const noexcept {return N;}
        //Returns the element next to be popped. Undefined behavior if queue is empty
        constexpr const T& front() const {return theArray[head].value;}
        constexpr T& front() {return theArray[head].value;}
        //Returns the element last to be popped. Undefined behavior if queue is empty
        constexpr const T& back() const {return theArray[tail - 1].value;}
        constexpr T& back() {return theArray[tail - 1].value;}
    protected:
        Idxtype head{0}, tail{0}, theSize{0};
        Cell<T, std::is_trivially_destructible<T>::value> theArray[N];
        constexpr void clear(){ //Destroys value in the queue when value is the active member
            if(head > tail || (head == tail && theSize == N)){
                for(; head < N; ++head){
                    theArray[head].value.~T();
                }
                head = 0;
            }
            for(; head < tail; ++head){
                theArray[head].value.~T();
            }
        }
        //If it's full, nothing is added to the queue.
        //If it reaches the array's end, construct T at index 0
        constexpr bool checkSizeAndIndex(){
            if(theSize == N){
                return false;//queue is full
            }
            if(tail == N){
                tail = 0;
            }
            return true;
        }
    };
    template<class T, LD::UInteger N, typename Idxtype>
    struct theQueue<T, N, false, Idxtype> : public theQueue<T, N, true, Idxtype>{
        template<typename... Args>
        theQueue(Args&&... theList) : theQueue<T, N, true, Idxtype>(LD::Forward<Args>(theList)...) {}

        ~theQueue(){this->clear();}
    };
    template<class T, LD::UInteger N, typename Idxtype = LD::UInteger>
    using CircularQueue = theQueue<T,N,std::is_trivially_destructible<T>::value, Idxtype>;
}
 */
namespace LD
{
    template<typename T, LD::UInteger N>
    class CircularQueue {
    public:
        CircularQueue() { clear(); }
        size_t capacity() const { return N; }
        bool empty() const { return head_ == tail_; }
        size_t size() const {
            return head_ >= tail_ ?
                   head_ - tail_ :
                   BUFSIZE - (tail_ - head_);
        }

        CircularQueue & operator >> (LD::Optional<T> & possibleObject) noexcept
        {
            if (this->size() > 0)
            {
                possibleObject = this->remove();
            }
            return (*this);
        }
        void Insert(const T& item) {
            buffer_[head_] = item;
            advance(head_);
            if (head_ == tail_) {
                advance(tail_); // Drop oldest entry, keep rest.
            }
        }
        const T& remove() {
            //assert(!empty());
            size_t old_tail = tail_;
            advance(tail_);
            return buffer_[old_tail];
        }
        void clear() { tail_ = head_ = 0U; }

    private:
        static const size_t BUFSIZE = N + 1U;
        void advance(size_t& value) { value = (value + 1) % BUFSIZE; }

        T buffer_[BUFSIZE];
        size_t head_;
        size_t tail_;
    };
}
namespace LD
{
    template<typename T, LD::UInteger Size>
    class BackInserter<LD::CircularQueue<T,Size>>
    {
    private:
        LD::ElementReference<LD::CircularQueue<T,Size>> mReference;
    public:
        BackInserter(LD::CircularQueue<T,Size> & inserter) noexcept:mReference{inserter}
        {

        }


        template<typename A, typename V = T>
        LD::Enable_If_T<LD::Require<true>,BackInserter &> operator = (A && object) noexcept
        {
            this->mReference->Insert(V{object});
            return (*this);
        }
    };
}

namespace LD
{


    template<typename T, LD::UInteger Size>
    class ExtractionIterator<LD::CircularQueue<T,Size>>
    {
    private:
        LD::ElementReference<LD::CircularQueue<T,Size>> mBuffer;
    public:
        ExtractionIterator() noexcept:mBuffer{nullptr} {}

        ExtractionIterator(LD::ElementReference<LD::CircularQueue<T,Size>> buffer) noexcept:mBuffer{buffer}
        {

        }

        operator T()
        {
            //T object;
            //while(this->mBuffer->TryPopFront(object) == false);
            //return object;
            return this->mBuffer->remove();
        }


        operator LD::Optional<T>()
        {
            if (!this->mBuffer->empty())
            {
                return LD::Optional<T>{this->mBuffer->remove()};
            }
            return LD::Optional<T>{};
        }
        T operator()() noexcept
        {
            return this->mBuffer->remove();
        }

        T Value() noexcept
        {
            return T{};
        }

        template<typename U, typename V = T>
        LD::Enable_If_T<LD::Require<LD::CT::IsSame(LD::Type<U>{},LD::Type<V>{})>,void> operator()(U & object) noexcept
        {

           // while(this->mBuffer->TryPopFront(object) == false);
        }


        template<typename U, typename V = T>
        LD::Enable_If_T<LD::Require<
                !LD::CT::IsSame(LD::Type<U>{},LD::Type<V>{}),
                LD::IsAssignable<U,V>::value
        >,void> operator()(U & object) noexcept
        {
            T usableObject;
            while(this->mBuffer->TryPopFront(usableObject) == false);
            object = usableObject;
        }


    };
}
#endif //LANDESSDEVCORE_CIRCULARQUEUE_HPP
