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
make
```
Execution:
```sh
./Garbage_collector
```
## Dev

### Documentation

Generation d'une documentation doxygen:
```sh
doxygen Doxyfile
```
La documentation HTML est générée dans le répertoire **doc**
