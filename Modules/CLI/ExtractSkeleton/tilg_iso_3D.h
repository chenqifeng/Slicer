/*=========================================================================

  Program:   Extract Skeleton
  Module:    $HeadURL: http://svn.slicer.org/Slicer4/trunk/Modules/CLI/ExtractSkeleton/tilg_iso_3D.h $
  Language:  C++
  Date:      $Date: 2011-12-07 04:49:19 +0800 (周三, 07 十二月 2011) $
  Version:   $Revision: 18864 $

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/
#ifndef _TILG_ISO_3D_H_
#define _TILG_ISO_3D_H_

#define OBJ  1
#define BG   0

int Env_Code_3_img(int loc[3], unsigned char *img, int dim[3]);
// returns the neighbor code including the center at position loc

int Tilg_Test_3(int c, int d, int type);

/* Calculation of Tilg-criterion, c is the Neighor-code of */
/* 3x3x3-Region, including the center                      */
// returns 0 if cannot be 'tilged'
// if type == 1 -> sheet preserving tilg
// if type == 0 -> full tilg
// d = for parrel tilg -> 0,1,2,3,4,5   N,S,E,W,T,D

void tilg_iso_3D(int dx, int dy, int dz, unsigned char *data, unsigned char *res, int type);

// 3D isotropic tilg-procedure that does a 3D thinning
// dx,dy,dz  are the dimensions of the input (data) and output (res) image
// output image has to be allocated
// if type == 1 -> sheet preserving tilg
// if type == 0 -> full tilg

#endif
