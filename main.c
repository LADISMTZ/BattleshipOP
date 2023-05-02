#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define FILAS 11
#define COLUMNAS 11
int b1 =0,b2=0,b3=0, error=0;
int semcont = 0;
sem_t semaforo;


void signal_handler(int signum)
{

    if (signum == SIGUSR1){
        system("clear");
        printf("\nEL JUGADOR 1 HA GANADO FELICIDADES");

    }

    if (signum == SIGUSR2){
        system("clear");
        printf("\nEL JUGADOR 2 HA GANADO FELICIDADES");
        //fd = creat("Victorias_Jugador2.txt",777);
        //n = write(fd, &num, sizeof(num));
    }

    if (signum == SIGINT){
        system("clear");
        printf("\nES UN EMPATE");
        //fd = creat("Empates.txt",777);
        //n = write(fd, &num, sizeof(num));
    }



    if (signum == SIGALRM){
        system("clear");

    }

}



void verificar(int fd) {

    if (fd < 0){
        if (errno == EAGAIN){

            printf("Archivo bloqueado.\n");
            exit(1);

        }
        if (errno == EACCES){

            printf("Problema de permisos del archivo.\n");
            exit(1);

        }

        if (errno ==  EBADF ){

            printf("Mal descriptor del archivo.\n");
            exit(1);

        }

        if (errno == EINTR){

            printf("Una llamada al sistema fue interrumpida.\n");
            exit(1);

        }
        else{

            printf("Error desconocido.\n");
            exit(1);
        }
    }
}


void llenarTablero(char tablero[FILAS][COLUMNAS]) {
    // Llena el encabezado de la matriz con los números de columna
    for (int i = 1; i < COLUMNAS; i++) {
        tablero[0][i] = i - 1 + '0';
    }

    // Llena el encabezado de la matriz con espacios
    tablero[0][0] = ' ';
    for (int i = 1; i < FILAS; i++) {
        tablero[i][0] = i - 1 + '0';
    }

    // Llena el cuerpo de la matriz con espacios
    for (int i = 1; i < FILAS; i++) {
        for (int j = 1; j < COLUMNAS; j++) {
            tablero[i][j] = '0';
        }
    }
}




void *contador (){

    if (b1>2 || b2>2 || b3>2){
        sem_wait(&semaforo);
       error = 1;
       semcont++;
        sem_post(&semaforo);
    }//end if
    pthread_exit(NULL);

}//end contador


//PONES BARCOS
void colocarBarcos(char tablero[FILAS][COLUMNAS]){
    int count=1;
    pthread_t hiloA;

    do {
        int selec, selec2;
        int selecrenglon = 0, seleccolumna = 0;
        printf("\nSeleccione un barco de cada numero\n");
        printf("Selecciona alguna opcion de barco\n");
        printf("Barco chico(1)\n");
        printf("Barco mediano(2)\n");
        printf("Barco grande(3)\nEscribe el numero correspondiente: ");
        scanf("%i", &selec);

        //Ship size 1
        switch (selec) {
            case 1:
                printf("Has seleccionado el barco chico\n");
                b1++;
                printf("Ingresa el renglon donde lo pondras (0-9): ");
                scanf("%i", &selecrenglon);
                printf("\nIngresa la columna donde lo pondras (0-9): ");
                scanf("%i", &seleccolumna);

                if (tablero[selecrenglon + 1][seleccolumna + 1] != '0') {
                    system("clear");
                    printf("Hay otro barco en esa posición.\n");
                    alarm(2);
                    pause();
                    kill(getpid(),SIGKILL);
                }

                if (selecrenglon > 10 || seleccolumna > 10) {
                    execl("opcionInvalida","",NULL);} else {
                    tablero[selecrenglon + 1][seleccolumna + 1] = '$';
                }
                break;

                //Ship size 2
            case 2:
                printf("Has seleccionado el barco mediano\n");
                b2++;
                printf("Lo vas a colocar en posicion horizontal(1) o vertical(2):");
                scanf("%i", &selec2);

                switch (selec2) {
                    case 1:
                        printf("\nIngresa el renglon donde lo pondras (0-9): ");
                        scanf("%i", &selecrenglon);
                        printf("Ingresa la columna donde lo pondras (0-9): ");
                        scanf("%i", &seleccolumna);

                        for (int i = 0; i < 2; i++) {
                            if (tablero[selecrenglon + 1][seleccolumna + 1 + i] != '0') {
                                system("clear");
                                printf("Hay otro barco en esa posición.\n");
                                alarm(2);
                                pause();
                                kill(getpid(),SIGKILL);
                            }
                        }

                        if (selecrenglon > 10 || seleccolumna > 10) {
                            execl("opcionInvalida","",NULL);} else {
                            tablero[selecrenglon + 1][seleccolumna + 1] = '$';
                            tablero[selecrenglon + 1][seleccolumna + 2] = '$';
                        }


                        break;
                    case 2:
                        printf("Ingresa el renglon donde lo pondras (0-9): ");
                        scanf("%i", &selecrenglon);
                        printf("Ingresa la columna donde lo pondras (0-9): ");
                        scanf("%i", &seleccolumna);

                        for (int i = 0; i < 2; i++) {
                            if (tablero[selecrenglon + 1+i][seleccolumna + 1] != '0') {
                                system("clear");
                                printf("Hay otro barco en esa posición.\n");
                                alarm(2);
                                pause();
                                kill(getpid(),SIGKILL);
                            }
                        }

                        if (selecrenglon > 10 || seleccolumna > 10) {
                            execl("opcionInvalida","",NULL);} else {
                            tablero[selecrenglon + 1][seleccolumna + 1] = '$';
                            tablero[selecrenglon + 2][seleccolumna + 1] = '$';
                        }

                        break;
                    default:
                        printf("Opcion no valida");
                        break;
                }


                break;

            case 3:
                printf("Has seleccionado el barco grande\n");
                b3++;
                printf("Lo vas a colocar en posicion horizontal(1) o vertical(2):");
                scanf("%i", &selec2);

                switch (selec2) {
                    case 1:
                        printf("\nIngresa el renglon donde lo pondras (0-9): ");
                        scanf("%i", &selecrenglon);
                        printf("Ingresa la columna donde lo pondras (0-9): ");
                        scanf("%i", &seleccolumna);

                        for (int i = 0; i < 3; i++) {
                            if (tablero[selecrenglon + 1 + i][seleccolumna + 1] != '0') {
                                system("clear");
                                printf("Hay otro barco en esa posición.\n");
                                alarm(2);
                                pause();
                                kill(getpid(),SIGKILL);
                            }
                        }

                        if (selecrenglon > 10 || seleccolumna > 10) {
                            execl("opcionInvalida","",NULL);} else {
                            tablero[selecrenglon + 1][seleccolumna + 1] = '$';
                            tablero[selecrenglon + 1][seleccolumna + 2] = '$';
                            tablero[selecrenglon + 1][seleccolumna + 3] = '$';
                        }


                        break;
                    case 2:
                        printf("Ingresa el renglon donde lo pondras (0-9): ");
                        scanf("%i", &selecrenglon);
                        printf("Ingresa la columna donde lo pondras (0-9): ");
                        scanf("%i", &seleccolumna);

                        for (int i = 0; i < 3; i++) {
                            if (tablero[selecrenglon + 1+i][seleccolumna + 1] != '0') {
                                system("clear");
                                printf("Hay otro barco en esa posición.\n");
                                alarm(2);
                                pause();
                                kill(getpid(),SIGKILL);
                            }
                        }

                        if (selecrenglon > 10 || seleccolumna > 10) {
                            execl("opcionInvalida","",NULL);} else {
                            tablero[selecrenglon + 1][seleccolumna + 1] = '$';
                            tablero[selecrenglon + 2][seleccolumna + 1] = '$';
                            tablero[selecrenglon + 3][seleccolumna + 1] = '$';
                        }

                        break;
                    default:
                        printf("\nOpcion no valida\n");
                        break;
                }


                break;

            default:
                printf("Opcion no valida");
                break;


        }//end switch

        //HILO ERROR
        sem_init(&semaforo, 0, 1);
        pthread_create(&hiloA,NULL,contador,NULL);
        pthread_join(hiloA,NULL);
        sem_destroy(&semaforo);
        if (error == 1){
            system("clear");
            printf("Solo se puede colocar un barco de cada numero\n");
            printf("Inicie de nuevo\n");
            alarm(5);
            pause();
            kill(getpid(),SIGKILL);
        }//end if

        count++;
    }while(count < 4);
}//end colocar barcos



//ATACAR AL TABLERO CONTRARIO
int atacar1 (char tablero[FILAS][COLUMNAS],int aciertos) {
    int selecrenglon = 0, seleccolumna = 0;

    printf("\nSeleccione la coordenada a la cual atacara\n");
    printf("\nIngresa el renglon: ");
    scanf("%i", &selecrenglon);
    printf("Ingresa la columna: ");
    scanf("%i", &seleccolumna);

    if (selecrenglon > 10 || seleccolumna > 10) {
        printf("\nOpcion no valida\n");
    } else {
        if (tablero[selecrenglon + 1][seleccolumna + 1] == '$') {
            printf("Usted ha undido un barco");
            tablero[selecrenglon + 1][seleccolumna + 1] = 'X';
            sleep(4);
            aciertos++;
        }//end if
        else {
            printf("\nUsted ha fallado");
            tablero[selecrenglon + 1][seleccolumna + 1] = 'F';
            sleep(2);
        }//end else
        return aciertos;
    }//end atacar
}

//ATACAR AL TABLERO CONTRARIO
int atacar2 (char tablero[FILAS][COLUMNAS], int aciertos2){
    int selecrenglon = 0, seleccolumna = 0;

    printf("\nSeleccione la coordenada a la cual atacara\n");
    printf("\nIngresa el renglon: ");
    scanf("%i", &selecrenglon);
    printf("Ingresa la columna: ");
    scanf("%i", &seleccolumna);
    if (tablero[selecrenglon + 1][seleccolumna + 1] == '$'){
        printf("\nUsted ha undido un barco");
        tablero[selecrenglon + 1][seleccolumna + 1] = 'X';
        sleep(2);
        aciertos2++;
    }//end if
    else {
        printf("\nUsted ha fallado");
        tablero[selecrenglon + 1][seleccolumna + 1] = 'F';
        sleep(5);
    }//end else
    return aciertos2;
}//end atacar



void imprimir(char tablero[FILAS][COLUMNAS]) {
    printf("\n\t");

    for (int i = 1; i < COLUMNAS; i++) {
        printf(" ");
        printf(" %d ", i - 1);
    }
    printf("\n\n\t");

    printf("+");
    for (int i = 1; i < COLUMNAS; i++) {
        printf("---+");
    }
    printf("\n");

    for (int i = 1; i < FILAS; i++) {
        printf("%d\t", i - 1);
        for (int j = 1; j < COLUMNAS; j++) {
            printf("| ");
            printf("%c ", tablero[i][j]);
        }
        printf("|\n\t");

        printf("+");
        for (int i = 1; i < COLUMNAS; i++) {
            printf("---+");
        }
        printf("\n");
    }
}




int main(){
    char tablero1[FILAS][COLUMNAS];
    char tablero2[FILAS][COLUMNAS];
    int aciertos = 0, aciertos2 = 0;
    int status, fd, n;
    int num = 1;

    signal(SIGINT,signal_handler);
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    signal(SIGALRM, signal_handler);

    //PUT SHIPS
    pid_t pidM, pidA, pidB;
    pidM = getpid();


  pidA = fork();
   if (pidA == 0){
        printf("\nEs el turno de que el primer jugador coloque sus barcos\n");
       return 0;
   }//end pidA
    waitpid(pidA, &status, 0);

        llenarTablero(tablero1);
        colocarBarcos(tablero1);
        imprimir(tablero1);
        alarm(5);
        pause();


    pidB = fork();
        if (pidB == 0){
            printf("\nEs el turno de que el segundo jugador coloque sus barcos\n");
            return 0;
        }//end pidB
    waitpid(pidB, &status, 0);

    llenarTablero(tablero2);
            colocarBarcos(tablero2);
            imprimir(tablero2);
            alarm(5);
            pause();


    //START PLAYING
    do{
    //turno jugador1
    printf("\nTurno jugador1 \n");
    aciertos = atacar1(tablero2, aciertos);
    printf("\nEstado actual de tu tablero\n");
    printf("TU contrincante te ha undido %i barcos\n", aciertos2);
    imprimir(tablero1);
    sleep(10);
    system("clear");

    //turno jugador2
    printf("\nTurno jugador2 \n");
    aciertos2 = atacar2(tablero1, aciertos2);
    printf("\nEstado actual de tu tablero\n");
    printf("TU contrincante te ha undido %i barcos\n", aciertos);
    imprimir(tablero2);
    sleep(10);
    system("clear");

    }while (aciertos<3 && aciertos2<3);

    if (aciertos >= 3 && aciertos2<3){
        kill(pidM,SIGUSR1);
        fd = creat("VictoriasJugador1.txt",777);
        n = write(fd,&num,sizeof(num));
        verificar(n);
        close(n);
    }


    if (aciertos2 >= 3 && aciertos<3){
        kill(pidM,SIGUSR2);
        fd = creat("VictoriasJugador2.txt",777);
        n = write(fd,&num,sizeof(num));
        verificar(n);
        close(n);
    }

    if (aciertos >= 3 && aciertos2 >= 3){
        fd = creat("Empates.txt",777);
        n = write(fd,&num,sizeof(num));
        close(n);
        verificar(n);
        kill(pidM,SIGINT);

    }
    return 0;
}

