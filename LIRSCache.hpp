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
        bool LIR_;
        node_t(T new_data) {
            data_ = new_data;
        }
    };
    using ListIt = typename std::list<node_t>::iterator;

    class list_t : public std::list<node_t> {
        public:
            size_t sz_;
            
            bool isLIR(ListIt node) {
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
                return this->begin();
            }
            ListIt back() {
                ListIt it = this->end();
                return --it;
            }
            void push_front_LIR(node_t key) {
                this->push_front(key);
                this->setToLIR(this->front());
            }
            void push_front_HIR(node_t key) {
                this->push_front(key);
                this->setToHIR(this->front());
            }
            void push_back_LIR(node_t key) {
                this->push_back(key);
                this->setToLIR(this->back());
            }
            void push_back_HIR(node_t key) {
                this->push_back(key);
                this->setToHIR(this->back());
            }
            void bringForward(ListIt it){
                this->push_front(*it);
                this->erase(it);
            }
    };

    size_t sz_;
    
    list_t Allcache_;
    list_t HIRcache_;
    bool full(list_t &myList) const {
        return (myList.size() >= myList.sz_);
    }
    using HashConstIt = typename std::unordered_map<KeyT, ListIt>::const_iterator;

    std::unordered_map<KeyT, ListIt> Allhash_;
    std::unordered_map<KeyT, ListIt> HIRhash_;
    cache_(size_t size) { // constructor
        sz_ = size;
        Allcache_.sz_ = ceil(0.9 * size);
        HIRcache_.sz_ = floor(0.1 * size);

    }
    ~cache_() {
        HIRhash_.clear();
        Allhash_.clear();
        Allcache_.clear();
        HIRcache_.clear();
        sz_ = 0;
    }
    template <typename F>
    int lookUpAndUpdate(KeyT key, F slow_get_page)
    {
        auto hit = Allhash_.find(key); // check if key is in Stack
        auto hit_hir = HIRhash_.find(key);
        
        if(hit != Allhash_.end()) { // in Stack
            if(hit_hir != HIRhash_.end()) {// as HIR
                Allcache_.changestatus(hit->second);
                erase_HIR(hit_hir);
                pop_back_All();
            }
            else if(!Allcache_.isLIR(hit->second)) {// non-resident in Stack
                Allcache_.changestatus(hit->second);
                auto old_lir_back = Allcache_.back();// iterator
                if(full(HIRcache_)) {
                    pop_back_HIR();
                }
                
                add_to_back_HIR(*old_lir_back);
                pop_back_All();
                return 1;
            }
            Allcache_.bringForward(hit->second);
            StackPrune();
            return 1;
        }
        else {// is NOT in Stack
            if(hit_hir != HIRhash_.end()) 
                HIRcache_.bringForward(hit_hir->second);
            else {// new element
                if(full(Allcache_)) {
                    if(full(HIRcache_)) 
                        pop_back_HIR();                

                    add_to_front_HIR(node_t(slow_get_page(key)));
                    add_to_front_All(node_t(slow_get_page(key)), HIR);
                }
                else 
                    add_to_front_All(node_t(slow_get_page(key)), LIR);
            }
        }
        return 0;    
    }
    void StackPrune()
    {
        if(Allcache_.empty())
            return;
        auto old_back = Allcache_.back();

        if(Allcache_.isLIR(old_back))
            return;
        pop_back_All();
        old_back = Allcache_.back();
        while(!Allcache_.isLIR(old_back)) {
            add_to_back_HIR(*old_back);
            pop_back_All();
            old_back = Allcache_.back();
        }
    }
    void printCache() {
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
    void erase_All(HashConstIt hit) {
        //assert(hit);
        Allhash_.erase(hit);
        Allcache_.erase(hit->second);
    }
    void erase_HIR(HashConstIt hit) {
        //assert(hit);
        HIRhash_.erase(hit);
        HIRcache_.erase(hit->second);
    }
    void pop_back_All() {
        if(Allcache_.empty())
            return;
        Allhash_.erase((*Allcache_.back()).data_);
        Allcache_.pop_back();
    }
    void pop_back_HIR() {
        if(HIRcache_.empty())
            return;
        HIRhash_.erase((*HIRcache_.back()).data_);
        HIRcache_.pop_back();
    }
};
}