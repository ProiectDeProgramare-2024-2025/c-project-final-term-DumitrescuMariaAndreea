#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define MAX_MESE 5
#define NUME_FISIER "mese.txt"

// culori ~
#define AC_BLACK "\x1b[30m"
#define AC_RED "\x1b[31m"
#define AC_GREEN "\x1b[32m"
#define AC_YELLOW "\x1b[33m"
#define AC_BLUE "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN "\x1b[36m"
#define AC_WHITE "\x1b[37m"
#define AC_NORMAL "\x1b[m"

struct mese
{
    int id;
    int capacitate;
    int disponibilitate; // 0 - libera, 1 - rezervata
    int ora_rezervare;
    int data_rezervare;
};

struct mese masa[MAX_MESE]; 
int nr_mese = 0;

// Culori
void cyan()
{
    printf("\031[1;36m");
}
void blue()
{
    printf("\031[1;34m");
}

void purple()
{
    printf("\033[0;35m");
}

void reset()
{
    printf("\033[0m");
}

// Funcții pentru persistența datelor
void salveaza_date() 
{
    FILE *fisier = fopen(NUME_FISIER, "w");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului pentru scriere!\n");
        return;
    }
    
    // Scriem un header descriptiv pentru fișier
    fprintf(fisier, "# Fisier de date pentru rezervări restaurant\n");
    fprintf(fisier, "# Format: ID_Masa Capacitate Disponibilitate(0=libera,1=rezervata) Ora_Rezervare Data_Rezervare(ziua lunii)\n");
    fprintf(fisier, "# Numar total de mese: %d\n", nr_mese);
    
    // Scriem datele fiecărei mese cu descriere
    for (int i = 0; i < nr_mese; i++) {
        fprintf(fisier, "Masa %d: %d %d %d %d # ID:%d, Capacitate:%d, %s, ", 
                masa[i].id, 
                masa[i].capacitate, 
                masa[i].disponibilitate,
                masa[i].ora_rezervare,
                masa[i].data_rezervare,
                masa[i].id,
                masa[i].capacitate,
                masa[i].disponibilitate == 0 ? "Libera" : "Rezervata");
        if (masa[i].disponibilitate == 1) {
            purple();
            fprintf(fisier, "Rezervata pentru ora %d în ziua %d\n", 
                   masa[i].ora_rezervare, masa[i].data_rezervare);
            reset();
        } else {
            fprintf(fisier, "Fără rezervare\n");
        }
    }
    
    fclose(fisier);
    printf("Datele au fost salvate cu succes in fisierul %s!\n", NUME_FISIER);
}

void incarca_date() 
{
    FILE *fisier = fopen(NUME_FISIER, "r");
    if (fisier == NULL) {
        printf("Nu s-a gasit fisierul de date. Se va crea unul nou la salvare.\n");
        // Inițializăm cu câteva mese implicite
        nr_mese = 3;
        masa[0].id = 1;
        masa[0].capacitate = 4;
        masa[0].disponibilitate = 0;
        masa[0].ora_rezervare = 0;
        masa[0].data_rezervare = 0;
        
        masa[1].id = 2;
        masa[1].capacitate = 2;
        masa[1].disponibilitate = 0;
        masa[1].ora_rezervare = 0;
        masa[1].data_rezervare = 0;
        
        masa[2].id = 3;
        masa[2].capacitate = 8;
        masa[2].disponibilitate = 0;
        masa[2].ora_rezervare = 0;
        masa[2].data_rezervare = 0;
        
        // Salvăm datele implicite imediat
        salveaza_date();
        return;
    }
    
    char linie[256];
    nr_mese = 0;
    
    // Sărim peste liniile de comentariu care încep cu #
    while (fgets(linie, sizeof(linie), fisier) != NULL) {
        if (linie[0] == '#') {
            continue;
        }
        
        if (strncmp(linie, "Masa ", 5) == 0) {
            int id, capacitate, disponibilitate, ora, data;
            if (sscanf(linie, "Masa %d: %d %d %d %d", &id, &capacitate, &disponibilitate, &ora, &data) == 5) {
                masa[nr_mese].id = id;
                masa[nr_mese].capacitate = capacitate;
                masa[nr_mese].disponibilitate = disponibilitate;
                masa[nr_mese].ora_rezervare = ora;
                masa[nr_mese].data_rezervare = data;
                nr_mese++;
            }
        }
    }
    
    fclose(fisier);
    printf("Datele au fost incarcate cu succes! Total mese: %d\n", nr_mese);
}

void meniu_principal()
{
    purple();
    printf("----Meniu Principal----\n");
    reset();
    printf("1. Adauga sau sterge masa\n");
    printf("2. Afiseaza toate mesele si rezervarile\n");
    printf("3. Cauta masa libera\n");
    printf("4. Rezerva masa\n");
    printf("5. Sterge rezervare\n");
    printf("6. Inchide aplicatie\n");
}

void sterge_ecran()
{
    system("cls || clear");
}

void adauga_masa()
{
    int capacitate;
    printf("Introdu capacitate masa noua: \n");
    scanf("%d", &capacitate);
    
    if (nr_mese >= MAX_MESE) {
        printf("Nu mai puteti adauga mese. Limita maxima atinsa!\n");
        return;
    }
    
    nr_mese++;
    masa[nr_mese-1].id = nr_mese;
    masa[nr_mese-1].capacitate = capacitate;
    masa[nr_mese-1].disponibilitate = 0;
    masa[nr_mese-1].ora_rezervare = 0;
    masa[nr_mese-1].data_rezervare = 0;
    
    printf("Masa cu ID-ul %d si capacitatea %d a fost adaugata cu succes!\n", nr_mese, capacitate);
    
    // Salvăm automat după modificare
    salveaza_date();
}

void sterge_masa()
{
    int numar_masa;
    printf("Selecteaza masa dupa numarul acesteia: \n");
    scanf("%d", &numar_masa);
    
    if (numar_masa < 1 || numar_masa > nr_mese) {
        printf("Numarul mesei este invalid!\n");
        return;
    }
    
    // Găsim indexul mesei în array
    int index = -1;
    for (int i = 0; i < nr_mese; i++) {
        if (masa[i].id == numar_masa) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Masa nu a fost gasita!\n");
        return;
    }
    
    // Mutăm toate mesele cu o poziție în sus pentru a elimina masa
    for (int i = index; i < nr_mese - 1; i++) {
        masa[i] = masa[i + 1];
    }
    
    nr_mese--;
    printf("Masa cu numarul %d a fost stearsa cu succes!\n", numar_masa);
    
    // Salvăm automat după modificare
    salveaza_date();
}

void lista_mese()
{
    printf("Mesele restaurantului:\n");
    for(int i = 0; i < nr_mese; i++) {
        printf("Masa %d: Capacitate: %d, Disponibilitate: ", masa[i].id, masa[i].capacitate);
        if(masa[i].disponibilitate == 0) 
            printf("libera\n");
        else 
            printf("rezervata (Data: %d, Ora: %d)\n", masa[i].data_rezervare, masa[i].ora_rezervare);
    }
    printf("\n");
}

void cauta_masa_libera()
{
    int capacitate;
    printf("Introduceti capacitatea dorita: ");
    scanf("%d", &capacitate);
    printf("\n");
    
    int gasit = 0;
    printf("Mese libere cu capacitatea %d:\n", capacitate);
    
    for(int i = 0; i < nr_mese; i++) {
        if (masa[i].capacitate >= capacitate && masa[i].disponibilitate == 0) {
            printf("Masa %d: Capacitate: %d\n", masa[i].id, masa[i].capacitate);
            gasit = 1;
        }
    }
    
    if (!gasit) {
        printf("Nu exista mese libere cu capacitatea dorita!\n");
    }
}

void rezervare()
{
    int numar_masa, data, ora;
    
    printf("Introduceti numarul mesei: ");
    scanf("%d", &numar_masa);
    printf("\n");
    
    int index = -1;
    for (int i = 0; i < nr_mese; i++) {
        if (masa[i].id == numar_masa) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Masa nu exista!\n");
        return;
    }
    
    if (masa[index].disponibilitate == 1) {
        printf("Masa este deja rezervata!\n");
        return;
    }
    
    printf("Introduceti data(ziua lunii) dorita: ");
    scanf("%d", &data);
    printf("\n");
    
    printf("Introduceti ora dorita: ");
    scanf("%d", &ora);
    printf("\n");
    
    masa[index].disponibilitate = 1;
    masa[index].data_rezervare = data;
    masa[index].ora_rezervare = ora;
    
    printf("Masa %d a fost rezervata cu succes pentru data %d, ora %d!\n", numar_masa, data, ora);
    
    // Salvăm automat după modificare
    salveaza_date();
}

void sterge_rezervare()
{
    int numar_masa;
    
    printf("Introduceti numarul mesei: ");
    scanf("%d", &numar_masa);
    printf("\n");
    
    int index = -1;
    for (int i = 0; i < nr_mese; i++) {
        if (masa[i].id == numar_masa) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Masa nu exista!\n");
        return;
    }
    
    if (masa[index].disponibilitate == 0) {
        printf("Aceasta masa nu are o rezervare activa!\n");
        return;
    }
    
    masa[index].disponibilitate = 0;
    masa[index].data_rezervare = 0;
    masa[index].ora_rezervare = 0;
    
    printf("Rezervarea pentru masa %d a fost stearsa cu succes!\n", numar_masa);
    
    // Salvăm automat după modificare
    salveaza_date();
}

int main()
{ 
    int optiune;
    
    // Încărcăm datele din fișier la pornirea aplicației
    incarca_date();
    
    while(1)
    {
        meniu_principal();
        printf("%s\n Alege optiune: ", AC_CYAN);
        scanf("%d", &optiune);
    
        if (optiune==6) 
        {
            sterge_ecran();
            purple();
            printf("Multumim ca ati utilizat aplicatia noastra!\n");
            printf("O zi frumoasa!");
            reset();
            
            // Salvăm datele înainte de închidere
            salveaza_date();
            return 0;
        }
        
        while(1)
        {
            if (optiune==1)
            {
                sterge_ecran();
                purple();
                printf("----Adauga sau sterge masa----\n");
                reset();
                printf("1. Adauga masa\n");
                printf("2. Sterge masa\n");
                printf("3. Inapoi\n");
                reset();
                printf("%sAlege optiune:\n",AC_CYAN);
                scanf("%d", &optiune);
        
                if (optiune==1)
                {
                    sterge_ecran();
                    printf("----Adauga masa----\n");
                    adauga_masa();
                }
        
                if (optiune==2)
                {
                    sterge_ecran();
                    printf("----Sterge masa----\n");
                    sterge_masa();
                }
        
                if (optiune==3)
                {
                    sterge_ecran();
                    break;
                }
            }
            
            if (optiune==2)
            {
                int click;
                sterge_ecran();
                purple();
                printf("----Afiseaza toate mesele----\n");
                reset();
                lista_mese();
                printf("%sPentru a te intoarce la meniul principal click 0\n",AC_CYAN);
                scanf("%d", &click);
                if(click==0)
                sterge_ecran();
                break;
            }
            
            if (optiune==3)
            {
                int click;
                sterge_ecran();
                purple();
                printf("----Cauta masa libera----\n");
                reset();
                cauta_masa_libera();
                printf("%sPentru a te intoarce la meniul principal click 0\n",AC_CYAN);
                scanf("%d", &click);
                if(click==0)
                sterge_ecran();
                break;
            }
            
            if (optiune==4)
            {
                int click;
                sterge_ecran();
                purple();
                printf("----Rezerva o masa----\n");
                reset();
                rezervare();
                printf("%sPentru a te intoarce la meniul principal click 0\n",AC_CYAN);
                reset();
                scanf("%d", &click);
                if(click==0)
                sterge_ecran();
                break;
            }
            
            if (optiune==5)
            {
                int click;
                sterge_ecran();
                purple();
                printf("----Sterge rezervare----\n");
                reset();
                sterge_rezervare();
                printf("%sPentru a te intoarce la meniul principal click 0\n",AC_CYAN);
                reset();
                scanf("%d", &click);
                if(click==0)
                sterge_ecran();
                break;
            }
        }
    }
    return 0;
}