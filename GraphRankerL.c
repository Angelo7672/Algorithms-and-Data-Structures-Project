#include <stdio.h>
#include <stdlib.h>

typedef struct catalogo_s{
	int posincoda;
	long int distanza;
}catalogo_t;

typedef struct coda_s{			
	int num;
	int precedente;
	long int costo;
}coda_t;

typedef struct classifica_s{
	int grafo;
	long int punteggio;
}classifica_t;

int scansiona(char []);
int creaGrafo(int ,long int**);
long int velocizza(long int** ,int ,int );
long int dijkstra(catalogo_t* ,long int** ,coda_t* ,int );
void accoda(coda_t* ,catalogo_t* ,int );
int cancellaMin(int ,coda_t* ,catalogo_t* );
void minHeapify(coda_t* ,int ,int ,catalogo_t* );
void decrementaPri(coda_t* ,int ,int ,catalogo_t* );
void sali(coda_t* ,int ,catalogo_t* );
long int inserisci(classifica_t* ,int ,int ,long int );
long int cambioalvertice(classifica_t* ,int ,long int ,int );
void maxHeapify(classifica_t* ,int ,int );

int main(void){
	int nodi,k;
	int i,cont,check,fine;
	long int**matrice;
	long int peso;
	catalogo_t*v;
	coda_t*coda;
	classifica_t*classificati;
	long int max;
	int ultimo;
	char stringa[14];
	
	if(scanf("%d %d",&nodi,&k)==EOF)
		exit(-1);
	
	//Creazione
	if((matrice=malloc(nodi*sizeof(long int*)))){					//crea matrice adiacenza
		for(i=0;i<nodi;i++){
			if((matrice[i]=malloc(nodi*sizeof(long int)))){
			}else printf("errore creazione matrice\n");
		}
	}else printf("errore creazione matrice\n");
	
	if(!(v=malloc(nodi*sizeof(catalogo_t))))		//crea catalogo
		printf("errore creazione catalogo\n");
	
	if(!(coda=malloc(nodi*sizeof(coda_t))))		//crea coda
		printf("errore creazione coda\n");
	
	if(!(classificati=malloc(k*sizeof(classifica_t))))	//crea classifica
		printf("errore creazione classifica\n");
	max=0;
	ultimo=-1;
	
	//Inizio lettura
	cont=-1;
	fine=scansiona(stringa);
	while(!fine){
		if(stringa[0]=='A'){
			cont++;
			check=creaGrafo(nodi,matrice);
			if(!check)
				peso=dijkstra(v,matrice,coda,nodi);
			else
				peso=(nodi-1)*matrice[0][1];
			if(peso<max||ultimo<(k-1)){
				if(ultimo<(k-1)){
					ultimo++;
					max=inserisci(classificati,ultimo,cont,peso);
				}else
					max=cambioalvertice(classificati,cont,peso,ultimo);
			}
		}else if(stringa[0]=='T'){
					if(ultimo!=-1){
						printf("%d",classificati[0].grafo);
						for(i=1;i<=ultimo;i++){
							printf(" ");
							printf("%d",classificati[i].grafo);
						}
					}
					printf("\n");
				}		
		fine=scansiona(stringa);
	}
	
	if(stringa[0]=='T'){
		if(ultimo!=-1){
			printf("%d",classificati[0].grafo);
			for(i=1;i<=ultimo;i++){
				printf(" ");
				printf("%d",classificati[i].grafo);
			}
		}
		printf("\n");
	}
	
	for(i=0;i<nodi;i++)
		free(matrice[i]);
	free(matrice);
	free(v);
	free(coda);
	free(classificati);
	
	return 0;
}

int scansiona(char stringa[14]){
	
	char supporto;
	int i,fine;
	
	i=0;
	fine=0;
	supporto=getchar_unlocked();
	while(supporto!='\n'&&!fine){
		stringa[i]=supporto;
		i++;
		if(supporto==EOF)
			fine=1;
		supporto=getchar_unlocked();
	}
	stringa[i]='\0';
	return fine;
}

int creaGrafo(int n,long int**matrice){				
	
	int i,j,check;
	long int num,num1;
	
	getchar_unlocked();
	
	check=1;
	
	num=velocizza(matrice,0,0);
	num=velocizza(matrice,0,1);
	for(j=2;j<n;j++){
		num1=velocizza(matrice,0,j);
		if(num!=num1&&check)
			check=0;
	}
	
	for(i=1;i<n;i++)
		for(j=0;j<n;j++){
			num1=velocizza(matrice,i,j);
			if(num!=num1&&i!=j&&check)
				check=0;
		}
	return check;
}

long int velocizza(long int**matrice,int i,int j){
	
	char supporto;
	long int peso;
	
	peso=0;
	supporto=getchar_unlocked();
	while(supporto!=','&&supporto!='\n'){
		peso=peso*10+(supporto-'0');
		supporto=getchar_unlocked();
	}
	matrice[i][j]=peso;
	return peso;
}

long int dijkstra(catalogo_t*v,long int**matrice,coda_t*coda,int n){
	
	int i,ok,u;
	int heapsize;
	long int sommacammini;
	
	ok=0;
	for(i=1;i<n&&!ok;i++){				//check per vedere se il nodo sorgente non e' isolato
		if(matrice[0][i])
			ok=1;
	}
	if(!ok)
		return 0;
	
	v[0].distanza=0;
	accoda(coda,v,0);			//nodo sorgente
	
	for(i=1;i<n;i++){				//pongo tutte le distanze ad inf e pred. a NULL
		v[i].distanza=-1;
		accoda(coda,v,i);
	}//fine inizializzazione
	
	sommacammini=0;
	
	for(heapsize=i-1;heapsize>=0;heapsize--){		//i-1 senno' sfora la coda
		u=cancellaMin(heapsize,coda,v);
		if(v[u].distanza!=-1)							//le distanze dei nodi non raggiungibili da 0 sono considerate nulle
			sommacammini=sommacammini+v[u].distanza;
		for(i=0;i<n;i++)
			if(v[u].distanza!=-1&&matrice[u][i]&&i!=u)
				if((matrice[u][i]+v[u].distanza)<v[i].distanza||v[i].distanza==-1){
					v[i].distanza=matrice[u][i]+v[u].distanza;
					coda[v[i].posincoda].precedente=u;
					coda[v[i].posincoda].costo=v[i].distanza;
					decrementaPri(coda,v[i].posincoda,heapsize,v);		//NON SU 0 MA SUL NODO v[i]
				}									
	}
	
	return sommacammini;
}

void decrementaPri(coda_t*coda,int n,int heapsize,catalogo_t*v){
	
	int l,r,p,posmin;
	coda_t tmp;
	
	p=(n-1)/2;
	
	if((p>=0&&(coda[p].costo>coda[n].costo&&coda[n].costo!=-1))||(coda[p].costo==-1&&coda[n].costo!=-1)){
		tmp=coda[n];
		
		coda[n]=coda[p];
		v[coda[n].num].posincoda=n;
		
		coda[p]=tmp;
		v[coda[p].num].posincoda=p;
		
		sali(coda,p,v);
		
	}else{
		l=2*n+1;
		r=2*n+2;
	
		if(l<=heapsize&&((coda[l].costo<coda[n].costo&&(coda[l].costo!=-1&&coda[n].costo!=-1))||(coda[n].costo==-1&&coda[l].costo!=-1)))
			posmin=l;
		else 
			posmin=n;
	
		if(r<=heapsize&&((coda[r].costo<coda[posmin].costo&&(coda[r].costo!=-1&&coda[posmin].costo!=-1))||(coda[posmin].costo==-1&&coda[r].costo!=-1)))
			posmin=r;
		if(posmin!=n){
			tmp=coda[n];
		
			coda[n]=coda[posmin];
			v[coda[n].num].posincoda=n;
		
			coda[posmin]=tmp;
			v[coda[posmin].num].posincoda=posmin;
		
			minHeapify(coda,posmin,heapsize,v);
		}
	}
}

void sali(coda_t*coda,int n,catalogo_t*v){
	
	int p;
	coda_t tmp;
	
	p=(n-1)/2;
	
	if((p>=0&&(coda[p].costo>coda[n].costo&&coda[n].costo!=-1))||(coda[p].costo==-1&&coda[n].costo!=-1)){
		tmp=coda[n];
		
		coda[n]=coda[p];
		v[coda[n].num].posincoda=n;
		
		coda[p]=tmp;
		v[coda[p].num].posincoda=p;
		
		sali(coda,p,v);
	}
	
}

void accoda(coda_t*coda,catalogo_t*v,int n){
	
	int heapsize;
	
	heapsize=n;
	coda[heapsize].num=n;
	coda[heapsize].precedente=0;
	coda[heapsize].costo=v[n].distanza;
	v[n].posincoda=heapsize;
	
}

int cancellaMin(int heapsize,coda_t*coda,catalogo_t*v){
	
	int tmp1;
	
	if(!heapsize)
		return coda[0].num;
	
	tmp1=coda[0].num;			
	
	coda[0]=coda[heapsize];
	v[coda[0].num].posincoda=0;

	heapsize--;
	minHeapify(coda,0,heapsize,v);
	
	return tmp1;
}

void minHeapify(coda_t*coda,int n,int heapsize,catalogo_t*v){
	
	int l,r,posmin;
	coda_t tmp;
	
	l=2*n+1;
	r=2*n+2;
	
	if(l<=heapsize&&((coda[l].costo<coda[n].costo&&(coda[l].costo!=-1&&coda[n].costo!=-1))||(coda[n].costo==-1&&coda[l].costo!=-1)))
		posmin=l;
	else 
		posmin=n;
	
	if(r<=heapsize&&((coda[r].costo<coda[posmin].costo&&(coda[r].costo!=-1&&coda[posmin].costo!=-1))||(coda[posmin].costo==-1&&coda[r].costo!=-1)))
		posmin=r;
	if(posmin!=n){
		tmp=coda[n];
		
		coda[n]=coda[posmin];
		v[coda[n].num].posincoda=n;
		
		coda[posmin]=tmp;
		v[coda[posmin].num].posincoda=posmin;
		
		minHeapify(coda,posmin,heapsize,v);
		
	}
}

long int inserisci(classifica_t*classificati,int ultimo,int cont,long int peso){
	
	int heapsize;
	int i;
	classifica_t tmp;
	
	heapsize=ultimo;
	classificati[heapsize].grafo=cont;
	classificati[heapsize].punteggio=peso;
	
	i=heapsize;

	while(i>0&&classificati[(i-1)/2].punteggio<=classificati[i].punteggio){
		tmp=classificati[(i-1)/2];
			
		classificati[(i-1)/2]=classificati[i];
			
		classificati[i]=tmp;
		
		i=(i-1)/2;
	}
	return classificati[0].punteggio;
}

long int cambioalvertice(classifica_t*classificati,int cont,long int peso,int ultimo){
	
	classificati[0].grafo=cont;
	classificati[0].punteggio=peso;

	maxHeapify(classificati,0,ultimo);
	
	return classificati[0].punteggio;
}

void maxHeapify(classifica_t*classificati,int n,int heapsize){
	
	int l,r,posmax;
	classifica_t tmp;
	
	l=2*n+1;
	r=2*n+2;
	
	if(l<=heapsize&&classificati[l].punteggio>=classificati[n].punteggio){
		if(classificati[l].punteggio==classificati[n].punteggio){
			if(classificati[l].grafo>classificati[n].grafo){
				posmax=l;
			}else 
				posmax=n;
		}
		posmax=l;
	}else
		posmax=n;		
			
	if(r<=heapsize&&classificati[r].punteggio>=classificati[posmax].punteggio){
		if(classificati[r].punteggio==classificati[posmax].punteggio){
			if(classificati[r].grafo>classificati[posmax].grafo)
				posmax=r;
		}else
			posmax=r;
	}
	
	if(posmax!=n){
		tmp=classificati[n];
		
		classificati[n]=classificati[posmax];
		
		classificati[posmax]=tmp;
		
		maxHeapify(classificati,posmax,heapsize);
	}
}