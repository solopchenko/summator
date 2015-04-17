#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  //Массив для отсылаемой информации
  int mas[3];

  //Дескриптор сокета
  int sockfd;

  //Структура для адреса сервера
  struct sockaddr_in servaddr;

  //Проверка наличия 2 аргумента командной строки
  if(argc != 2)
  {
    printf("Необходимо указать IP-адрес сервера: client <IP-адрес сервера>.  Программа завершена.");
    exit(1);
  }

  //Адрес сервера: семейство протоколов TCP/IP, сетевой интерфейс из аргумента командной строки, номер порта 7777.
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(7777);
  if(inet_aton(argv[1], &servaddr.sin_addr) == 0)
  {
    printf("Неверный IP! Программа завершена.\n");
    exit(1);
  }
  
  //Создаем UDP сокет: PF_INET - TCP/IP, SOCK_DGRAM - обмен инф-ией ч/з сообщения, 
  //0 - конкретный протокол для выбранного семейства протоколов и способа обмена информацией. 
  //0 т. к. выбранные семейство протоколв и способ обмена информации идентифицирую протокол однозначно
  if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror(NULL);
    exit(1);
  }
  
  //Запрашиваем числа
  for(int i = 0; i < 3; i++)
  {
    printf("Введите число %d: ", i + 1);
    int res = scanf("%d", &mas[i]);

    //Проверка корректности ввода числа
    if(res == 0)
    {
      printf("Нужно вводить только целые числа. Программа завершена.\n");
      exit(1);
    }

    //Проверка уникальности введенного числа
    for(int j = 0; j < i; j++)
    {
      if(mas[i] == mas[j])
      {
        printf("Повтор числа! Программа завершена.\n");
        exit(1);
      }
    }
  }

  //Отправка массива на сервер: sockfd - дескриптор сокета, ч/з который отправляется массив,.
  //mas - адрес области памяти, с которой будет браться информация для передачи,
  //sizeof(int)*3 - кол-во байт, которое должно быть передано.
  //servaddr - ссылка на структуру, содержащую адрес сокета получателя информации (т. е. сервера)
  //длина структуры, содержащей адрес сервера
  if(sendto(sockfd, mas, sizeof(int)*3, 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
  {
    perror(NULL);
    close(sockfd);
    exit(1);
  }

  close(sockfd);
  return 0;
}
