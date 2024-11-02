#include<stdio.h>
#include<string.h>
#include"math.h"

#ifndef __STRUCTURES_H
#define __STRUCTURES_H


struct Vect{
double* data;
double norm;
char id; // V / W / S
} Vect;

struct Neuron{
int posx;
int posy;
double * w; //weight
double act ;//degré de match
char etiq ; //type de fleur V/ W / S
} Neuron;


struct BMU {
    int l;
    int c;
    struct BMU * next;
} BMU;

struct bmuList{
    struct BMU* first;
    int nb_elems;
} bmuList;

struct Net{
    int nb_it;
    double alpha; //coefficient d'apprentissage
    int nb_nodes;
    int nb_l;
    int nb_c;
    int nbd_size; //voisinage
    struct BMU* BMU ;
    struct Neuron** Map;
} Net;


#endif
