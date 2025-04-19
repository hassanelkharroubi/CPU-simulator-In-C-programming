#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_SIZE 128

typedef struct hashentry
{
    char *key;
    void *value;
} HashEntry;

typedef struct hashmap
{
    int size;
    HashEntry *table;
} HashMap;


HashEntry *table[TABLE_SIZE];

void fill_table()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = NULL;
    }
}

void show_hash_entry(HashEntry *data)
{
    printf("key : %s\n", data->key);
    printf("age : %d\n", data->age);
}

void *create_hash_entry()
{
    HashEntry *h;
    h = (HashEntry *)malloc(sizeof(HashEntry));
    if (h == NULL)
    {
        printf("There is no more space for allocation this item\n");
        return h;
    }
    printf("Give me key : ");
    scanf("%s", h->key);
    printf("Give me age : ");
    scanf("%d", &h->age);
    return h;
}

void show_table()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (table[i] != NULL)
            show_hash_entry(table[i]);
    }
}

int hash(char key[])
{
    int s = 0;
    for (int i = 0; i < strlen(key); i++)
    {
        s = s + key[i];
    }
    
    return s % TABLE_SIZE;
}

void insert_data(HashEntry *h)
{
    if (h == NULL)
    {
        printf("data is empty\n");
        return;
    }

    int index = hash(h->key), i = 0;    

    while (table[(index + i)] != NULL)
        i += 1;
    table[(index + i) ] = h;
}


HashEntry *search(char key[])
{
    int index = hash(key);
    HashEntry *h = table[index];
    int i=0;
    if (h)
    {

        while (strcmp(h->key,key)!=0)
        {
            i++;
            h=table[index+i];
        }
        return h;

    }
    else{
        printf("No data found for this key :  %s\n",key);
        return NULL;
    }
    
    return h;
}
int main(int argc, char const *argv[])
{

// initialisation ogf the table
    fill_table();
    //create first hash entry
    HashEntry *h = (HashEntry *)create_hash_entry();
    //insert data
    insert_data(h);
    HashEntry *h1 = (HashEntry *)create_hash_entry();
    insert_data(h1);
    show_table(table);




    return 0;
}

