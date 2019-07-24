#ifndef LIANG_MAP_HPP
#define LIANG_MAP_HPP
namespace liang{
    #include "pair.hpp"
    #include "queue.hpp"
    typedef unsigned int uint;
    #ifndef MAX_SIZE 
    #define MAX_SIZE 1024
    #endif
    template<typename keyTy,typename valTy>
    struct map{
        template<typename kTy,typename vTy>
        struct decorator{
            public:
            pair<kTy,vTy> _pair;
            uint _index;
            bool operator ==(const decorator<kTy,vTy>&dec){
                return this->_pair == dec._pair;
            }
            bool operator >(const decorator<kTy,vTy>&dec){
                return this->_pair >dec._pair;
            }
            bool operator < (const decorator<kTy,vTy>&dec){
                return this->_pair < dec._pair;
            }
            bool operator !=(const decorator<kTy,vTy>&dec){
                return this->_pair!=dec._pair;
            }
            //取值运算符重载后，返回键值对
            pair<kTy,vTy> &operator *(const decorator<kTy,vTy>&dec){
                return dec._pair;
            }
            decorator<kTy,vTy>& operator =(const decorator<kTy,vTy>&dec){
                this->_pair=dec._pair;
                this->_index=dec._index;
                return *this;
            }
        };
        template<typename N_kTy,typename N_vTy>
        struct Node{
            decorator<N_kTy,N_vTy> *_val;  
            Node *left;
            Node *right;
            Node *parent;
            uint left_height;
            uint right_height;
            Node(decorator<N_kTy,N_vTy> *val):_val(val),left(NULL),right(NULL),parent(NULL),left_height(0),right_height(0)
            {}
        };
        //每个dec动态分配，pair数组保存pair地址,tree保存dec地址
        template<typename _kTy,typename _vTy>
        struct m_tree{
            bool insert(Node<_kTy,_vTy> *root,decorator<_kTy,_vTy>*_val){
                if(*(root->_val)==(*_val)){
                    return false;
                }
                if(*(root->_val)>*_val){
                    if(root->left){
                        return insert(root->left,_val);
                    }
                    else{
                        root->left_height++;
                        Node<_kTy,_vTy>*pos=new Node<_kTy,_vTy>(_val);
                        pos->parent=root;
                        root->left=pos;
                        
                    }
                }
                else{
                    if(root->right){
                        return insert(root->right,_val);
                    }
                    else{
                        root->right_height++;
                        Node<_kTy,_vTy>*pos=new Node<_kTy,_vTy>(_val);
                        pos->parent=root;
                        root->right=pos;
                    }
                }
                return true;
            }             
            uint height(Node<_kTy,_vTy>*root){
                if(!root)return 0;
                uint lt=height(root->left);
                uint rt=height(root->right);
                root->left_height=lt;
                root->right_height=rt;
                return lt>rt?(lt+1):(rt+1);           
            }
            //更新各节点的左右子树高度
            void updateHeight(Node<_kTy,_vTy>*&root){
                if(!root)return;
                root->left_height=height(root->left);
                root->right_height=height(root->right);
            }

            bool isbalanced(Node<_kTy,_vTy>*root,Node<_kTy,_vTy>*&who){
                if(!root)return true;
                int det=root->left_height-root->right_height;  //计算平衡因子
                if(abs(det)>1){
                    who=root;
                    return false;
                }
                return isbalanced(root->left,who) && isbalanced(root->right,who);
            }
            void adjust(Node<_kTy,_vTy>*&root,Node<_kTy,_vTy>*&who){
                if(who->left_height>who->right_height){  //左子树高于右子树，则说明左子树需要调整
                    Node<_kTy,_vTy>*who_left=who->left;
                    if(who_left->left_height>who_left->right_height){  //左左情况,直接对不平衡节点右旋
                        if(root==who){          //不平衡节点是树根节点
                            root=adjustRight(root);
                            root->parent=NULL;
                        }
                        else{
                            Node<_kTy,_vTy>*pwho=who->parent;
                            pwho->left=adjustRight(who);
                            pwho->left->parent=pwho;
                        }
                    }
                    else{                //左右情况，先左旋后右旋
                        if(root==who){
                            root->left=adjustLeft(who_left);  //左旋子树
                            root->left->parent=root;
                            root=adjustRight(root);  //右旋自身
                            root->parent=NULL;
                        }
                        else{
                            Node<_kTy,_vTy>*pwho=who->parent;
                            who->left=adjustLeft(who_left);  //左旋子树
                            who->left->parent=who;
                            pwho->left=adjustRight(who);  //右旋自身
                            pwho->left->parent=pwho;  
                        }
                    }
                }
                else{
                    Node<_kTy,_vTy>*who_right=who->right;
                    if(who_right->left_height<who_right->right_height){  //右右
                        if(root==who){
                            root=adjustLeft(root);
                            root->parent=NULL;
                        }
                        else{
                            Node<_kTy,_vTy>*pwho=who->parent;
                            pwho->right=adjustLeft(who);
                            pwho->right->parent=pwho;
                        }
                    }
                    else{          //右左
                        if(root==who){
                            root->right=adjustRight(who_right);   //右旋右子树
                            root->right->parent=root;
                            root=adjustLeft(root);   //左旋自身
                            root->parent=NULL;
                        }
                        else{
                            Node<_kTy,_vTy>*pwho=who->parent;
                            who->right=adjustRight(who_right);
                            who->right->parent=who;
                            pwho->right=adjustLeft(who);
                            pwho->right->parent=pwho;
                        }
                    }
                }
            }   
            /* 左旋 */
            Node<_kTy,_vTy>*adjustLeft(Node<_kTy,_vTy>*&who){
                Node<_kTy,_vTy> *right=who->right;
                who->parent=right;
                who->right=right->left;
                if(who->right)
                    who->right->parent=who;
                right->left=who;
                return right;
            }
            /* 右旋 */
            Node<_kTy,_vTy>*adjustRight(Node<_kTy,_vTy>*&who){
                Node<_kTy,_vTy> *left=who->left;
                who->parent=left;
                who->left=left->right;
                if(who->left)
                    who->left->parent=who;
                left->right=who;
                return left;
            }                  
            /*寻找某个节点 */
            bool findNode(Node<_kTy,_vTy>*&root,decorator<_kTy,_vTy> &val,Node<_kTy,_vTy>*&who){
                if(!root)return false;
                if(root->_val->_pair == val._pair){
                    who=root;
                    return true;
                }
                if(root->_val->_pair > val._pair)return findNode(root->left,val,who);
                else return findNode(root->right,val,who);
            }   
            //找到节点右子树的最左子节点
            void rtreeleftest(Node<_kTy,_vTy>*root,Node<_kTy,_vTy>*&leftest){ 
                if(!root)return;     
                if(root->left){
                    rtreeleftest(root->left,leftest);
                }
                else
                    leftest=root;
                return;
            }
            //找左子树最左子节点
            void ltreerightest(Node<_kTy,_vTy>*root,Node<_kTy,_vTy>*&rightest){
                if(!root)return;
                if(root->right)ltreerightest(root->right,rightest);
                else
                    rightest=root;
                return;
            }  
            /* 删除一个节点 */
            Node<_kTy,_vTy>* delNode(Node<_kTy,_vTy>*&who){
                Node<_kTy,_vTy>*newNode=NULL;
                rtreeleftest(who->right,newNode);  //寻找右子树最左节点
                if(!newNode){     //没有右子树最左节点
                    ltreerightest(who->left,newNode); //寻找左子树的最右子节点
                    if(!newNode){  //没有左右子树
                        delete who->_val;  //释放decorator
                        who->_val=NULL;
                        delete who;
                        who=NULL;
                        return NULL;
                    }
                    else{      //找到左子树的最右子节点
                        Node<_kTy,_vTy>*pwho=who->parent;
                        if(pwho){
                            if(pwho->left==who)pwho->left=newNode; 
                            if(pwho->right==who)pwho->right=newNode;
                            newNode->parent=pwho;
                        }
                        else 
                            newNode->parent=NULL;

                        if(who->left==newNode){
                            newNode->right=who->right;
                            if(who->right)who->right->parent=newNode;
                        }
                        else{
                            Node<_kTy,_vTy>*pnew=newNode->parent;
                            pnew->right=newNode->left;
                            if(newNode->left)newNode->parent=pnew;
                            newNode->right=who->right;
                            if(who->right)who->right->parent=newNode;
                            newNode->left=who->left;
                            who->left->parent=newNode;
                        }
                    }
                }
                else{       //存在右子树最左子节点
                    Node<_kTy,_vTy>*pwho=who->parent;
                    if(pwho){
                        if(pwho->left==who)pwho->left=newNode;
                        if(pwho->right==who)pwho->right=newNode;
                        newNode->parent=pwho;
                    }
                    else 
                        newNode->parent=NULL;

                    if(who->right==newNode){
                        newNode->left=who->left;
                        if(who->left)
                            who->left->parent=newNode;
                    }
                    else{
                        Node<_kTy,_vTy>*pnew=newNode->parent;
                        pnew->left=newNode->right;
                        if(newNode->right)
                            newNode->right->parent=pnew;
                        newNode->left=who->left;
                        if(who->left)who->left->parent=newNode;
                        newNode->right=who->right;
                        who->right->parent=newNode;
                    } 
                }
                delete who->_val;
                who->_val=NULL;
                delete who;
                who=NULL;
                return newNode;
            }
            void del(Node<_kTy,_vTy>*&root,Node<_kTy,_vTy>*&who){
                if(!who->left && !who->right){   //删除叶子节点
                    if(who->parent->left==who){
                        who->parent->left=NULL;
                    }
                    else
                        who->parent->right=NULL;
                    delete who->_val;
                    who->_val=NULL;
                    delete who;
                    who=NULL;
                }
                else{
                    if(who==root)
                        root=delNode(who);
                    else{
                        Node<_kTy,_vTy>*pwho=who->parent;
                        if(pwho->left==who)pwho->left=delNode(who);
                        else{pwho->right=delNode(who);}
                    }
                }
                updateHeight(root);
                Node<_kTy,_vTy>*which=NULL;
                if(!isbalanced(root,which))
                {
                    adjust(root,which);
                    updateHeight(root);
                }
            } 
            void del(decorator<_kTy,_vTy> &_val){
                Node<_kTy,_vTy>*who=NULL;
                if(!findNode(tree_root,_val,who)){
                    std::cout<<"not exist this node\n";
                    return;
                }
                del(tree_root,who);
            }           
            void destroy(Node<_kTy,_vTy> *&root){
                if(!root)return;
                destroy(root->left);
                destroy(root->right);
                delete root->_val;
                root->_val=NULL;
                delete root;
                root=NULL;
            }   
          
            bool find(decorator<_kTy,_vTy> &_val){
                Node<_kTy,_vTy>*who=NULL;
                if(findNode(tree_root,_val,who)){
                    _val=*(who->_val);
                    return true;
                }
                return false;
            }   

            void adjust(){
                updateHeight(tree_root);
                Node<_kTy,_vTy>*who=NULL;
                if(!isbalanced(tree_root,who)){
                    adjust(tree_root,who);
                    updateHeight(tree_root);
                }
            }    
            void updateIndex(Node<_kTy,_vTy>*&root,uint _index){
                if(!root)return;
                if(root->_val->_index > _index)
                    root->_val->_index--;
                updateIndex(root->left,_index);
                updateIndex(root->right,_index);
            } 
            void destroy(){
                destroy(tree_root);
            }   
            void bfsprint(){
                liang::queue<Node<_kTy,_vTy>*>que;
                que.push(tree_root);
                while(!que.empty()){
                    uint size=que.size();
                    while(size--){
                        Node<_kTy,_vTy>*_n=que.front();
                        que.pop();
                        if(_n){
                            std::cout<<_n->_val->_pair.first<<" ";
                            que.push(_n->left);
                            que.push(_n->right);
                        }
                        else{
                            std::cout<<"nil"<<" ";
                        }
                    }
                    std::cout<<"\n";
                }
            }
            Node<_kTy,_vTy>*tree_root; 
            m_tree():tree_root(NULL){
            }
        };
     
        private:
        m_tree<keyTy,valTy>_tree;   //创建树
        uint _size;
        pair<keyTy,valTy>*_pairs[MAX_SIZE]; //指针数组保存pair地址

        void delpair(uint _index){
            for(;_index!=_size-1;++_index){
                _pairs[_index]=_pairs[_index+1];
            }
            _pairs[_index]=NULL;
            _size--;
        }  
        public:
        void insert(pair<keyTy,valTy>&_p){
            if(_size==MAX_SIZE)return;
            decorator<keyTy,valTy>*dec=new decorator<keyTy,valTy>;
            dec->_pair=_p;
            dec->_index=_size;
            if(_size==0){
                _tree.tree_root=new Node<keyTy,valTy>(dec);
                _pairs[_size]=&dec->_pair;
                _size++;
                return;
            }
            if(_tree.insert(_tree.tree_root,dec))
            {
                _pairs[_size]=&dec->_pair;
                _size++;
                _tree.adjust();  //插入后树进行调整
            }
            else{
                std::cout<<"insert fail\n";
                delete dec;
                dec=NULL;
            }
        }
        pair<keyTy,valTy>*at(uint i){
            if(i>=_size)return NULL;
            else 
                return _pairs[i]; 
        }
        pair<keyTy,valTy>*find(const keyTy &_k){
            decorator<keyTy,valTy>dec;
            dec._pair.first=_k;
            if(_tree.find(dec))return _pairs[dec._index];
            return NULL;
        }
        uint size(){
            return _size;
        }
        void erase(keyTy _key){
            decorator<keyTy,valTy>_dec;
            _dec._pair.first=_key;
            if(_tree.find(_dec)){
                delpair(_dec._index);   //从数组中删除
                _tree.del(_dec);
                _tree.updateIndex(_tree.tree_root,_dec._index);  //在树中更新索引
            }           
        }  
        uint count(keyTy _k){
            decorator<keyTy,valTy>dec;
            dec._pair.first=_k;
            if(_tree.find(dec))return 1;
            return 0;
        }
        pair<keyTy,valTy>* operator [](const uint index){
            if(index>=_size)return NULL;
            return _pairs[index];
        }
        void bfsprint(){
            _tree.bfsprint();
        }
        void destroy(){
            _tree.destroy();
        }
        map():_size(0){}
    };
}
#endif