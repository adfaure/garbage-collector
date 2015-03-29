# Garbage Collector
Ce projet est une implémentation de pointeurs intelligents et d'un ramasse-miette capablent de gérer les cas de référencements circulaires.
## Build avec Premake4
Génération du Makefile:
```sh
premake4 gmake
```
Il est possible de préciser le compilateur que l'on souhaite utiliser via la
variable d'env CXX:
```sh
export CXX=/usr/bin/clang++
```
Ou
```
export CXX=/usr/bin/g++
```
### Compilation en mode RELEASE
```sh
make config=release
```
### Execution
```sh
./Garbage_collector
```

## Dev
IDEM sauf que l'on peut activer l'affichage debug
### Compilation en mode DEBUG
```sh
make config=debug
```

### Génération d'une Documentation
Generation d'une documentation doxygen:
```sh
doxygen Doxyfile
```
La documentation HTML et LATEX est générée dans le répertoire **doc**

## Définitions
### Block Valide / Block Invalide
Un block valide est un block dont le destructeur n'a pas encore été appelé.
### Block Vivant / Block Mort
Un block vivant est un block accessible depuis le pile.

## Niveaux de garbage collection
### Niveau 1 (small_garbage_collection)
Parcours de tous les blocks de mémoire et suppression des blocks invalides
### Niveau 2 (full_garbage_collection)
- Detection de cycle
- On force les blocks morts à se détacher pour que le destructeur puisse etre executé
- On peut désormais utiliser une garbage collection de niveau 1 pour libérer la mémoire

## Fonctionnement du compteur de références
Notre compteur de référence est matérialisé par deux opérations possibles sur le garbage collector.

## Fonctionnalités ok et testées:
- SmartPointer sur un type primitif et sur des types complexes.
- Tableaux de SmartPointer.
- Liste doublement chainée circulaire de SmartPointer.
- Nous detectons indirectement les cycles via l'usage d'une coloration puis d'un traitement afin de pouvoir désalouer l'espace à récupérer.
- La garbage collection de Niveau 1 (small_garbage_collection) s'effectue par défaut dés que la taille de la collection de blocs mémoire invalides dépasse est supérieure à 10.
- On peut executer une garbage collection de Niveau 2 (full_garbage_collection) quand on le souhaite grace l'utilisation d'un verrou.
- Actuellement notre garbage collection complète (avec detection de cycles) s'exécute avec l'heuristique suivante : 
    - Si nous détachons un SmartPointer d'un block mémoire et qu'il reste encore des SmartPointer le référeçant.
- Nous pourrions aisément imaginer des heuristiques plus efficaces:
    - Lancer une full garbage collection toutes les 10 min.
    - lancer une full garbage collection tous les 1000 accès mémoires.
    - lancer une full garbage collection tous les 10 appels à un petite garbage collection, etc...
- Une garbage collection de Niveau 2 (full_garbage_collection) est executée à la fin du programme.

## Fonctionnalités futures:
- Execution automatique des garbage collection de Niveau 2 (full_garbage_collection) selon plusieurs statégies personalisables et définitions d'une heuristique.
- Gestion des tableaux à deux dimensions avec notre new[]. (Le décallage sur l'adresse renvoyé gérée par le compilateur nous pose actuellement problème)