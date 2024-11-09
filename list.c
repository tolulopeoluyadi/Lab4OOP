#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// Allocate memory for a new list and initialize its head to NULL
list_t *list_alloc() {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    list->head = NULL;
    return list;
}

// Free all nodes in the list and then free the list itself
void list_free(list_t *l) {
    node_t *current = l->head;
    while (current != NULL) {
        node_t *next = current->next;
        free(current);
        current = next;
    }
    free(l);
}

// Print the elements of the list
void list_print(list_t *l) {
    node_t *current = l->head;
    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
}

// Return the length of the list
int list_length(list_t *l) {
    int count = 0;
    node_t *current = l->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// Add a new element to the front of the list
void list_add_to_front(list_t *l, elem value) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = l->head;
    l->head = new_node;
}

// Add a new element to the back of the list
void list_add_to_back(list_t *l, elem value) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = NULL;

    if (l->head == NULL) {
        l->head = new_node;
    } else {
        node_t *current = l->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Add a new element at a specific index
void list_add_at_index(list_t *l, elem value, int index) {
    if (index < 0 || index > list_length(l)) {
        printf("Index out of bounds\n");
        return;
    }

    if (index == 0) {
        list_add_to_front(l, value);
        return;
    }

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->value = value;

    node_t *current = l->head;
    int i;
    for (i = 0; i < index - 1; i++) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}

// Remove the first element of the list and return its value
elem list_remove_from_front(list_t *l) {
    if (l->head == NULL) {
        return -1;
    }

    node_t *temp = l->head;
    elem value = temp->value;
    l->head = l->head->next;
    free(temp);
    return value;
}

// Remove the last element of the list and return its value
elem list_remove_from_back(list_t *l) {
    if (l->head == NULL) {
        return -1;
    }

    node_t *current = l->head;
    if (current->next == NULL) {
        elem value = current->value;
        free(current);
        l->head = NULL;
        return value;
    }

    while (current->next->next != NULL) {
        current = current->next;
    }

    elem value = current->next->value;
    free(current->next);
    current->next = NULL;
    return value;
}

// Remove the element at a specific index and return its value
elem list_remove_at_index(list_t *l, int index) {
    if (index < 0 || index >= list_length(l)) {
        printf("Index out of bounds\n");
        return -1;
    }

    if (index == 0) {
        return list_remove_from_front(l);
    }

    node_t *current = l->head;
    int i;
    for (i = 0; i < index - 1; i++) {
        current = current->next;
    }

    node_t *temp = current->next;
    elem value = temp->value;
    current->next = temp->next;
    free(temp);
    return value;
}

// Check if a specific element is in the list
bool list_is_in(list_t *l, elem value) {
    node_t *current = l->head;
    while (current != NULL) {
        if (current->value == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Return the element at a specific index
elem list_get_elem_at(list_t *l, int index) {
    if (index < 0 || index >= list_length(l)) {
        printf("Index out of bounds\n");
        return -1;
    }

    node_t *current = l->head;
    int i;
    for (i = 0; i < index; i++) {
        current = current->next;
    }

    return current->value;
}
char* listToString(list_t* list) {
    static char buffer[1024];  // Using a static buffer to store the string representation
    buffer[0] = '\0';  // Start with an empty string

    node_t* temp = list->head;
    while (temp != NULL) {
        char num_str[16];
        sprintf(num_str, "%d -> ", temp->value);  // Use temp->value instead of temp->data
        strcat(buffer, num_str);  // Append it to the buffer
        temp = temp->next;
    }
    strcat(buffer, "NULL");  // End the list with NULL

    return buffer;
}
// Return the index of a specific element in the list
int list_get_index_of(list_t *l, elem value) {
    int index = 0;
    node_t *current = l->head;
    while (current != NULL) {
        if (current->value == value) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}
