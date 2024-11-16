#include "BinaryTree.h"
using namespace std;

template<typename K, typename V>
BinaryTree<K, V>::BinaryTree() {
    root=NULL;
    tsize=0;
    makeEmpty();
}

/**
 *  Returns the number of entries stored in the dictionary.  Entries with
 *  the same key (or even the same key and value) each still count as
 *  a separate entry.
 *  @return number of entries in the dictionary.
 **/
template<typename K, typename V>
int BinaryTree<K, V>::size() {
    return tsize;
}

/**
 *  Tests if the dictionary is empty.
 *
 *  @return true if the dictionary has no entries; false otherwise.
 **/
template<typename K, typename V>
bool BinaryTree<K, V>::isEmpty() {
    return size() == 0;
}

template<typename K, typename V>
void BinaryTree<K, V>::insertHelper(Entry<K, V>* entry, const K& key, BinaryTreeNode<K, V>* node) {
    if (key.compareTo(node->entry->getkey()) <= 0) {
        if (node->leftChild == NULL) {
            node->leftChild = new BinaryTreeNode<K, V>(entry, node);
        } else {
            insertHelper(entry, key, node->leftChild);
        }
    } else {
        if (node->rightChild == NULL) {
            node->rightChild = new BinaryTreeNode<K, V>(entry, node);
        } else {
            insertHelper(entry, key, node->rightChild);
        }
    }
}

/**
 *  Create a new Entry object referencing the input key and associated value,
 *  and insert the entry into the dictionary.
 *  Multiple entries with the same key (or even the same key and
 *  value) can coexist in the dictionary.
 *
 *  @param key the key by which the entry can be retrieved.
 *  @param value an arbitrary object.
 **/
template<typename K, typename V>
void BinaryTree<K, V>::insert(const K& key, const V& value) {
    Entry<K, V>* entry = new Entry<K, V>(key, value);
    if (root == NULL) {
        root = new BinaryTreeNode<K, V>(entry);
    } else {
        insertHelper(entry, key, root);
    }
    tsize++;
}

/**
 *  Search for a node with the specified key, starting from "node".  If a
 *  matching key is found (meaning that key1.compareTo(key2) == 0), return
 *  a node containing that key.  Otherwise, return null.
 *
 *  Be sure this method returns null if node == null.
 **/
template<typename K, typename V>
BinaryTreeNode<K, V>* BinaryTree<K, V>::findHelper(const K& key, BinaryTreeNode<K, V>* node) {
    if (key.compareTo(node->entry->getkey()) < 0) {
        if (node->leftChild == NULL) {
            return NULL;
        } else {
            return findHelper(key, node->leftChild);
        }
    } else if (key.compareTo(node->entry->getkey()) > 0){
        if (node->rightChild == NULL) {
            return NULL;
        } else {
            return findHelper(key, node->rightChild);
        }
    }
    else if (key.compareTo(node->entry->getkey()) == 0) {
        return node;
    }
    return NULL;
}

/**
 *  Search for an entry with the specified key.  If such an entry is found,
 *  return true; otherwise return false.
 *
 *  @param key the search key.
 *  @return an entry containing the key and an associated value, or null if
 *          no entry contains the specified key.
 **/
template<typename K, typename V>
Entry<K, V>* BinaryTree<K, V>::find(const K& key) {
    BinaryTreeNode<K, V>* node = findHelper(key, root);
    if (node != NULL) {
        return node->entry;
    } else {
        return NULL;
    }
}

/**
 *  Remove an entry with the specified key.  If such an entry is found,
 *  remove it from the table.
 *  If several entries have the specified key, choose one arbitrarily, then
 *  remove it.
 *
 *  @param key the search key.
 */
template<typename K, typename V>
void BinaryTree<K, V>::remove(const K& key) {
    BinaryTreeNode<K, V>* node = findHelper(key, root);
    BinaryTreeNode<K, V>* parent_node = node->parent;
    if (node != NULL) {
        tsize--;
        if (parent_node==NULL){
            //Case1. node沒有兒子 (這個node是root)
            if(node->leftChild==NULL && node->rightChild==NULL){
                node=NULL;
            }
            //Case2. node有左兒子 (這個node是root)
            else if(node->leftChild!=NULL && node->rightChild==NULL){
                root=node->leftChild;
                node->leftChild->parent=NULL;
                node->leftChild=NULL;
                free(node);
            }
            //Case3. node有右兒子 (這個node是root)
            else if(node->leftChild==NULL && node->rightChild!=NULL){
                root=node->rightChild;
                node->rightChild->parent=NULL;
                node->rightChild=NULL;
                free(node);
            }
            // Case4. node有左兒子&右兒子 (這個node是root)
            else if(node->leftChild!=NULL && node->rightChild!=NULL){
                BinaryTreeNode<K, V>* right_min = findRightMin(node->rightChild);
                Entry<K, V>* entry = new Entry<K, V>(Integer(right_min->entry->getkey().getvalue()), String(right_min->entry->getvalue().getvalue()));
                BinaryTreeNode<K, V>* new_node = new BinaryTreeNode<K, V>(entry);
                root = new_node;
                new_node->leftChild=node->leftChild;
                node->leftChild->parent=new_node;
                node->leftChild=NULL;
                new_node->rightChild=node->rightChild;
                node->rightChild->parent=new_node;
                node->rightChild=NULL;
                free(node);
                // 刪節點 情形一:right_min沒有兒子
                if (right_min->rightChild==NULL){
                    if(right_min->entry->getkey().getvalue() < right_min->parent->entry->getkey().getvalue()){
                        right_min->parent->leftChild=NULL;
                        right_min->parent=NULL;
                    }
                    else{
                        right_min->parent->rightChild=NULL;
                        right_min->parent=NULL;
                    }
                }
                // 刪節點 情形二:right_min有右兒子
                else{
                    if(right_min->entry->getkey().getvalue() < right_min->parent->entry->getkey().getvalue()){
                        right_min->parent->leftChild=right_min->rightChild;
                        right_min->rightChild->parent=right_min->parent;
                        right_min->parent=NULL;
                        right_min->rightChild=NULL;
                    }
                    else{
                        right_min->parent->rightChild=right_min->rightChild;
                        right_min->rightChild->parent=right_min->parent;
                        right_min->parent=NULL;
                        right_min->rightChild=NULL;
                    }
                }
                free(right_min);
            }
        }else{
            //Case1. node沒有兒子
            if(node->leftChild==NULL && node->rightChild==NULL){
                //node是老爸的左邊兒子
                if(node->entry->getkey().getvalue() <= parent_node->entry->getkey().getvalue()){
                    parent_node->leftChild=NULL;
                    node->parent=NULL;
                }
                else{
                    parent_node->rightChild=NULL;
                    node->parent=NULL;
                }
                free(node);
                return;
            }
            //Case2. node有左兒子
            else if(node->leftChild!=NULL && node->rightChild==NULL){
                BinaryTreeNode<K, V>* left_node = node->leftChild;
                //node是老爸的左邊兒子
                if(node->entry->getkey().getvalue() <= parent_node->entry->getkey().getvalue()){
                    parent_node->leftChild=left_node;
                    left_node->parent=parent_node;
                    node->parent=NULL;
                    node->leftChild=NULL;
                }
                //node是老爸的右邊兒子
                else{
                    parent_node->rightChild=left_node;
                    left_node->parent=parent_node;
                    node->parent=NULL;
                    node->leftChild=NULL;
                }
                free(node);
                return;
            }
            //Case3. node有右兒子
            else if(node->leftChild==NULL && node->rightChild!=NULL){
                BinaryTreeNode<K, V>* right_node = node->rightChild;
                //node是老爸的左邊兒子
                if(node->entry->getkey().getvalue() <= parent_node->entry->getkey().getvalue()){
                    parent_node->leftChild=right_node;
                    right_node->parent=parent_node;
                    node->parent=NULL;
                    node->rightChild=NULL;
                }
                //node是老爸的右邊兒子
                else{
                    parent_node->rightChild=right_node;
                    right_node->parent=parent_node;
                    node->parent=NULL;
                    node->rightChild=NULL;
                }
                free(node);
                return;
            }
            // Case4. node有左兒子&右兒子
            else if(node->leftChild!=NULL && node->rightChild!=NULL){
                BinaryTreeNode<K, V>* right_min = findRightMin(node->rightChild);
                Entry<K, V>* entry = new Entry<K, V>(Integer(right_min->entry->getkey().getvalue()), String(right_min->entry->getvalue().getvalue()));
                BinaryTreeNode<K, V>* new_node = new BinaryTreeNode<K, V>(entry);
                //node是老爸的左邊兒子
                if(node->entry->getkey().getvalue() <= parent_node->entry->getkey().getvalue()){
                    //Step1. 處理parent<->new_node
                    parent_node->leftChild=new_node;
                    new_node->parent=parent_node;
                    node->parent=NULL;
                    //Step2. 處理leftChild<->new_node
                    new_node->leftChild=node->leftChild;
                    node->leftChild->parent=new_node;
                    node->leftChild=NULL;
                    //Step3. 處理rightChild<->new_node
                    new_node->rightChild=node->rightChild;
                    node->rightChild->parent=new_node;
                    node->rightChild=NULL;
                }
                //node是老爸的右邊兒子
                else{
                    //Step1. 處理parent<->new_node
                    parent_node->rightChild=new_node;
                    new_node->parent=parent_node;
                    node->parent=NULL;
                    //Step2. 處理leftChild<->new_node
                    new_node->leftChild=node->leftChild;
                    node->leftChild->parent=new_node;
                    node->leftChild=NULL;
                    //Step3. 處理rightChild<->new_node
                    new_node->rightChild=node->rightChild;
                    node->rightChild->parent=new_node;
                    node->rightChild=NULL;
                }
                free(node);
                // 刪節點 情形一:right_min沒有兒子
                if (right_min->rightChild==NULL){
                    if(right_min->entry->getkey().getvalue() < right_min->parent->entry->getkey().getvalue()){
                        right_min->parent->leftChild=NULL;
                        right_min->parent=NULL;
                    }
                    else{
                        right_min->parent->rightChild=NULL;
                        right_min->parent=NULL;
                    }
                }
                // 刪節點 情形二:right_min有右兒子
                else{
                    if(right_min->entry->getkey().getvalue() < right_min->parent->entry->getkey().getvalue()){
                        right_min->parent->leftChild=right_min->rightChild;
                        right_min->rightChild->parent=right_min->parent;
                        right_min->parent=NULL;
                        right_min->rightChild=NULL;
                    }
                    else{
                        right_min->parent->rightChild=right_min->rightChild;
                        right_min->rightChild->parent=right_min->parent;
                        right_min->parent=NULL;
                        right_min->rightChild=NULL;
                    }
                }
                free(right_min); 
            }
        }
        
    }
}

template<typename K, typename V>
BinaryTreeNode<K, V>* BinaryTree<K, V>::findRightMin(BinaryTreeNode<K, V>* node) {
    // 右邊已經沒有其他節點 是最大值
    if(node->leftChild==NULL){
        return node;
    }
    return findRightMin(node->leftChild);
}

/**
 *  Remove all entries from the dictionary.
 */
template<typename K, typename V>
void BinaryTree<K, V>::makeEmpty() {
    deleteTree(root);
}

template<typename K, typename V>
void BinaryTree<K, V>::deleteTree(BinaryTreeNode<K, V>* node){
    if (root == nullptr) {
        return; // 如果節點為空，直接返回
    }
    // 遞歸刪除左子樹
    deleteTree(root->leftChild);
    // 遞歸刪除右子樹
    deleteTree(root->rightChild);
    // 釋放當前節點
    delete root;
    // 將指標設為 NULL
    root = nullptr;
    tsize=0;
}

/**
 *  Convert the tree into a string.
 **/
template<typename K, typename V>
std::string BinaryTree<K, V>::toString() {
    if (root == NULL) {
        return "";
    } else {
        return root->toString();
    }
}
