#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define M 8
typedef struct List
    {
        char * str;
        struct List * next;
    } List;
List * Output_List(List * p)
{
    List * q = p;
    List * q1 = p;
    while (q!=NULL)
    {
        printf("%s\n",q->str);
        q1 = q;
        q = q->next;
        free(q1->str);
        free(q1);
    }
    p = NULL;
    return p;
}
char * get_s(void)
{
    int n, k = 0;
    char *s = malloc(M);
    if (!s) 
    {
        free(s);
        return NULL;
    }
    while(fgets(s + k, M, stdin))
    {
        n = strlen(s);
        if(s[n-1] != '\n')
        {
            k = k + M - 1;
            s = realloc(s, k + M);
            if (!s) 
            { 
                free(s);
                return NULL;
            }
        }
        else 
        {
            s[n-1] = '\0';
            return s;
        }
    }
    free(s);
    return NULL;
}
List * Input_List(List * p)
{
    int x = 1;
    char * s;
    while (x == 1)
    {
        while (p == NULL)
        {
            s = get_s();
            if (s[0] != '\0')
            {
                p = malloc(sizeof(List));
                p->str = s;
                p->next = NULL;
            }
            else
            {
                free(s);
                printf("Если вы хотите продолжить ввод напишите 1, 0 иначе\n");
                scanf("%d",&x);
                getchar();
            }
        }
        List * q;
        List * q1;
        s = get_s();
        while (s[0]!='\0')
        {
            q = p;
            while ((q!=NULL) && (strcmp(s,q->str) >= 0))
            {
                q1 = q;
                q = q->next;       
            }
            if (q == NULL)
            {
                q1->next = malloc(sizeof(List));
                q1->next->str = s;
                q1->next->next = NULL;
            }
            else if (q == p)
            {
                p = malloc(sizeof(List));
                p->str = s;
                p->next = q; 
            }
            else
            {
                q1->next = malloc(sizeof(List));
                q1->next->str = s;
                q1->next->next = q;
            }
            s = get_s();
        }
        if (s[0] == '\0')
        {
            free(s);
            p = Output_List(p);
            printf("Если вы хотите продолжить ввод напишите 1, 0 иначе\n");
            scanf("%d",&x);
            getchar();
        }
    }
    return p;
}
int main()
{
    List * p = NULL;
    p = Input_List(p);
    free(p);
    return 0;
}