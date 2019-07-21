#ifndef LIANG_DEQUE_H__
#define LAING_DEQUE_H__
namespace liang{
    #ifndef MAX_SIZE
    #define MAX_SIZE 1024
    #endif
    template<typename T>
    struct deque
    {
        private:
            typedef unsigned int uint;
            uint _size;
            T _dat[MAX_SIZE];
            typedef T* iterator;
            iterator _begin;
            iterator _end;
        public:
            T front(){
                return *_begin;
            }
            T back(){
                return *(_end-1);
            }
            void push_back(T elem){
                if(_end==_dat+MAX_SIZE){
                    if(_begin==_dat)
                        return;
                    auto step=_begin-_dat;
                    step/=2;
                    auto move=_begin;
                    for(move;move<_end;++move){
                        *(move-step)=*move;
                    }
                    _begin-=step;
                    _end-=step;
                }
                *_end=elem;
                _end++;
                _size++;
            }
            void push_front(T elem){
                if(_begin==_dat){      //前面的空间被填满则需要向后移动
                    if(_end==_dat+MAX_SIZE)
                        return ;
                    auto step=(_dat+MAX_SIZE)-_end;
                    step/=2;
                    auto move=_end;
                    for(move;move>=_begin;--move){
                        *(move+step)=*move;
                    }
                    _end+=step;
                    _begin+=step;
                }
                --_begin;
                *_begin=elem;
                _size++;
            }
            void pop_back(){
                if(_end==_begin)
                    return;
                _end--;
                _size--;
            }
            void pop_front(){
                if(_begin==_end)
                    return;
                _begin++;
                _size--;
            }
            T at(uint pos){
                return *(_begin+pos);
            }
            iterator begin(){
                return _begin;
            }
            iterator end(){
                return _end;
            }
            void clear(){
                _begin=_dat+400;
                _end=_dat+400;
                _size=0;
            }
            uint size(){
                return _size;
            }
            bool empty(){
                return _size==0;
            }
            T &operator [](uint pos){
                return *(_begin+pos);
            }
            //双端队列在两端均可以插入，预留前面一部分空间，减少复制移动操作
            deque():_size(0),_begin(_dat+400),_end(_dat+400){}
    };
    
}
#endif