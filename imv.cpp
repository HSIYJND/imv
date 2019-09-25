/* m3ta3: reimagination of a (late 2011, early 2012) personal, primordial visualization library
that inspired further developments at UVic, CFS and elsewhere.. */
#include "newzpr.h"
#include<fstream>
#include<iostream>
#include"util.h"
int main(int argc, char ** argv){
  if(sizeof(float) != 4){
    printf("Error: sizeof(float) != 4\n");
    exit(1);
  }
  if(argc < 2){
    printf("imv.cpp: [infile]\n");
    exit(1);
  }
  char * infile = argv[1];
  long int NRow, NCol, NBand;
  parseHeaderFile(getHeaderFileName(string(infile)), NRow, NCol, NBand);
  printf(" infile: %s nrow %ld ncol %ld nband %ld\n", infile, NRow, NCol, NBand);
  printf(" getFileSize %ld expected %ld\n", getFileSize(infile), NRow * NCol * NBand * sizeof(float));

  SA<float> dat(NRow * NCol * NBand);
  FILE * f = fopen(infile, "rb");
  long int nread = fread(&dat[0], sizeof(float), NRow * NCol * NBand, f);
  if(nread != NRow * NCol * NBand){
    printf("Error (imv.cpp): number of elements read (%d) different than expected (%d)\n", nread, NRow*NCol*NBand);
    exit(1);
  }
  fclose(f);

  myImg a;
  if(NBand == 1){
    SA<float> dat2(NRow * NCol * 3);
    int i, j, k;
    k = 0;
    for(i = 0; i < 3; i++){
      for(j = 0; j < NRow * NCol; j++){
        dat2[k++] = dat[j];
      }
    }
    a.initFrom(&dat2, NRow, NCol, 3);
  }
  else{
    a.initFrom(&dat, NRow, NCol, NBand);
  }

  zprManager * myManager = zprManager::Instance(argc, argv);
  zprInstance * myZpr = myManager->newZprInstance(NRow, NCol, NBand);
  glImage * myImage = new glImage(myZpr, &a);
  glutMainLoop();
  return 0;
}
