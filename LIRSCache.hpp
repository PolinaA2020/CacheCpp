#pragma once

#include <iostream>
#include <cstdio>
#include <list>
#include <iterator>
#include <unordered_map>
#include <cmath>

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
            void push_front_LIR(KeyT key) {
                this->push_front(key);
                this->LIR = 1;
            }
            void push_front_HIR(KeyT key) {
                this->push_front(key);
                this->LIR = 0;
            }
            void push_back_LIR(KeyT key) {
                this->push_back(key);
                this->LIR = 1;
            }
            void push_back_HIR(KeyT key) {
                this->push_back(key);
                this->LIR = 0;
            }
    };

    size_t sz_;
    
    list_t Allcache_;
    list_t HIRcache_;
    bool full(list_t &myList) const {
        return (myList.size() >= myList.sz_);
    }
    
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
                HIRhash_.erase(hit_hir);
                HIRcache_.erase(hit_hir->second);
            }
            else if(!Allcache_.isLIR(hit->second)) {// non-resident in Stack
                Allcache_.changestatus(hit->second);
                auto old_lir_back = Allcache_.back();// iterator
                if(full(HIRcache_))
                	HIRcache_.pop_back();
                	
                HIRcache_.push_back(*old_lir_back);
                HIRcache_.setToHIR(HIRcache_.back());
                HIRhash_[(*HIRcache_.back()).data_] = HIRcache_.back();
                Allhash_.erase((*old_lir_back).data_);
                Allcache_.erase(old_lir_back);
                return 1;
            }
            bringForwardStack(hit->second);
            // printCache();
            StackPrune();
            // printCache();
            return 1;
        }
        else {// is NOT in Stack
            if(hit_hir != HIRhash_.end()) 
                bringForwardHIR(hit_hir->second);
            else {// new element
                if(full(Allcache_)) {
                    if(full(HIRcache_))
                        HIRcache_.pop_back();

                    HIRcache_.push_front(node_t(slow_get_page(key)));
                    Allcache_.push_front(node_t(slow_get_page(key)));
                    Allcache_.setToHIR(Allcache_.front());
                    HIRhash_[(*HIRcache_.front()).data_] = HIRcache_.front();
                    Allhash_[(*Allcache_.front()).data_] = Allcache_.front();
                }
                else {
                    Allcache_.push_front(node_t(slow_get_page(key)));
                    Allcache_.setToLIR(Allcache_.front());
                    Allhash_[(*Allcache_.front()).data_] = Allcache_.front();
                }
            }
            
        }
        return 0;    
    }
    bool StackPrune()
    {
        auto old_back = Allcache_.back();
        if(Allcache_.isLIR(old_back))
            return true;
        Allhash_.erase((*old_back).data_);
        Allcache_.pop_back();
        old_back = Allcache_.back();
        while(!Allcache_.isLIR(old_back)) {
            //auto new_hir_back = HIRcache_.push_back(*old_back);
            HIRcache_.push_back(*old_back);
            //HIRhash_.insert(new_hir_back);
            HIRhash_[(*HIRcache_.back()).data_] = HIRcache_.back();

            Allhash_.erase((*old_back).data_);
            Allcache_.pop_back();
        }
        return true;
    }
    void bringForwardStack(ListIt it)
    {
        Allcache_.push_front(*it);
        Allcache_.erase(it);
    }
    void bringForwardHIR(ListIt it)
    {
        HIRcache_.push_front(*it);
        HIRcache_.erase(it);
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
        /*if((*Allcache_.back()).LIR_)
                std::cout << "LIR ";
            else
                std::cout << "HIR ";
        std::cout<<(*Allcache_.back()).data_ << "\n";
        if((*Allcache_.front()).LIR_)
                std::cout << "LIR ";
            else
                std::cout << "HIR ";
        std::cout<<(*Allcache_.front()).data_ << "\n";
        */std::cout << "------------\n" << "HIR elements\n";
        for(ListIt it = HIRcache_.begin(); it != HIRcache_.end(); it++){
            std::cout << (*it).data_ << "\n";
        }
    }
};
}