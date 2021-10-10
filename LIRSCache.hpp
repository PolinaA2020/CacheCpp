#pragma once

#include <iostream>
#include <cstdio>
#include <list>
#include <iterator>
#include <unordered_map>
#include <cmath>
#include <cassert>

bool LIR = 1; // maybe #define better
bool HIR = 0;

namespace caches {
template <typename T, typename KeyT> struct cache_ {
    struct node_t {
        T data_;
        bool LIR_ = 0;
        node_t(T new_data) {
            data_ = new_data;
        }
    };
    using ListIt = typename std::list<node_t>::iterator;

    class list_t {
        public:
        std::list<node_t> lst_;
        size_t sz_;
        
        bool isLIR(ListIt node) const {
            return (*node).LIR_;
        }
        void setToLIR(ListIt node) {
            (*node).LIR_ = 1;
        }
        void setToHIR(ListIt node) {
            (*node).LIR_ = 0;
        }
        void changestatus(ListIt node) {
            (*node).LIR_ = !((*node).LIR_);
        } 
        ListIt front() {
            return lst_.begin();
        }
        ListIt back() {
            return std::prev(lst_.end());
        }
        void push_front_LIR(node_t key) {
            lst_.push_front(key);
            setToLIR(this->front());
        }
        void push_front_HIR(node_t key) {
            lst_.push_front(key);
            setToHIR(this->front());
        }
        void push_back_LIR(node_t key) {
            lst_.push_back(key);
            setToLIR(this->back());
        }
        void push_back_HIR(node_t key) {
            lst_.push_back(key);
            setToHIR(this->back());
        }
    };

    size_t sz_;
    
    list_t Allcache_;
    list_t HIRcache_;
    bool full(list_t &myList) const {
        return (myList.lst_.size() >= myList.sz_);
    }
    using HashConstIt = typename std::unordered_map<KeyT, ListIt>::const_iterator;

    std::unordered_map<KeyT, ListIt> Allhash_;
    std::unordered_map<KeyT, ListIt> HIRhash_;
    cache_(size_t size) { // constructor
        sz_ = size;
        Allcache_.sz_ = ceil(0.9 * size);
        HIRcache_.sz_ = floor(0.1 * size);

    }
/*=================================================================================*/
    template <typename F>
    int lookUpAndUpdate(KeyT key, F slow_get_page)
    {
        auto hit = Allhash_.find(key); // check if key is in Stack
        auto hit_hir = HIRhash_.find(key);
        
        // hash_map_check_all(key); // for debug

        if(hit != Allhash_.end()) { // in Stack
            if(hit_hir != HIRhash_.end()) {// as HIR
                Allcache_.changestatus(hit->second);
                erase_HIR(hit_hir);
                if(full(HIRcache_)) {
                    pop_back_HIR();
                }
                add_to_front_HIR(*Allcache_.back());
                pop_back_All();
            }
            else if(!Allcache_.isLIR(hit->second)) {// non-resident in Stack
                Allcache_.changestatus(hit->second);
                pop_back_All();
            }
            bringForwardAll(hit);
            StackPrune();
            return 1;
        }
        else {// is NOT in Stack
            if(hit_hir != HIRhash_.end())  { // in HIRs
                add_to_front_All(*(hit_hir->second), LIR);
                erase_HIR(hit_hir);
            }
            else // new element
                add_new_element<F>(key, slow_get_page); 
        }
        return 0;    
    }
/*=================================================================================*/
    template <typename F>
    void add_new_element(KeyT key, F slow_get_page) {
        if(full(Allcache_)) {
            if(full(HIRcache_)) {
                pop_back_HIR();            
            }
            if(full(HIRcache_)) { // if HIR.sz == 0
                pop_back_All();
                add_to_front_All(node_t(slow_get_page(key)), LIR);
            }
            else {
                add_to_front_HIR(node_t(slow_get_page(key)));
                add_to_front_All(node_t(slow_get_page(key)), HIR);
            }
        }
        else 
            add_to_front_All(node_t(slow_get_page(key)), LIR);
    }
/*=================================================================================*/
    void bringForwardAll(HashConstIt hit) {
        auto data = *(hit->second);
        erase_All(hit);
        add_to_front_All(data, LIR);
    }
    void bringForwardHIR(HashConstIt hit) {
        auto data = *(hit->second);
        erase_HIR(hit);
        add_to_front_HIR(data);
    }
/*=================================================================================*/
    void StackPrune() {
        if(Allcache_.lst_.empty())
            return;
        auto old_back = Allcache_.back();

        while(!Allcache_.isLIR(old_back)) {
            if(full(HIRcache_)) //
                    pop_back_HIR(); //
            if(!full(HIRcache_))
                add_to_front_HIR(*old_back); //
            
            pop_back_All();
            old_back = Allcache_.back();
        }
    }
/*=================================================================================*/
    void printCache() const {
        std::cout << "------------\n" << "All elements\n";
        for(ListIt it = Allcache_.begin(); it != Allcache_.end(); it++){
            if((*it).LIR_)
                std::cout << "LIR ";
            else
                std::cout << "HIR ";
            std::cout << (*it).data_ << "\n";
        }
        std::cout << "------------\n" << "HIR elements\n";
        for(ListIt it = HIRcache_.begin(); it != HIRcache_.end(); it++){
            std::cout << (*it).data_ << "\n";
        }
    }
/*=================================================================================*/
    void add_to_front_All(node_t key, bool status) {
        if(status == LIR)
            Allcache_.push_front_LIR(key);
        else
            Allcache_.push_front_HIR(key);
        Allhash_[(*Allcache_.front()).data_] = Allcache_.front();
    }
    void add_to_back_All(node_t key, bool status) {
        if(status == LIR)
            Allcache_.push_back_LIR(key);
        else
            Allcache_.push_back_HIR(key);
        Allhash_[(*Allcache_.back()).data_] = Allcache_.back();
    }
    void add_to_front_HIR(node_t key) {
        HIRcache_.push_front_HIR(key);
        HIRhash_[(*HIRcache_.front()).data_] = HIRcache_.front();
    }
    void add_to_back_HIR(node_t key) {
        HIRcache_.push_back_HIR(key);
        HIRhash_[(*HIRcache_.back()).data_] = HIRcache_.back();
    }
/*=================================================================================*/
    void erase_All(HashConstIt hit) {
        Allhash_.erase(hit);
        Allcache_.lst_.erase(hit->second);
    }
    void erase_All(ListIt it) {
        auto hit = Allhash_.find((*it).data_);
        if(hit == Allhash_.end())
            return;
        Allhash_.erase(hit);
        Allcache_.lst_.erase(hit->second);
    }
    void erase_HIR(HashConstIt hit) {
        HIRhash_.erase(hit);
        HIRcache_.lst_.erase(hit->second);
    }
/*=================================================================================*/
    void pop_back_All() {
        if(Allcache_.lst_.empty())
            return;
        Allhash_.erase((*Allcache_.back()).data_);
        Allcache_.lst_.pop_back();
    }
    void pop_back_HIR() { // check if trouble with back & front 
        if(HIRcache_.lst_.empty())
            return;
        HIRhash_.erase((*HIRcache_.back()).data_);
        HIRcache_.lst_.pop_back();
    }
    void pop_front_HIR() {
        if(HIRcache_.lst_.empty())
            return;
        HIRhash_.erase((*HIRcache_.front()).data_);
        HIRcache_.lst_.pop_front();
    }
/*=================================================================================*/
    void hash_map_check_all(KeyT key) const {
        for(auto it = Allhash_.begin(); it != Allhash_.end(); ++it) {
            if(it->first != (*it->second).data_) {
                std::cout << "ERORR here, key is " << key << std::endl;
                std::cout << it->first << " " << (*it->second).data_ << std::endl;
            }
        }
    }
};
}