#include<iostream>
#include "smart_ptr.hpp"

#define POINTEUR(T) smart_ptr<T>


int main() {
    POINTEUR(POINTEUR(long)) smartptrArr;
   
    smartptrArr = new(0) POINTEUR(long)[1];
    for(int i = 0 ; i < 1 ; i++) {
        smartptrArr[i] = new(0) POINTEUR(long); 
    }
  
    return 0;
}


// #include<iostream>
// #include "smart_ptr.hpp"

// #define POINTEUR(T) smart_ptr<T>

// struct CC;
// struct CC
// {
//     // possiblement plusieurs champs
//     // dont un champ de type pointeur :
//     POINTEUR(CC) p;
//     CC() : p(0){}
// };

// int main()
// {
//     {
//         POINTEUR(double) test = new(0) double(10.0);
//         POINTEUR(char) test2 = new(0) char(1);
//         POINTEUR(int) pp;
//         {
//             POINTEUR(int) p = new(1) int; // ou bien new(1) int pour une surcharge d??di??e du new
//             pp = p;
//             *pp=3;
//         }
//         std::cout << *pp << std::endl;
//         //delete pp;
//         pp=new(1) int;                      // ou bien new(1) int
//         *pp=4;
//         std::cout << *pp << std::endl;
//         //delete pp;
//     }

//     POINTEUR(CC) ppile = new(1) CC; // ou bien new(1) CC
//     ppile->p=new(1) CC;                        // ou bien new(1) CC
//     ppile->p->p=ppile;
//     std::cout << ppile << std::endl;
//     //delete ppile->p;
//     //delete ppile;
//     ppile=new(1) CC;                             // ou bien new(1) CC
//     std::cout << ppile << std::endl;
//     //delete ppile;
//     return 0;
// }
