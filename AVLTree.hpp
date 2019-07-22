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
                    //root->left_height++;
                    //bool ret=insert(root->left,val);  
                    /*if(!ret){       //插入失败，回溯把高度-1
                        root->left_height--;
                        return false;
                    }*/
                    return insert(root->left,val);
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
                  /*  root->right_height++;
                    bool ret=insert(root->right,val);
                    if(!ret){
                        root->left_height--;
                        return false;
                    }*/
                    return insert(root->right,val);
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
        bool isbalanced(Node<valTy>*root,Node<valTy>*&who){
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
            root->left_height=lt;
            root->right_height=rt;
            return lt>rt?(lt+1):(rt+1);           
        }

        //找到不平衡节点，如果不平衡节点的左子树高度更高导致不平衡，证明需要对左子树做出调整
        //对左子树的左右子树进行高度判断，如果左子树的左子树更高则为左左，如果左子树的右子树更高为左右
        //左左：直接对不平衡节点右旋
        //左右：先对不平衡节点的左子树进行左旋，化为上面的左左情况，再对不平衡节点右旋、
        //右子树导致不平衡情况一样...对旋转对象是根节点的特殊处理
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
                        root->left=adjustLeft(who_left);  //左旋子树
                        root->left->parent=root;
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
                        root->right=adjustRight(who_right);   //右旋右子树
                        root->right->parent=root;
                        root=adjustLeft(root);   //左旋自身
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
            if(who->right)
                who->right->parent=who;
            right->left=who;
            return right;
        }
        /* 右旋 */
        Node<valTy>*adjustRight(Node<valTy>*&who){
            Node<valTy> *left=who->left;
            who->parent=left;
            who->left=left->right;
            if(who->left)
                who->left->parent=who;
            left->right=who;
            return left;
        } 
        //更新各节点的左右子树高度
        void updateHeight(Node<valTy>*&root){
            if(!root)return;
            root->left_height=height(root->left);
            root->right_height=height(root->right);
        }
        /*寻找某个节点 */
        bool findNode(Node<valTy>*root,valTy val,Node<valTy>*&who){
            if(!root)return false;
            if(root->_val ==val){
                who=root;
                return true;
            }
            return findNode(root->left,val,who)||findNode(root->right,val,who);
        }
        //找到节点右子树的最左子节点
        void rtreeleftest(Node<valTy>*root,Node<valTy>*&leftest){ 
            if(!root)return;     
            if(root->left){
                rtreeleftest(root->left,leftest);
            }
            else
                leftest=root;
            return;
        }
        //找左子树最左子节点
        void ltreerightest(Node<valTy>*root,Node<valTy>*&rightest){
            if(!root)return;
            if(root->right)ltreerightest(root->right,rightest);
            else
                rightest=root;
            return;
        }
        /* 删除一个节点 */
        Node<valTy>* delNode(Node<valTy>*&who){
            Node<valTy>*newNode=NULL;
            rtreeleftest(who->right,newNode);  //寻找右子树最左节点
            if(!newNode){     //没有右子树最左节点
                ltreerightest(who->left,newNode); //寻找左子树的最右子节点
                if(!newNode){  //没有左右子树
                    delete who;
                    who=NULL;
                    return NULL;
                }
                else{      //找到左子树的最右子节点
                    Node<valTy>*pwho=who->parent;
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
                        Node<valTy>*pnew=newNode->parent;
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
                Node<valTy>*pwho=who->parent;
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
                    Node<valTy>*pnew=newNode->parent;
                    pnew->left=newNode->right;
                    if(newNode->right)
                        newNode->right->parent=pnew;
                    newNode->left=who->left;
                    if(who->left)who->left->parent=newNode;
                    newNode->right=who->right;
                    who->right->parent=newNode;
                } 
            }
            delete who;
            who=NULL;
            return newNode;
        }
        //1.删除根节点，找到根节点的右子树的最左节点作为新的根
        //2.删除叶子节点
        //3.删除非叶子节点
        void del(Node<valTy>*&root,valTy val,Node<valTy>*&who){
            if(!who->left && !who->right){   //删除叶子节点
                if(who->parent->left==who){
                    who->parent->left=NULL;
                }
                else
                    who->parent->right=NULL;
                delete who;
                who=NULL;
            }
            else{
                if(who==root)
                    root=delNode(who);
                else{
                    Node<valTy>*pwho=who->parent;
                    if(pwho->left==who)pwho->left=delNode(who);
                    else{pwho->right=delNode(who);}
                }
            }
            updateHeight(root);
            Node<valTy>*which=NULL;
            if(!isbalanced(root,which))
            {
                adjust(root,which);
                updateHeight(root);
            }
        }
        /* 前序打印*/
        void preprint(Node<valTy>*root){
            if(!root)
                return;
            std::cout<<root->_val<<" ";
            preprint(root->left);
            preprint(root->right);
        }
        
        /* 中序打印 */
        void inprint(Node<valTy>*root){
            if(!root)
                return;
            inprint(root->left);
            std::cout<<root->_val<<" ";
            inprint(root->right);
        }
        
        /*打印节点的左右子树高度*/
        void print_height(Node<valTy>*root){
            if(!root)return;
            std::cout<<" val:"<<root->_val<<" lh:"<<root->left_height<<" rh:"<<root->right_height<<"\n";
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
            updateHeight(tree);
            Node<valTy>*who=NULL;
            if(!isbalanced(tree,who)){
                adjust(tree,who);
                updateHeight(tree);
            }
        }
        void del(valTy val){
            Node<valTy>*who=NULL;
            if(!findNode(tree,val,who)){
                std::cout<<"not exist this node\n";
                return;
            }
            del(tree,val,who);
        }
        void inprint(){
            inprint(tree);
        }
        void preprint(){
            preprint(tree);
        }

        /* 宽度优先打印*/
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