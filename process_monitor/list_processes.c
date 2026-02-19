#include "list_processes.h"

/// Checks if the target pid is valid
int is_pid(const char *name) {
    if (!name || !*name) return 0;
    for (int i = 0; name[i]; i++)
    {
        if (!isdigit(name[i])) return 0;
    }    
    return 1;
}

/// Used for obtaining a list of processes found in '/proc' virtual folder
process * getProcesses()
{
    process * ret = (process*)calloc(MAX_PROCESSES, sizeof(process));
    int count = 0;
    /// Initialize all fields
    for(int i =0; i< MAX_PROCESSES; i++)
    {
        ret[i].pid = -1;
        ret[i].ppid = -1;
        ret[i].name[0] = '\0';
        ret[i].path[0] = '\0';
    }
    
    /// Try to open the directory
    DIR *dir = opendir("/proc");
    struct dirent *entry;
    if(dir == NULL)
    {
        printf("Could not open process directory");
        return ret;
    }

    /// For each folder in said directory
    while((entry = readdir(dir))!=NULL)
    {   
        /// Get the pid
        int pid = atoi(entry->d_name);
        if(pid <=0 || !is_pid(entry->d_name)) continue;
        
        ret[count].pid = pid;
        
        char path[256];
        /// Open status file
        snprintf(path, sizeof(path), "/proc/%d/status", pid);
        FILE * f = fopen(path,"r");
        
        if(f)
        {
            char line[256];
            while(fgets(line,sizeof(line),f))
            {
                if(strncmp(line,"Name:",5)==0)
                {                
                    /// Get the name of the process
                    sscanf(line, "Name:\t%255[^\n]", ret[count].name);
                }
                else if(strncmp(line, "PPid:",5) == 0)
                {   
                    /// And the pid of the parent process
                    sscanf(line, "PPid:\t%d", &ret[count].ppid);
                }
            }
        }
        /// The path for the target process
        snprintf(ret[count].path, sizeof(ret[count].path), "/proc/%d", pid);
        
        count++;
    }

    closedir(dir);
    return ret;
}

/*
    The functions bellow were used for checking 
    the output of the functions
    defined above
*/
void clearScreen()
{
    printf("\033[H\033[J");
    fflush(stdout);
}

void prettyPrintProcesses(process * target)
{
    clearScreen();
    printf("|%-6s|%-30s|%-6s|\n","PID","NAME","PPID");
    for(int i = 0; i< MAX_PROCESSES; i++)
    {
        if(target[i].pid < 0) break;
        printf("|%-6d|%-30s|%-6d|\n", target[i].pid, target[i].name, target[i].ppid);
    }
}

void runtimeTestLoop()
{
    process * previousProcesses = getProcesses();
    clearScreen();
    printf("PID\n");
    prettyPrintProcesses(previousProcesses);
    while(1)
    {
        int rewrite = 0;
        process * processes = getProcesses();
        for(int i = 0; i< MAX_PROCESSES;i++)
        {
            if(previousProcesses[i].pid != processes[i].pid)
            {
                rewrite = 1;
            }
        }
        if(rewrite)
        {
            prettyPrintProcesses(processes);
            process * aux = previousProcesses;
            previousProcesses = processes;
            free(aux);
        }
        else
        {
            free(processes);
        }
        sleep(5);
    }

} 