
#include "main.h"


int main(int argc, char* argv[]){

    int i,j,count;
    pid_t pid;
    char target[3];
    serial_Info *sInfo;
    system("adb devices > adb.list");
    list_fp = fopen("./adb.list","r");
    id_resource=get_adblist(&sInfo,TRUE);

    #if LOG_ENABLE
        printf("<1> Make adb.list file\n");
    #endif

    while(1){  //  Get Log data for each devices using fork.
        for(i=0; i <id_resource; i++){

            if( used_resource[i] == ENABLE){
                #if LOG_ENABLE
                    printf("<2> Starting get LOG FILE(%d).\n", i);
                #endif
                used_resource[i] = RUNNING;
                #if LOG_ENABLE
                    printf("<2-1> used_resource(%d) : ENABLE -> RUNNING\n", i);
                #endif
                pid = fork();
                switch(pid){
                    case -1 :   perror("failed to create process.");exit(-1);
                    case 0  :   /*child process*/

                                if(sInfo[i].serial_target < 10){
                                    target[0] = sInfo[i].serial_target +'0';
                                    target[1] = '\0';
                                }else if(sInfo[i].serial_target < DEVICE_FULL){
                                    target[0] = 1;
                                    target[1] = (sInfo[i].serial_target % 10) +'0';
                                    target[2] = '\0';
                                }

                                strcpy(sInfo[i].filename, logFile);
                                strcat(sInfo[i].filename, target);
    /*
    */
                                strcpy(command, "adb");
                                strcat(command, " -s ");
                                strcat(command, sInfo[i].serial_No);

                                /*  if you want to get log data whatever, you can adjust adb options. */
                                if(argc < 2)
                                    strcat(command, " logcat");
                                else{
                                    for(j=1; j < argc; j++){
                                        strcat(command, " ");
                                        strcat(command, argv[j]);
                                    }
                                    strcat(command, " ");
                                }


                                strcat(command, " >> ");
                                strcat(command, sInfo[i].filename);

                                #if LOG_ENABLE
                                    printf("<2-2> system command : %s\n", command);
                                #endif
                                system(command);

                                strcpy(command, "echo ");
                                strcat(command, endFile);
                                strcat(command, " >> ");
                                strcat(command, sInfo[i].filename);

                                system(command);

                                exit(0);

                    case 1  :   /*parent process*/
                                printf("fork Success : %d\n", sInfo[i].serial_target);break;

                }
            }
        }
         check_adblist(&sInfo, id_resource);/*somethine is changed.*/
         sleep(5);

    }

    return 0;
}

int get_adblist(serial_Info **sInfo, int status){ //Get adb devices list (* Check aperformance?)

    int device_count,i;

    if(list_fp < 0){
        perror("not found \"adb.list\" file\n");
        exit(-2);
    }else{
        device_count=0;
        do{

            if(fscanf(list_fp, "%s", buff) < 0){
                break;
            }

            if(strstr(message, buff) != NULL){
                continue;
            }
            
            if(device_count == 0 ){
                *sInfo = (serial_Info *)malloc((device_count+1) * sizeof(serial_Info));
                (*sInfo)[device_count].serial_No = (char *) malloc(sizeof(char) * ARRA_SIZE);
            }
            else{
                *sInfo = (serial_Info *)realloc(*sInfo, (device_count+1) * sizeof(serial_Info));
                (*sInfo)[device_count].serial_No = (char *) malloc(sizeof(char) * ARRA_SIZE);
            }
            if(sInfo == 0){
                printf("malloc erre\n");
                exit(-2);
            }

            if((*sInfo)[device_count].serial_No == 0 ){
                printf("failed to malloc\n");
                exit(-2);
            }
            sscanf(buff,"%s",(*sInfo)[device_count].serial_No);
            (*sInfo)[device_count].serial_target = device_count+1;



            if(status == TRUE){
                used_resource[device_count]=ENABLE;
            }
            device_count++;
        }while(1);
    }
/*
    sscanf(buff, "%s", id_target[device_count]);
    if(device_count == 0) return 0;
    else{
        *sInfo = (serial_Info *)malloc((device_count) * sizeof(serial_Info));
        for(i=0; i<device_count; i++){
            (*sInfo)[device_count].serial_No = (char *) malloc(sizeof(char) * ARRA_SIZE);
            sscanf( id_target[i], "%s", (*sInfo)[device_count].serial_No);
            (*sInfo)[device_count].serial_target = i+1;
        }

    }
*/

    fclose(list_fp);
    return device_count;
}

int check_adblist(serial_Info **old_sInfo, int old_count){

    #if LOG_ENABLE
        printf("<3> Refresh ADB LIST.\n");
    #endif

    int i,j,k;
    int location;
    system("adb devices > adb.list_temp");
    serial_Info *new_sInfo;
    int new_count, terminated;

    list_fp = fopen("./adb.list_temp","r");
    new_count = get_adblist(&new_sInfo, FALSE);

    /* If added devices is existed, */
    for(k=0; k < id_resource; k++){
        if( strcmp((*old_sInfo)[k].serial_No, buff ) == 0 ){

            #if LOG_ENABLE
                printf("<3-2> Add original devices.\n");
            #endif
            used_resource[k] = ENABLE;
            break;
        }
    }


        //   1. ADD new devices
        for(i=0; i<new_count; i++){
            for(j=0; j<old_count; j++){
                if( strcmp( new_sInfo[i].serial_No, (*old_sInfo)[j].serial_No) == 0 ){
                    terminated = 0;
                    break;
                }else{
                    terminated = -1;
                    strcpy(buff, new_sInfo[i].serial_No);

                }

            }// 1 for 2 loop state
            if(terminated == -1 ){ //if true , Add new devices

                #if LOG_ENABLE
                    printf("<3-1> Add new devices.\n");
                #endif

                *old_sInfo = (serial_Info *)realloc(*old_sInfo, (id_resource+1) * sizeof(serial_Info));
                if(*old_sInfo == NULL)free(old_sInfo);
                //(*old_sInfo)[id_resource].serial_No = (char *) realloc((*old_sInfo)[id_resource].serial_No, sizeof(char) * ARRA_SIZE);
                (*old_sInfo)[id_resource].serial_No = (char *) malloc( sizeof(char) * ARRA_SIZE);
                if((*old_sInfo)[id_resource].serial_No == NULL)free((*old_sInfo)[id_resource].serial_No);

                sscanf(buff,"%s",(*old_sInfo)[id_resource].serial_No);
                (*old_sInfo)[id_resource].serial_target = id_resource+1;

                used_resource[id_resource]=ENABLE;

                id_resource++;
            }
        }// 2 for 2 loop state

        // 2. STOP original devices
        for(i=0; i<old_count; i++){
            for(j=0; j<new_count; j++){
                if( strcmp((*old_sInfo)[i].serial_No , new_sInfo[j].serial_No) == 0 ){
                    terminated = 0;
                    if(used_resource[i]== STOP){
                        #if LOG_ENABLE
                            printf("<3-2> Add original devices.\n");
                            printf("<3-3> used_resource(%d) : STOP -> ENABLE.\n",i);
                        #endif
                        used_resource[i]=ENABLE;
                        sleep(3);
                    }
                    break;
                }else{

                    terminated = -1;
                    location = i;
                }

            }// 1 for 2 loop state
            if(terminated == -1 ){

                if(used_resource[i]==STOP) continue;
                #if LOG_ENABLE
                    printf("<3-4> Stop original devices.\n");
                #endif

                #if LOG_ENABLE
                    printf("<3-5> used_resource(%d) : RUNNING -> STOP\n", i);
                #endif
                used_resource[location] = STOP;

            }
        }// 2 for 2 loop state

    system("cp adb.list_temp adb.list");
    //system("rm adb.list_temp");

    return 1;
}
