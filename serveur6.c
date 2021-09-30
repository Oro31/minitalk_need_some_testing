#define _POSIX_C_SOURCE 199309L
 
#include <stdlib.h>    /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <signal.h>    /* Pour signal */
#include <stdio.h>     /* Pour printf */
#include <unistd.h>     /* Pour sleep */
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

/*int 	cpt = 0;*/    /* Permet de marquer les signaux recus */
/*int 	d = 0;
int		octet[8];
int 	size = 11;*/
int		**str;

/**
 * Ceci est le gestionnaire qui est associe aux signaux SIGUSR1 et SIGUSR2.
 * @param signum le numero du signal recu
 */

int	ft_pow(int d, int p)
{
	int	i;
	int	res;

	res = 1;
	i = p;
	while (i > 0)
	{
		res = res * d;
		i--;
	}
	return (res);
}

void	ft_print(int *str, int size)
{
	int		i;
	char	c;

	c = '\0';
	i = -1;
	while (++i < size)
	{
		c = str[i];
		if (c != '\0')
			write(1, &c, 1);
	}
	write(1, "\n", 1);
}

int		*ft_realloc(char res, int *str)
{
	int		i;
	int		*tmp;

	i = 0;
	while (str[i])
		i++;
	tmp = malloc(sizeof(int) * (i + 2));
	if (!tmp)
		return (NULL);
	tmp[i + 1] = '\0';
	tmp[i] = res;
	while (--i >= 0)
		tmp[i] = str[i];
	free(str);
	return (tmp);
}

int		*ft_putchar(int *octet, int *str)
{
	int		i;
	int		res;
	char	c;

	c = '\0';
	res = 0;
	i = -1;
	while (++i < 8)
	{
		res = res + (octet[i] * ft_pow(2, i));
	}
	c = res;
	str = ft_realloc(res, str);
	return (str);
	//write(1, &c, 1);
}

void handler(int signum) {
	int		i;

	i = -1;
	if(signum == SIGUSR1 && str[2][0] == 0) {
		printf("Serveur : Signal 1 recu\n");
		free(str[0]);
		str[0] = malloc(sizeof(char));
		str[0][0] = '\0';
		str[2][0] = 1;
	}
	else if (signum == SIGUSR1 && str[2][0] == 1) {
		str[2][2]++;
	}
	else if (signum == SIGUSR2 && str[2][0] == 1) {
		printf("d = %d\n", str[2][2]);
		str[2][1] = str[2][2];
		str[2][0] = 2;
	}
	else if (signum == SIGUSR1 && str[2][0] > 1 && str[2][0] < 10 && str[2][0] > 0) {
		str[1][str[2][0] - 2] = 1;
		str[2][0]++;
	}
	else if (signum == SIGUSR2 && str[2][0] > 1 && str[2][0] < 10 && str[2][2] > 0) {
		str[1][str[2][0] - 2] = 0;
		str[2][0]++;
	}
	else if (signum == SIGUSR1 && str[2][0] == 10)
	{
		str[0] = ft_putchar(str[1], str[0]);
		while (++i < 8)
			str[1][i] = 0;
		str[2][2]--;
		str[2][0] = 2;
	}
	else if (signum == SIGUSR2 && str[2][2] == 0 && str[2][0] == 2) {
		printf("Serveur : signal 2 pour fin de message recu\n");
		usleep(100);
		ft_print(str[0], str[2][1]);
		str[2][0] = 0; 
	}
}
 
int main() {
	struct sigaction action;
 
	str = malloc(sizeof(int *) * 3);
	str[0] = malloc(sizeof(int));
	str[0][0] = '\0';
	str[1] = malloc(sizeof(int) * 8);
	str[2] = malloc(sizeof(int) * 3);
	str[2][0] = 0;
	str[2][1] = 11;
	str[2][2] = 0;
  /* Positionnement du gestionnaire pour SIGUSR1 */
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if(sigaction(SIGUSR1, &action, NULL) == -1) {
		perror("Erreur lors du positionnement ");
		exit(EXIT_FAILURE);
	}
 
  /* Positionnement du gestionnaire pour SIGUSR2 */
	if(sigaction(SIGUSR2, &action, NULL) == -1) {
		perror("Erreur lors du positionnement ");
		exit(EXIT_FAILURE);
	}
 
	printf("Pret a recevoir des signaux. Mon PID : %d\n", getpid());
 
  /* Mise en attente jusqu'a recevoir au moins un signal SIGUSR1 et SIGUSR2 */
	/*while (str[2][0] != (str[2][1] + 20)) {
		pause();
	}*/
	while (1)
	{
		pause();
	}
	return EXIT_SUCCESS;
}
