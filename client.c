#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;
int validateDate(char *date) 
{
    // verificam dacs lungimea datei este corecta
    if (strlen(date) != 10) {
        return 0;
    }
    // verificam formatul DD.MM.YYYY
    for (int i = 0; i < 10; i++) 
    {
        if (i == 2 || i == 5) 
        {
            // verificam punctele
            if (date[i] != '.') 
            {
                return 0;
            }
        } else {
            // verificam cifrele
            if (!isdigit(date[i])) 
            {
                return 0;
            }
        }
    }

    return 1;
}
int validare_cantitate(int a)
{
    return 1;
}

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  // mesajul trimis
  int nr=0;
  char buf[10], comand[100], username[100], parola[100], meniu[100];
  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }
  /* stabilim portul */
  port = atoi (argv[2]);
  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }
  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  server.sin_family = AF_INET;/* familia socket-ului */
  server.sin_addr.s_addr = inet_addr(argv[1]);/* adresa IP a serverului */
  server.sin_port = htons (port);/* portul de conectare */
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
  int ok=0; 
  int ok1;
  int ig = 1;
  while(1)
  {
    printf("Alege o optiune:\n");
        printf("Autentificare\n");
        printf("Inregistrare\n");
        printf("Iesire\n");
    scanf("%s", comand);
    if(write(sd, &comand, sizeof(comand)) <= 0)
    {
        perror ("[client]Eroare la write() spre server.\n");
        return errno;
    }
    char nume[100];
    if(strcmp(comand,"Inregistrare")==0)
    {       
            printf("Username:");
            scanf("%s", username);
            printf("Parola:");
            scanf("%s", parola);
            if(write(sd, &username, sizeof(username)) <= 0)
            {
                    perror ("[client]Eroare la trimitere username spre server.\n");
                    return errno;
            }
            if(write(sd, &parola, sizeof(parola)) <= 0)
            {
                    perror ("[client]Eroare la trimitere parola spre server.\n");
                    return errno;
            }
            char msj[100];
            bzero(msj,sizeof(msj));
            if(read(sd,&msj,sizeof(msj))<=0)
            {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
            }
            printf("%s\n", msj);
            bzero(username, sizeof(username));
            bzero(parola, sizeof(parola));
    }
    else
     if(strcmp(comand,"Autentificare")==0)
    {
        printf("Username:");
        scanf("%s", username);
        printf("Parola:");
        scanf("%s", parola);
        if(write(sd, &username, sizeof(username)) <= 0)
        {
                perror ("[client]Eroare la trimitere username spre server.\n");
                return errno;
        }
        if(write(sd, &parola, sizeof(parola)) <= 0)
        {
                perror ("[client]Eroare la trimitere parola spre server.\n");
                return errno;
        }
        bzero(&ok, sizeof(ok));
        char msj[100];
        bzero(msj,sizeof(msj));

        if(read(sd,&msj,sizeof(msj))<=0)
        {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
        }
        if(strcmp(msj,"da"))//nu s-a autentificat ok
        {
            printf("%s\n", msj);
        }
        else //autentificarea e ok
        { 
          while(1)
            { 
              printf("\n");
              printf("Va rugam sa ne spuneti din ce categorie faceti parte:\n");
              printf("Magazin\n");
              printf("Restaurant\n");
              printf("Organizatie\n");
              printf("Persoana\n");
              char categorie[100];
              bzero(&categorie, sizeof(categorie));
              scanf("%s", categorie);
              char catverif[4][20];
              strcpy(catverif[0],"Magazin");
              strcpy(catverif[1],"Restaurant");
              strcpy(catverif[2],"Organizatie");
              strcpy(catverif[3],"Persoana");
              int gasitc = 0; 
              for (int i = 0; i < 4; ++i) 
              {
                if (strcmp(categorie, catverif[i]) == 0) 
                {
                    gasitc = 1;
                    break; 
                }
            }
            if(gasitc == 1)
            {
                if(write(sd, &categorie, sizeof(categorie)) <= 0)
                {
                    perror ("[client]Eroare la trimitere meniu spre server.\n");
                    return errno;
                }
            }
              if(strcmp(categorie,"Magazin")==0 || strcmp(categorie,"Restaurant")==0)
              {
                    
                    while(1)
                    {
                    printf("\n");
                    printf("Meniu:\n");
                    printf("Adaugati_Produse\n");
                    printf("Vizualizare_Organizatii\n");
                    printf("Vizualizare_Alimente\n");
                    printf("Vizualizare_Persoane\n");
                    printf("Sterge_Produse\n");
                    printf("Doneaza. Inainte de a alege aceasta optiune asigurati-va ca persoana/organizatia exista\n");
                    printf("Iesire\n");
                    bzero(meniu, sizeof(meniu));
                    fflush (stdout);
                    scanf("%s", meniu);
                    char meniuverif[8][30];
                    strcpy(meniuverif[0],"Adaugati_Produse");
                    strcpy(meniuverif[1],"Vizualizare_Organizatii");
                    strcpy(meniuverif[2],"Vizualizare_Alimente");
                    strcpy(meniuverif[3],"Vizualizare_Persoane");
                    strcpy(meniuverif[4],"Sterge_Produse");
                    strcpy(meniuverif[5],"Doneaza");
                    strcpy(meniuverif[6],"Iesire");
                    int gasit = 0; 
                    for (int i = 0; i < 7; ++i) 
                    {
                        if (strcmp(meniu, meniuverif[i]) == 0) 
                        {
                            gasit = 1;
                            break; 
                        }
                    }
                    if(gasit == 1)
                    {
                        if(write(sd, &meniu, sizeof(meniu)) <= 0)
                        {
                            perror ("[client]Eroare la trimitere meniu spre server.\n");
                            return errno;
                        }
                    }
                    if(strcmp(meniu,"Adaugati_Produse")==0)
                    {    int aici = 1, aici2 = 1, aici3 = 1;

                        do {
                            printf("Adaugati aliment:\n");
                            char aliment[100];
                            bzero(aliment, sizeof(aliment));
                            fflush(stdout);
                            scanf("%s", aliment);

                            int ok_aliment = 1;
                            for (int i = 0; aliment[i] != '\0'; i++) {
                                if (!isalpha(aliment[i])) {
                                    ok_aliment = 0;
                                    aici = 0;
                                    break;
                                }
                            }

                            if (ok_aliment == 0) 
                            {
                                printf("Nu puteti introduce cifre. Incercati din nou\n");
                            } else 
                            {
                                if (write(sd, &aliment, sizeof(aliment)) <= 0) 
                                {
                                    perror("[client]Eroare la trimitere categorie spre server.\n");
                                    return errno;
                                }

                                int cantitate;
                            do {
                                printf("Adaugati cantitate:\n");
                                bzero(&cantitate, sizeof(cantitate));
                                fflush(stdout);
                                if (scanf("%d", &cantitate) != 1) {
                                    printf("Nu ati introdus o cantitate valida. Incercati din nou.\n");
                                    aici2 = 1;
                                    while (getchar() != '\n');  
                                } else {
                                    aici2 = 0;
                                }
                            } while (aici2 == 1);

                            if (write(sd, &cantitate, sizeof(cantitate)) <= 0) {
                                perror("[client]Eroare la trimitere categorie spre server.\n");
                                return errno;
                            }

                            do {
                                printf("Adaugati data expirare in format: DD.MM.YYYY:\n");
                                char data_expirare[100];
                                bzero(data_expirare, sizeof(data_expirare));
                                fflush(stdout);
                                if (scanf("%s", data_expirare) != 1 || !validateDate(data_expirare)) {
                                    printf("Data introdusa nu este valida. Incercati din nou.\n");
                                    aici3 = 1;
                                    while (getchar() != '\n');  
                                } else {
                                    if (write(sd, &data_expirare, sizeof(data_expirare)) <= 0) {
                                        perror("[client]Eroare la trimitere categorie spre server.\n");
                                        return errno;
                                    }
                                    aici3 = 0; 
                                }
                            } while (aici3 == 1);
                            aici = 1; 
                        }
                    } while (aici == 0);
                            char msj[100];
                            bzero(msj,sizeof(msj));
                            if(read(sd,&msj,sizeof(msj))<=0)
                            {
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                            }
                            printf("%s", msj);
                    }
                    else
                        if(strcmp(meniu,"Vizualizare_Organizatii")==0)
                        {
                            char *organizatii = (char *)malloc(1000000);
                            memset(organizatii, 0, 1000000);
                            size_t totalCitit = 0;
                            //asteapta pana cand toti octetii pot fi cititi!!
                            while (totalCitit < 1000000) {
                                ssize_t citit = read(sd, organizatii + totalCitit, 1000000 - totalCitit);
                                if (citit <= 0) {
                                    perror("[client] Eroare la read() de la server.\n");
                                    return errno;
                                }
                                totalCitit += citit;
                            }
                            printf("%s\n", organizatii);
                            free(organizatii);
                        }
                    else
                          if (strcmp(meniu, "Vizualizare_Alimente") == 0)
                           {
                                char *alimente = (char *)malloc(1000000);
                                memset(alimente, 0, 1000000);
                                size_t totalCitit = 0;
                                while (totalCitit < 1000000) 
                                {
                                    ssize_t citit = read(sd, alimente + totalCitit, 1000000 - totalCitit);
                                    if (citit <= 0) {
                                        perror("[client] Eroare la read() de la server.\n");
                                        return errno;
                                    }
                                    totalCitit += citit;
                                }

                                printf("%s\n", alimente);
                                free(alimente);
                            }

                    else
                          if(strcmp(meniu,"Vizualizare_Persoane")==0)
                        {
                            char *pers = (char *)malloc(1000000);
                            memset(pers, 0, 1000000);
                            size_t totalCitit = 0;
                            //asteapta pana cand toti octetii pot fi cititi!!
                            while (totalCitit < 1000000) {
                                ssize_t citit = read(sd, pers + totalCitit, 1000000 - totalCitit);
                                if (citit <= 0) {
                                    perror("[client] Eroare la read() de la server.\n");
                                    return errno;
                                }
                                totalCitit += citit;
                            }

                                printf("%s\n", pers);
                                free(pers);
                        }
                    else
                            if(strcmp(meniu,"Sterge_Produse")==0)
                        {
                              int aici = 1, aici2 = 1, aici3 = 1;

                        do {
                            printf("Adaugati alimentul pe care doriti sa-l stergeti:\n");
                            char aliment[100];
                            bzero(aliment, sizeof(aliment));
                            fflush(stdout);
                            scanf("%s", aliment);

                            int ok_aliment = 1;
                            for (int i = 0; aliment[i] != '\0'; i++) {
                                if (!isalpha(aliment[i])) {
                                    ok_aliment = 0;
                                    aici = 0;
                                    break;
                                }
                            }

                            if (ok_aliment == 0) {
                                printf("Nu puteti introduce cifre. Incercati din nou\n");
                            } else {
                                if (write(sd, &aliment, sizeof(aliment)) <= 0) {
                                    perror("[client]Eroare la trimitere categorie spre server.\n");
                                    return errno;
                                }

                                int cantitate;
                            do {
                                printf("Adaugati cantitate:\n");
                                bzero(&cantitate, sizeof(cantitate));
                                fflush(stdout);
                                if (scanf("%d", &cantitate) != 1) {
                                    printf("Nu ati introdus o cantitate valida. Incercati din nou.\n");
                                    aici2 = 1;
                                    while (getchar() != '\n');  
                                } else {
                                    aici2 = 0;
                                }
                            } while (aici2 == 1);

                            if (write(sd, &cantitate, sizeof(cantitate)) <= 0) {
                                perror("[client]Eroare la trimitere categorie spre server.\n");
                                return errno;
                            }

                            do {
                                printf("Adaugati data expirare in format: DD.MM.YYYY:\n");
                                char data_expirare[100];
                                bzero(data_expirare, sizeof(data_expirare));
                                fflush(stdout);
                                if (scanf("%s", data_expirare) != 1 || !validateDate(data_expirare)) {
                                    printf("Data introdusa nu este valida. Incercati din nou.\n");
                                    aici3 = 1;
                                    while (getchar() != '\n');  
                                } else {
                                    if (write(sd, &data_expirare, sizeof(data_expirare)) <= 0) {
                                        perror("[client]Eroare la trimitere categorie spre server.\n");
                                        return errno;
                                    }
                                    aici3 = 0; 
                                }
                            } while (aici3 == 1);
                            aici = 1; 
                        }
                    } while (aici == 0);
                            char msj[100];
                            bzero(msj,sizeof(msj));
                            if(read(sd,&msj,sizeof(msj))<=0)
                            {
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                            }
                            printf("%s", msj);
                        }
                    else
                            if(strcmp(meniu,"Doneaza")==0)
                            {
                              printf("Alegeti persoana/organizatia pentru care donati:\n");
                              char cine[100];
                              bzero(cine, sizeof(cine));
                              fflush (stdout);
                              scanf("%s", cine);
                              if(write(sd, &cine, sizeof(cine)) <= 0)
                              {
                                  perror ("[client]Eroare la trimitere categorie spre server.\n");
                                  return errno;
                              }
                              
                              int aici = 1, aici2 = 1, aici3 = 1;

                        do {
                            printf("Adaugati alimentulm pe care doriti sa-l donati:\n");
                            char aliment[100];
                            bzero(aliment, sizeof(aliment));
                            fflush(stdout);
                            scanf("%s", aliment);

                            int ok_aliment = 1;
                            for (int i = 0; aliment[i] != '\0'; i++) {
                                if (!isalpha(aliment[i])) {
                                    ok_aliment = 0;
                                    aici = 0;
                                    break;
                                }
                            }

                            if (ok_aliment == 0) 
                            {
                                printf("Nu puteti introduce cifre. Incercati din nou\n");
                            } else 
                            {
                                if (write(sd, &aliment, sizeof(aliment)) <= 0) 
                                {
                                    perror("[client]Eroare la trimitere categorie spre server.\n");
                                    return errno;
                                }

                                int cantitate;
                            do {
                                printf("Adaugati cantitate:\n");
                                bzero(&cantitate, sizeof(cantitate));
                                fflush(stdout);
                                if (scanf("%d", &cantitate) != 1) 
                                {
                                    printf("Nu ati introdus o cantitate valida. Incercati din nou.\n");
                                    aici2 = 1;
                                    while (getchar() != '\n');  
                                } else 
                                {
                                    aici2 = 0;
                                }
                            } while (aici2 == 1);

                            if (write(sd, &cantitate, sizeof(cantitate)) <= 0) 
                            {
                                perror("[client]Eroare la trimitere categorie spre server.\n");
                                return errno;
                            }

                            do {
                                printf("Adaugati data expirare in format: DD.MM.YYYY:\n");
                                char data_expirare[100];
                                bzero(data_expirare, sizeof(data_expirare));
                                fflush(stdout);
                                if (scanf("%s", data_expirare) != 1 || !validateDate(data_expirare)) 
                                {
                                    printf("Data introdusa nu este valida. Incercati din nou.\n");
                                    aici3 = 1;
                                    while (getchar() != '\n');  
                                } else {

                                    if (write(sd, &data_expirare, sizeof(data_expirare)) <= 0) {
                                        perror("[client]Eroare la trimitere categorie spre server.\n");
                                        return errno;
                                    }
                                    aici3 = 0; 
                                }
                            } while (aici3 == 1);
                            aici = 1; 
                        }
                    } while (aici == 0);
                            char msj[100];
                            bzero(msj,sizeof(msj));
                            if(read(sd,&msj,sizeof(msj))<=0)
                            {
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                            }
                            printf("%s", msj);
                            
                            }
                    else
                        if(strcmp(meniu,"Iesire")==0)
                        {
                          char iesire[100];
                          bzero(iesire, sizeof(iesire));
                          strcpy(iesire,"Iesire");
                          if(write(sd, &iesire, sizeof(iesire)) <= 0)
                          {
                              perror ("[client]Eroare la iesire\n");
                              return errno;
                          }
                          close(sd);
                          return 0;
                        }
                    else
                    {
                      printf("Comanda necunoscuta. Try again\n");
                    }
                    }
              }
              else
                  if(strcmp(categorie,"Organizatie") == 0 || strcmp(categorie, "Persoana")==0)
                  {
                    
                    while(1)
                    {
                    printf("\n");
                    printf("Meniu:\n");
                    printf("Vizualizare_Organizatii\n");
                    printf("Vizualizare_Alimente\n");
                    printf("Vizualizare_Persoane\n");
                    printf("Ia_Produse\n");
                    printf("Vizualizare_Produse_Primite\n");
                    printf("Iesire\n");
                    bzero(meniu, sizeof(meniu));
                    fflush (stdout);
                    scanf("%s", meniu);
                    char meniuverif[7][30];
                    strcpy(meniuverif[0],"Vizualizare_Organizatii");
                    strcpy(meniuverif[1],"Vizualizare_Alimente");
                    strcpy(meniuverif[2],"Vizualizare_Persoane");
                    strcpy(meniuverif[3],"Ia_Produse");
                    strcpy(meniuverif[4],"Vizualizare_Produse_Primite");
                    strcpy(meniuverif[5],"Iesire");
                    int gasit = 0; 
                    for (int i = 0; i < 7; ++i) 
                    {
                        if (strcmp(meniu, meniuverif[i]) == 0) 
                        {
                            gasit = 1;
                            break; 
                        }
                    }
                    if(gasit == 1)
                    {
                        if(write(sd, &meniu, sizeof(meniu)) <= 0)
                        {
                            perror ("[client]Eroare la trimitere meniu spre server.\n");
                            return errno;
                        }
                    }
                  if(strcmp(meniu,"Vizualizare_Organizatii")==0)
                        {
                            char *organizatii = (char *)malloc(1000000);
                            memset(organizatii, 0, 1000000);
                            size_t totalCitit = 0;
                            //asteapta pana cand toti octetii pot fi cititi!!
                            while (totalCitit < 1000000) 
                            {
                                ssize_t citit = read(sd, organizatii + totalCitit, 1000000 - totalCitit);
                                if (citit <= 0) 
                                {
                                    perror("[client] Eroare la read() de la server.\n");
                                    return errno;
                                }
                                totalCitit += citit;
                            }
                            if(strlen(organizatii) ==0)
                                printf("Nu exista organizatii");
                            else
                                printf("%s\n", organizatii);
                            free(organizatii);
                        }
                    else
                          if (strcmp(meniu, "Vizualizare_Alimente") == 0) {
                                char *alimente = (char *)malloc(1000000);
                                memset(alimente, 0, 1000000);

                                size_t totalCitit = 0;
                                //asteapta pana cand toti octetii pot fi cititi!!
                                while (totalCitit < 1000000) {
                                    ssize_t citit = read(sd, alimente + totalCitit, 1000000 - totalCitit);
                                    if (citit <= 0) {
                                        perror("[client] Eroare la read() de la server.\n");
                                        return errno;
                                    }
                                    totalCitit += citit;
                                }

                                printf("%s\n", alimente);
                                free(alimente);
                            }

                    else
                          if(strcmp(meniu,"Vizualizare_Persoane")==0)
                        {
                            char *pers = (char *)malloc(1000000);
                            memset(pers, 0, 1000000);
                            size_t totalCitit = 0;
                            //asteapta pana cand toti octetii pot fi cititi!!
                            while (totalCitit < 1000000) {
                                ssize_t citit = read(sd, pers + totalCitit, 1000000 - totalCitit);
                                if (citit <= 0) {
                                    perror("[client] Eroare la read() de la server.\n");
                                    return errno;
                                }
                                totalCitit += citit;
                            }
                                printf("%s\n", pers);
                                free(pers);
                        }
                         else
                          if(strcmp(meniu,"Vizualizare_Produse_Primite")==0)
                        {
                            char *primit = (char *)malloc(1000000);
                            memset(primit, 0, 1000000);
                            size_t totalCitit = 0;
                            //asteapta pana cand toti octetii pot fi cititi!!
                            while (totalCitit < 1000000) {
                                ssize_t citit = read(sd, primit + totalCitit, 1000000 - totalCitit);
                                if (citit <= 0) {
                                    perror("[client] Eroare la read() de la server.\n");
                                    return errno;
                                }
                                totalCitit += citit;
                            }
                                printf("%s\n", primit);
                                free(primit);
                        }
                        else if(strcmp(meniu,"Ia_Produse")==0)
                        {
                         int aici = 1, aici2 = 1, aici3 = 1;

                        do {
                            printf("Adaugati aliment:\n");
                            char aliment[100];
                            bzero(aliment, sizeof(aliment));
                            fflush(stdout);
                            scanf("%s", aliment);

                            int ok_aliment = 1;
                            for (int i = 0; aliment[i] != '\0'; i++) {
                                if (!isalpha(aliment[i])) {
                                    ok_aliment = 0;
                                    aici = 0;
                                    break;
                                }
                            }

                            if (ok_aliment == 0) 
                            {
                                printf("Nu puteti introduce cifre. Incercati din nou\n");
                            } else 
                            {
                                if (write(sd, &aliment, sizeof(aliment)) <= 0) 
                                {
                                    perror("[client]Eroare la trimitere categorie spre server.\n");
                                    return errno;
                                }

                                int cantitate;
                            do {
                                printf("Adaugati cantitate:\n");
                                bzero(&cantitate, sizeof(cantitate));
                                fflush(stdout);
                                if (scanf("%d", &cantitate) != 1) {
                                    printf("Nu ati introdus o cantitate valida. Incercati din nou.\n");
                                    aici2 = 1;
                                    while (getchar() != '\n');  
                                } else {
                                    aici2 = 0;
                                }
                            } while (aici2 == 1);

                            if (write(sd, &cantitate, sizeof(cantitate)) <= 0) {
                                perror("[client]Eroare la trimitere categorie spre server.\n");
                                return errno;
                            }

                            do {
                                printf("Adaugati data expirare in format: DD.MM.YYYY:\n");
                                char data_expirare[100];
                                bzero(data_expirare, sizeof(data_expirare));
                                fflush(stdout);
                                if (scanf("%s", data_expirare) != 1 || !validateDate(data_expirare)) {
                                    printf("Data introdusa nu este valida. Incercati din nou.\n");
                                    aici3 = 1;
                                    while (getchar() != '\n');  
                                } else {
                                    if (write(sd, &data_expirare, sizeof(data_expirare)) <= 0) {
                                        perror("[client]Eroare la trimitere categorie spre server.\n");
                                        return errno;
                                    }
                                    aici3 = 0; 
                                }
                            } while (aici3 == 1);
                            aici = 1; 
                        }
                    } while (aici == 0);
                            char msj[100];
                            bzero(msj,sizeof(msj));
                            if(read(sd,&msj,sizeof(msj))<=0)
                            {
                            perror ("[client]Eroare la read() de la server.\n");
                            return errno;
                            }
                            printf("%s", msj);
                                }
                        else
                        if(strcmp(meniu,"Iesire")==0)
                        {
                          char iesire[100];
                          bzero(iesire, sizeof(iesire));
                          strcpy(iesire,"Iesire");
                          if(write(sd, &iesire, sizeof(iesire)) <= 0)
                          {
                              perror ("[client]Eroare la iesire\n");
                              return errno;
                          }
                          close(sd);
                          return 0;
                        }
                  else
                  {
                    printf("Categorie necunoscuta. Try again\n");
                    printf("\n");
                  }
                  }
            }
            else
            {
                printf("Categorie necunoscuta. Try again\n");

            }




            }


            
        }
    } 
    else
    if(strcmp(comand,"Iesire")==0)
    {
        char iesire[100];
        bzero(iesire, sizeof(iesire));
        strcpy(iesire,"Iesire");
        if(write(sd, &iesire, sizeof(iesire)) <= 0)
        {
            perror ("[client]Eroare la iesire\n");
            return errno;
        }
        close(sd);
        return 0;
    } 
    else 
    {
      printf("Comanda necunoscuta. Try again\n");
    }  
    
 
}
    close (sd);
}