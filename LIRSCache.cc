// LIRS Cache algorithm c++ realizaition 
#include <iostream>
#include <cstdio>
#include <list>

template <typename T, typename KeyT = int>
struct cache_t {
    size_t size_;
    std::list<T> Allcache_;
    std::list<T> HIRcache_;
    std::unordered_map<KeyT, list<T> > Allhash_;
    std::unordered_map<KeyT, list<T> > HIRhash_;
    


    bool lookUp(KeyT key, slow_get_page);
    bool StackPrune();
};

bool lookUp(KeyT key, slow_get_page)
{
    auto hit = Allhash_.find(key); // check if key is in Stack
    auto hit_hir = HIRhash_.find(key);
    
    if(hit) { // in Stack
        if(hit_hir) {// as HIR
            Allcache_.changestatus(hit);
            HIRhash_.erase(&hit_hir);
            HIRcache_.erase(hit_hir);
        }
        else if(!Allcache_.isLIR(hit)) {// non-resident in Stack
            Allcache_.changestatus(hit);
            auto old_lir_back = Allcache_.back();
            auto new_hir_back = HIRcache_.push_front(Allcache_.data(old_lir_back));
            HIRhash_.insert(&new_hir_back);// what if no space in hirs
            Allhash_.erase(&old_lir_back);
            Allcache_.erase(old_lir_back);
            return true;
        }
        Allcache_.bringforward(hit);
        Allcache_.prune();
        /*if(prune) { // add to prune func
            HIRcache_.insert(old_lir_back);
            HIRhash_.insert(&old_lir_back);
        }*/
    }
    else {// is NOT in Stack
        if(hit_hir) 
            HIRcache_.bringforward(hit_hir);
        else {// new element
            if(Allcache_.full()) {
                if(HIRcache_.full())
                    HIRcache_.popback();
                auto new_hir_front = HIRcache_.push_front(key);
                HIRhash_.insert(&new_hir_front);
            }
            else {
                auto new_all_front = Allcache_.push_front(key);
                Allhash_.insert(new_all_front);
            }
        }
        
    }
    return true;    
}

bool StackPrune()
{
    auto old_back = Allcache_.back();
    while(Allcache_.isLIR(old_back)) {
        HIRcache_.push_back(Allcache_.data(old_back));
        Allcache_.popback();

    }
}
