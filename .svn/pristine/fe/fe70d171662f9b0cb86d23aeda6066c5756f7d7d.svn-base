/**
 * @file    imc_proc.c
 * @brief This file contains code to spwan a process
 *
 * @details Copyright PowerWave Technologies, Inc., 2009
 *
 * @author Manu Sharma
 *
 * @date 08 July 2010  Added PID parameter to startExe function - it will
                       store and return the PID of the process it spawned : Shweta
 */

#include "imc_proc.h"

/**
 * @param program The name of program to be spawned
 * @param arg_list Command line arguments to program
 * @details 
 * Spawn a child process running a new program.  PROGRAM is the name
 * of the program to run; the path will be searched for this program.
 * ARG_LIST is a NULL-terminated list of character strings to be
 * passed as the program's argument list.  Returns the process id of
 * the spawned process.
 */
int spawn (char* program, char *IN arg_list[])
{
  pid_t child_pid;
  int ec;
  /* m_sharma, 17Aug09:
   * The program may have been downloaded using FTP.
   * In this case, the execute permission will not be set on that file.
   * set the permission
   */
  if(chmod(program, S_IRWXU | S_IRWXG ) NOTEQ 0)
  {
    fprintf (stderr, "could not set S_IRWXU | S_IRWXG for prog=%s\n", program);
    perror(program);
    return -1;
  }
  /* Duplicate this process.  */

  child_pid = fork ();
  if (child_pid NOTEQ 0)
    /* This is the parent process.  */
    return child_pid;
  else {
    /* Now execute PROGRAM, searching for it in the path.  */
    ec = execvp (program, arg_list);
    /* The execvp function returns only if an error occurs.  */
    fprintf (stderr, "an error occurred in execvp, error=%d\n", ec);
    perror(program);
    exit(EXIT_FAILURE);
  }
}

/**
 * @param   exeDir  The complete path of the executable
 * @param   exeName The name of executable
 * @param   pid     PID of the process will be stored.
 * @details This function starts the executable exeName, in the directory exeDir
 */
int startExe(
  IN UINT8  *exeDir,
  IO UINT8  *exeName,
  IO SINT32 *pid,
  char *IN args[]
)
{
  String100 cmd;
  char *argList[] = { exeName, NULL };

  sprintf(&cmd[0], exeDir);
  strcat(&cmd[0], "/");
  strcat(&cmd[0], exeName);

  if(NULL NOTEQ args){
  	*pid = spawn((char*)&cmd[0], args);
  }
  else{
  	*pid = spawn(&cmd[0], argList);
  }
  return TRUE;
}

