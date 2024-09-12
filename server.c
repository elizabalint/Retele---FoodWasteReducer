#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>

/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;
static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);
typedef struct  {
    int zi;
    int luna;
    int an;
}Data;
typedef struct Aliment{
    char nume[100];
    int cantitate;
    Data expirare;
    char magazinadd[100];
    } Aliment;
typedef struct Magazin{
    char nume[100];
    char locatie[100];
 } Magazin; 
 typedef struct Restaurant{
    char nume[100];
    char locatie[100];
 } Restaurant;
 typedef struct Organizatie{
    char nume[100];
 } Organizatie;
 typedef struct Persoana{
   char nume [100];
 } Persoana;

int AdaugaAliment(Aliment *alimentadd, char *numeMag) {
    FILE *file = fopen("Alimente.txt", "a");
    if (file == NULL) 
    {
        printf("Nu s-a putut deschide fisierul.\n");
        return errno;
    }
        // adauga aliment nou la sfarsit
    fseek(file, 0, SEEK_END);
    fprintf(file, "%s %d %02d/%02d/%04d %s\n", alimentadd->nume, alimentadd->cantitate,
            alimentadd->expirare.zi, alimentadd->expirare.luna, alimentadd->expirare.an,
            alimentadd->magazinadd);
    fclose(file);
    return 0;
}
int StergeAliment(Aliment *aliments, char *numeMag, char *aliment_de_sters, int cantitate, int zi, int luna, int an) 
{
    FILE *file = fopen("Alimente.txt", "r");
    if (file == NULL) 
    {
        printf("Nu s-a putut deschide fisierul.\n");
        return errno;
    }
    int ok =0 ;
    char vizualizare[10000] = "";
    Aliment aliment;
    while (fscanf(file, "%s %d %02d/%02d/%04d %s", aliment.nume, &aliment.cantitate,
                  &aliment.expirare.zi, &aliment.expirare.luna, &aliment.expirare.an, aliment.magazinadd) != EOF) 
    {
        if (strcmp(aliment.nume, aliment_de_sters) == 0 && aliment.cantitate == cantitate &&
            strcmp(aliment.magazinadd, numeMag) == 0 && aliment.expirare.zi == zi && aliment.expirare.luna == luna
            && aliment.expirare.an == an) 
            {
            ok = 1; // s-a sters un element
            continue;
            }
    char buffer[250];
    sprintf(buffer, "%s %d %02d/%02d/%04d %s\n", aliment.nume, aliment.cantitate,
            aliment.expirare.zi, aliment.expirare.luna, aliment.expirare.an, aliment.magazinadd);
    strcat(vizualizare, buffer);
    }
    fclose(file);

    FILE *file1 = fopen("Alimente.txt", "w");
    if (file1 == NULL) 
    {
        printf("Nu s-a putut deschide fisierul.\n");
        return errno;
    }
    fprintf(file1, "%s", vizualizare);
    fclose(file1);
    if(ok == 1)
        return 1; //adica element sters
    else
        return 0; //nu s-a sters nimic

}
int Donatie(Aliment *a, char *cine)//cine e pers pt care se doneaza
{
    FILE *persoane = fopen("Persoane.txt", "r");  // deschide in mod citire
    FILE *organizatii = fopen("Organizatii.txt", "r");  // deschide in mod citire
    if (persoane == NULL || organizatii == NULL) 
    {
        printf("Nu s-a putut deschide unul dintre fisierul.\n");
        return errno;
    } 
    int ok;
    char pp[100];
    while ((fscanf(persoane, "%s", pp)) != EOF)
      if(strcmp(pp,cine)==0)
      {        
          char nume_fisier[20];
          strcpy(nume_fisier, cine);
          strcat(nume_fisier, ".txt");
          FILE *file = fopen(nume_fisier, "a"); 
          if (file == NULL) 
          {
              printf("Nu s-a putut deschide fisierul %s\n", nume_fisier);
              return errno;
          }
          fprintf(file, "%s %d %02d/%02d/%04d %s\n",a->nume, a->cantitate,a->expirare.zi,a->expirare.luna, a->expirare.an,a->magazinadd );
          fprintf(file, "\n");
          fclose(file);
          fclose(persoane);  
          ok= 1;
      }
    char oo[100];
    while ((fscanf(organizatii, "%s", oo)) != EOF)
      if(strcmp(oo,cine)==0)
      {      
          char nume_fisier[20];
          strcpy(nume_fisier, cine);
          strcat(nume_fisier, ".txt");
          FILE *file = fopen(nume_fisier, "a"); 
          if (file == NULL) 
          {
              printf("Nu s-a putut deschide fisierul %s\n", nume_fisier);
              return errno;
          }
          fprintf(file, "%s %d %02d/%02d/%04d %s\n",a->nume, a->cantitate,a->expirare.zi,a->expirare.luna, a->expirare.an,a->magazinadd );
          fprintf(file, "\n");
          fclose(file);
          fclose(organizatii);  
          ok= 1;
      }
    if(ok==1)
        return 1;
    else
    {
        fclose(persoane);  
        fclose(organizatii);
        return 0;
    }
}
int VerifPers(char *numeVerif)
{
  FILE *file = fopen("Persoane.txt", "r");
  if (file == NULL) 
  {
      printf("Nu s-a putut deschide fisierul.\n");
  }
  Persoana pers;
  while (fscanf(file, "%s", pers.nume) != EOF) 
  {
      if(strcmp(pers.nume,numeVerif)==0)
      {
        fclose(file);
        return 1;
      }
  }
  fclose(file);
  return 0;
}
int VerifOrg(char *numeVerif)
{
  FILE *file = fopen("Organizatii.txt", "r");
  if (file == NULL) 
  {
      printf("Nu s-a putut deschide fisierul.\n");
  }
  Organizatie org;
  while (fscanf(file, "%s", org.nume) != EOF) 
  {
      if(strcmp(org.nume,numeVerif)==0)
      {
        fclose(file);
        return 1;
      }
  }
  fclose(file);
  return 0;
}
int Inregistrare(char *username, char *parola)
{
    FILE *file = fopen("Clienti.txt", "a");
    if (file == NULL) 
    {
        printf("Nu s-a putut deschide fisierul.\n");
        return errno;
    }
    fseek(file, 0, SEEK_END);
    fprintf(file,"%s %s\n", username, parola);
    fclose(file);
}
int Autentificare(char *username, char *parola)
{
  FILE *file = fopen("Clienti.txt", "r");
  if (file == NULL) 
  {
      printf("Nu s-a putut deschide fisierul.\n");
      return errno;
  }
  char usernamefis[100],parolafis[100], numefis[100];
  while(fscanf(file,"%s %s",usernamefis, parolafis) != EOF)
    if(strcmp(username,usernamefis)==0 && strcmp(parola,parolafis)==0) 
      {
        fclose(file);
        return 1;
      }

  fclose(file);
  return 0;
}
int VerificareUsername(char *username, char *parola)
{
  FILE *file = fopen("Clienti.txt", "r");
  if (file == NULL) 
  {
      printf("Nu s-a putut deschide fisierul.\n");
      return errno;
  }
  char usernamefis[100],parolafis[100];
  while(fscanf(file,"%s %s",usernamefis, parolafis) != EOF)
    if(strcmp(username,usernamefis)==0) 
      {
        fclose(file);
        return 1;
      }

  fclose(file);
  return 0;
}

int GolireFisier()
{
  FILE *file = fopen("Clienti.txt", "w");
  if (file == NULL) 
  {
        printf("Nu s-a putut deschide fisierul.\n");
        return errno; 
  }
    fclose(file);
    return 0;
}

char *lista_alimente;
void Vizualizare_Alimente()
{
  FILE *file = fopen("Alimente.txt", "r");
  if (file == NULL) 
  {
     printf("Nu s-a putut deschide fisierul.\n");
  }
  lista_alimente = (char *)malloc(1000000);
  char vizualizare[10000] = "";
  Aliment aliment;
  while (fscanf(file, "%s %d %02d/%02d/%04d %s", aliment.nume, &aliment.cantitate,
         &aliment.expirare.zi, &aliment.expirare.luna, &aliment.expirare.an, aliment.magazinadd) != EOF) 
  {
      char buffer[250]; 
      sprintf(buffer, "%s %d %02d/%02d/%04d %s\n", aliment.nume, aliment.cantitate,
              aliment.expirare.zi, aliment.expirare.luna, aliment.expirare.an, aliment.magazinadd);
      strcat(vizualizare, buffer);
  }
  //printf("%s\n",vizualizare);
  strcpy(lista_alimente,vizualizare);
  fclose(file);
}
char *lista_organizatii;
void Vizualizare_Organizatii()
{
  FILE *file = fopen("Organizatii.txt", "r");
  if (file == NULL) 
  {
      printf("Nu s-a putut deschide fisierul.\n");
  }
  lista_organizatii = (char *)malloc(1000000);
  char vizualizareo[10000] = "";
  Organizatie org;
  while (fscanf(file, "%s", org.nume) != EOF) 
  {
      char buffer[250]; 
      sprintf(buffer, "%s\n", org.nume);
      strcat(vizualizareo, buffer);
  }
  printf("%s\n",vizualizareo);
  strcpy(lista_organizatii, vizualizareo);
  fclose(file);
}
char *lista_persoane;
void Vizualizare_Persoane()
{
  FILE *file = fopen("Persoane.txt", "r");
  if (file == NULL) 
  {
      printf("Nu s-a putut deschide fisierul.\n");
  }
  lista_persoane = (char *)malloc(1000000);
  char vizualizarep[10000] = "";
  Persoana pers;
  while (fscanf(file, "%s", pers.nume) != EOF) 
  {
      char buffer[250]; 
      sprintf(buffer, "%s\n", pers.nume);
      strcat(vizualizarep, buffer);
  }
  //printf("%s\n",vizualizarep);
  strcpy(lista_persoane, vizualizarep);
  //printf("%s", lista_persoane);
  fclose(file);
}
char *lista_produse_primite;
int Vizualizare_Produse_Primite(char *nume)
{  
  char nume_fisier[20];
  strcpy(nume_fisier, nume);
  strcat(nume_fisier, ".txt");
  FILE *file = fopen(nume_fisier, "r"); 
  if (file == NULL) 
  {
      printf("Nu s-a putut deschide fisierul %s\n", nume_fisier);
      return 0;
  }
    lista_produse_primite = (char *)malloc(1000000);
  char vizualizarepr[10000] = "";
  Aliment aliment;
while (fscanf(file, "%s %d %02d/%02d/%04d %s", aliment.nume, &aliment.cantitate,
         &aliment.expirare.zi, &aliment.expirare.luna, &aliment.expirare.an, aliment.magazinadd) != EOF) 
  {
      char buffer[250]; 
      sprintf(buffer, "%s %d %02d/%02d/%04d %s\n", aliment.nume, aliment.cantitate,
              aliment.expirare.zi, aliment.expirare.luna, aliment.expirare.an, aliment.magazinadd);
      strcat(vizualizarepr, buffer);
  }  strcpy(lista_produse_primite, vizualizarepr);
  //printf("%s", lista_persoane);
  //fclose(filecl);
  fclose(file);
  return 1;
}
int AdaugaOrganizatie(char *numeOrganizatie)
{
  FILE *file = fopen("Organizatii.txt", "a");
  if (file == NULL) 
  {
      printf("Nu s-a putut deschide fisierul.\n");
  }
  char usernamefis[100],parolafis[100];
  while(fscanf(file,"%s %s",usernamefis, parolafis) != EOF)
    if(strcmp(numeOrganizatie,usernamefis)==0) 
      {
        fclose(file);
        return 1;
      }
  FILE *file1 = fopen("Organizatii.txt", "a");
  fseek(file1, 0, SEEK_END);
  fprintf(file1,"%s\n",numeOrganizatie);
  fclose(file);
  fclose(file1);
  return 0;
}
int AdaugaPersoane(char *numePersoana)
{
  FILE *file = fopen("Persoane.txt", "r");
  if (file == NULL) 
  {
      printf("Nu s-a putut deschide fisierul.\n");
  }
  char usernamefis[100],parolafis[100];
  while(fscanf(file,"%s %s",usernamefis, parolafis) != EOF)
    if(strcmp(numePersoana,usernamefis)==0) 
      {
        fclose(file);
        return 1;
      }
  FILE *file1 = fopen("Persoane.txt", "a");
  fseek(file1, 0, SEEK_END);
  fprintf(file1,"%s\n",numePersoana);
  fclose(file);
  fclose(file1);
  return 0;
}

int IaProdus(Aliment *alimente, char *numeclient)
{
  //deschid fisierul cu alimente si extrag doar cantitatea necesara 
  //deschid fisierul cu numele persoanei si adaug alimentul
  FILE *file = fopen("Alimente.txt", "r");
    if (file == NULL) 
    {
        printf("Nu s-a putut deschide fisierul.\n");
        return errno;
    }
    int ok =1 ;
    int prod_luat =0;
    char vizualizare[10000] = "";
    Aliment aliment;
    while (fscanf(file, "%s %d %02d/%02d/%04d %s", aliment.nume, &aliment.cantitate,
                  &aliment.expirare.zi, &aliment.expirare.luna, &aliment.expirare.an, aliment.magazinadd) != EOF) 
    {
        if (strcmp(aliment.nume, alimente->nume) == 0 && aliment.expirare.zi == alimente->expirare.zi &&
            aliment.expirare.luna == alimente->expirare.luna
            && aliment.expirare.an == alimente->expirare.an && ok==1) 
            {
            int t = aliment.cantitate= aliment.cantitate - alimente->cantitate;
            if(t==0)
            { prod_luat = 1;
              continue;
            }
            else
            if(t<0)
                aliment.cantitate= aliment.cantitate + alimente->cantitate;
            else
            {
              prod_luat = 1;
              ok = 0;}
            }
    char buffer[250];
    sprintf(buffer, "%s %d %02d/%02d/%04d %s\n", aliment.nume, aliment.cantitate,
            aliment.expirare.zi, aliment.expirare.luna, aliment.expirare.an, aliment.magazinadd);
    strcat(vizualizare, buffer);
    }
    fclose(file);
    FILE *file1 = fopen("Alimente.txt", "w");
    if (file1 == NULL) 
    {
        printf("Nu s-a putut deschide fisierul.\n");
        return errno;
    }
    fprintf(file1, "%s", vizualizare);
    fclose(file1);
    char nume_fisier[20];
    strcpy(nume_fisier, numeclient);
    strcat(nume_fisier, ".txt");
    FILE *file2 = fopen(nume_fisier, "a"); 
    if (file2 == NULL) 
    {
        printf("Nu s-a putut deschide fisierul %s\n", nume_fisier);
        return errno;
    }
    if(prod_luat ==1)
          fprintf(file2, "%s %d %02d/%02d/%04d %s\n",alimente->nume, alimente->cantitate,alimente->expirare.zi,alimente->expirare.luna, alimente->expirare.an,aliment.magazinadd );
    fprintf(file2, "\n");
    fclose(file2);
    if(prod_luat == 1)
        return 1;
    else
        return 0;
}

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int nr;		//mesajul primit de trimis la client 
  int sd;		//descriptorul de socket 
  int pid;
  pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
	int i=0;

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  /** se deschide un socket si se obtine descriptorul acestuia**/
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
    server.sin_family = AF_INET;/* stabilirea familiei de socket-uri */
    server.sin_addr.s_addr = htonl (INADDR_ANY);/* acceptam orice adresa */
    server.sin_port = htons (PORT);/* utilizam un port utilizator */
  
  /* atasam socketul */
  /** se leaga socketul la adresa si portul specificate**/
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
    {
      int client; /** stoca descriptorul noului client care se conectează.**/
      thData * td; //parametru functia executata de thread     
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      /** Forțează golirea bufferului de ieșire, astfel încât mesajul să fie afișat imediat.**/
      fflush (stdout);

      // client= malloc(sizeof(int));
      /* acceptam un client (stare blocanta pana la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
      {
        perror ("[server]Eroare la accept().\n");
        continue;
      }
      
    /* s-a realizat conexiunea, se astepta mesajul */
    
	int idThread; //id-ul threadului
	int cl; //descriptorul intors de accept

	td=(struct thData*)malloc(sizeof(struct thData));	
	td->idThread=i++; /**Se atribuie un id unic thread-ului în structura thData.**/
	td->cl=client; /**Se atribuie descriptorul clientului în structura thData.**/
    /**Se creează un nou thread pentru a trata conexiunea utilizând funcția treat 
    și se transmite structura thData pentru a furniza informații despre client.**/
	pthread_create(&th[i], NULL, &treat, td);	      
				
	}//while    
};				
static void *treat(void * arg)
{		
		struct thData tdL; /**tld este var de tip thdata**/
		tdL= *((struct thData*)arg);/**copie in tdl a struct thdata**/
		printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());		
		raspunde((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		
};


void raspunde(void *arg)
{
  int  i=0;
	struct thData tdL; 
	tdL= *((struct thData*)arg);
	char access[100], username[100], parola[10], tip[100], comanda[100], nume[100];
	while(1)
  {
    if(read(tdL.cl, &comanda, sizeof(comanda)) <=0)
    {
        printf("[Thread %d]\n",tdL.idThread);
        perror ("Eroare la citire comanda de la client.\n");
    }
    int pt_add = 0;

    if(strcmp(comanda,"Inregistrare")==0)
    {
        printf("Clientul %d doreste sa se inregistreze\n",tdL.idThread);
        if(read(tdL.cl, &username, sizeof(username)) <=0)
        {
          printf("[Thread %d]\n",tdL.idThread);
          perror ("Eroare la citire username de la client.\n");
        }
        printf("[Thread %d]Clientul are username-ul %s\n",tdL.idThread, username);

        if(strlen(username)==0)
        {
          break;
        }
        if(read(tdL.cl, &parola, sizeof(parola)) <=0)
        {
          printf("[Thread %d]\n",tdL.idThread);
          perror ("Eroare la citire parola de la client.\n");
        }
        printf("[Thread %d]Clientul are parola %s\n",tdL.idThread, parola);
        if(VerificareUsername(username, parola) ==1)
        {   
            char msj[100];
            bzero(msj,sizeof(msj));
            strcpy(msj,"Acest username este deja folosit. Incercati altul");
            if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
            {
              printf("[Thread %d]\n",tdL.idThread);
              perror ("Eroare la write() de la client.\n");
            }

            printf("[Thread %d]Clientul este deja inregistrat\n",tdL.idThread);
        }
        else
            { 
              Inregistrare(username,parola);
              char msj[100];
              bzero(msj,sizeof(msj));
              strcpy(msj,"Inregistrare cu succes");
              if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
              {
                printf("[Thread %d]\n",tdL.idThread);
                perror ("Eroare la write() de la client.\n");
              }
              printf("[Thread %d]Clientul s-a inregistrat cu succes\n",tdL.idThread);
            }
    }
    else
    if(strcmp(comanda,"Autentificare")==0)
    {
      printf("Clientul %d doreste sa se autentifice\n",tdL.idThread);
      if(read(tdL.cl, &username, sizeof(username)) <=0)
      {
          printf("[Thread %d]\n",tdL.idThread);
          perror ("Eroare la citire username de la client.\n");
      }
      if(strlen(username)==0)
        {
          break;
        }
      if(read(tdL.cl, &parola, sizeof(parola)) <=0)
      {
          printf("[Thread %d]\n",tdL.idThread);
          perror ("Eroare la citire parola de la client.\n");
      }
        printf("[Thread %d]Clientul are username-ul %s\n",tdL.idThread, username);
        printf("[Thread %d]Clientul are parola %s\n",tdL.idThread, parola);

      int ok1 = Autentificare(username,parola);
      if (ok1 == 1)//s-a autentificat, poate fol specificatiile
      {   
        char msj[100];
        strcpy(msj,"da");
        if (write (tdL.cl, &msj,sizeof(msj)) <= 0)//ii trimite da inainte de if(strcmp(msj,"da"))
        {
          printf("[Thread %d]\n",tdL.idThread);
          perror ("Eroare la write() de la client.\n");
        }
        char categorie1 [100];
        bzero(categorie1,sizeof(categorie1));
        if(read(tdL.cl, &categorie1, sizeof(categorie1)) <=0)//citeste rasp clientului dupa autentificare -> ar trebui sa returneze magazin/restaurant...
        {
          printf("[Thread %d]\n",tdL.idThread);
          perror ("Eroare la citire comanda de la client.\n");
        } 

        char categorie [100];
        bzero(categorie,sizeof(categorie));
        if(read(tdL.cl, &categorie, sizeof(categorie)) <=0)//citeste rasp clientului dupa autentificare -> ar trebui sa returneze magazin/restaurant...
        {
          printf("[Thread %d]\n",tdL.idThread);
          perror ("Eroare la citire comanda de la client.\n");
        }  
        if(strlen(categorie)==0)
        {
          break;
        }
        printf("Clientul face parte din categoria %s\n", categorie);
        
        if(strcmp(categorie, "Organizatie")==0 )
        {
          AdaugaOrganizatie(username);
        }
        if(strcmp(categorie, "Persoana")==0)
        {
          AdaugaPersoane(username);
        }

        while(1)
        {
        char meniu[100];
        bzero(meniu, sizeof(meniu));
        if(read(tdL.cl, &meniu, sizeof(meniu)) <=0)
        {
          printf("[Thread %d]\n",tdL.idThread);
          perror ("Eroare la citire comanda de la client.\n");
        }
        printf("[Thread %d]Clientul a ales sa %s\n",tdL.idThread, meniu);
        
        if(strcmp(meniu,"Adaugati_Produse") == 0)
        {
            char alimentNou[100];
            bzero(alimentNou, sizeof(alimentNou));
            if(read(tdL.cl, &alimentNou, sizeof(alimentNou)) <=0)//citeste numele restaurant/asoc/mag...
            {
              printf("[Thread %d]\n",tdL.idThread);
              perror ("Eroare la citire comanda de la client.\n");
            }
            printf("[Thread %d]Clientul a adaugat %s\n",tdL.idThread, alimentNou);
            int cantitate;
            bzero(&cantitate, sizeof(cantitate));
            if(read(tdL.cl, &cantitate, sizeof(cantitate)) <=0)
            {
              printf("[Thread %d]\n",tdL.idThread);
              perror ("Eroare la citire comanda de la client.\n");
            }
            printf("[Thread %d]Clientul a adaugat o cantitate de %o\n",tdL.idThread, cantitate);
            char data_expirare[100];
            bzero(data_expirare, sizeof(data_expirare));
            if(read(tdL.cl, &data_expirare, sizeof(data_expirare)) <=0)
            {
              printf("[Thread %d]\n",tdL.idThread);
              perror ("Eroare la citire comanda de la client.\n");
            }
            printf("[Thread %d]Clientul a adaugat data de expirare ca fiind %s pentru %s\n",tdL.idThread, data_expirare,alimentNou);

            Aliment aliment;
            strcpy(aliment.nume, alimentNou);
            aliment.cantitate = cantitate;
            sscanf(data_expirare, "%d.%d.%d", &aliment.expirare.zi, &aliment.expirare.luna, &aliment.expirare.an);
            strcpy(aliment.magazinadd,username);
            AdaugaAliment(&aliment,username);
            printf("[Thread %d] Produsul a fost adaugat cu secces\n",tdL.idThread);
            char msj[100];
            bzero(msj,sizeof(msj));
            strcpy(msj,"Prodului a fost adaugat cu succes\n");
            if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
            {
              printf("[Thread %d]\n",tdL.idThread);
              perror ("Eroare la write() de la client.\n");
            }

        }
        else
            if(strcmp(meniu,"Vizualizare_Organizatii") == 0)
            {
              Vizualizare_Organizatii();
              //printf("%s\n",lista_organizatii);
              char *msj = (char *)malloc(1000000);
                if(strlen(lista_organizatii)>0)
                {       strcpy(msj,lista_organizatii);
                }
                else strcpy(msj,"Nu exista organizatii in sistem");
              if (write (tdL.cl, lista_organizatii,1000000) <= 0)
              {
                printf("[Thread %d]\n",tdL.idThread);
                perror ("Eroare la write() de la client.\n");
              }
              free(msj);
              free(lista_organizatii);

            }
            else
              if(strcmp(meniu,"Vizualizare_Alimente") == 0)
              {
                Vizualizare_Alimente();
                char *msj = (char *)malloc(1000000);
                if(strlen(lista_alimente)>0)
                {       strcpy(msj,lista_alimente);
                        printf("%s\n", msj);
                }
                else strcpy(msj,"Nu exista elimente in sistem");
                if (write (tdL.cl, msj,1000000) <= 0)
                {
                  printf("[Thread %d]\n",tdL.idThread);
                  perror ("Eroare la write() de la client.\n");
                }
                free(msj);
                free(lista_alimente);
              }
              else
              if(strcmp(meniu,"Vizualizare_Persoane") == 0)
              {
                Vizualizare_Persoane();
                printf("%s\n",lista_persoane);
                char *msj = (char *)malloc(1000000);
                if(strlen(lista_persoane)>0)
                {       strcpy(msj,lista_persoane);
                        printf("%s\n", msj);
                }
                else strcpy(msj,"Nu exista persoane in sistem");
                if (write (tdL.cl, msj,1000000) <= 0)
                {
                  printf("[Thread %d]\n",tdL.idThread);
                  perror ("Eroare la write() de la client.\n");
                }
                free(msj);
                free(lista_persoane);
              }
              else
              if(strcmp(meniu,"Vizualizare_Produse_Primite") == 0)
              {
                //printf("%s\n",numeclient);
                int ok = Vizualizare_Produse_Primite(username);
                //printf("%s\n",numeclient);
                //printf("%s %ld\n",lista_produse_primite, sizeof(lista_produse_primite));
                char *msj = (char *)malloc(1000000);
                if(ok==0)
                    strcpy(msj,"Nu aveti niciun produs");
                else
                {       strcpy(msj,lista_produse_primite);
                        printf("%s\n", msj);
                }
                    if (write (tdL.cl, msj,1000000) <= 0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la write() de la client.\n");
                    }
                free(msj);
                free(lista_produse_primite);
              }
              else
                if(strcmp(meniu,"Sterge_Produse")==0)
                {
                    char alimentDeSters[100];
                    bzero(alimentDeSters, sizeof(alimentDeSters));
                    if(read(tdL.cl, &alimentDeSters, sizeof(alimentDeSters)) <=0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul a ales %s\n",tdL.idThread, alimentDeSters);
                    int cantitates;
                    bzero(&cantitates, sizeof(cantitates));
                    if(read(tdL.cl, &cantitates, sizeof(cantitates)) <=0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul a adaugat o cantitate de %d\n",tdL.idThread, cantitates);
                    char data_expirares[100];
                    bzero(data_expirares, sizeof(data_expirares));
                    if(read(tdL.cl, &data_expirares, sizeof(data_expirares)) <=0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul a adaugat data de expirare ca fiind %s pentru %s\n",tdL.idThread, data_expirares,alimentDeSters);

                    Aliment aliment;
                    strcpy(aliment.nume, alimentDeSters);
                    aliment.cantitate = cantitates;
                    sscanf(data_expirares, "%d.%d.%d", &aliment.expirare.zi, &aliment.expirare.luna, &aliment.expirare.an);
                    strcpy(aliment.magazinadd,username);
                    int ok = StergeAliment(&aliment,username,alimentDeSters,cantitates,aliment.expirare.zi, aliment.expirare.luna, aliment.expirare.an);
                    printf("%d   3456789", ok);
                    if(ok == 1) //
                    {
                      printf("[Thread %d]Produs sters cu succes\n",tdL.idThread);
                      char msj[100];
                      bzero(msj,sizeof(msj));
                      strcpy(msj,"Produs sters cu succes\n");
                      if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
                      {
                        printf("[Thread %d]\n",tdL.idThread);
                        perror ("Eroare la write() de la client.\n");
                      }
                    }
                    else
                    {
                      printf("[Thread %d]Incercati din nou\n",tdL.idThread);                     
                      char msj[100];
                      bzero(msj,sizeof(msj));
                      strcpy(msj,"Incercati din nou\n");
                      if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
                      {
                        printf("[Thread %d]\n",tdL.idThread);
                        perror ("Eroare la write() de la client.\n");
                      }
                    
                    }
                }
                else
                  if(strcmp(meniu,"Doneaza")==0)
                  {
                    char cine[100];
                    bzero(cine, sizeof(cine));
                    if(read(tdL.cl, &cine, sizeof(cine)) <=0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul clientul doneaza catre %s\n",tdL.idThread, cine);
                    char msj[100];
                      bzero(msj,sizeof(msj));
                      if(VerifPers(cine)==1 || VerifOrg(cine)==1)
                            strcpy(msj,"e ok\n");
                      else
                            strcpy(msj,"Persoana/Organizatia nu exista. Incercati din nou");
                      if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
                      {
                        printf("[Thread %d]\n",tdL.idThread);
                        perror ("Eroare la write() de la client.\n");
                      }
                    char donatieA[100];
                    bzero(donatieA, sizeof(donatieA));
                    if(read(tdL.cl, &donatieA, sizeof(donatieA)) <=0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul doneaza alimentul %s\n",tdL.idThread, donatieA);
                     int cantitate;
                    bzero(&cantitate, sizeof(cantitate));
                    if(read(tdL.cl, &cantitate, sizeof(cantitate)) <=0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul a adaugat o cantitate de %d\n",tdL.idThread, cantitate);
                    char data_expirare[100];
                    bzero(data_expirare, sizeof(data_expirare));
                    if(read(tdL.cl, &data_expirare, sizeof(data_expirare)) <=0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul a adaugat data de expirare ca fiind %s pentru %s\n",tdL.idThread, data_expirare,donatieA);

                    Aliment alimentD;
                    strcpy(alimentD.nume, donatieA);
                    alimentD.cantitate = cantitate;
                    sscanf(data_expirare, "%d.%d.%d", &alimentD.expirare.zi, &alimentD.expirare.luna, &alimentD.expirare.an);
                    strcpy(alimentD.magazinadd,username);
                    int ok = Donatie(&alimentD,cine);
                    char msj1[100];
                    bzero(msj1,sizeof(msj1));
                    strcpy(msj1,"Prodului a fost adaugat cu succes\n");
                    if (write (tdL.cl, &msj1,sizeof(msj1)) <= 0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la write() de la client.\n");
                    }
                    if(ok == 1) 
                    {
                      printf("[Thread %d]Ati donat cu succes\n",tdL.idThread);
                      char msj[100];
                      bzero(msj,sizeof(msj));
                      strcpy(msj,"Produs donat cu succes\n");
                      if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
                      {
                        printf("[Thread %d]\n",tdL.idThread);
                        perror ("Eroare la write() de la client.\n");
                      }
                    }
                    else
                    {
                      printf("[Thread %d]Incercati din nou o asociatie sau o persoana valida\n",tdL.idThread);                     
                      char msj[100];
                      bzero(msj,sizeof(msj));
                      strcpy(msj,"Incercati din nou o asociatie sau o persoana valiza\n");
                      if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
                      {
                        printf("[Thread %d]\n",tdL.idThread);
                        perror ("Eroare la write() de la client.\n");
                      }
                    
                    }
                  }
                  else
                  if(strcmp(meniu,"Ia_Produse")==0)
                  {
                    char alimentNou[100];
                    bzero(alimentNou, sizeof(alimentNou));
                    if(read(tdL.cl, &alimentNou, sizeof(alimentNou)) <=0)//citeste numele restaurant/asoc/mag...
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul a adaugat %s\n",tdL.idThread, alimentNou);
                    int cantitate;
                    bzero(&cantitate, sizeof(cantitate));
                    if(read(tdL.cl, &cantitate, sizeof(cantitate)) <=0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul a adaugat o cantitate de %o\n",tdL.idThread, cantitate);
                    char data_expirare[100];
                    bzero(data_expirare, sizeof(data_expirare));
                    if(read(tdL.cl, &data_expirare, sizeof(data_expirare)) <=0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la citire comanda de la client.\n");
                    }
                    printf("[Thread %d]Clientul a adaugat data de expirare ca fiind %s pentru %s\n",tdL.idThread, data_expirare,alimentNou);
                    Aliment aliment;
                    strcpy(aliment.nume, alimentNou);
                    aliment.cantitate = cantitate;
                    sscanf(data_expirare, "%d.%d.%d", &aliment.expirare.zi, &aliment.expirare.luna, &aliment.expirare.an);
                    strcpy(aliment.magazinadd,"magazin");
                    int ok = IaProdus(&aliment,username);
                    char msj[100];
                    bzero(msj,sizeof(msj));
                    if(ok == 1)
                    {
                        printf("[Thread %d] Produsul a fost adaugat cu secces\n",tdL.idThread);
                        strcpy(msj,"Prodului a fost adaugat cu succes\n");
                    }
                    else
                    {
                        strcpy(msj,"Prodului nu a putut fi adaugat. Incercati din nou\n");
                        printf("[Thread %d] Prodului nu a putut fi adaugat. Incercati din nou\n",tdL.idThread);
                    }
                    if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
                    {
                      printf("[Thread %d]\n",tdL.idThread);
                      perror ("Eroare la write() de la client.\n");
                    }
                  }
              else
              if(strcmp(meniu,"Iesire")==0)
                {
                  printf("[Thread %d]Clientul a iesit\n",tdL.idThread);
                  break;
                }


                else
                if(strlen(meniu)==0)
                  break;
        }

       }
       else
       {
          printf("[Thread %d]Clientul nu este autentificat\n",tdL.idThread);
          char msj[100];
          bzero(msj,sizeof(msj));
          strcpy(msj,"Nu v-ati putut autentifica cu aceste date. Incercati din nou\n");
          if (write (tdL.cl, &msj,sizeof(msj)) <= 0)
          {
            printf("[Thread %d]\n",tdL.idThread);
            perror ("Eroare la write() de la client.\n");
          }
       }
  } 
  else
      if(strcmp(comanda,"Iesire")==0)
      {
        printf("[Thread %d]Clientul a iesit\n",tdL.idThread);
        break;
      }
  }

}