/* In the name of Allah, the Most Gracious, the Most Merciful. We are starting to learn the C programming language!                                                                       with CS50 conducted by Mr.David Malan */

/*
#include <stdio.h>
#include <string.h> 
int main ()
{
      char name [20];
      printf("What's you name? "); 
      fgets (name, sizeof(name), stdin);
      name [strcspn(name, "\n")] = '\0';
      printf ("Hello, %s!\n", name);

printf ( "I’m enthusiastic about starting my journey in C programming, aiming to become a skilled exploit developer and make meaningful contributions to my team, Fuzzraiders\n"); 

}

*/

#include <stdio.h>
#include <string.h> 
#include <ctype.h>

void toLowerCase(char str[]) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    char Ahmed[] = "Huuh! Ahmed I know him from a long time ago, he lives in Egypt now";
    char Mostafa[] = "He's an old friend of mine, he stays in Brazil";
    char Gazaali[] = "He lives in Ethiopia";

    char name[50];

    printf("Who are you looking for? ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    char inputLower[50];
    strcpy(inputLower, name);
    toLowerCase(inputLower);

    if (strcmp(inputLower, "ahmed") == 0) {
        printf("%s\n", Ahmed);
    } else if (strcmp(inputLower, "mostafa") == 0) {
        printf("%s\n", Mostafa);
    } else if (strcmp(inputLower, "gazaali") == 0) {
        printf("%s\n", Gazaali);
    } else {
        printf("Sorry, I don't know that person.\n");
    }

}

