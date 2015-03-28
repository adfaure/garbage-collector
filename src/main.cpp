#include<iostream>
#include "smart_ptr.hpp"

#define POINTEUR(T) smart_ptr<T>

struct CC;
struct CC
{
    // possiblement plusieurs champs
    // dont un champ de type pointeur :
    POINTEUR(CC) p;
    CC() : p(0){}
};

int main()
{
    {
        POINTEUR(int) pp;
        {
            POINTEUR(int) p = new(1) int; // ou bien new(1) int pour une surcharge d??di??e du new
            pp = p;
            *pp=3;
        }
        std::cout << *pp << std::endl;
        //delete pp;
        pp=new(1) int;                      // ou bien new(1) int
        *pp=4;
        std::cout << *pp << std::endl;
        //delete pp;
    }

    POINTEUR(CC) ppile = new(1) CC; // ou bien new(1) CC
    ppile->p=new(1) CC;                        // ou bien new(1) CC
    ppile->p->p=ppile;
    std::cout << ppile << std::endl;
    //delete ppile->p;
    //delete ppile;
    ppile=new(1) CC;                             // ou bien new(1) CC
    std::cout << ppile << std::endl;
    //delete ppile;
    return 0;
}
