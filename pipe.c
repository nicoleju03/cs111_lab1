#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{
  if(argc<=1){
    return EINVAL;
  }
  if(argc==2){
    int ret = fork();
    if(ret<0) return errno;
    if(ret==0){
      //child
      if(execlp(argv[1],argv[1],NULL)<0){
	return errno;
      }
      exit(1);
    }
    if(ret>0){
      //parent
      int status = 0;
      waitpid(ret,&status,0);
      exit(WEXITSTATUS(status));
    }
  }
  else{
    int fds[2];
    int* pids = (int*)malloc(sizeof(int)*(argc-1));
    int i=1;
    while(i<argc-1){
      if(pipe(fds)<0) return errno;
      int ret = fork();
      if(ret<0) return errno;
      if(ret==0){
	dup2(fds[1],STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
	if(execlp(argv[i],argv[i],NULL)<0){
	  return errno;
	}
	exit(1);
      }
      else{
	dup2(fds[0],STDIN_FILENO);
	close(fds[0]);
	close(fds[1]);
	pids[i-1]=ret;
      }
      i++;
    }
    int ret2 = fork();
    if(ret2<0) return errno;
    if(ret2==0){
      if(execlp(argv[argc-1],argv[argc-1],NULL)<0){
	return errno;
      }
    }
    else{
      close(fds[0]);
      close(fds[1]);
      pids[argc-1]=ret2;
      int status=0;
      for(int i=0; i<argc-1; i++){
	waitpid(pids[i],&status,0);
	if(!WIFEXITED(status)){
	  return ECHILD;
	}
	if(WEXITSTATUS(status)!=0){
	  return WEXITSTATUS(status);
	}
      }
    }
    free(pids);
  }
  return 0;
}
