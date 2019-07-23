#ifndef LIANG_PAIR_HPP__
#define LIANG_PAIR_HPP__
namespace liang{
    #ifndef MAX_SIZE
    #define MAX_SIZE 1024
    #endif
    template<typename keyTy,typename valTy>
    struct pair
    {
        private:
            typedef pair<keyTy,valTy>* iterator;
            iterator _iter;
        public:
            keyTy first;
            valTy second;
            pair():_iter(this){};
            pair(keyTy key,valTy val){
                this->first=key;
                this->second=val;
                _iter=this;
            }
            iterator get(){
                return _iter;
            }
            pair<keyTy,valTy> &operator =(const pair<keyTy,valTy> &_pair){
                this->first=_pair.first;
                this->second=_pair.second;
                return *this;
            }
            bool operator >(const pair<keyTy,valTy>&_pair){
                return this->first>_pair.first;
            }
            bool operator <(const pair<keyTy,valTy>&_pair){
                return this->first<_pair.first;
            }
            bool operator ==(const pair<keyTy,valTy> &_pair){
                return this->first==_pair.first;
            }
            bool operator !=(const pair<keyTy,valTy>&_pair){
                return this->first!=_pair.first;
            }

    };
    template<typename keyTy,typename valTy>
    pair<keyTy,valTy> make_pair(keyTy key,valTy val){
        pair<keyTy,valTy>_pair(key,val);
        return _pair;
    }
}

#endif