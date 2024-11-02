#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structure.h"
#include<math.h>
#include<time.h>


//defines
#define MAXVALUE 99999
#define FILESIZE 150
#define NBC 6
#define NBL 10
#define map_size 100
//public variables
struct Vect *v; // Data vector
struct Net net; // Network structure
struct Neuron **map; // Neurons map
double  mean_vect[4]; //Vector of mean values for intialization
char versicolor[] = "versicolor", setosa[] = "setosa", virginica[] = "virginica"; // plant types
int indices[FILESIZE]; // for the data order


//functions
double max (double x , double y){ if (x>y) return x; else return y;}
double min (double x , double y){ if (x<y) return x; else return y;}
double sqr (double x) {return x*x;}
double randr(double min, double max){ // returns random value between min and max

    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
void print_data_vect(){
for (int i=0;i<FILESIZE;i++) printf("%d : %f %1lf %1lf %1lf\n",i, v[i].data[0], v[i].data[1] , v[i].data[2], v[i].data[3]);
}
void print_mean_vect(){
for (int i=0;i<4;i++)
    printf("%1lf ", mean_vect[i]);}
void print_map (){
for(int x = 0; x < NBC; x++) {
    for(int y = 0; y < NBL; y++) {
    printf("\n map[%d][%d]" , map[x][y].posx,map[x][y].posy);
     for(int i=0; i<4;i++) {
       printf( "%f ", map[x][y].w[i]);
      }
    }}}
void print_map_full (){
    for(int x = 0; x < NBC; x++) {
    for(int y = 0; y < NBL; y++) {
    if (map[x][y].etiq=='V')   printf("\033[1;31m");
    if (map[x][y].etiq=='W')   printf("\033[1;32m");
    if (map[x][y].etiq=='S')   printf("\033[0;34m");
    printf("%c " , map[x][y].etiq);
    }
     printf("\n");
    }
     printf("\n");
     printf("\033[0m");

}
void print_indice (int indices[]){ // print index vector
   for (int i=0;i<FILESIZE;i++){
        printf("%d: %d|",i,indices[i]);
    }

}
void init_shuffle (int indices[]){ //randomize pick order
    int swap,randomint;

    for (int i=0;i<FILESIZE;i++){ //intialize index table with values [0..149]
        indices[i]= i;
    }

    for (int i=0;i<FILESIZE;i++){ // swap the values randomly
        randomint= (int) rand()%FILESIZE;
        swap= indices[i];
        indices[i]=indices[randomint];
        indices[randomint]=swap;

    }
}

void shuffle (int indices[]){
	    int swap,randomint;
	    for (int i=0;i<FILESIZE;i++){ // swap the values randomly
        randomint= (int) rand()%FILESIZE;
        swap= indices[i];
        indices[i]=indices[randomint];
        indices[randomint]=swap;

    }
}


void normalize_data (struct Vect* v){ //normalizing vector
    double value;
    for(int i=0;i<FILESIZE;i++){
        value= sqrt(sqr(v[i].data[0])+sqr(v[i].data[1])+sqr(v[i].data[2])+sqr(v[i].data[3]));
        v[i].norm=value;
           for(int w=0;w<4;w++)
    v[i].data[w]/=value;
    }

        }

void mean_vector(struct Vect* p){// calculating the mean vector values

    for(int i=0;i<FILESIZE;i++){
        mean_vect[0]+=v[i].data[0];
        mean_vect[1]+=v[i].data[1];
        mean_vect[2]+=v[i].data[2];
        mean_vect[3]+=v[i].data[3];
        }
    for(int i=0;i<4;i++)
            mean_vect[i]/=FILESIZE;


}
void initialize_net ( struct BMU * bmu, int nbIter , double learningRate, int nbd_size){
    net.nb_it=nbIter;
    net.alpha=learningRate;
    net.nb_nodes=map_size;
    net.nb_c=NBC;
    net.nb_l=NBL;
    net.nbd_size=nbd_size;
    net.BMU=bmu;

}
void initialize_map(){
    map = (struct Neuron **) malloc(sizeof(struct Neuron *) * NBC);

  int x, y;
  for(x = 0; x < NBC; x++) {
      map[x] = (struct Neuron *) malloc(sizeof( struct Neuron) * NBL);
  }

  for(x = 0; x < NBC; x++) {
    for(y = 0; y < NBL; y++) {
      map[x][y].w = ( double *) malloc(sizeof(double) * 4);
      map[x][y].etiq='*';

     for(int i=0; i<4;i++) {
        map[x][y].w[i] = randr(mean_vect[i]-0.02, mean_vect[i]+0.05 ); // random weight values

        }
    map[x][y].posx=x;
    map[x][y].posy=y;
    }
  }

}
double distance_euclidienne (struct Vect *vec , struct Neuron* n){ // weight distance between data and neuron

    double res =0,diff;
    for (int i=0;i<4;i++){
        diff=vec->data[i] - n->w[i];
        res+= pow(fabs(diff),2);
    }
    return sqrt(res);
}
double node_dist(struct Neuron *node1, struct Neuron *node2) {
    return sqrt(pow((node2->posx - node1->posx), 2) + pow((node2->posy - node1->posy), 2));
}
void openFile_readData (char * nomfichier){  //reads data from file to weight vector

    v =(struct Vect*)malloc(FILESIZE * sizeof(struct Vect));      //initialize data vector
    FILE* fichier= NULL;
    int i=0;
    char tmp[10];
    double pw,pl,sw,sl;
    fichier=fopen("iris.data","r");
    if (fichier != NULL)
{

      while(fscanf(fichier, "%lf,%lf,%lf,%lf,Iris-%s\n",&sw,&sl,&pw,&pl,&tmp )!=EOF){
        v[i].data= malloc (4*sizeof(double));
        v[i].data[0]=sw; //SW
        v[i].data[1]=sl; //SL
        v[i].data[2]=pw; //PW
        v[i].data[3]=pl; //PL

        int res=(strcmp(tmp,versicolor));
        if (res==0) v[i].id='V';
        else if (res>0) v[i].id='W';
        else if (res<1) v[i].id='S';

        i++;
      }

    }
    else{
        printf("Impossible d'ouvrir le fichier iris.data");
    }
    fclose( fichier);
}
struct Neuron *trouver_bmu(struct Vect* vec){ //find BMU to vec from map
    double min=MAXVALUE,tmp;
    struct Neuron * res;

    for(int x = 0; x < NBC; x++) {
        for(int y = 0; y < NBL; y++) {
        tmp=distance_euclidienne(vec,&map[x][y]);
        if (tmp < min){

            res= &(map[x][y]);
            min=tmp;
            }
        }

        }

    return res;
}
double alpha (int t ){ return net.alpha*(1-(t/net.nb_it));} //calculate alpha value for iteration t

void cycle_apprentissage (struct bmuList * bmu_list , int iteration){
        //if(iteration==net.nb_it) return;

    shuffle(indices);


    struct BMU * bmus = (struct BMU*)(malloc(sizeof(struct BMU)));// create BMU structure
    struct Neuron * best;
    //finding bmu
    for (int i=0;i<FILESIZE;i++){ //for each data vector

    struct BMU * current_bmu = (struct BMU*)(malloc(sizeof(struct BMU))); 

        best=trouver_bmu(&v[indices[i]]); //find BMU
        best->etiq=v[indices[i]].id; //Change flower type
        current_bmu->c=best->posx;  // save location in BMU list
        current_bmu->l=best->posy;

        current_bmu->next=bmu_list->first; //add current BMU to list
        bmu_list->first=current_bmu;
        bmu_list->nb_elems++;

        // changing the BMU's weight

        for(int x=0;x<4;x++)
        best->w[x] = best->w[x] + alpha(iteration) * v[indices[i]].data[x] - (best->w[x]) ;


        //

      //changing weights in the BMU's neighborhood 
        for (int x = 0; x < NBC; x++) {
            for (int y = 0; y < NBL; y++) {
                if (node_dist(best, &(map[x][y])) <= net.nbd_size) {
                    for (int w = 0; w < 4; w++) {
                        map[x][y].w[w] += alpha(iteration) * (v[indices[i]].data[w] - map[x][y].w[w]);
                    }
                }
            }
        }
}
	free(bmus);

}

void learning (int epochs,int steps){
  struct BMU * bmu;
    struct bmuList * bmu_list;
    double nbd = node_dist(&(map[0][0]), &(map[NBC-1][NBL-1])) / 2 + 1; //calculate the distance between the two ends of the map

    bmu_list = (struct bmuList*)(malloc(sizeof(struct bmuList)));
    bmu_list->nb_elems=0;

    bmu = (struct BMU*)(malloc(sizeof(struct BMU)));

    initialize_net (bmu,epochs ,0.9,nbd);


    for (int iter_counter=0; iter_counter<=net.nb_it ;iter_counter++){
        cycle_apprentissage(bmu_list,iter_counter);
    if (iter_counter==net.nb_it/4) net.nbd_size=1; // first 1/4 with max neighborhood and rest 3/4 with adjacent neighborhood
     if (iter_counter % (net.nb_it/steps) ==0){ // print
     // printf("Epoch: %d Neighborhood size: %d \n",iter_counter,net.nbd_size);
      printf("Epoch %d : \n",iter_counter);
      
      print_map_full ();
      }
      
    
    }

       }





int main (int argc, char **argv){

  double time_spent = 0.0;
 
    clock_t begin = clock();

	if(argc != 3 )  {
    fprintf(stderr, "USAGE : '' ./iris Epochs Steps[1..epochs]'' \n ");
    exit(1);
}

    int  epochs = atoi(argv[1]); 
    int  steps = atoi(argv[2]);       
    char *nomfichier="iris.data";

    openFile_readData(nomfichier);

    init_shuffle(indices);

    normalize_data(v);

    mean_vector(v);


    initialize_map();


    learning(epochs,steps);

    clock_t end = clock();
    
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 
    printf("The elapsed time is %f seconds \n", time_spent);


     return 0;}
