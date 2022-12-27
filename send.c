#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define MSGSZ     128


/*
 * Declare the message structure.
 */

typedef struct msgbuf {
         long    mtype;
         char    mtext[MSGSZ];
         } message_buf;

main()
{


    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;

    /*
     * Get the message queue id for the
     * "name" 1234, which was created by
     * the server.
     */
    key = 1234;

//(void) fprintf(stderr, "\nmsgget: Calling msgget(%#lx,\%#o)\n",key, msgflg);

    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }



    /*
     * We'll send message type 1
     */
    int i=0;
	do
	{

	    sbuf.mtype = 1;

	    printf("msgget: msgget succeeded: msqid = %d\n", msqid);
	    char text[10];
	    printf("Mesaj giriniz: \n");
	    fflush(stdout);
	    scanf("%s",text);
	    fflush(stdout);
	    printf("%s\n",&text);
	    (void) strcpy(sbuf.mtext, text);

	    printf("msgget: msgget succeeded: msqid = %d\n", msqid);
	    fflush(stdout);
	    buf_length = strlen(sbuf.mtext) + 1 ;



	    /*
	     * Send a message.
	     */
	    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
	       printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
		perror("msgsnd");
		exit(1);
	    }

	   else
	      	printf("Message: \"%s\" Sent\n", sbuf.mtext);
	  	fflush(stdout);

	} while(strcmp(sbuf.mtext,"exit")!=0);

    exit(0);
}