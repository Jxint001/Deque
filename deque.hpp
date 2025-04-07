#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include <cstddef>
#include <cmath>
#include <iostream>
using std::cout, std::endl;

void dflag() {cout << "dq ok" << endl;}

namespace sjtu {
template<class T> class double_list{
  public:
    struct Node{
      T item;
      Node *prev = nullptr, *next = nullptr;
      Node(const T& x): item(x) {}
      ~Node() {
        prev = nullptr, next = nullptr;
      }
    };
  public:
    Node *head = nullptr, *tail = nullptr;
    int* endn = nullptr;

    double_list(){
      endn = new int(0);
    }
    double_list(const double_list<T> &other) {
      endn = new Node(T());
      Node *cur = other.head;
      while (cur) {
        insert_tail(cur->item);
        cur = cur->next;
      }
    }
    double_list & operator=(const double_list<T> & other) {
      if (this == &other) {return *this; }
      clear();
      Node *cur = other.head;
      while (cur) {
        insert_tail(cur->item);
        cur = cur->next;
      }
      return *this;
    }
    ~double_list(){
      clear();
      delete endn;  endn = nullptr;
    }
  
    void clear() {
      //std::cout << "list_clear" << std::endl;
      Node *cur = head, *nxt;
      while (cur) {
        nxt = cur->next;
        delete cur;
        cur = nxt;
      }
      head = nullptr;
      tail = nullptr;
    }
  
    class iterator{
    public:
      Node* it = nullptr;
      int* end_node = nullptr;
      double_list<T>* container;
  
      iterator(Node* node = nullptr, int* ed = nullptr, double_list<T>* cont = nullptr): it(node), end_node(ed), container(cont) {}
      iterator(const iterator &t): it(t.it), end_node(t.end_node), container(t.container) {
      }
      ~iterator(){it = nullptr; }
  
      iterator operator++(int) {
        Node* cur = it;
        if (it == nullptr) {throw index_out_of_bound(); }
        it = it->next;
        return iterator(cur, this->end_node, this->container);
      }
    
      iterator &operator++() {
        if (it == nullptr) {throw index_out_of_bound(); }
        it = it->next;
        return *this;
      }
  
      iterator operator--(int) {
        Node* cur = it;
        if (it == nullptr || it->prev == nullptr) {
          if (end_node == container->endn) {
            it = container->tail;
          } else {
            throw index_out_of_bound();
          }
        } else {
          it = it->prev;
        }
        return iterator(cur, this->end_node, this->container);
      }
        
      iterator &operator--() {
        if (it == nullptr || it->prev == nullptr) {
          if (end_node == container->endn) {
            it = container->tail;
          } else {
            throw index_out_of_bound();
          }
        } else {
          it = it->prev;
        }
        return *this;
      }
      /**
        * if the iter didn't point to a value
        * throw " invalid"
      */
      T &operator*() const {
        if (it == nullptr) {throw invalid_iterator(); }
        return it->item;
      }
          /**
            * other operation
          */
      T *operator->() const noexcept {
        return &(it->item);
      }
      bool operator==(const iterator &rhs) const {
        //cout << "list====" << endl;
        //cout << it << " " << rhs.it << " "<<end_node <<" "<< rhs.end_node << endl;
        return it == rhs.it && end_node == rhs.end_node;
      }
      bool operator!=(const iterator &rhs) const {
        return it != rhs.it || end_node != rhs.end_node;
      }
    };
  
    class const_iterator {
      public:
        Node* it;
        int* end_node;
        const double_list<T>* container;
        const_iterator(Node* node = nullptr, int* ed = nullptr, const double_list<T>* cont = nullptr) : it(node), end_node(ed), container(cont) {}
        const_iterator(const const_iterator &t) : it(t.it), end_node(t.end_node), container(t.container) {}
        ~const_iterator() { it = nullptr; }
  
      const_iterator operator++(int) {
        const Node* cur = it;
        if (it == nullptr) { throw index_out_of_bound(); }
        it = it->next;
        return const_iterator(cur, this->end_node, this->container);
      }
  
      const_iterator &operator++() {
        if (it == nullptr) { throw index_out_of_bound(); }
        it = it->next;
        return *this;
      }
  
      const_iterator operator--(int) {
        const Node* cur = it;
        if (it == nullptr || it->prev == nullptr) {
          if (end_node == container->endn) {
              it = container->tail;
          } else {
              throw index_out_of_bound();
          }
        } else {
          it = it->prev;
        }
        return const_iterator(cur, this->end_node, this->container);
      }
  
      const_iterator &operator--() {
        if (it == nullptr || it->prev == nullptr) {
          if (end_node == container->endn) {
              it = container->tail;
          } else {
              throw index_out_of_bound();
          }
        } else {
          it = it->prev;
        }
        return *this;
      }
  
      const T &operator*() const {
              if (it == nullptr) { throw invalid_iterator(); }
              return it->item;
          }
  
          const T *operator->() const noexcept {
              return &(it->item);
          }
  
          bool operator==(const const_iterator &rhs) const {
              return it == rhs.it;
          }
  
          bool operator!=(const const_iterator &rhs) const {
              return it != rhs.it;
          }
    };
  
    /**
      * return an iterator to the beginning
      */
    iterator begin() {
      return iterator(head, endn, this);
    }
    /**
      * return an iterator to the ending
      * in fact, it returns the iterator point to nothing,
      * just after the last element.
      */
    iterator end() {
      return iterator(nullptr, endn, this);
    }
  
    const_iterator cbegin() const {
          return const_iterator(head, endn, this);
    }
  
    const_iterator cend() const {
        return const_iterator(nullptr, endn, this);
    }
  
    iterator back() {
      //cout << "list_back()" << endl;
      return iterator(tail, endn, this);
    }
    
    const_iterator cback() const {
      return const_iterator(tail, endn, this);
    }
    /**
      * if the iter didn't point to anything, do nothing,
      * otherwise, delete the element pointed by the iter
      * and return the iterator point at the same "index"
      * e.g.
      * 	if the origin iterator point at the 2nd element
      * 	the returned iterator also point at the
      *  2nd element of the list after the operation
      *  or nothing if the list after the operation
      *  don't contain 2nd elememt.
    */
    iterator erase(iterator pos){
      //std::cout << "list_erase" << std::endl;
      if (pos.it == nullptr) {return iterator(nullptr); }
      Node *nxt = pos.it->next, *pre = pos.it->prev;
      //relink
      if (pre != nullptr) {
        pre->next = nxt;
      }
      if (nxt != nullptr) {
        nxt->prev = pre;
      }
      if (pos.it == head) {
        head = nxt;
      }
      if (pos.it == tail) {
        tail = pre;
      }
      delete pos.it;
      //innerflag();
      return iterator(nxt, endn, this);
    }

    iterator insert(iterator pos, const T &val) {  // 在pos之前插入一个节点
      if (pos.it == nullptr) {
        insert_tail(val);
        return iterator(tail);
      }
      Node* new_node = new Node(val);
      Node* cur = pos.it;
      Node* pre = cur->prev;
      new_node->next = cur;
      new_node->prev = pre;
      cur->prev = new_node;
      if (pre != nullptr) {
        pre->next = new_node;
      } else {
        head = new_node;
      }
      return iterator(new_node, endn, this);
    }
  
    void move_to_tail(iterator pos) {
      if (pos.it == nullptr || pos.it == tail) {return; }
      Node *nxt = pos.it->next, *pre = pos.it->prev;
      // relink
      if (pre != nullptr) {
        pre->next = nxt;
      }
      if (nxt != nullptr) {
        nxt->prev = pre;
      }
      if (pos.it == head) {
        head = nxt;
      }
      if (pos.it == tail) {
        tail = pre;
      }
      // to tail
      if (tail != nullptr) {tail->next = pos.it; }
      pos.it->next = nullptr;
      pos.it->prev = tail;
      tail = pos.it;
    }
  
    /**
      * the following are operations of double list
    */
    void insert_head(const T &val){
      Node* new_node = new Node(val);
      if (head == nullptr) {
        head = new_node;
        tail = head;
        return;
      }
      new_node->next = head;
      if (head != nullptr) {head->prev = new_node; }
      head = new_node;
    }
    void insert_tail(const T &val){
      //std::cout << "insert_tail" << std::endl;
      Node* new_node = new Node(val);
      //dflag();
      if (head == nullptr) {
        head = new_node;
        tail = head;
        return;
      }
      new_node->prev = tail;
      if (tail != nullptr) {tail->next = new_node; }
      tail = new_node;
    }
    void delete_head(){
      if (head == nullptr) {throw container_is_empty(); }
      Node* new_head = head->next;
      delete head;
      head = new_head;
      if (head != nullptr) {
        head->prev = nullptr;
      } else {
        tail = nullptr;
      }
    }
    void delete_tail(){
      if (head == nullptr) {throw container_is_empty(); }
      Node* new_tail = tail->prev;
      delete tail;
      tail = new_tail;
      if (tail != nullptr) {
        tail->next = nullptr;
      } else {
        head = nullptr;
      }
    }
    /**
      * if didn't contain anything, return true, 
      * otherwise false.
      */
    bool empty() const{
      return head == nullptr;
    }
};


template<class T> class Block{
public:
  size_t maxSize = 0, curL = 0; // 本块最多存maxSize-1个元素,已经存了curL个
  size_t front = 0, rear = 0;  // 循环数组中的队头和队尾
  T** elem = nullptr;  // 存放本块的循环队列

public:
  Block() {}
  Block(const Block& other): maxSize(other.maxSize), curL(other.curL), front(other.front), rear(other.rear) {
    //cout << "block copy_constructor" << endl;
    if (other.elem != nullptr) {
      elem = new T*[maxSize]();
      for (size_t i = 0; i < maxSize; ++i) {
        if (other.elem[i] != nullptr) 
          elem[i] = new T(*other.elem[i]);
      }
    }
    //cout << "constructed" << endl;
  }
  Block(size_t ms, size_t curl, size_t fro, size_t re, T** e):maxSize(ms), curL(curl), front(fro), rear(re) {
    if (e != nullptr) {
      elem = new T*[maxSize]();
      for (size_t i = 0; i < maxSize; ++i) {
        if (e[i] != nullptr) 
          elem[i] = new T(*e[i]);
      }
    }
  }

  ~Block() {
    for (size_t i = 0; i < maxSize; ++i) {
      delete elem[i];  elem[i] = nullptr;
    }
    delete[] elem;
    elem = nullptr;
    front = rear = maxSize = curL = 0;
  }

  void expand() {
    size_t new_maxSize = maxSize * 2;
    T** new_elem = new T*[new_maxSize]();
    for (size_t i = 0; i < curL; ++i) {
      if (elem[(front + i) % maxSize] != nullptr) {
        new_elem[i] = elem[(front + i) % maxSize];
        //elem[(front + i) % maxSize] = nullptr;
      }
    }
    // for (size_t i = 0; i < maxSize; ++i) {
    //   delete elem[i];  elem[i] = nullptr;
    // }
    delete[] elem;
    elem = new_elem;
    maxSize = new_maxSize;
    front = 0;
    rear = curL;
  }
};


template <class T> class deque {
public:
  class iterator;
  class const_iterator;

public:
  size_t n = 0;  // 当前总元素数量
  double_list<Block<T> > dq;  // 链表套数组
  const static size_t default_size = 100;

  iterator insert_in_arr(iterator pos, const T &value) {
    auto block_it = pos.liter;
    size_t block_index = pos.index;
    size_t maxSize = block_it->maxSize;
    size_t front = block_it->front;
    size_t rear = block_it->rear;
    size_t curL = block_it->curL;

    size_t vdex = (block_index - front + maxSize) % maxSize;

    if (vdex <= curL / 2) { // 插入点在前半部分，向前移动元素
      size_t startpos = (front - 1 + maxSize ) % maxSize;
      size_t endpos = (front + vdex) % maxSize;
      size_t newpos;
      for (size_t i = startpos; i != endpos; i = newpos) {
        newpos = (i + 1) % maxSize;
        block_it->elem[i] = block_it->elem[newpos];
      }
      block_it->elem[(front + vdex - 1 + maxSize) % maxSize] = new T(value);
      block_it->front = (front - 1 + maxSize) % maxSize;
      pos.index = (pos.index - 1 + maxSize) % maxSize;

    } else { // 插入点在后半部分，向后移动元素
      size_t prev;
      for (size_t i = block_it->rear; i != block_index; i = prev) {
        prev = (i - 1 + block_it->maxSize) % block_it->maxSize;
        block_it->elem[i] = block_it->elem[prev];
      }
      block_it->elem[(front + vdex) % maxSize] = new T(value);
      block_it->rear = (rear + 1) % maxSize; // 调整rear
    }
    ++block_it->curL;
    ++n;
    return pos;
  }

  iterator split(typename double_list<Block<T>>::iterator & block_it, size_t idx) {  // 返回分完块之后的包括原来位置的块的迭代器
    Block<T> new_block;
    new_block.maxSize = block_it->maxSize;
    new_block.elem = new T*[new_block.maxSize]();
    size_t half = block_it->curL / 2;
    for (size_t i = 0; i < half; ++i) {
      size_t src = (block_it->front + i) % block_it->maxSize;
      new_block.elem[i] = block_it->elem[src];
      block_it->elem[src] = nullptr;
    }
    new_block.front = 0;
    new_block.rear = half;
    new_block.curL = half;

    block_it->front = (block_it->front + half) % block_it->maxSize;
    block_it->curL -= half;
    block_it->rear = (block_it->front + block_it->curL) % block_it->maxSize;

    auto new_block_it = dq.insert(block_it, new_block);
    if (idx < half) {
      return iterator(new_block_it, idx, this);
    } else {
      return iterator(block_it, ((idx - half) + block_it->front) % block_it->maxSize, this);
    }
  }
  iterator erase_in_arr(iterator pos) {
    auto block_it = pos.liter;
    size_t block_index = pos.index;
    size_t maxSize = block_it->maxSize;
    size_t front = block_it->front;
    size_t rear = block_it->rear;
    size_t curL = block_it->curL;

    size_t vdex = (block_index - front + maxSize) % maxSize;

    delete block_it->elem[block_index];
    //block_it->elem[block_index] = nullptr;

    if (vdex < curL / 2) { // 删除点在前半部分，向后移动元素
      size_t tmp;
      for (size_t i = block_index; i != front; i = tmp) {
        tmp = (i - 1 + maxSize) % maxSize;
        block_it->elem[i] = block_it->elem[tmp];
      }
      block_it->elem[front] = nullptr;
      block_it->front = (front + 1) % maxSize;
      pos.index = (pos.index + 1 ) % maxSize;

    } else { // 删除点在后半部分，向前移动元素
      size_t tmp;
      for (size_t i = block_index; i != rear; i = tmp) {
        tmp = (i + 1) % maxSize;
        block_it->elem[i] = block_it->elem[tmp];
      }
      pos.liter->elem[rear] = nullptr;
      pos.liter->rear = (pos.liter->rear - 1 + pos.liter->maxSize) % pos.liter->maxSize;
      pos.liter->elem[pos.liter->rear] = nullptr;
    }
    --block_it->curL;
    --n;

    if (pos.index == pos.liter->rear) {
      ++(pos.liter);
      if (pos.liter == pos.container->dq.end()) {
          return pos.container->end();
      }
      pos.index = pos.liter->front;
    }

    return pos;
}

  size_t merge(typename double_list<Block<T>>::iterator & block_it, typename double_list<Block<T>>::iterator & next_it, size_t idx) {  // 把第一个和第二个块合并，删除第二个块
    if (block_it->maxSize > block_it->curL + next_it->curL) {
      for (size_t i = 0; i < next_it->curL; ++i) {
        size_t tmp = (next_it->front + i) % next_it->maxSize;
        block_it->elem[(block_it->rear + i) % block_it->maxSize] = next_it->elem[tmp];
        next_it->elem[tmp] = nullptr;
      }
      block_it->rear = (block_it->rear + next_it->curL) % block_it->maxSize;
      block_it->curL += next_it->curL;
      dq.erase(next_it);

    } else {
      size_t new_size = (block_it->curL + next_it->curL) * 1.5;
      T** tmp = new T*[new_size]();
      for (size_t i = 0; i < block_it->curL; ++i) {
        size_t tmppos = (block_it->front + block_it->maxSize + i) % block_it->maxSize;
        tmp[i] = block_it->elem[tmppos];
        block_it->elem[tmppos] = nullptr;
      }
      for (size_t i = block_it->curL; i < next_it->curL + block_it->curL; ++i) {
        size_t tmppos = (next_it->front + next_it->maxSize + i - block_it->curL) % next_it->maxSize;
        tmp[i] = next_it->elem[tmppos];
        next_it->elem[tmppos] = nullptr;
      }
      delete[] block_it->elem;
      block_it->elem = tmp;
      block_it->maxSize = new_size;
      block_it->front = 0;
      block_it->curL += next_it->curL;
      block_it->rear = block_it->curL;
      dq.erase(next_it);
    }

    return (block_it->front + idx) % block_it->maxSize;
  }

public:
  class iterator {
  public:
    using ListIt = typename double_list<Block<T> >::iterator; // list iterator
    ListIt liter;
    size_t index = 0; // 内层数组索引
    deque<T>* container = nullptr;

  public:
    iterator() {}
    iterator(const iterator& other):liter(other.liter), index(other.index), container(other.container) {}
    iterator(const ListIt &it, size_t idx, deque<T>* cont): liter(it), index(idx), container(cont) {}
    iterator(const const_iterator& other):liter(other.liter), index(other.index), container(other.container) {}
    ~iterator() {}
    /**
     * return a new iterator which points to the n-next element.
     * if there are not enough elements, the behaviour is undefined.
     * same for operator-.
     啥叫undefined？不用手动throw？那可太好了！
     */
    iterator operator+(const int &n) const {
      // << "operator +" << endl;
      if (n < 0)  return *this - (-n);
      iterator now = *this;
      int k = n;
      while (k > 0) {
          int vdex = (now.index + now.liter->maxSize - now.liter->front) % now.liter->maxSize;
          int remain = now.liter->curL - vdex;
          if (k < remain) {
            now.index = (now.index + k) % now.liter->maxSize;
            k = 0;
          } else {
            k -= remain;
            ++(now.liter);
            if (now.liter == now.container->dq.end()) {
               if (k == 0) {
                return container->end(); 
              } else {throw invalid_iterator(); }
            }
            now.index = now.liter->front;
          }
      }
      return now;
    }
    iterator operator-(const int &n) const {
      //cout << "iterator - num" << endl;
      if (n < 0)  return *this + (-n);
      iterator now = *this;
      int k = n;
      while (k > 0) {
        if (now.liter == container->dq.end()) {
          //cout << "liter is end()" << endl;
          --now.liter;
          now.index = now.liter->rear;
        }
        int vdex = (now.index + now.liter->maxSize - now.liter->front) % now.liter->maxSize;
        if (k <= vdex) {
          now.index = (now.index + now.liter->maxSize - k) % now.liter->maxSize;
          k = 0;
        } else {
          k -= vdex;
          --(now.liter);
          now.index = now.liter->rear;
        }
      }
      return now;
    }

    /**
     * return the distance between two iterators.
     * if they point to different vectors, throw
     * invaild_iterator.
     */
    int operator-(const iterator &rhs) const {
      if (container != rhs.container)  throw invalid_iterator();
      int idx = 0, rdx = 0;
      bool end1 = false, end2 = false;
      if (*this == container->end()) {idx = container->size();  end1 = true; }
      if (rhs == rhs.container->end()) {rdx = rhs.container->size();  end2 = true; }
      for (auto it = container->dq.begin(); !end1 && it != liter; ++it) {
        idx += it->curL;
      }
      for (auto it = rhs.container->dq.begin(); !end2 && it != rhs.liter; ++it) {
        rdx += it->curL;
      }
      int of, ofr;
      if (!end1) {
        of = (index + liter->maxSize - liter->front) % liter->maxSize;
        idx += of;
      }
      if (!end2) {
        ofr = (rhs.index + rhs.liter->maxSize - rhs.liter->front) % rhs.liter->maxSize;
        rdx += ofr;
      }
      return idx - rdx;
    }
    
    iterator &operator+=(const int &n) {
      *this = *this + n;
      return *this;
    }

    iterator &operator-=(const int &n) {
      *this = *this - n;
      return *this;
    }

    /**
     * iter++
     */
    iterator operator++(int) {
      iterator it = *this;
      if (liter == container->dq.end()) {throw invalid_iterator(); }
      int vdex = (index + liter->maxSize - liter->front) % liter->maxSize;
      int remain = liter->curL - vdex - 1;
      
      if (remain > 0) {
        index = (index + 1) % liter->maxSize;
      } else {
        ++liter;
        if (liter != container->dq.end())  index = liter->front;
        else  index = 0;
      }
      return it;
    }
    /**
     * ++iter
     */
    iterator &operator++() {
      if (liter == container->dq.end()) {throw invalid_iterator(); }
      int vdex = (index + liter->maxSize - liter->front) % liter->maxSize;
      int remain = liter->curL - vdex - 1;
      if (remain > 0) {
        index = (index + 1) % liter->maxSize;
      } else {
        ++liter;
        if (liter != container->dq.end())  index = liter->front;
        else  index = 0;
      }
      return *this;
    }
    /**
     * iter--
     */
    iterator operator--(int) {
      iterator it = *this;
      --*this;
      return it;
    }
    /**
     * --iter
     */
    iterator &operator--() {
      //cout << "--it" << endl;
      if (*this == container->end()) {
        //cout << "--end()" << endl;
        liter = container->dq.back();
        index = (liter != container->dq.end()) ? (liter->rear - 1 + liter->maxSize) % liter->maxSize : 0;
        //cout << index << "p:"<< endl;
        //cout << liter->rear << endl;
        return *this;
      }
      int vdex = (index + liter->maxSize - liter->front) % liter->maxSize;
      if (vdex > 0) {
        index = (index - 1 + liter->maxSize) % liter->maxSize;
      } else {
        if (liter == container->dq.begin()) {throw invalid_iterator(); }
        --liter;
        index = (liter->rear - 1 + liter->maxSize) % liter->maxSize;
      }
      return *this;
    }

    /**
     * *it
     */
    T &operator*() const {
      if (container == nullptr || liter == container->dq.end() || liter->elem[index] == nullptr) {
        //cout << "in*it" << endl; 
        throw invalid_iterator();
      }
      return *(liter->elem[index]);
    }
    /**
     * it->field
     */
    T *operator->() const noexcept {
      // if (container == nullptr || liter == container->dq.end())  {throw invalid_iterator();}
      //if (liter->elem[index] == nullptr) {cout << "* nullptr" << endl;}
      return liter->elem[index];
    }

    /**
     * check whether two iterators are the same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {
      return liter == rhs.liter && index == rhs.index;
    }
    bool operator==(const const_iterator &rhs) const {
      return liter == rhs.liter && index == rhs.index;
    }
    /**
     * some other operator for iterators.
     */
    bool operator!=(const iterator &rhs) const {
      return liter != rhs.liter || index != rhs.index;
    }
    bool operator!=(const const_iterator &rhs) const {
      return liter != rhs.liter || index != rhs.index;
    }
  };

  class const_iterator {
    /**
     * it should has similar member method as iterator.
     * you can copy them, but with care!
     * and it should be able to be constructed from an iterator.
     */
  public:
    using CListIt = typename double_list<Block<T> >::const_iterator; // list const_iterator
    CListIt liter;
    size_t index = 0; // 内层数组索引
    const deque<T>* container;

  public:
    const_iterator() {}
    const_iterator(const CListIt &it, size_t idx, const deque<T>* cont): liter(it), index(idx), container(cont) {}
    const_iterator(const iterator& other):liter(other.liter), index(other.index), container(other.container) {}
    const_iterator(const const_iterator& other):liter(other.liter), index(other.index), container(other.container) {}
    ~const_iterator() {}

    const_iterator operator+(const int &n) const {
      if (n < 0)  return *this - (-n);
      const_iterator now = *this;
      int k = n;
      while (k > 0) {
          int vdex = (now.index + now.liter->maxSize - now.liter->front) % now.liter->maxSize;
          int remain = now.liter->curL - vdex;
          if (k < remain) {
              now.index = (now.index + k) % now.liter->maxSize;
              k = 0;
          } else {
            k -= remain;
            ++(now.liter);
            if (now.liter == now.container->dq.cend()) {
              if (k == 0) {return container->cend(); }
              else {throw invalid_iterator(); }
            }
              now.index = now.liter->front;
          }
      }
      return now;
    }

    const_iterator operator-(const int &n) const {
      if (n < 0)  return *this + (-n);
      const_iterator now = *this;
      int k = n;
      while (k > 0) {
        if (now.liter == container->dq.cend()) {
          //cout << "liter is end()" << endl;
          --now.liter;
          now.index = now.liter->rear;
        }
        int vdex = (now.index + now.liter->maxSize - now.liter->front) % now.liter->maxSize;
        if (k <= vdex) {
            now.index = (now.index + now.liter->maxSize - k) % now.liter->maxSize;
            k = 0;
        } else {
            k -= vdex;
            --(now.liter);
            now.index = now.liter->rear;
        }
      }
      return now;
    }

    int operator-(const const_iterator &rhs) const {
      if (container != rhs.container)  throw invalid_iterator();
      int idx = 0, rdx = 0;
      bool end1 = false, end2 = false;
      if (*this == container->cend()) {idx = container->size();  end1 = true; }
      if (rhs == rhs.container->cend()) {rdx = rhs.container->size();  end2 = true; }
      for (auto it = container->dq.cbegin(); !end1 && it != liter; ++it) {
        idx += it->curL;
      }
      for (auto it = container->dq.cbegin(); !end2 && it != rhs.liter; ++it) {
        rdx += it->curL;
      }
      int of, ofr;
      if (!end1) {
        of = (index + liter->maxSize - liter->front) % liter->maxSize;
        idx += of;
      }
      if (!end2) {
        ofr = (rhs.index + rhs.liter->maxSize - rhs.liter->front) % rhs.liter->maxSize;
        rdx += ofr;
      }
      return idx - rdx;
    }

    const_iterator &operator+=(const int &n) {
      *this = *this + n;
      return *this;
    }

    const_iterator &operator-=(const int &n) {
      *this = *this - n;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator it = *this;
      if (liter == container->dq.cend())  throw invalid_iterator();
      int vdex = (index + liter->maxSize - liter->front) % liter->maxSize;
      int remain = liter->curL - vdex - 1;
      if (remain > 0) {
        index = (index + 1) % liter->maxSize;
      } else {
        ++liter;
        if (liter != container->dq.cend())  index = liter->front;
        else  index = 0;
      }
      return it;
    }

    const_iterator &operator++() {
      if (liter == container->dq.cend())  throw invalid_iterator();
      int vdex = (index + liter->maxSize - liter->front) % liter->maxSize;
      int remain = liter->curL - vdex - 1;
      if (remain > 0) {
        index = (index + 1) % liter->maxSize;
      } else {
        ++liter;
        if (liter != container->dq.cend())  index = liter->front;
        else  index = 0;
      }
      return *this;
    }

    const_iterator operator--(int) {
      const_iterator it = *this;
      --*this;
      return it;
    }

    const_iterator &operator--() {
      if (*this == container->cend()) {
        liter = container->dq.cback();
        index = (liter != container->dq.cend()) ? (liter->rear - 1 + liter->maxSize) % liter->maxSize : 0;
        return *this;
      }
      int vdex = (index + liter->maxSize - liter->front) % liter->maxSize;
      if (vdex > 0) {
        index = (index - 1) % liter->maxSize;
      } else {
        if (liter == container->dq.cbegin()) {throw invalid_iterator(); }
        --liter;
        index = (liter->rear - 1) % liter->maxSize;
      }
      return *this;
    }

    const T &operator*() const {
      if (container == nullptr || liter == container->dq.cend() || liter->elem[index] == nullptr)  throw invalid_iterator();
      return *(liter->elem[index]);
    }

    T *operator->() const noexcept {
      //if (container == nullptr || liter == container->dq.cend())  throw invalid_iterator();
      return liter->elem[index];
    }

    bool operator==(const iterator &rhs) const {
      return liter == rhs.liter && index == rhs.index;
    }
    bool operator==(const const_iterator &rhs) const {
      return liter == rhs.liter && index == rhs.index;
    }
    bool operator!=(const iterator &rhs) const {
      return liter != rhs.liter || index != rhs.index;
    }
    bool operator!=(const const_iterator &rhs) const {
      return liter != rhs.liter || index != rhs.index;
    }
  };

  deque() {}
  deque(const deque &other): dq(), n(other.n) {
    for (auto it = other.dq.cbegin(); it != other.dq.cend(); ++it) {
      dq.insert_tail(*it);
    }
  }

  ~deque() {clear(); }

  deque &operator=(const deque &other) {
    if (this == &other)  return *this;
    this->clear();
    n = other.n;
    for (auto it = other.dq.cbegin(); it != other.dq.cend(); ++it) {
      dq.insert_tail(*it);
    }
    return *this;
  }

  /**
   * access a specified element with bound checking.
   * throw index_out_of_bound if out of bound.
   */
  T &at(const size_t &pos) {
    if (pos >= n)  throw index_out_of_bound();
    int sum = 0;
    auto it = dq.begin();
    while (it != dq.end() && sum + it->curL <= pos) {
      sum += it->curL;
      ++it;
    }
    iterator find(it, it->front, this);
    int offset = pos - sum;
    find = find + offset;
    return *find;
  }
  const T &at(const size_t &pos) const {
    if (pos >= n)  throw index_out_of_bound();
    int sum = 0;
    auto it = dq.cbegin();
    while (it != dq.cend() && sum + it->curL <= pos) {
      sum += it->curL;
      ++it;
    }
    const_iterator find(it, it->front, this);
    int offset = pos - sum;
    find = find + offset;
    return *find;
  }
  T &operator[](const size_t &pos) {
    //cout << "op []" << endl;
    if (pos >= n)  throw index_out_of_bound();
    int sum = 0;
    auto it = dq.begin();
    while (it != dq.end() && sum + it->curL <= pos) {
      sum += it->curL;
      ++it;
    }
    iterator find(it, it->front, this);
    int offset = pos - sum;
    find = find + offset;
    return *find;
  }
  const T &operator[](const size_t &pos) const {
    if (pos >= n)  throw index_out_of_bound();
    int sum = 0;
    auto it = dq.cbegin();
    while (it != dq.cend() && sum + it->curL <= pos) {
      sum += it->curL;
      ++it;
    }
    const_iterator find(it, it->front, this);
    int offset = pos - sum;
    find = find + offset;
    return *find;
  }

  /**
   * access the first element.
   * throw container_is_empty when the container is empty.
   */
  const T &front() const {
    //cout << "front" << endl;
    if (empty()) throw container_is_empty();
    return *cbegin();
  }
  /**
   * access the last element.
   * throw container_is_empty when the container is empty.
   */
  const T &back() const {
    //cout << "back" << endl;
    if (empty()) throw container_is_empty();

    auto block = dq.cback();
    if (block->curL == 0) { // 保护检查
        throw container_is_empty();
    }
    size_t index = (block->rear - 1 + block->maxSize) % block->maxSize;
    //if (block->elem[index] == nullptr) {cout << "back() is a nullptr!" << endl; }
    //dflag();
    return *(block->elem[index]);
  }

  /**
   * return an iterator to the beginning.
   */
  iterator begin() {
    return iterator(dq.begin(), dq.head? dq.head->item.front: 0, this);
  }
  const_iterator cbegin() const {
    return const_iterator(dq.cbegin(), dq.head? dq.head->item.front: 0, this);
  }

  /**
   * return an iterator to the end.
   */
  iterator end() {
    return iterator(dq.end(), 0, this);
  }
  const_iterator cend() const {
    return const_iterator(dq.cend(), 0, this);
  }

  /**
   * check whether the container is empty.
   */
  bool empty() const {//cout << "empty" << endl;
    return n == 0; }

  /**
   * return the number of elements.
   */
  size_t size() const {return n; }

  /**
   * clear all contents.
   */
  void clear() {dq.clear();  n = 0; }

  /**
   * insert value before pos.
   * return an iterator pointing to the inserted value.
   * throw if the iterator is invalid or it points to a wrong place.
   */
  iterator insert(iterator pos, const T &value) {
    //cout << "insert" << endl;
    if (pos.container != this) throw invalid_iterator();

    if (dq.empty()) {
      Block<T> new_block;
      new_block.maxSize = default_size;
      new_block.elem = new T*[new_block.maxSize]();
      new_block.elem[0] = new T(value);
      new_block.front = 0;
      new_block.rear = 1;
      new_block.curL = 1;
      dq.insert_tail(new_block);
      //dflag();
      ++n;
      return iterator(dq.begin(), 0, this);
    }

    if (pos.liter == dq.end()) {
      //cout << "insert end" << endl;
      if (pos.index != 0)  throw invalid_iterator();
      return insert(iterator(dq.back(), dq.tail? dq.tail->item.rear : 0, this), value);
    }

    if (pos == pos.container->begin()) {
      pos.container->push_front(value);
      return pos.container->begin();
    }

    if ((pos.index + pos.liter->maxSize - pos.liter->front) % pos.liter->maxSize > pos.liter->curL) {
        throw invalid_iterator();
    }
    //cout << "sur" << endl;

    

    if (pos.liter->curL < pos.liter->maxSize - 1) {
      return insert_in_arr(pos, value);
    } else {
      if (pos.liter->maxSize > 2 * sqrt(n) && pos.liter->curL > default_size) {
        int vdex = (pos.index - pos.liter->front + pos.liter->maxSize ) % pos.liter->maxSize;
        return (insert_in_arr(split(pos.liter, vdex), value));

      } else {
        pos.index = (pos.index + pos.liter->maxSize - pos.liter->front) % pos.liter->maxSize;
        pos.liter->expand();  // expand之后font是0，rear是curL
        return insert_in_arr(pos, value);
      }
    }
  }

  /**
   * remove the element at pos.
   * return an iterator pointing to the following element. if pos points to
   * the last element, return end(). throw if the container is empty,
   * the iterator is invalid, or it points to a wrong place.
   */
  iterator erase(iterator pos) {
    //cout << "erase" << endl;
    if (pos.container != this || empty()) throw invalid_iterator();
    if (pos.liter == dq.end() || (pos.index + pos.liter->maxSize - pos.liter->front) % pos.liter->maxSize >= pos.liter->curL) {
        throw invalid_iterator();
    }

    if (pos.liter->curL == 1) {
      if (size() == 1) {dq.erase(pos.liter);  n = 0;  return end(); }
      //cout << "special case" << endl;
      auto next_block_it = pos.liter;
      ++next_block_it;
      dq.erase(pos.liter);
      --n;
      return iterator(next_block_it, next_block_it == dq.end()? 0 : next_block_it->front, this);
    }

    pos = erase_in_arr(pos);

    if (pos.liter != pos.container->dq.end() && pos.liter->curL < sqrt(n) / 2) {
      if (pos.liter.it != dq.tail) {
        auto next_block_it = pos.liter;
        ++next_block_it;
        int vdex = (pos.index - pos.liter->front + pos.liter->maxSize) % pos.liter->maxSize;
        pos.index = merge(pos.liter, next_block_it, vdex);
      }
    }

    return pos;
  }

  /**
   * add an element to the end.
   */
  void push_back(const T &value) {
    insert(iterator(dq.back(), dq.tail? dq.tail->item.rear : 0, this), value);
    return;
  }

  /**
   * remove the last element.
   * throw when the container is empty.
   */
  void pop_back() {
    //cout << "pop_back" << endl;
    if (empty()) throw container_is_empty();
    erase(iterator(dq.back(), (dq.tail->item.rear + dq.tail->item.maxSize - 1) % dq.tail->item.maxSize, this));
  }

  /**
   * insert an element to the beginning.
   */
  void push_front(const T &value) {
    if (empty()) {
      insert(begin(), value);
      return;
    }
    if (dq.begin()->curL + 1 < dq.begin()->maxSize) {
      int idx = (dq.begin()->front - 1 + dq.begin()->maxSize) % dq.begin()->maxSize;
      dq.begin()->elem[idx] = new T(value);
      dq.begin()->front = idx;
      ++(dq.begin()->curL);
      ++n;
    } else {
      if (dq.begin()->maxSize > 2 * sqrt(n) && dq.begin()->curL > default_size) {
        auto block_it = dq.begin();
        split(block_it, 0);
        push_front(value);
      } else {
        dq.begin()->expand();
        push_front(value);
      }
    }
  }

  /**
   * remove the first element.
   * throw when the container is empty.
   */
  void pop_front() {
    if (empty()) throw container_is_empty(); 
    erase(begin());
  }
  
  void print() {
    for (auto it = begin(); it != end(); ++it) {
      if (it.liter->elem && it.liter->elem[it.index] != nullptr) {
        T tmp = *it;
        cout << tmp << endl; 
      }
    }
  }
};

} // namespace sjtu

#endif
