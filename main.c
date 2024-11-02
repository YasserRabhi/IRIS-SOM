#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structure.h"
#include<math.h>
#include<time.h>


//defines

#define MAXVALUE 99999
#define FILESIZE 150
#define VECTORSIZE 4
#define NBC 6
#define NBL 10
#define map_size 100


//public variables
struct Vect *v; // Data vector
struct Net net; // Network structure
struct Neuron **map; // Neurons map
double  mean_vect[VECTORSIZE]; //Vector of mean values for intialization
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
	for (int i=0;i<VECTORSIZE;i++)
    	printf("%1lf ", mean_vect[i]);
}

void print_map (){
	for(int x = 0; x < NBC; x++) {
		for(int y = 0; y < NBL; y++) {
   		 	printf("\n map[%d][%d]" , map[x][y].posx,map[x][y].posy);
    	 		for(int i=0; i<VECTORSIZE;i++) {
       				printf( "%f ", map[x][y].w[i]);
      			}
    		}
    	}
 }
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
           for(int w=0;w<VECTORSIZE;w++)
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
    for(int i=0;i<VECTORSIZE;i++)
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
      map[x][y].w = ( double *) malloc(sizeof(double) * VECTORSIZE);
      map[x][y].etiq='*';

     for(int i=0; i<VECTORSIZE;i++) {
        map[x][y].w[i] = randr(mean_vect[i]-0.02, mean_vect[i]+0.05 ); // random weight values

        }
    map[x][y].posx=x;
    map[x][y].posy=y;
    }
  }

}

double distance_euclidienne (struct Vect *vec , struct Neuron* n){ // weight distance between data and neuron

    double res =0,diff;
    for (int i=0;i<VECTORSIZE;i++){
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

      while(fscanf(fichier, "%lf,%lf,%lf,%lf,Iris-%s\n",&sw,&sl,&pw,&pl,tmp )!=EOF){
        v[i].data= malloc (VECTORSIZE*sizeof(double));
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

void add_list ( struct bmuList* bmu_list , struct BMU * bmu){
	bmu->next=bmu_list->first;
	bmu_list->nb_elems++;
	bmu_list->first=bmu;
}


void free_list ( struct bmuList* bmu_list){ // free BMU list
 	
 	if(bmu_list == NULL) return;
  	struct BMU * tmp;

	while (bmu_list->first!=NULL){ // while list has elements
		tmp=bmu_list->first->next;
		free(bmu_list->first);
		bmu_list->first=tmp;
		}
	
	free(bmu_list);
	}


void print_list (struct bmuList* bmu_list){
	printf(" Printing BMU List of %d elems :\n",(bmu_list->nb_elems));
	if (bmu_list == NULL) {printf(" ERROR BMU LIST NULL POINTER"); return; }
	struct BMU * bmu_tmp;
	bmu_tmp=bmu_list->first;
	for (int i=1;i<=bmu_list->nb_elems;i++){
		if (bmu_tmp == NULL) { printf(" ERROR BMU NULL POINTER"); return; }
		printf("%d %d |", bmu_tmp->c , bmu_tmp->l);
		bmu_tmp=bmu_tmp->next;
	}
	printf("\n");
}



struct bmuList* find_bmus(struct Vect* vec){ //find BMU to vec from map
    struct bmuList * bmu_list;
    bmu_list = (struct bmuList*)(malloc(sizeof(struct bmuList)));
    struct BMU * bmu;
    
    double min=MAXVALUE,tmp;
    struct Neuron * res;

    for(int x = 0; x < NBC; x++) {
        for(int y = 0; y < NBL; y++) {
        tmp=distance_euclidienne(vec,&map[x][y]);

        if (tmp <= min){ // if bmu is better
        	bmu = (struct BMU*)(malloc(sizeof(struct BMU)));   // save location in BMU 
        	bmu->c=x;
        	bmu->l=y;

		if(tmp==min){ // if another equal BMU add to BMU list 

		add_list(bmu_list,bmu);
		}
		else{ // if BMU is superior
		//free_list(bmu_list);
		bmu_list = (struct bmuList*)(malloc(sizeof(struct bmuList))); // create new BMU list with BMU
		bmu_list->first=bmu;
		bmu_list->nb_elems=1;;
		}
            min=tmp;
            }
        }

        }

    return bmu_list;
}

struct BMU* pick_bmu ( struct bmuList * bmu_list){
	
	if (bmu_list == NULL) {fprintf(stderr," ERROR BMU LIST NULL POINTER"); return NULL; }
	struct BMU * bmu=bmu_list->first;
	if (bmu_list->nb_elems==1) // if bmuList has 1 BMU then send it
		return bmu_list->first;


	int rand_bmu =(int) rand() % (bmu_list->nb_elems+ 1); // randomly choose bmu 

	for(int i=0;i<rand_bmu && (bmu->next!=NULL);i++)
		bmu=bmu->next;

	if (bmu == NULL) {fprintf(stderr," ERROR BMU PICK NULL POINTER"); return NULL; }
	return bmu;
	
}
double alpha (int t ){ return net.alpha*(1-(t/net.nb_it));} //calculate alpha value for iteration t

void learning_cycle (int iteration){

    init_shuffle(indices);

    struct bmuList * bmu_list; // create BMU structure
    struct BMU * bmu; 
    struct Neuron * best;
    //finding bmu
    for (int i=0;i<FILESIZE;i++){ //for each data vector


     bmu_list=find_bmus(&v[indices[i]]); //find BMU list for node

     bmu=pick_bmu(bmu_list); // randomly pick a BMU out of the list
           
     best=&(map[bmu->c][bmu->l]); // find the BMU node
        
     best->etiq=v[indices[i]].id; //Change flower type
       
      //changing the BMU's weight and changing weights in the BMU's neighborhood 
        for (int x = 0; x < NBC; x++) {
            for (int y = 0; y < NBL; y++) {
                if (node_dist(best, &(map[x][y])) <= net.nbd_size) {
                    for (int w = 0; w < VECTORSIZE; w++) {
                        map[x][y].w[w] += alpha(iteration) * (v[indices[i]].data[w] - map[x][y].w[w]);
                    }
                }
            }
        }
       //free_list(bmu_list);

     }
}

void learning (int steps){

    double nbd = node_dist(&(map[0][0]), &(map[NBC-1][NBL-1])) / 2 + 1; //calculate the distance between the two ends of the map
    for (int iter_counter=1; iter_counter<=net.nb_it ;iter_counter++){
        learning_cycle(iter_counter);
    	if (iter_counter==net.nb_it/4) net.nbd_size=1; // first 1/4 with max neighborhood and rest 3/4 with adjacent neighborhood
     	if (iter_counter % (net.nb_it/steps) ==0 && iter_counter !=0){ // print
      	printf("Epoch %d : \n",iter_counter);
      	print_map_full ();
      	}
    }
}



int main (int argc, char **argv){

	double time_spent = 0.0; //set up timer for exection 
	srand(time(0));
	clock_t begin = clock();

	if(argc != 3 )  { fprintf(stderr, "USAGE : '' ./iris Epochs Steps[1..epochs]'' \n "); exit(1);}

	int  epochs = atoi(argv[1]); 
	int  steps = atoi(argv[2]);  
	epochs=max(epochs,1); // avoid negative epoch value
	steps=max(steps,1); // avoid devision by 0 and negative numbers     
	char *nomfichier="iris.data";


	openFile_readData(nomfichier);

	init_shuffle(indices);

	normalize_data(v);

	mean_vector(v);

	initialize_map();

	struct BMU * bmu; bmu = (struct BMU*)(malloc(sizeof(struct BMU)));

	initialize_net (bmu,epochs ,0.9,6);
	
	learning(steps);

	clock_t end = clock();
	    
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	 
	printf("The elapsed time is %f seconds \n", time_spent);


     return 0;}
