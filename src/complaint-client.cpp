/*
 *    complaint-client.c: client for logging and managing complaints
 *
 *    Copyright (c) 2020 SoftPrayog.in
 *
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define COMPLAINT_SOCKET "/tmp/complaint-server.socket"

#define MAX_MESSAGE_SIZE 256

#define LOG_COMPLAINT 1
#define GIVE_ME_A_COMPLAINT 2
#define GIVE_COMPLAINT4APT 3
#define RESOLVE_COMPLAINT 4
#define QUIT 0

#define NEXT_COMPLAINT 11
#define NO_MORE_COMPLAINTS 12
#define NO_COMPLAINT4THIS_APT 13
#define COMPLAINT_ADDED 14
#define COMPLAINT_DELETED 15

struct message {
  int32_t message_id;
  char apartment_id[8];
  char remarks[128];
};

struct message message;
int sock_fd;

int get_input(void);
void error(char *msg);

int main(int argc, char **argv) {

  if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    error("socket");

  struct sockaddr_un socket_address;

  memset(&socket_address, 0, sizeof(struct sockaddr_un));
  socket_address.sun_family = AF_UNIX;
  strncpy(socket_address.sun_path, COMPLAINT_SOCKET,
          sizeof(socket_address.sun_path) - 1);

  if (connect(sock_fd, (const struct sockaddr *)&socket_address,
              sizeof(struct sockaddr_un)) == -1)
    error("connect");

  int option;
  bool over = false;

  while (!over) {
    option = get_input();

    // send request to server
    if (write(sock_fd, &message, sizeof(struct message)) == -1)
      error("write");
    memset(&message, '\0', sizeof(struct message));
    // receive response from server
    if (read(sock_fd, &message, sizeof(struct message)) == -1)
      error("read");

    // process server response
    switch (message.message_id) {
    case COMPLAINT_ADDED:
      printf("\nComplaint for Apartment id: %s has been added.\n\n",
             message.apartment_id);
      break;

    case NEXT_COMPLAINT:
      printf("\nCOMPLAINT\n\tApartment id: %s\n\tRemarks: %s\n\n",
             message.apartment_id, message.remarks);
      break;

    case NO_MORE_COMPLAINTS:
      printf("\nNo more complaints\n\n");
      break;

    case NO_COMPLAINT4THIS_APT:
      printf("\nThere is no existing Complaint for Apartment id: %s.\n\n",
             message.apartment_id);
      break;

    case COMPLAINT_DELETED:
      printf("\nComplaint for Apartment id: %s has been deleted.\n\n",
             message.apartment_id);
      break;

    case QUIT:
      over = true;
      break;

    default:
      printf("\nUnrecongnized message from server\n\n");
    }
  }

  exit(EXIT_SUCCESS);
}

char inbuf[512];

int get_input(void) {
  int option;

  while (1) {
    printf("COMPLAINTS\n\n");
    printf("\tRecord a new complaint\t1\n");
    printf("\tGet next complaint\t2\n");
    printf("\tGet complaint\t3\n");
    printf("\tClear complaint\t4\n");
    printf("\tQuit\t\t0\n\n");
    printf("Your option: ");
    if (fgets(inbuf, sizeof(inbuf), stdin) == NULL)
      error("fgets");
    sscanf(inbuf, "%d", &option);

    int len;

    switch (option) {

    case 1:
      message.message_id = LOG_COMPLAINT;
      printf("Apartment id: ");
      if (fgets(inbuf, sizeof(inbuf), stdin) == NULL)
        error("fgets");
      len = strlen(inbuf);
      if (inbuf[len - 1] == '\n')
        inbuf[len - 1] = '\0';
      strcpy(message.apartment_id, inbuf);
      printf("Description: ");
      if (fgets(inbuf, sizeof(inbuf), stdin) == NULL)
        error("fgets");
      len = strlen(inbuf);
      if (inbuf[len - 1] == '\n')
        inbuf[len - 1] = '\0';
      strcpy(message.remarks, inbuf);
      break;

    case 2:
      message.message_id = GIVE_ME_A_COMPLAINT;
      break;

    case 3:
      message.message_id = GIVE_COMPLAINT4APT;
      printf("Apartment id: ");
      if (fgets(inbuf, sizeof(inbuf), stdin) == NULL)
        error("fgets");
      len = strlen(inbuf);
      if (inbuf[len - 1] == '\n')
        inbuf[len - 1] = '\0';
      strcpy(message.apartment_id, inbuf);
      break;

    case 4:
      message.message_id = RESOLVE_COMPLAINT;
      printf("Apartment id: ");
      if (fgets(inbuf, sizeof(inbuf), stdin) == NULL)
        error("fgets");
      len = strlen(inbuf);
      if (inbuf[len - 1] == '\n')
        inbuf[len - 1] = '\0';
      strcpy(message.apartment_id, inbuf);
      break;

    case 0:
      message.message_id = QUIT;
      break;

    default:
      printf("Illegal option, try again\n\n");
      continue;
    }

    return option;
  }
}

void error(char *msg) {
  perror(msg);
  exit(1);
}
