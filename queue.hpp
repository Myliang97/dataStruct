#ifndef LIANG_QUEUE_HPP__
#define LIANG_QUEUE_HPP__
namespace liang
{
    #ifndef MAX_SIZE
    #define MAX_SIZE 1024
    #endif
    template<typename T>
    struct queue
    {
        private:
            typedef unsigned int uint;
            T _dat[MAX_SIZE];
            uint _size;
            typedef T* iterator;
            iterator begin;
            iterator end;
        public:
            T front(){
                if(_size==0)
                    return T(0);
                return *begin;
            }
            void push(T elem){
                if(_size==MAX_SIZE)
                    return;
                *end=elem;
                end++;
                _size++;
            }
            void pop(){
                if(begin==end)
                    return;
                begin++;
                _size--;
            }
            T &back(){
                return *(end-1);
            }
            uint size(){
                return _size;
            }
            bool empty(){
                return _size==0;
            }
            void swap(queue<T>&que){
                queue<T>temp;
                while(!que.empty()){
                    temp.push(que.front());
                    que.pop();
                }
                while(!this->empty()){
                    que.push(this->front());
                    this->pop();
                }
                while(!temp.empty()){
                    this->push(temp.front());
                    temp.pop();
                }
            }
            queue():_size(0),begin(_dat),end(_dat){}
    };
}
#endif
