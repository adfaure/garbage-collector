# Garbage Collector
Ce projet est une implémentation de pointeurs intelligents et d'un ramasse-miette capablent de gérer les cas de référencements circulaires.

## Execution avec CMAKE
```sh
cmake .
```
```sh
make
```
```sh
./Garbage_collector
```

Il est possible de préciser le compilateur que l'on souhaite utiliser:
```sh
cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ .
```
