#include<stdio.h>

struct test
{
    long int a;
    int b;
    int c;
}stu[1001];

int main()
{
    int n,i,m,p[1000];
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        scanf("%ld %d %d",&stu[i].a,&stu[i].b,&stu[i].c);
    }
    scanf("%d",&m);
    for(i=0;i<m;i++)
    {
        scanf("%d",&p[i]);
    }
    for(i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(p[i]==stu[j].b)
                printf("%ld %d\n",stu[j].a,stu[j].c);
        }
    }
}