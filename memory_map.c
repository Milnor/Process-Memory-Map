#include <elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FMT_CHR         "%-16s  %p  %c\n"
#define FMT_HEX         "%-16s  %p  %-.32x\n"
#define FMT_INT         "%-16s  %p  %d\n"
#define FMT_STR         "%-16s  %p  %-.32s\n"

#define ALLOC_SIZE      1024
#define ONE_DIGIT_MAX   9

extern char etext, edata, end;

const char ** find_auxv(const char * env[])
{
    size_t i = 0;
    while (NULL != env[i])
    {
       i++;
    }
   return &env[i + 1]; 
}

size_t get_last_envp(char * environ[])
{
    size_t i = 0;
    while (NULL != environ[i])
    {
        i++;
    }
    return i;
}

int main(int argc, char * argv[], char * envp[])
{
    
    // stack
    printf(FMT_INT, "argc", &argc, argc);
    for (size_t i = 0; i < argc && i <= ONE_DIGIT_MAX; i++)
    {
        char args[] = "argv[N] ";
        snprintf(args, strlen(args), "argv[%ld]", i);
        printf(FMT_STR, args, &argv[i], argv[i]);
    }

    for (size_t i = 0; i <= ONE_DIGIT_MAX; i++)
    {
        char envars[] = "envp[N ] ";
        snprintf(envars, strlen(envars), "envp[%ld]", i);
        printf(FMT_STR, envars, &envp[i], envp[i]);
    }

    //size_t last = get_last_envp(envp);

    //printf(FMT_STR, "envp[N]", &envp[last], envp[last]);

    //Elf64_auxv_t * auxv[] = envp[last];
    //printf(FMT_STR, "auxv0", &auxv[0], auxv[0]);

  Elf64_auxv_t *auxv;
  while(*envp++ != NULL);

  /*from stack diagram above: *envp = NULL marks end of envp*/
  int i = 0 ;
  for (auxv = (Elf64_auxv_t *)envp; auxv->a_type != AT_NULL; auxv++)
    /* auxv->a_type = AT_NULL marks the end of auxv */
  {
    printf("%lu %u %u \n", (auxv->a_type), AT_PLATFORM, i++);
    if( auxv->a_type == AT_PLATFORM)
      printf("AT_PLATFORM is: %s\n", ((char*)auxv->a_un.a_val));
  }

    //printf(FMT_STR, "auxv[0]", &envp[last + 2], envp[last + 2]);

    //printf(FMT_STR, "auxv[0]", &envp[last + 3], envp[last + 3]);
    //printf(FMT_STR, "auxv[0]", &envp[last + 4], envp[last + 4]);
    //char ** auxv = find_auxv(envp);
    //printf(FMT_STR, "auxv", &auxv, auxv);

    // heap
    char * alloc1 = malloc(ALLOC_SIZE);
    char * alloc2 = malloc(ALLOC_SIZE);
    printf(FMT_STR, "alloc1", &alloc1, alloc1);
    printf(FMT_STR, "alloc2", &alloc2, alloc2);

    printf(FMT_CHR, "end", &end, end); 
    printf(FMT_CHR, "edata", &edata, edata); 
    // .text
    printf(FMT_HEX, "main()", (void *) main,  main);
    printf(FMT_CHR, "etext", &etext, etext); 

    free(alloc1);
    free(alloc2);
}