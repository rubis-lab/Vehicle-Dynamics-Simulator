/***************************************************************************

    file                 : main.cpp
    created              : Sat Mar 18 23:54:30 CET 2000
    copyright            : (C) 2000 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: main.cpp,v 1.14.2.1 2008/11/09 17:50:22 berniw Exp $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>

#include <GL/glut.h>

#include <tgfclient.h>
#include <client.h>

#include "linuxspec.h"

/*** kswe ***/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#include "../data_list.h"

#define SHMEM 1230
#define SHMEM2 4320

// for shared mem (input)
int shmid_input;
float *user_input;
int size_user_input = 10;
void *shared_memory_input = (void *)0;

// for shared mem (output)
int shmid_output;
float *torcs_output;
int size_torcs_output = 200;
void *shared_memory_output = (void *)0;

void *init_shared_mem(int *id, int key, int data_size, int num_of_data, void *mem);
int free_shared_mem(void *data, int id);
void do_exit();
void signal_handler(int signal)
{
	do_exit();
}
/************/

static void
init_args(int argc, char **argv)
{
    int		i;
    char	*buf;

    i = 1;
    while (i < argc) {
	if (strncmp(argv[i], "-l", 2) == 0) {
	    i++;
	    if (i < argc) {
		buf = (char *)malloc(strlen(argv[i]) + 2);
		sprintf(buf, "%s/", argv[i]);
		SetLocalDir(buf);
		free(buf);
		i++;
	    }
	} else if (strncmp(argv[i], "-L", 2) == 0) {
	    i++;
	    if (i < argc) {
		buf = (char *)malloc(strlen(argv[i]) + 2);
		sprintf(buf, "%s/", argv[i]);
		SetLibDir(buf);
		free(buf);
		i++;
	    }
	} else if (strncmp(argv[i], "-D", 2) == 0) {
	    i++;
	    if (i < argc) {
		buf = (char *)malloc(strlen(argv[i]) + 2);
		sprintf(buf, "%s/", argv[i]);
		SetDataDir(buf);
		free(buf);
		i++;
	    }
	} else if (strncmp(argv[i], "-s", 2) == 0) {
	    i++;
	    SetSingleTextureMode ();
	    
#ifndef FREEGLUT
	} else if (strncmp(argv[i], "-m", 2) == 0) {
	    i++;
	    GfuiMouseSetHWPresent(); /* allow the hardware cursor */
#endif
	} else {
	    i++;		/* ignore bad args */
	}
    }
#ifdef FREEGLUT
    GfuiMouseSetHWPresent(); /* allow the hardware cursor (freeglut pb ?) */
#endif
}

/*
 * Function
 *	main
 *
 * Description
 *	LINUX entry point of TORCS
 *
 * Parameters
 *	
 *
 * Return
 *	
 *
 * Remarks
 *	
 */
int 
main(int argc, char *argv[])
{
    init_args(argc, argv);

    /*** kswe ***/
    	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	user_input = (float*)init_shared_mem(&shmid_input, SHMEM, sizeof(float), size_user_input, shared_memory_input);		// shm start (input)
	torcs_output = (float*)init_shared_mem(&shmid_output, SHMEM2, sizeof(float), size_torcs_output, shared_memory_output);
	for(int i = 0; i < size_user_input; i++)
		user_input[i] = 0.0;
	for(int i = 0; i < size_torcs_output; i++)
		torcs_output[i] = 0.0;
	user_input[STEER_MODE] = 1.0;	// steering mode: lane keeping
    /************/
    
    LinuxSpecInit();		/* init specific linux functions */
    
    GfScrInit(argc, argv);	/* init screen */

    TorcsEntry();		/* launch TORCS */
    
    glutMainLoop();		/* event loop of glut */

    return 0;			/* just for the compiler, never reached */
}

/*** kswe ***/
void *init_shared_mem(int *id, int key, int data_size, int num_of_data, void *mem)
{
	*id = shmget((key_t)key, data_size*num_of_data, 0666|IPC_CREAT);
	if(*id == -1)
	{
		perror("shmget failed : ");
		exit(0);
	}

	mem = shmat(*id, (void *)0, 0);
	if(mem == (void *)-1)
	{
		perror("shmat failed : ");
		exit(0);
	}

	return mem;
}

int free_shared_mem(void *data, int id)
{
	shmdt(data);
	return shmctl(id, IPC_RMID, 0);
}

void do_exit()
{
    STOP_ACTIVE_PROFILES();
    PRINT_PROFILE();
/*     glutSetKeyRepeat(GLUT_KEY_REPEAT_ON); */
    GfScrShutdown();
    	int re;
	re = free_shared_mem(user_input, shmid_input);		// shm end (input)
	re = free_shared_mem(torcs_output, shmid_output);	// shm end (output)
    	printf("free shared memory with %d\n", re);
    exit(0);
}

/************/
