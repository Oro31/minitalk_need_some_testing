#define _POSIX_SOURCE
#define MS 1000
 
#include <signal.h>    /* Pour kill */
#include <stdlib.h>    /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h>     /* Pour printf */
#include <unistd.h>    /* Pour sleep */
#include <sys/types.h> /* Pour pid_t */
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	pid_t pidServeur;
	int	c;
	int	i;
	int	j;
	int	b;
 
  /* Recuperation des arguments */
	if(argc != 3) {
		printf("Wrong arguments\n");
		exit(EXIT_FAILURE);
	}
	pidServeur = atoi(argv[1]);
 
  /* Envoi du premier signal */
	printf("Attente avant envoi premier signal\n");
	usleep(MS);
	printf("Envoi premier signal\n");
	if(kill(pidServeur, SIGUSR1) == -1) {
		perror("Erreur lors de l'envoi du signal ");
		exit(EXIT_FAILURE);
	}

	printf("Attente avant l'envoie du nombre de lettres \n");
	usleep(MS);
	i = 0;
	printf("Client : Envoie du nombre de lettres\n");
	while (argv[2][i]) {
		if (kill(pidServeur, SIGUSR1) == -1) {
			perror("Erreur lors de l'evoie du signal ");
			exit(EXIT_FAILURE);
		}
		usleep(MS);
		i++;
	}
	if (kill(pidServeur, SIGUSR2) == -1) {
		perror("Erreur lors de l'envoie du signal ");
		exit(EXIT_FAILURE);
	}
	printf("Attente avant l'envoie de la chaine de caracteres\n");
	usleep(MS);
	i = 0;
	printf("Client : Envoie de la chaine de caracteres\n");
	while (argv[2][i]) {
		c = (int)argv[2][i];
		j = 0;
		while (j < 8) {
			b = c % 2;
			c = c / 2;
			if (b == 1)
			{
				if (kill(pidServeur, SIGUSR1) == -1) {
					perror("Erreur lors de l'envoie du signal ");
					exit(EXIT_FAILURE);
				}
			} 
			else
			{
				if (kill(pidServeur, SIGUSR2) == -1) {
					perror("Erreur lors de l'envoi du signal ");
					exit(EXIT_FAILURE);
				}
			}
			usleep(MS);
			j++;
		}
		if (kill(pidServeur, SIGUSR1) == -1) {
			perror("Erreur lors de l'envoi du signal ");
			exit(EXIT_FAILURE);
		}
		usleep(MS);
		i++;
	}
	printf("Client : envoie du dernier signal 2\n");
	if (kill(pidServeur, SIGUSR2) == -1) {
		perror("Erreur lors de l'envoie du signal ");
		exit(EXIT_FAILURE);
	}
	printf("Fin message client\n");
	return (0);	
}
