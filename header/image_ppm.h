// ----------------------------------------------------------------------------
// Filename        : image_ppm.c
// Description     :
// Created On      : Tue Mar 31 13:26:36 2005
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define arrayAllocation(nom, type, nombre)                                      \
   if ((nom = (type *)calloc(nombre, sizeof(type))) == NULL)                    \
   {                                                                            \
      printf("\n Allocation dynamique impossible pour un pointeur-tableau \n"); \
      exit(1);                                                                  \
   }

typedef unsigned char OCTET;

void ignoreComments(FILE *f);
void writeImagePPM(char nom_image[], OCTET *pt_image, int nb_lignes, int nb_colonnes);
void readRowColPPM(char nom_image[], int *nb_lignes, int *nb_colonnes);
void readImagePPM(char nom_image[], OCTET *pt_image, int taille_image);