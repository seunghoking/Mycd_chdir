// ex) ./mycd /tmp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
void rwx(file_mode){
  if(S_ISDIR(file_mode)){ //디렉토리이면 앞에 d를 출력
    printf("d");
  }
  else printf("-");
  //유저 권한
  if(file_mode&S_IRUSR){
    printf("r");
  }
  else printf("-");

  if(file_mode&S_IWUSR){
    printf("w");
  }
  else printf("-");

  if(file_mode&S_IXUSR){
    printf("x");
  }
  else printf("-");
  //그룹 권한
  if(file_mode&S_IRGRP){
    printf("r");
  }
  else printf("-");

  if(file_mode&S_IWGRP){
    printf("w");
  }
  else printf("-");

  if(file_mode&S_IXGRP){
    printf("x");
  }
  else printf("-");
  //기타 권한
  if(file_mode&S_IROTH){
    printf("r");
  }
  else printf("-");

  if(file_mode&S_IWOTH){
    printf("w");
  }
  else printf("-");

  if(file_mode&S_IXOTH){
    printf("x");
  }
  else printf("-");

}

int main(int argc, char *argv[]){
  char cwd[100];
  DIR *dp; //디렉토리 포인터
  struct dirent *dirp;
  struct passwd *pw;  //pw_name 얻기위해 선언
  struct group *group;  //gr_name 얻기위해 선언
  struct tm *time_info; // 파일 생성시간 얻기 위해 선언
  struct stat file_info;  //파일 정보 얻기위해 선언
  mode_t file_mode;
  char buf[100]; //시간을 불러올때 사용

  if(argc < 2){
    printf("error : 잘못된 파일 경로 입력\n");  //경로를 입력하지않으면 오류
    exit(1);
  }

  if(argc == 2){
    if(chdir(argv[1])==-1){
        printf("error : 잘못된 디렉토리 변경\n");
        exit(1);
    }  //change directory
    getcwd(cwd,sizeof(cwd));  //현재 디렉토리의 위치를 알려주는 함수
    printf("현재 위치 : %s\n",cwd); //현재 디렉토리 출력
    dp = opendir("."); //현재 디렉토리를 지정해서 오픈 ".."은 상위, '.'은 현재
    while(dirp = readdir(dp)){ //readdir함수로 읽어옴
      stat(dirp->d_name,&file_info);
      if(dirp->d_name[0]=='.') continue; // (. , ..) 이라는 숨김파일 두개가 계속 떠서 사용.
      else{
        file_mode = file_info.st_mode; // rwx
        rwx(file_mode);
        printf(" %2d",file_info.st_nlink); //link의 개수 출력

        pw = getpwuid(file_info.st_uid);
        group = getgrgid(file_info.st_gid);
        printf(" %s ",pw->pw_name);
        printf(" %s ",group->gr_name);
        printf(" %lld ",file_info.st_size);
        time_info = localtime(&(file_info.st_mtime));
        strftime(buf,sizeof(buf),"%m월 %d일 %H:%M",time_info); //날짜/시간을 스트링으로 변환
        printf(" %s ",buf);
        printf("%s\n",dirp->d_name);
      }
    }
    closedir(dp);
  }
  return 0;
}
