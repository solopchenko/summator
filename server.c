#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
  
int main()
{
  //Дескриптор сокета
  int sockfd;

  //Массив от клиента
  int climas[3];

  //Сумма чисел от всех клиентов
  int sum = 0;

  //Структуры для адресов сервера и клиента
  struct sockaddr_in servaddr, cliaddr;

  //Размер адреса клиента                                     
  int clilen;

  int n = 0;
                                                            
  //Обнуляем стркутуру для хранения адреса сервера
  bzero(&servaddr, sizeof(servaddr));                         

  //Заполняем структуру для хранения адреса сервера
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(7777);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  //Создаем UDP сокет
  if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror(NULL);
    exit(1);
  }

  //Настраиваем адрес сокета
  if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
  {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  //Основной цикл обслуживания
  while(1)
  {
    //Получаем максимальную длину ожидаемого адреса клиента
    clilen = sizeof(cliaddr);

    //Ожидаем прихода запроса от клиента и читаем его:
    //sockfd - дескриптор сокета, через который будет получаться сообщение,
    //climas - адрес области памяти, с которого будет записываться принятая информация,
    //sizeof(int)*3 - максимальное количество байт, которое мб размещено в приёмном буфере,
    //ссылка на структуру, в которую будет занесен адрес отправителя,
    //ссылка на переменную, в которую будет занесена фактическая длина структуры адреса сокета отправителя
    if((n = recvfrom(sockfd, climas, sizeof(int)*3, 0, (struct sockaddr *) &cliaddr, &clilen)) < 0)
    {
      perror(NULL);
      close(sockfd);
      exit(1);
    }

    //Печатаем принятый массив на экране
    for(int i = 0; i < 3; i++)
    {
      sum = sum + climas[i];
      printf("Число %d: %d\n", i + 1, climas[i]);
    }
    //Вывод суммы чисел
    printf("Сумма: %d\n", sum);
  }
  return 0;
}
