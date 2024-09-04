#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#include<limits.h>

#define len_array_stazioni 45000
#define len_array_macchine 512
#define len_array_coda 45000

typedef struct veicolo{

    int autonomia;

    int n_auto;

    struct veicolo * prev;

    struct veicolo * next;

}veicolo;

typedef struct veicolo* VEICOLI;

typedef struct{

    int kilometro;

    int max_autonomia;

    VEICOLI p_macchine;

}stazione;

typedef struct{

    stazione * autostrada;

    int n_stazioni;

}AUTOSTRADA;

typedef struct coda{

    int * elementi;

    int indice_testa;

    int indice_coda;

}coda;

typedef struct coda * CODA;

typedef struct percorso{

    int kilometro;

    struct percorso * next;

    struct percorso * prev;

}percorso;

typedef struct percorso * PERCORSO;

void Sposta_In_Avanti_Sotto_Array(stazione autostrada[], int pos_iniziale, int len){

    int i;

    for(i = len - 1; i > pos_iniziale; i--){

        autostrada[i] = autostrada[i - 1];

    }

}

int Aggiungi_Stazione(AUTOSTRADA *stazioni_autostrada, VEICOLI veicoli, int kilometro, int max_autonomia){

    int i;

    for(i = 0; (i < stazioni_autostrada->n_stazioni) && (stazioni_autostrada->autostrada[i].kilometro <= kilometro); i++){

        if(stazioni_autostrada->autostrada[i].kilometro == kilometro){

            return 0;

        }

    }

    stazioni_autostrada->n_stazioni++;

    if(stazioni_autostrada->n_stazioni <= len_array_stazioni){

        Sposta_In_Avanti_Sotto_Array(stazioni_autostrada->autostrada, i, stazioni_autostrada->n_stazioni);

        stazioni_autostrada->autostrada[i].kilometro = kilometro;

        stazioni_autostrada->autostrada[i].max_autonomia = max_autonomia;

        stazioni_autostrada->autostrada[i].p_macchine = veicoli;

        return 1;

    }
    else{

        return -1;

    }

}

VEICOLI Inserisci_In_Testa_Auto(VEICOLI p_macchine, int autonomia){

    veicolo * head;

    head = malloc(sizeof(veicolo));

    head->autonomia = autonomia;

    head->n_auto = 1;

    head->prev = NULL;

    head->next = p_macchine;

    if(p_macchine){

        p_macchine->prev = head;

    }

    p_macchine = head;

    return p_macchine;

}

VEICOLI Definisci_Parco_Macchine(int macchine[], int len_macchine, int *max_autonomia){

    VEICOLI p_macchine = NULL;

    int i;

    bool found;

    *max_autonomia = 0;

    for(i = 0; i < len_macchine; i++){

        if(macchine[i] > *max_autonomia){

            *max_autonomia = macchine[i];

        }

        found = false;

        veicolo* head = p_macchine;

        while(head != NULL){

            if(head->autonomia == macchine[i]){

                found = true;

                break;

            }

            head = head->next;

        }

        if(found){

            head->n_auto++;

        }
        else{

            p_macchine = Inserisci_In_Testa_Auto(p_macchine, macchine[i]);

        }

    }

    return p_macchine;

}

int Indice_Stazione_Ricerca_Binaria(stazione stazioni_autostrada[], int sx, int dx, int kilometro){

    int med;

    if(sx <= dx){

        med = sx + (int)((dx - sx)/2);

        if(stazioni_autostrada[med].kilometro == kilometro){

            return med;

        }
        else{

            if(stazioni_autostrada[med].kilometro < kilometro){

                return Indice_Stazione_Ricerca_Binaria(stazioni_autostrada, med +1, dx, kilometro);

            }
            else{

                return Indice_Stazione_Ricerca_Binaria(stazioni_autostrada, sx, med -1, kilometro);

            }

        }

    }

    return -1;

}

int Aggiungi_Auto(AUTOSTRADA *stazioni_autostrada, int kilometro, int autonomia){

    int i = Indice_Stazione_Ricerca_Binaria(stazioni_autostrada->autostrada, 0, stazioni_autostrada->n_stazioni - 1, kilometro);

    bool trovata;

    if(i != -1){

        if(stazioni_autostrada->autostrada[i].max_autonomia < autonomia){

            stazioni_autostrada->autostrada[i].max_autonomia = autonomia;

            stazioni_autostrada->autostrada[i].p_macchine = Inserisci_In_Testa_Auto(stazioni_autostrada->autostrada[i].p_macchine, autonomia);

            return 1;

        }
        else{

            veicolo * head = stazioni_autostrada->autostrada[i].p_macchine;

            trovata = false;

            while(head != NULL){

                if(head->autonomia == autonomia){

                    head->n_auto++;

                    trovata = true;

                    break;

                }

                head = head->next;

            }

            if(!trovata){

                stazioni_autostrada->autostrada[i].p_macchine = Inserisci_In_Testa_Auto(stazioni_autostrada->autostrada[i].p_macchine, autonomia);

            }

        }

        return 1;

    }
    else{

        return 0;

    }

}

VEICOLI Elimina_Auto(veicolo * da_eliminare, VEICOLI p_macchine){

    if(da_eliminare->next == NULL){

        if(da_eliminare->prev == NULL){

            free(da_eliminare);

            p_macchine = NULL;

        }
        else{

            (da_eliminare->prev)->next = NULL;

            free(da_eliminare);

        }

    }
    else{

        if(da_eliminare->prev == NULL){

            (da_eliminare->next)->prev = NULL;

            p_macchine = da_eliminare->next;

            free(da_eliminare);

        }
        else{

            (da_eliminare->prev)->next = da_eliminare->next;

            (da_eliminare->next)->prev = da_eliminare->prev;

            free(da_eliminare);

        }

    }

    return p_macchine;

}

void Free_Lista_Auto(VEICOLI p_macchine){

    VEICOLI head;

    while(p_macchine != NULL){

        head = p_macchine;

        p_macchine = p_macchine->next;

        free(head);

    }

}

int Rottama_Auto(AUTOSTRADA * stazioni_autostrada, int kilometro, int autonomia){

    int i = Indice_Stazione_Ricerca_Binaria(stazioni_autostrada->autostrada, 0, stazioni_autostrada->n_stazioni - 1, kilometro);

    int max;

    bool found;

    if(i != -1){

        veicolo * head = stazioni_autostrada->autostrada[i].p_macchine;

        found = false;

        while(head != NULL){

            if(head->autonomia == autonomia){

                found = true;

                break;

            }

            head = head->next;

        }

        if(found){

            if(head->n_auto > 1){

                head->n_auto--;

            }
            else{

                stazioni_autostrada->autostrada[i].p_macchine = Elimina_Auto(head, stazioni_autostrada->autostrada[i].p_macchine);

            }

            head = stazioni_autostrada->autostrada[i].p_macchine;

            max = 0;

            while(head != NULL){

                if(head->autonomia > max){

                    max = head->autonomia;

                }

                head = head->next;

            }

            stazioni_autostrada->autostrada[i].max_autonomia = max;

            return 1;

        }

    }

    return 0;

}

void Sposta_Indietro_Sotto_Array(stazione autostrada[], int pos_iniziale, int len){

    int i;

    for(i = pos_iniziale; i < len-1; i++){

        autostrada[i] = autostrada[i + 1];

    }

}

int Demolisci_Stazione(AUTOSTRADA * stazioni_autostrada, int kilometro){

    int i = Indice_Stazione_Ricerca_Binaria(stazioni_autostrada->autostrada, 0, stazioni_autostrada->n_stazioni - 1, kilometro);

    if(i != -1){

        Free_Lista_Auto(stazioni_autostrada->autostrada[i].p_macchine);

        Sposta_Indietro_Sotto_Array(stazioni_autostrada->autostrada, i, stazioni_autostrada->n_stazioni);

        stazioni_autostrada->n_stazioni--;

        return 1;

    }
    else{

        return 0;

    }

}

int Coda_Vuota(CODA array_coda){

    if(array_coda->indice_coda == array_coda->indice_testa){

        return 1;

    }

    return 0;

}

CODA Crea_Coda(){

    CODA array_coda = (CODA) malloc(sizeof(struct coda));

    if(array_coda){

        array_coda->elementi = (int *) malloc(len_array_coda * sizeof(int));

        array_coda->indice_testa = 0;

        array_coda->indice_coda = 0;

    }
    else{

        array_coda = NULL;

    }

    return array_coda;

}

int Rimuovi_Dalla_Coda(CODA array_coda){

    if(Coda_Vuota(array_coda) == 0){

        int x = array_coda->elementi[array_coda->indice_testa];

        if(array_coda->indice_testa == len_array_coda + 1){

            array_coda->indice_testa = 1;

        }
        else{

            array_coda->indice_testa++;

        }

        return x;

    }

    return -1;

}

int Coda_Piena(CODA array_coda){

    if(array_coda->indice_coda + 1 == array_coda->indice_testa){

        return 1;

    }

    return 0;

}

void Inserisci_Nella_Coda(CODA array_coda, int kilometro){

    if(Coda_Piena(array_coda) == 0){

        array_coda->elementi[array_coda->indice_coda] = kilometro;

        if(array_coda->indice_coda == len_array_coda + 1){

            array_coda->indice_coda = 1;

        }
        else{

            array_coda->indice_coda++;

        }

    }

}

void Pianifica_Percorso_Ritorno(AUTOSTRADA stazioni_autostrada, int kilometro_partenza, int kilometro_arrivo, int predecessori[], int distanze[]){

    bool * visitato = malloc(stazioni_autostrada.n_stazioni * sizeof(bool));

    CODA array_coda = Crea_Coda();

    int i, indice_arrivo;

    for(i = 0; i < stazioni_autostrada.n_stazioni; i++){

        visitato[i] = false;

        distanze[i] = INT_MAX;

        predecessori[i] = -1;

    }

    indice_arrivo = Indice_Stazione_Ricerca_Binaria(stazioni_autostrada.autostrada, 0, stazioni_autostrada.n_stazioni - 1, kilometro_arrivo);

    visitato[indice_arrivo] = true;

    distanze[indice_arrivo] = 0;

    Inserisci_Nella_Coda(array_coda, indice_arrivo);

    while(Coda_Vuota(array_coda) != 1){

        int indice_rimosso = Rimuovi_Dalla_Coda(array_coda);

        i = indice_rimosso;

        if(Coda_Vuota(array_coda) == 0){

            i = array_coda->elementi[array_coda->indice_coda-1];

        }

        i++;

        while((i < stazioni_autostrada.n_stazioni) && (stazioni_autostrada.autostrada[i].kilometro <= kilometro_partenza)){

            if(visitato[i] == false){

                if((stazioni_autostrada.autostrada[i].kilometro - stazioni_autostrada.autostrada[i].max_autonomia <= stazioni_autostrada.autostrada[indice_rimosso].kilometro)){

                    visitato[i] = true;

                    distanze[i] = distanze[indice_rimosso] + 1;

                    predecessori[i] = indice_rimosso;

                    Inserisci_Nella_Coda(array_coda, i);

                    if(stazioni_autostrada.autostrada[i].kilometro == kilometro_arrivo){

                        return;

                    }

                }

            }

            i++;

        }

    }

    free(array_coda);

}

int Pianifica_Percorso_Andata(AUTOSTRADA stazioni_autostrada, int kilometro_partenza, int kilometro_arrivo, int predecessori[], int distanze[]){

    bool * visitato = malloc(stazioni_autostrada.n_stazioni * sizeof(bool));

    CODA array_coda = Crea_Coda();

    int i, indice_partenza;

    for(i = 0; i < stazioni_autostrada.n_stazioni; i++){

        visitato[i] = false;

        distanze[i] = INT_MAX;

        predecessori[i] = -1;

    }

    indice_partenza = Indice_Stazione_Ricerca_Binaria(stazioni_autostrada.autostrada, 0, stazioni_autostrada.n_stazioni - 1, kilometro_partenza);

    visitato[indice_partenza] = true;

    distanze[indice_partenza] = 0;

    Inserisci_Nella_Coda(array_coda, indice_partenza);

    while(Coda_Vuota(array_coda) != 1){

        int indice_rimosso = Rimuovi_Dalla_Coda(array_coda);

        i = indice_rimosso;

        if(Coda_Vuota(array_coda) == 0){

            i = array_coda->elementi[array_coda->indice_coda -1];

        }

        i++;

        while((i < stazioni_autostrada.n_stazioni) && (stazioni_autostrada.autostrada[indice_rimosso].kilometro + stazioni_autostrada.autostrada[indice_rimosso].max_autonomia >= stazioni_autostrada.autostrada[i].kilometro)){

            if(visitato[i] == false){

                visitato[i] = true;

                distanze[i] = distanze[indice_rimosso] + 1;

                predecessori[i] = indice_rimosso;

                Inserisci_Nella_Coda(array_coda, i);

                if(stazioni_autostrada.autostrada[i].kilometro == kilometro_arrivo){

                    return 1;

                }

            }

            i++;

        }

    }

    free(array_coda);

    return 0;

}

PERCORSO Inserisci_In_Testa_Percorso(PERCORSO percorso_stazioni, int kilometro){

    percorso * head;

    head = malloc(sizeof(percorso));

    head->kilometro = kilometro;

    head->prev = NULL;

    head->next = percorso_stazioni;

    if(percorso_stazioni){

        percorso_stazioni->prev = head;

    }

    percorso_stazioni = head;

    return percorso_stazioni;

}

PERCORSO Pianifica_Percorso(AUTOSTRADA stazioni_autostrada, int kilometro_partenza, int kilometro_arrivo, int * percorso_trovato){

    int * predecessori = malloc(stazioni_autostrada.n_stazioni * sizeof(int));

    int * distanze = malloc(stazioni_autostrada.n_stazioni * sizeof(int));

    PERCORSO percorso_stazioni = NULL;

    int indice_arrivo = Indice_Stazione_Ricerca_Binaria(stazioni_autostrada.autostrada, 0, stazioni_autostrada.n_stazioni - 1, kilometro_arrivo);

    int indice_partenza = Indice_Stazione_Ricerca_Binaria(stazioni_autostrada.autostrada, 0, stazioni_autostrada.n_stazioni - 1, kilometro_partenza);

    if(kilometro_partenza <= kilometro_arrivo){

        if(Pianifica_Percorso_Andata(stazioni_autostrada, kilometro_partenza, kilometro_arrivo, predecessori, distanze) == 1){

            while(predecessori[indice_arrivo] != -1){

                percorso_stazioni = Inserisci_In_Testa_Percorso(percorso_stazioni, stazioni_autostrada.autostrada[predecessori[indice_arrivo]].kilometro);

                indice_arrivo = predecessori[indice_arrivo];

            }

            *percorso_trovato = 1;

            return percorso_stazioni;

        }

        *percorso_trovato = 0;

        return NULL;

    }
    else{

        Pianifica_Percorso_Ritorno(stazioni_autostrada, kilometro_partenza, kilometro_arrivo, predecessori, distanze);

        if(predecessori[indice_partenza] != -1){

            printf("%d ", kilometro_partenza);

            while(predecessori[indice_partenza] != -1){

                if(predecessori[predecessori[indice_partenza]] != -1){

                    printf("%d ", stazioni_autostrada.autostrada[predecessori[indice_partenza]].kilometro);

                }
                else{

                    printf("%d", stazioni_autostrada.autostrada[predecessori[indice_partenza]].kilometro);

                }

                indice_partenza = predecessori[indice_partenza];

            }

            *percorso_trovato = 1;

        }
        else{

            *percorso_trovato = 0;

        }

        return NULL;

    }

}

int main(){

    AUTOSTRADA autostrada;

    autostrada.autostrada = malloc(len_array_stazioni * sizeof(stazione));

    autostrada.n_stazioni = 0;

    VEICOLI p_macchine;

    PERCORSO percorso_stazioni;

    char command[20];

    int kilometro, n_veicoli, autonomia, max_autonomia, percorso_trovato, kilometro_partenza, kilometro_arrivo, ret_scanf;

    int * p_veicoli;

    int i;

    while(scanf("%s", command) != EOF){

        if(strcmp(command, "aggiungi-stazione") == 0){

            ret_scanf = scanf("%d", &kilometro);

            if(ret_scanf == 0){ return -1; }

            ret_scanf = scanf("%d", &n_veicoli);

            if(ret_scanf == 0){ return -1; }

            p_veicoli = malloc(n_veicoli * sizeof(int));

            for(i = 0; i < n_veicoli; i++){

                ret_scanf = scanf("%d", &p_veicoli[i]);

                if(ret_scanf == 0){ return -1; }

            }

            max_autonomia = 0;

            p_macchine = Definisci_Parco_Macchine(p_veicoli, n_veicoli, &max_autonomia);

            free(p_veicoli);

            if(Aggiungi_Stazione(&autostrada, p_macchine, kilometro, max_autonomia) == 1){

                printf("aggiunta\n");

            }
            else{

                printf("non aggiunta\n");

            }

        }

        if(strcmp(command, "aggiungi-auto") == 0){

            ret_scanf = scanf("%d %d", &kilometro, &autonomia);

            if(ret_scanf == 0){ return -1; }

            if(Aggiungi_Auto(&autostrada, kilometro, autonomia) == 1){

                printf("aggiunta\n");

            }
            else{

                printf("non aggiunta\n");

            }

        }

        if(strcmp(command, "rottama-auto") == 0){

            ret_scanf = scanf("%d %d", &kilometro, &autonomia);

            if(ret_scanf == 0){ return -1; }

            if(Rottama_Auto(&autostrada, kilometro, autonomia) == 1){

                printf("rottamata\n");

            }
            else{

                printf("non rottamata\n");

            }

        }

        if(strcmp(command, "demolisci-stazione") == 0){

            ret_scanf = scanf("%d", &kilometro);

            if(ret_scanf == 0){ return -1; }

            if(Demolisci_Stazione(&autostrada, kilometro) == 1){

                printf("demolita\n");

            }
            else{

                printf("non demolita\n");

            }

        }

        if(strcmp(command, "pianifica-percorso") == 0){

            ret_scanf = scanf("%d %d", &kilometro_partenza, &kilometro_arrivo);

            if(ret_scanf == 0){ return -1; }

            percorso_stazioni = Pianifica_Percorso(autostrada, kilometro_partenza, kilometro_arrivo, &percorso_trovato);

            if(percorso_trovato == 1){

                if(percorso_stazioni != NULL){

                    while(percorso_stazioni != NULL){

                        printf("%d ", percorso_stazioni->kilometro);

                        percorso_stazioni = percorso_stazioni->next;

                    }

                    printf("%d", kilometro_arrivo);

                }

                printf("\n");

            }
            else{

                printf("nessun percorso\n");

            }

        }

    }

    return 0;

}

