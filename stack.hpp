#ifndef LIANG_STACK_HPP__
#define LIANG_STACK_HPP__
namespace liang
{
    #ifndef MAX_SIZE
    #define MAX_SIZE 1024
    #endif
    template<typename T>
    struct stack
    {
    private:
        typedef unsigned int uint;
        typedef T*  iterator;
        iterator begin;
        iterator end;
        T _dat[MAX_SIZE];
        uint _size;
    public:
        void pop(){
            if(_size==0)
                return;
            _size--;
            end--;
        }
        void push(T elem){
            if(size==MAX_SIZE)
                return;
            _dat[_size]=elem;
            _size++;
            end++;
        }
        uint size(){
            return _size;
        }
        void emplace(T elem){
            push(elem);
        }
        bool empty(){
            return _size==0;
        }
        T top(){
            if(_size==0)
                return T(0);
            return *(end-1);
        }
        stack():_size(0),begin(_dat),end(_dat){}
    };
}
#endif