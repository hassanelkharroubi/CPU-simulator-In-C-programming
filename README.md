
# Projet: Réalisation d’un simulateur de CPU

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

**Contact** : AKKA Boutaina && AYED Yasmine

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

## **Contexte et objectifs**

Ce projet vise à développer un simulateur de processeur (CPU) simplifié en langage C, intégrant :

* Un **gestionnaire de mémoire** dynamique (allocation/libération de segments

* Un **parser** pour un pseudo-assembleur (.DATA/.CODE) 

* Des **modes d’adressage** (immédiat, registre, direct, indirect, override segment)

* La **gestion d’un segment de données** (“DS”), de code (“CS”), de pile (“SS”) et d’un **extra segment** (“ES”)

Le simulateur supporte également l’exécution pas à pas des instructions, la gestion des registres généraux et de drapeaux (ZF, SF, IP) ainsi que les opérations arithmétiques et logiques fondamentales.

## 

## **📂 Structure du projet**

.  
├── include/      \# Fichiers d’en-tête (.h)  
│   ├── hashmap.h  
│   ├── memory\_handler.h  
│   ├── assembler\_parser.h  
│   ├── parser\_result.h  
│   ├── cpu.h  
│   ├── code\_segment.h  
│   ├── stack\_segment.h  
│   └── extra\_segment.h  
├── src/          \# Code source (.c)  
│   ├── hashmap.c  
│   ├── memory\_handler.c  
│   ├── assembler\_parser.c  
│   ├── parser\_result.c  
│   ├── cpu.c  
│   ├── code\_segment.c  
│   ├── stack\_segment.c  
│   └── extract\_segment.c  
├── tests/        \# Tests unitaires  
│   ├── test\_memory.c  
│   ├── test\_parser.c  
│   ├── test\_cpu.c  
│   └── test\_assembler\_parser.c  
├── obj/          \# Objets compilés (.o)  
├── bin/          \# Binaries des tests  
└── Makefile      \# Automatisation de la compilation

**Perspectives d’amélioration :**

* Gestion de la mémoire virtuelle et caches

* Ajout d’instructions supplémentaires (MUL, DIV, I/O)

* Interface graphique pour visualisation de la RAM et des registres

