#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

static Node *head = NULL;

void insert_serial(int value)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = head;
    head = new_node;
}

void delete_serial(int value)
{
    Node *curr = head;
    Node *prev = NULL;

    while (curr != NULL && curr->data != value)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr != NULL)
    {
        if (prev == NULL)
        {
            head = curr->next;
        }
        else
        {
            prev->next = curr->next;
        }
        free(curr);
    }
}

int member_serial(int value)
{
    Node *curr = head;
    while (curr != NULL)
    {
        if (curr->data == value)
            return 1;
        curr = curr->next;
    }
    return 0;
}

// Function to free the linked list
void free_list_serial()
{
    Node *current = head;
    Node *next;

    while (current != NULL)
    {
        next = current->next; // Save the next node
        free(current);        // Free the current node
        current = next;       // Move to the next node
    }

    head = NULL; // Reset the head pointer to NULL (empty list)
}
