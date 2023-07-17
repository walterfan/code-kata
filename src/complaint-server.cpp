

/*
 *    complaint-server.c: Log and manage complaints
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

#define BACKLOG 10

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

#define MAX_CLIENTS 200

struct message {
  int32_t message_id;
  char apartment_id[8];
  char remarks[128];
};

struct element {
  char *apartment_id;
  char *remarks;
  bool accessed;
  struct element *next;
};

struct element *tail;

struct message recv_message, send_message;

void add_to_complaint_q(char *source_apartment_id, char *source_remarks,
                        char *dest_apartment_id);
int give_next_complaint(char *apartment_id, char *remarks);
int give_complaint(char *source_apartment_id, char *dest_apartment_id,
                   char *remarks);
int del_complaint(char *source_apartment_id, char *dest_apartment_id);
void reset_accessed_all(void);
void error(char *msg);

int main(int argc, char **argv) {
  printf("Complaint-server: Hello, World!\n");
  // initialize the complaint queue
  tail = NULL;

  // create a unix domain socket, COMPLAINT_SOCKET
  // unlink, if already exists
  struct stat statbuf;
  if (stat(COMPLAINT_SOCKET, &statbuf) == 0) {
    if (unlink(COMPLAINT_SOCKET) == -1)
      error("unlink");
  }

  int listener;

  if ((listener = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    error("socket");

  struct sockaddr_un socket_address;

  memset(&socket_address, 0, sizeof(struct sockaddr_un));
  socket_address.sun_family = AF_UNIX;
  strncpy(socket_address.sun_path, COMPLAINT_SOCKET,
          sizeof(socket_address.sun_path) - 1);

  if (bind(listener, (const struct sockaddr *)&socket_address,
           sizeof(struct sockaddr_un)) == -1)
    error("bind");

  // Mark socket for accepting incoming connections using accept
  if (listen(listener, BACKLOG) == -1)
    error("listen");

  fd_set fds, readfds;
  FD_ZERO(&fds);
  FD_SET(listener, &fds);
  int fdmax = listener;

  printf("Complaint-server: Waiting for a message from a client.\n");
  while (1) {
    readfds = fds;
    // monitor readfds for readiness for reading
    if (select(fdmax + 1, &readfds, NULL, NULL, NULL) == -1)
      error("select");

    // Some sockets are ready. Examine readfds
    for (int fd = 0; fd < (fdmax + 1); fd++) {
      if (FD_ISSET(fd, &readfds)) { // fd is ready for reading
        if (fd == listener) {       // request for new connection
          int fd_new;
          if ((fd_new = accept(listener, NULL, NULL)) == -1)
            error("accept");

          FD_SET(fd_new, &fds);
          if (fd_new > fdmax)
            fdmax = fd_new;
          fprintf(stderr, "complaint-server: new client\n");
        } else // data from an existing connection, receive it
        {
          memset(&recv_message, '\0', sizeof(struct message));
          ssize_t numbytes = read(fd, &recv_message, sizeof(struct message));

          if (numbytes == -1)
            error("read");
          else if (numbytes == 0) {
            // connection closed by client
            fprintf(stderr, "Socket %d closed by client\n", fd);
            if (close(fd) == -1)
              error("close");
            FD_CLR(fd, &fds);
          } else {
            // data from client
            memset(&send_message, '\0', sizeof(struct message));
            switch (recv_message.message_id) {
            case LOG_COMPLAINT:
              add_to_complaint_q(recv_message.apartment_id,
                                 recv_message.remarks,
                                 send_message.apartment_id);
              send_message.message_id = COMPLAINT_ADDED;
              if (write(fd, &send_message, sizeof(struct message)) == -1)
                error("write");
              break;

            case GIVE_ME_A_COMPLAINT:
              if (give_next_complaint(send_message.apartment_id,
                                      send_message.remarks) == -1) {
                // error: No more complaints
                send_message.message_id = NO_MORE_COMPLAINTS;
                if (write(fd, &send_message, sizeof(struct message)) == -1)
                  error("write");
              } else {
                send_message.message_id = NEXT_COMPLAINT;
                if (write(fd, &send_message, sizeof(struct message)) == -1)
                  error("write");
              }
              break;

            case GIVE_COMPLAINT4APT:
              if (give_complaint(recv_message.apartment_id,
                                 send_message.apartment_id,
                                 send_message.remarks) == -1) {
                // no complaint for this apartment
                send_message.message_id = NO_COMPLAINT4THIS_APT;
                if (write(fd, &send_message, sizeof(struct message)) == -1)
                  error("write");
              } else {
                send_message.message_id = NEXT_COMPLAINT;
                if (write(fd, &send_message, sizeof(struct message)) == -1)
                  error("write");
              }
              break;

            case RESOLVE_COMPLAINT:
              if (del_complaint(recv_message.apartment_id,
                                send_message.apartment_id) == -1) {
                // error: No complaint found for this apartment
                send_message.message_id = NO_COMPLAINT4THIS_APT;
                if (write(fd, &send_message, sizeof(struct message)) == -1)
                  error("write");
              } else {
                // complaint deleted
                send_message.message_id = COMPLAINT_DELETED;
                if (write(fd, &send_message, sizeof(struct message)) == -1)
                  error("write");
              }
              break;

            case QUIT:
              if (close(fd) == -1)
                error("close");
              FD_CLR(fd, &fds);
              break;

            default:
              fprintf(stderr,
                      "Complaint-server: Unexpected message from client\n");
            }
          }
        }
      } // if (fd == ...
    }   // for
  }     // while (1)
  exit(EXIT_SUCCESS);
} // main

// add a new complaint to the complaint queue
void add_to_complaint_q(char *source_apartment_id, char *source_remarks,
                        char *dest_apartment_id) {
  struct element *ptr;
  char *cp1, *cp2;

  if ((ptr = (struct element *)malloc(sizeof(struct element))) == NULL)
    error("malloc");
  if ((cp1 = (char *)malloc(strlen(source_apartment_id) + 1)) == NULL)
    error("malloc");

  strcpy(cp1, source_apartment_id);
  strcpy(dest_apartment_id, source_apartment_id);
  ptr->apartment_id = cp1;

  if ((cp2 = (char *)malloc(strlen(source_remarks) + 1)) == NULL)
    error("malloc");

  strcpy(cp2, source_remarks);
  ptr->remarks = cp2;
  ptr->accessed = false;

  if (tail == NULL) {
    ptr->next = ptr;
  } else {
    ptr->next = tail->next;
    tail->next = ptr;
  }
  tail = ptr;
}

int del_complaint(char *source_apartment_id,
                  char *dest_apartment_id) // returns -1 on error
{
  struct element *ptr, *prev;
  char *cp;

  strcpy(dest_apartment_id, source_apartment_id);

  if (!tail) {
    fprintf(stderr, "del_complaint: Queue is empty\n");
    return -1;
  }
  // get the head
  prev = tail;
  ptr = tail->next;
  while (1) {
    if (strcmp(source_apartment_id, ptr->apartment_id) == 0)
      break;

    if (ptr == tail)
      return -1;

    prev = ptr;
    ptr = ptr->next;
  }

  free(ptr->apartment_id);
  free(ptr->remarks);

  if (ptr == tail) {
    if (ptr == prev) {
      tail = NULL;
      free(ptr);
      return 0;
    } else {
      tail = prev;
    }
  }
  prev->next = ptr->next;
  free(ptr);
  return 0;
}

int give_next_complaint(char *apartment_id, char *remarks) {
  struct element *ptr;

  if (!tail) {
    fprintf(stderr, "give_next_complaint: Queue is empty\n");
    return -1;
  }
  // get the head
  ptr = tail->next;
  while (1) {
    if (!ptr->accessed)
      break;

    if (ptr == tail) {
      reset_accessed_all();
      ptr = tail->next;
      continue;
    }
    ptr = ptr->next;
  }

  strcpy(apartment_id, ptr->apartment_id);
  strcpy(remarks, ptr->remarks);
  ptr->accessed = true;

  return 0;
}

void reset_accessed_all(void) {
  struct element *ptr;

  if (!tail) {
    fprintf(stderr, "reset_accessed_all: Queue is empty\n");
    return;
  }
  // get the head
  ptr = tail->next;
  while (1) {
    ptr->accessed = false;

    if (ptr == tail)
      return;

    ptr = ptr->next;
  }
}

int give_complaint(char *source_apartment_id, char *dest_apartment_id,
                   char *remarks) {
  struct element *ptr;
  char *cp;

  strcpy(dest_apartment_id, source_apartment_id);
  if (!tail) {
    fprintf(stderr, "give_complaint: Queue is empty\n");
    return -1;
  }
  // get the head
  ptr = tail->next;
  while (1) {
    if (strcmp(source_apartment_id, ptr->apartment_id) == 0)
      break;

    if (ptr == tail)
      return -1;

    ptr = ptr->next;
  }
  strcpy(remarks, ptr->remarks);

  return 0;
}

void error(char *msg) {
  perror(msg);
  exit(1);
}
