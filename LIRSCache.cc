// LIRS Cache algorithm c++ realizaition 
#include <iostream>
#include <cstdio>
#include <list>
#include <iterator>
#include <cassert>
#include <unordered_map>
#include <cmath>

template <typename T, typename KeyT = int> struct cache_ {
    class list_t : public std::list<T> {
        size_t sz_;
        bool LIR;
        
        bool isLIR() {
            return LIR;
        }
        void changestatus() {
            LIR = !LIR;
        }
    };
    size_t sz_;
    
    list_t Allcache_;
    list_t HIRcache_;
    bool full(std::list<T> *myList) const {
            return (myList->size() == myList->sz_);
    }
    using ListIt = typename std::list<T>::iterator;
    std::unordered_map<KeyT, ListIt > Allhash_;
    std::unordered_map<KeyT, ListIt > HIRhash_;
    
    cache_(size_t size) { // constructor
        sz_ = size;
        Allcache_.sz_ = ceil(0.9 * size);
        HIRcache_.sz_ = floor(0.1 * size);

    }

    bool lookUpAndUpdate(KeyT key, int slow_get_page)
    {
        auto hit = Allhash_.find(key); // check if key is in Stack
        auto hit_hir = HIRhash_.find(key);
        
        if(hit) { // in Stack
            if(hit_hir) {// as HIR
                Allcache_[hit].changestatus();
                HIRhash_.erase(hit_hir);
                HIRcache_.erase(hit_hir);
            }
            else if(!Allcache_.isLIR(hit)) {// non-resident in Stack
                Allcache_[hit].changestatus();
                auto old_lir_back = Allcache_.end();// iterator
                auto new_hir_back = HIRcache_.push_back(*old_lir_back);
                HIRhash_.insert(new_hir_back);// what if no space in hirs
                Allhash_.erase(old_lir_back);
                Allcache_.erase(old_lir_back);
                return true;
            }
            bringForwardStack(hit);
            StackPrune();
        }
        else {// is NOT in Stack
            if(hit_hir) 
                bringForwardHIR(hit_hir);
            else {// new element
                if(full(&Allcache_)) {
                    if(full(&HIRcache_))
                        HIRcache_.pop_back();
                    auto new_hir_front = HIRcache_.push_front(slow_get_page(key));
                    HIRhash_.insert(new_hir_front);
                }
                else {
                    auto new_all_front = Allcache_.push_front(slow_get_page(key));
                    Allhash_.insert(new_all_front);
                }
            }
            
        }
        return true;    
    }
    bool StackPrune()
    {
        auto old_back = Allcache_.end();
        Allhash_.erase(old_back);
        Allcache_.pop_back();
        old_back = Allcache_.end();
        while(!Allcache_[old_back].isLIR()) {
            auto new_hir_back = HIRcache_.push_back(*old_back);
            HIRhash_.insert(new_hir_back);
        
            Allhash_.erase(old_back);
            Allcache_.pop_back();
        }
        return true;
    }
    void bringForwardStack(ListIt it)
    {
        Allcache_.push_front(*it);
        Allhash_.insert(Allcache_.begin());
        Allhash_.erase(it);
        Allcache_.erase(it);
    }
    void bringForwardHIR(ListIt it)
    {
        HIRcache_.push_front(*it);
        HIRhash_.insert(HIRcache_.begin());
        HIRhash_.erase(it);
        HIRcache_.erase(it);
    }
};