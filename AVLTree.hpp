#ifndef LIANG_AVLTREE_HPP__
#define LIANG_AVLTREE_HPP__
#include<iostream>
#include<math.h>
#include<queue>
namespace liang{
    template<typename valTy>
    struct AVLtree{
        typedef unsigned int uint;
        template<typename Ty>
        struct Node{
            valTy _val;
            Node *left;
            Node *right;
            Node *parent;
            uint left_height;
            uint right_height;
            Node(Ty val):_val(val),left(NULL),right(NULL),parent(NULL),left_height(0),right_height(0)
            {}
        };
        private:
        Node<valTy> *tree;
        bool insert(Node<valTy> *root,valTy val){
            if(root->_val==val){
                return false;
            }
            if(root->_val>val){
                if(root->left){
                    root->left_height++;
                    bool ret=insert(root->left,val);
                    if(!ret){
                        root->left_height--;
                        return false;
                    }
                }
                else{
                    root->left_height++;
                    Node<valTy>*pos=new Node<valTy>(val);
                    pos->parent=root;
                    root->left=pos;
                }
            }
            else{
                if(root->right){
                    root->right_height++;
                    bool ret=insert(root->right,val);
                    if(!ret){
                        root->left_height--;
                        return false;
                    }
                }
                else{
                    root->right_height++;
                    Node<valTy>*pos=new Node<valTy>(val);
                    pos->parent=root;
                    root->right=pos;
                }
            }
            return true;
        }
        bool isbalanced(Node<valTy>*&root,Node<valTy>*&who){
            if(!root)return true;
            int det=root->left_height-root->right_height;  //计算平衡因子
            if(abs(det)>1){
                who=root;
                return false;
            }
            return isbalanced(root->left,who) && isbalanced(root->right,who);
        }
        uint height(Node<valTy>*root){
            if(!root)return 0;
            uint lt=height(root->left);
            uint rt=height(root->right);
            return lt>rt?(lt+1):(rt+1);           
        }

        //找到不平衡节点，如果不平衡节点的左子树高度更高导致不平衡，证明需要对左子树做出调整
        //对左子树的左右子树进行高度判断，如果左子树的左子树更高则为左左，如果左子树的右子树更高为左右
        //左左：直接对不平衡节点右旋
        //左右：先对不平衡节点的左子树进行左旋，化为上面的左左情况，再对不平衡节点右旋、
        //右子树导致不平衡情况一样...
        //在左旋右旋过程中，需要对树的左右子树高度进行重新计算
        void adjust(Node<valTy>*&root,Node<valTy>*&who){
            if(who->left_height>who->right_height){  //左子树高于右子树，则说明左子树需要调整
                Node<valTy>*who_left=who->left;
                if(who_left->left_height>who_left->right_height){  //左左情况,直接对不平衡节点右旋
                    if(root==who){          //不平衡节点是树根节点
                        root=adjustRight(root);
                        root->parent=NULL;
                    }
                    else{
                        Node<valTy>*pwho=who->parent;
                        pwho->left=adjustRight(who);
                        pwho->left->parent=pwho;
                    }
                }
                else{                //左右情况，先左旋后右旋
                    if(root==who){
                        root=adjustLeft(who_left);  //左旋子树
                        root->parent=NULL;
                        root=adjustRight(root);  //右旋自身
                        root->parent=NULL;
                    }
                    else{
                        Node<valTy>*pwho=who->parent;
                        who->left=adjustLeft(who_left);  //左旋子树
                        who->left->parent=who;
                        pwho->left=adjustRight(who);  //右旋自身
                        pwho->left->parent=pwho;  
                    }
                    

                }
            }
            else{
                Node<valTy>*who_right=who->right;
                if(who_right->left_height<who_right->right_height){  //右右
                    if(root==who){
                        root=adjustLeft(root);
                        root->parent=NULL;
                    }
                    else{
                        Node<valTy>*pwho=who->parent;
                        pwho->right=adjustLeft(who);
                        pwho->right->parent=pwho;
                    }
                }
                else{          //右左
                    if(root==who){
                        root=adjustRight(who_right);   //右旋右子树
                        root->parent=NULL;
                        root=adjustLeft(root);   //左旋右子树
                        root->parent=NULL;
                    }
                    else{
                        Node<valTy>*pwho=who->parent;
                        who->right=adjustRight(who_right);
                        who->right->parent=who;
                        pwho->right=adjustLeft(who);
                        pwho->right->parent=pwho;
                    }
                }
            }
        }
        /* 左旋 */
        Node<valTy>*adjustLeft(Node<valTy>*&who){
            Node<valTy> *right=who->right;
            who->parent=right;
            who->right=right->left;
            who->right_height=right->left_height;   //重新计算左右子树高度
            right->left=who;
            right->left_height=(who->left_height>who->right_height?(who->left_height+1):(who->right_height+1));
            return right;
        }
        /* 右旋 */
        Node<valTy>*adjustRight(Node<valTy>*&who){
            Node<valTy> *left=who->left;
            who->parent=left;
            who->left=left->right;
            who->left_height=left->right_height;
            left->right=who;
            left->right_height=(who->left_height>who->right_height?(who->left_height+1):(who->right_height+1));
            return left;
        }
        void preprint(Node<valTy>*root){
            if(!root)
                return;
            std::cout<<root->_val<<" ";
            preprint(root->left);
            preprint(root->right);
        }
        void inprint(Node<valTy>*root){
            if(!root)
                return;
            inprint(root->left);
            std::cout<<root->_val<<" ";
            inprint(root->right);
        }
        void print_height(Node<valTy>*root){
            if(!root)return;
            std::cout<<root->_val<<"lh:"<<root->left_height<<"rh:"<<root->right_height<<"\n";
            print_height(root->left);
            print_height(root->right);
        }
        public:
        void insert(valTy val){
            if(!tree){
                tree=new Node<valTy>(val);
                return;
            }
            if(!insert(tree,val)){
                std::cout<<"tree has the same val\n";
                return;
            }
            Node<valTy>*who=NULL;
            if(!isbalanced(tree,who)){
                adjust(tree,who);
            }
        }
        void inprint(){
            inprint(tree);
        }
        void preprint(){
            preprint(tree);
        }
        void bfsprint(){
            std::queue<Node<valTy>*>que;
            que.push(tree);
            while(!que.empty()){
                int size=que.size();
                while(size--){
                    Node<valTy>*_node=que.front();
                    que.pop();
                    if(_node){
                        std::cout<<_node->_val<<" ";
                        que.push(_node->left);
                        que.push(_node->right);                        
                    }
                    else
                        std::cout<<-1<<" ";
                }
                std::cout<<"\n";
            }
        }
        void print_height(){
            print_height(tree);
        }
        AVLtree():tree(NULL){
        }
    };

}
#endif