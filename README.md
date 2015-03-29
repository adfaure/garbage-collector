# Garbage Collector
Ce projet est une implémentation de pointeurs intelligents et d'un ramasse-miette capablent de gérer les cas de référencements circulaires.

## Execution avec CMAKE
Génération du Makefile:
```sh
cmake .
```
Il est possible de préciser le compilateur que l'on souhaite utiliser:
```sh
cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ .
```
Nettoyage éventuel:
```
make clean
```
Compilation et édition de liens:
```sh
make config=release
```
Execution:
```sh
./Garbage_collector
```
## Dev
IDEM sauf que l'on peut activer l'affichage debug
Compilation et édition de liens:
```sh
make config=debug
```

### Documentation

Generation d'une documentation doxygen:
```sh
doxygen Doxyfile
```
La documentation HTML est générée dans le répertoire **doc**


## Niveaux de garbage collection
### Niveau 1 (small_garbage_collection)
Parcours de tous les blocks de mémoire 
### Niveau 2 (full_garbage_collection)
- Detection de cycle
- On force les blocks morts à se détacher pour que le destructeur
- 

## Définitions
### Block Valide / Block Invalide
Un block valide est un block dont le destructeur n'a pas encore été appelé.
### Block Vivant / Block Mort
Un block vivant est un block accessible depuis le pile.

