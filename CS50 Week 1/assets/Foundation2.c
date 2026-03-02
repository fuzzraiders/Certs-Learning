/* Bismiallah let's start  */
/*
#include <stdio.h>
#include <cs50.h>

int main()
{

   char Location [30];
   get_string("Where you located at? ");
   fflush (stdou.h);
if (location) == "Denmark\n"
{
    printf ( "%s!!", "Very good place to be!! huuh?\n")
}
else if (location) == "Iceland\n" 
{
    scanf ("How is the wheather out there?")
}


return (0)
}


*/

/*

#include <stdio.h>
#include <string.h>
int main()
{

    char Location [20];

    fgets("where you located at? \n"); 
    if (strcmp(Location, "Denmark")==0)
    {
        printf("%s!", "Really, what a good place to be!\n");
    }
    else if (strcmp(Location, "Swizz")==0)
    {
        printf("Some say" "/The heaven of the earth/");
    }
    else
     {
        printf("I haven't heard that Country\n")
    }


return 0;
}

*/




#include <stdio.h>
#include <string.h>

int main(void)      // Halkaani weeyaan halka uu ka bilaawdo C program //
{

    char Location [30];  
// Halkani waxaynu samaysanaynaa wax loogu yeedho "Variable" oo ah sida sanduuq oo kale oo aad wax 'value' qiimo ku ridanaysid"
    
    
    printf("where you do you live? ");  
// inatana, waxaynu rabnaa ruuxa arki doono program-kan in aynu waydiino Su'aal //
   
    fgets (Location, sizeof(Location), stdin);  
    // fgets --> " Howshaydu waxa weeyaan in ay aqriso a line of text from an input stream which is 'stdin' dabadeed waxa ay ku keedinaysaa character arry which in out case 'Location'    <---         "
              
                //sizeof(Location) → the maximum number of characters to read//
    
    Location [ strcspn (Location, "\n")] = '\0';  // or you can use | Location [strcspn(Location, "\n")] =0;//  | //
    
/*
---- Finds the newline in the string  "\n"

---- Replaces it with null terminator "\0"

---- Cleans up the input for safe use                           

                                    [FROM CHATGPT]

Perfect! Let’s clearly show **the difference before and after removing the newline**.

We have this code:

```c
char Location[30];
fgets(Location, sizeof(Location), stdin);

// Before removing newline
printf("Before: You typed: %s\n", Location);

// Remove newline
Location[strcspn(Location, "\n")] = '\0';

// After removing newline
printf("After: You typed: %s\n", Location);
```

---

# 🔹 User Input

```
Paris
```

(They type `Paris` and press Enter)

---

# 🔹 Memory After `fgets()`

```
Index:    0   1   2   3   4   5   6
Value:    P   a   r   i   s  \n  \0
```

---

# 🔹 Output Before Removing `\n`

```text
Before: You typed: Paris
```

Notice the extra line **because `\n` is included in the string**:

```
Before: You typed: Paris
                                   <- extra line here
```

---

# 🔹 Output After Removing `\n`

After executing:

```c
Location[strcspn(Location, "\n")] = '\0';
```

Memory becomes:

```
Index:    0   1   2   3   4   5
Value:    P   a   r   i   s  \0
```

Output:

```text
After: You typed: Paris
```

Clean, no extra line.

---

So the **newline from Enter** is the culprit for the extra line in output.

If you want, I can make a **quick diagram showing both memory layouts and outputs side by side** — it’s super visual. Do you want me to do that?


*/    
    
    if (strcmp(Location, "Swittzerland") == 0)
    {
        printf("%s!! Some say \" The heaven of the earth\"\n", Location);
    }

/* if --> Jumlad shardigaan waxa ay Hubinaysaa in Variable-ka " Location " ay u dhiganto Swittzerland,
 hadii ay u dhiganto waxa uu ka shaqaysiinayaa block-ga ku xiga oo waxa uu soo dabacaya wixii aan u qornay */
 
 /* STRCMP stands for string compare    Full form: “string compare function” in C  
                          
 strcmp()

Compares two strings.

Returns 0 if they are exactly equal.

That’s why you check == 0.
 



strcmp() Returns a Number

When you do:

strcmp(str1, str2)

It compares strings character by character.

It doesn’t return true or false.

Instead, it returns an integer:

Return Value	Meaning
0	str1 is exactly equal to str2
<0	str1 is less than str2 (first differing character has smaller ASCII value)
>0	str1 is greater than str2 (first differing character has larger ASCII value)
Why == 0 in if?

We only want the code block to run when the two strings are exactly equal.

if (strcmp(Location, "Swittzerland") == 0)

strcmp(Location, "Swittzerland") → returns 0 if user typed "Swittzerland" exactly

== 0 → checks for equality

If it’s 0 → execute the if block
 */

    else if (strcmp(Location, "Iceland") == 0)
    {
        printf("So, you are The \"Snow guy\" Aren't you?\n");
    }

//hadii uusan ahayn if -ta Koowaad waxu weydinaya su'aal ah user's input ma u dhigmaa else if waxa ku jidha oo ah Iceland?
// hadii ay 0 tahy waxa ay ka dhigantahy in uu udhigmo sidaa daradeet waxa la fulinayaa code block-ka
// else if kahoosaya ku jidha 


    else
{
    printf (" I haven't heard that country in my life!\n");
}
// Hadii labadii aynu horay usoo sheegnay midna usan ahayn 
//waxaa execute lagu samaynayaa code else statement hoostiisa ku jidho 


return 0;
//Signals that the program ran successfully.
//Standard practice in main() function. 
}











